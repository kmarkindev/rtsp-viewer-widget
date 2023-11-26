#include <QApplication>
#include <gst/gst.h>
#include "Utils/GLibMainLoopAdapter.h"
#include "Widgets/QCameraViewer/QCameraViewer.h"

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

	if(argc < 2) {
		return -1;
	}

	QByteArray address = argv[1];

	QCameraViewer viewer {};
	viewer.start(address);

	return QApplication::exec();
}
