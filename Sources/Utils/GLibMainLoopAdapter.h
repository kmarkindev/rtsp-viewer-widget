#pragma once

#include <QTimer>

struct _GMainLoop;
typedef struct _GMainLoop GMainLoop;

class GLibMainLoopAdapter : public QObject
{
	Q_OBJECT

public:

	explicit GLibMainLoopAdapter(QObject* parent);

	void setLoopState(bool isActive);
	[[nodiscard]] bool getLoopState() const;

	~GLibMainLoopAdapter() override;

private slots:

	void handleMainLoopTick();

private:

	GMainLoop* glibMainLoop {};
	QTimer mainLoopTickTimer {};

};
