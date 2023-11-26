#include <QApplication>
#include <gst/gst.h>
#include <QMessageBox>
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
	viewer.resize(800, 600);
	viewer.show();

	QObject::connect(&viewer, &QCameraViewer::errorOccurred, &viewer, [](QString msg)
	{
		QMessageBox::warning(nullptr, "Error from RTSP viewer", msg);
	});

	viewer.start(address);

	return QApplication::exec();
}
