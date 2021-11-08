#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QComboBox;
class QWidgetAction;

namespace ads {
class CDockManager;
class CDockAreaWidget;
class CDockWidget;
}

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *t_parent = nullptr);
    ~MainWindow();

protected:
    virtual void closeEvent(QCloseEvent* t_event) override;

private:
    Ui::MainWindow *ui;
    QAction* m_save_perspective_action;
    QWidgetAction* m_perspective_list_action;
    QComboBox* m_perspective_combo_box;

    ads::CDockManager* m_dock_manager;
    ads::CDockAreaWidget* m_status_dock_area;
    ads::CDockWidget* m_timeline_dock_widget;

    void createPerspectiveUi();

private slots:
    void savePerspective();
};

#endif // MAINWINDOW_H
