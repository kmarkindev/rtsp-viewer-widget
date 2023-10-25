#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
	class QProjectSelectorWindow;
}
QT_END_NAMESPACE

struct QProjectSelectorUiDeleter
{
	void operator()(Ui::QProjectSelectorWindow* ptr) const;
};

class QProjectSelectorWindow : public QWidget
{

	Q_OBJECT

public:

	explicit QProjectSelectorWindow(QWidget* parent = nullptr);
	~QProjectSelectorWindow() override = default;

private:

	std::unique_ptr<Ui::QProjectSelectorWindow, QProjectSelectorUiDeleter> ui {};

	void onOpenProjectClicked(bool);

	void onNewProjectClicked(bool);

};
