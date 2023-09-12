#include "QCameraViewer.h"

#include <utility>

QCameraViewer::QCameraViewer(QWidget* parent)
	: QCameraViewer({}, parent)
{

}

QCameraViewer::QCameraViewer(QString cameraUrl, QWidget* parent)
	: QWidget(parent), cameraUrl(std::move(cameraUrl))
{

}
