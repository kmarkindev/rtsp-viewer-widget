#include "QProjectWindow.h"
#include "ui_QProjectWindow.h"


QProjectWindow::QProjectWindow(QWidget* parent, Project project) :
		QMainWindow(parent), ui(new Ui::QProjectWindow), project(std::move(project))
{
	ui->setupUi(this);
}

void QProjectWindowUiDeleter::operator()(Ui::QProjectWindow* ptr) const
{
	delete ptr;
}
