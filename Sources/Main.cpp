#include <QApplication>
#include <gst/gst.h>
#include "Utils/GLibMainLoopAdapter.h"
#include "Widgets/QProjectSelectorWindow/QProjectSelectorWindow.h"

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

	auto selectorWindow = new QProjectSelectorWindow {};
	selectorWindow->show();

	return QApplication::exec();
}
