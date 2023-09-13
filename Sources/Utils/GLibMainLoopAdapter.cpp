#include "GLibMainLoopAdapter.h"

#include <gst/gst.h>

GLibMainLoopAdapter::GLibMainLoopAdapter(QObject* parent)
	: QObject(parent)
{
	connect(&mainLoopTickTimer, &QTimer::timeout, this, &GLibMainLoopAdapter::handleMainLoopTick);

	glibMainLoop = g_main_loop_new(nullptr, false);
}

void GLibMainLoopAdapter::handleMainLoopTick()
{
	GMainContext* loopContext = g_main_loop_get_context(glibMainLoop);

	g_main_context_iteration(loopContext, false);
}

GLibMainLoopAdapter::~GLibMainLoopAdapter()
{
	g_main_loop_unref(glibMainLoop);
}

void GLibMainLoopAdapter::setLoopState(bool isActive)
{
	if(isActive)
		mainLoopTickTimer.start();
	else
		mainLoopTickTimer.stop();
}

bool GLibMainLoopAdapter::getLoopState() const
{
	return mainLoopTickTimer.isActive();
}
