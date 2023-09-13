#include "QCameraViewer.h"

#include <functional>
#include <utility>
#include <gst/gst.h>

QCameraViewer::QCameraViewer(QWidget* parent)
	: QWidget(parent)
{
	clearContent();
}

bool QCameraViewer::setupPipeline(QByteArray address)
{
	// Make sure we are not creating memory leaks by rewriting pointers
	destructPipeline();

	// Create pipeline and elements
	pipeline = gst_pipeline_new(nullptr);
	std::array elements {
		rtspSourceElement = gst_element_factory_make("rtspsrc", nullptr),
		decodebin3Element = gst_element_factory_make("decodebin3", nullptr),
		videoConvertElement = gst_element_factory_make("videoconvert", nullptr),
		autoVideoSinkElement = gst_element_factory_make("autovideosink", nullptr)
	};

	// Make sure all of them were created
	bool wasElementsCreated = std::ranges::none_of(
		elements,
		std::bind_front(std::equal_to<GstElement*> {}, nullptr)
	);
	if(!pipeline || !wasElementsCreated)
	{
		std::ranges::for_each(elements, [&](GstElement*& element){
			if(element)
			{
				gst_object_unref(element);
				element = nullptr;
			}
		});

		if(pipeline)
		{
			gst_object_unref(pipeline);
			pipeline = nullptr;
		}

		updateState(false);
		errorOccurred("Can't create gstreamer elements");
		return false;
	}

	// Add elements to pipeline
	std::ranges::for_each(elements, [&](GstElement* element){
		gst_bin_add(GST_BIN(pipeline), element);
	});

	// set properties
	g_object_set(rtspSourceElement, "location", address.data(), nullptr);
	g_object_set(autoVideoSinkElement, "sync", false, nullptr);

	// Link elements that can be safely linked
	bool sinkLinked = gst_element_link(videoConvertElement, autoVideoSinkElement);
	if(!sinkLinked)
	{
		updateState(false);
		errorOccurred("Can't link gstreamer elements");
		return false;
	}

	// Set up callback to link other elements dynamically
	g_signal_connect(rtspSourceElement, "pad-added", G_CALLBACK(&QCameraViewer::handleRtspSourcePadAdded), this);
	g_signal_connect(decodebin3Element, "pad-added", G_CALLBACK(&QCameraViewer::handleDecodeBinPadAdded), this);

	// Get bus and set up callbacks for events
	bus = gst_element_get_bus(pipeline);
	gst_bus_add_signal_watch(bus);

	g_signal_connect(G_OBJECT(bus), "message::error", G_CALLBACK(&QCameraViewer::handleBusError), this);
	g_signal_connect(G_OBJECT(bus), "message::eos", G_CALLBACK(&QCameraViewer::handleBusEos), this);

	// Finally start pipeline
	GstStateChangeReturn stateChangeResult = gst_element_set_state(pipeline, GST_STATE_PLAYING);
	if(stateChangeResult == GST_STATE_CHANGE_FAILURE)
	{
		updateState(false);
		errorOccurred("Can't set pipeline to PLAYING state");
		return false;
	}

	updateState(true);
	return true;
}

void QCameraViewer::destructPipeline()
{
	if (pipeline)
		gst_element_set_state(pipeline, GST_STATE_NULL);

	if (bus)
	{
		gst_object_unref(bus);
		bus = nullptr;
	}

	if(pipeline)
	{
		gst_object_unref(pipeline);
		pipeline = nullptr;
	}

	rtspSourceElement = nullptr;
	decodebin3Element = nullptr;
	videoConvertElement = nullptr;
	autoVideoSinkElement = nullptr;

	clearContent();

	updateState(false);
}

QCameraViewer::~QCameraViewer()
{
	destructPipeline();
}

void QCameraViewer::handleDecodeBinPadAdded(GstElement* src, GstPad* newPad, QCameraViewer* widget)
{
	GstPad* sinkPad = gst_element_get_static_pad(widget->videoConvertElement, "sink");

	if(!gst_pad_is_linked(sinkPad))
	{
		// TODO: make sure that we don't need to check pad caps before link here

		GstPadLinkReturn linkResult = gst_pad_link(newPad, sinkPad);

		if(GST_PAD_LINK_FAILED(linkResult))
		{
			widget->destructPipeline();
			widget->errorOccurred("Can't link gstreamer pads");
		}
	}

	gst_object_unref(sinkPad);
}

void QCameraViewer::handleRtspSourcePadAdded(GstElement* src, GstPad* newPad, QCameraViewer* widget)
{
	GstPad* sinkPad = gst_element_get_static_pad(widget->decodebin3Element, "sink");

	if(!gst_pad_is_linked(sinkPad))
	{
		GstCaps* newPadCaps = gst_pad_get_current_caps(newPad);
		GstStructure* newPadStruct = gst_caps_get_structure(newPadCaps, 0);
		const gchar* newPadType = gst_structure_get_name(newPadStruct);

		if(g_str_has_prefix(newPadType, "application/x-rtp"))
		{
			GstPadLinkReturn linkResult = gst_pad_link(newPad, sinkPad);

			if(GST_PAD_LINK_FAILED(linkResult))
			{
			  	widget->destructPipeline();
				widget->errorOccurred("Can't link gstreamer pads");
			}
		}

		gst_caps_unref(newPadCaps);
	}

	gst_object_unref(sinkPad);
}

void QCameraViewer::handleBusError(GstBus* bus, GstMessage* message, QCameraViewer* widget)
{
	GError* error;
	gchar* debugInfo;

	gst_message_parse_error(message, &error, &debugInfo);

	widget->destructPipeline();
	widget->errorOccurred(error->message);

	g_clear_error(&error);
	g_free(debugInfo);
}

void QCameraViewer::handleBusEos(GstBus* bus, GstMessage* message, QCameraViewer* widget)
{
	widget->destructPipeline();
}

bool QCameraViewer::start(QByteArray cameraAddress)
{
	return setupPipeline(std::move(cameraAddress));
}

bool QCameraViewer::started() const
{
	return isStarted;
}

void QCameraViewer::stop()
{
	destructPipeline();
}

void QCameraViewer::clearContent()
{
	// TODO:
}

void QCameraViewer::updateState(bool newState)
{
	bool oldState = isStarted;

	// make sure widget state is updated before emitting the signal
	isStarted = newState;

	if(oldState != newState)
		emit stateChanged(newState);
}
