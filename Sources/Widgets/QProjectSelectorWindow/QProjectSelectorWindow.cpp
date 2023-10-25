#include "QProjectSelectorWindow.h"
#include "ui_QProjectSelectorWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include "Project/ProjectSaveLoad.h"
#include "Project/Project.h"
#include "Widgets/QProjectWindow/QProjectWindow.h"

QProjectSelectorWindow::QProjectSelectorWindow(QWidget* parent) :
		QWidget(parent), ui(new Ui::QProjectSelectorWindow)
{
	ui->setupUi(this);

	setAttribute(Qt::WA_DeleteOnClose, true);

	connect(ui->newProjectBtn, &QPushButton::clicked, this, &QProjectSelectorWindow::onNewProjectClicked);
	connect(ui->openProjectBtn, &QPushButton::clicked, this, &QProjectSelectorWindow::onOpenProjectClicked);
}

void QProjectSelectorWindow::onOpenProjectClicked(bool)
{
	QFileDialog dialog {this};
	dialog.setWindowTitle("Open Project");
	dialog.setNameFilter("Project Files (*.ipcamproj)");

	if(!dialog.exec())
		return;

	bool error = false;
	Project loadedProject = loadFromFile(dialog.selectedFiles().first(), error);

	if(error)
	{
		QMessageBox::critical(this, "Error", "Can't load project from selected file");
		return;
	}

	auto projectWindow = new QProjectWindow { nullptr, std::move(loadedProject) };
	projectWindow->show();

	close();
}

void QProjectSelectorWindow::onNewProjectClicked(bool)
{
	auto projectWindow = new QProjectWindow { nullptr, {} };
	projectWindow->show();

	close();
}

void QProjectSelectorUiDeleter::operator()(Ui::QProjectSelectorWindow* ptr) const
{
	delete ptr;
}
