#include <QApplication>
#include <gst/gst.h>
#include <QTimer>

#include "Widgets/QCamView/QCameraViewer.h"
#include "Utils/GLibMainLoopAdapter.h"

void initializeGStreamer(int argc, char* argv[])
{
	gst_init(&argc, &argv);

	auto* adapter = new GLibMainLoopAdapter { QCoreApplication::instance() };
	adapter->setLoopState(true);
}

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	initializeGStreamer(argc, argv);

	QCameraViewer viewer { nullptr };
	viewer.start("rtsp://127.0.0.1:554/live");
	viewer.resize(800, 600);
	viewer.show();

	QObject::connect(&viewer, &QCameraViewer::errorOccurred, &a, [](QString error) {
		qDebug() << "Error signal in v1: " << error;
	});

	QCameraViewer viewer2 { nullptr };
	viewer2.start("rtsp://user:pass@127.0.0.1:554/live");
	viewer2.move(500, 500);
	viewer2.resize(800, 600);
	viewer2.show();

	QObject::connect(&viewer2, &QCameraViewer::errorOccurred, &a, [](QString error) {
		qDebug() << "Error signal in v2: " << error;
	});

	return QApplication::exec();
}
