#pragma once

#include <QMainWindow>
#include "Project/Project.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
	class QProjectWindow;
}
QT_END_NAMESPACE

struct QProjectWindowUiDeleter
{
	void operator()(Ui::QProjectWindow* ptr) const;
};

class QProjectWindow : public QMainWindow
{
Q_OBJECT

public:
	QProjectWindow(QWidget* parent, Project project);
	~QProjectWindow() override = default;

private:
	std::unique_ptr<Ui::QProjectWindow, QProjectWindowUiDeleter> ui {};
	Project project {};

};
