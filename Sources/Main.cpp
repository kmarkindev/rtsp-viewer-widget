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

	QCameraViewer viewer(nullptr);
	viewer.resize(800, 600);
	viewer.show();

	return QApplication::exec();
}
