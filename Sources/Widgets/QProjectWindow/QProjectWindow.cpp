#include "QProjectWindow.h"
#include "ui_QProjectWindow.h"
#include "Widgets/QCameraViewer/QCameraViewer.h"

QProjectWindow::QProjectWindow(QWidget* parent, Project project) :
		QMainWindow(parent), ui(new Ui::QProjectWindow), project(std::move(project))
{
	ui->setupUi(this);

	fillData();

	connect(ui->listWidget, &QListWidget::itemClicked, this, &QProjectWindow::handleSourceClicked);
	connect(ui->addSourceTool, &QAction::triggered, this, &QProjectWindow::handleAddSourceTool);
	connect(ui->removeSourceTool, &QAction::triggered, this, &QProjectWindow::handleRemoveSourceTool);
}

void QProjectWindow::fillData()
{
	ui->tabWidget->clear();
	ui->listWidget->clear();

	std::ranges::for_each(project.rtspDataSources, [this](const auto& dataSource)
	{
		addSourceToList(dataSource);
	});
}

void QProjectWindow::handleAddSourceTool(bool)
{
	// TODO: show dialog to grab data
	RtspDataSource newSource {
		.name = "test name here"
	};

	project.rtspDataSources.push_back(newSource);
	addSourceToList(newSource);
}

void QProjectWindow::addSourceToList(const RtspDataSource& source)
{
	QListWidgetItem* newItem = new QListWidgetItem {};
	newItem->setText(source.name);

	QVariantMap varMap {};
	varMap["url"] = source.url;
	varMap["tab_id"] = -1;

	newItem->setData(Qt::UserRole, varMap);

	ui->listWidget->addItem(newItem);
}

void QProjectWindow::handleSourceClicked(QListWidgetItem* item)
{
	// TODO: add null and type checks
	QVariantMap varMap = item->data(Qt::UserRole).toMap();

	int tabId = varMap.value("tab_id").toInt();

	if(tabId == -1)
	{
		int newTabId = addStreamTab(item->text(), varMap.value("url").toByteArray());

		varMap["tab_id"] = newTabId;
		item->setData(Qt::UserRole, varMap);
	}
	else
	{
		focusStreamTab(tabId);
	}
}

int QProjectWindow::addStreamTab(QString name, QByteArray url)
{
	QCameraViewer* newCameraViewer = new QCameraViewer {};
	newCameraViewer->start(url);

	int tabIndex = ui->tabWidget->addTab(newCameraViewer, name);
	focusStreamTab(tabIndex);

	return tabIndex;
}

void QProjectWindow::focusStreamTab(int tabIndex)
{
	ui->tabWidget->setCurrentIndex(tabIndex);
}

void QProjectWindow::handleRemoveSourceTool(bool)
{
	QListWidgetItem* selected = ui->listWidget->currentItem();

	if(!selected)
		return;

	// TODO: add null and type checks
	QVariantMap varMap = selected->data(Qt::UserRole).toMap();
	int tabIndex = varMap["tab_id"].toInt();

	if(tabIndex != -1)
	{
		ui->tabWidget->removeTab(tabIndex);
	}

	ui->listWidget->removeItemWidget(selected);
}

void QProjectWindowUiDeleter::operator()(Ui::QProjectWindow* ptr) const
{
	delete ptr;
}
