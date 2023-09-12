#pragma once

#include <QWidget>

class QCameraViewer : public QWidget
{
	Q_OBJECT

public:

	explicit QCameraViewer(QWidget* parent);
	QCameraViewer(QString cameraUrl, QWidget* parent);


private:

	QString cameraUrl {};

};
