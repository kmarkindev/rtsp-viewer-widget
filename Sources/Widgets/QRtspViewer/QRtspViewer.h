#pragma once

#include <QWidget>

struct _GstElement;
typedef struct _GstElement GstElement;

struct _GstPad;
typedef struct _GstPad GstPad;

struct _GstBus;
typedef struct _GstBus GstBus;

struct _GstMessage;
typedef struct _GstMessage GstMessage;

class QRtspViewer : public QWidget
{
	Q_OBJECT

signals:

	void errorOccurred(QString error);
	void stateChanged(bool wasStarted);

public:

	explicit QRtspViewer(QWidget* parent = nullptr);

	~QRtspViewer() override;

	bool start(QByteArray cameraAddress);
	[[nodiscard]] bool started() const;
	void stop();

private:

	void setupUi();

	void updateState(bool newState);

	void clearContent();

	bool setupPipeline(QByteArray address);
	void destructPipeline();

	static void handleDecodeBinPadAdded(GstElement* src, GstPad* newPad, QRtspViewer* widget);
	static void handleRtspSourcePadAdded(GstElement* src, GstPad* newPad, QRtspViewer* widget);

	static void handleBusError(GstBus* bus, GstMessage* message, QRtspViewer* widget);
	static void handleBusEos(GstBus* bus, GstMessage* message, QRtspViewer* widget);

	bool isStarted { false };

	GstElement* pipeline {};
	GstBus* bus {};
	GstElement* rtspSourceElement {};
	GstElement* decodebin3Element {};
	GstElement* videoConvertElement {};
	GstElement* videoSinkElement {};

	QWidget* canvas {};

};
