#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QComboBox>
#include <QWidgetAction>
#include <QPlainTextEdit>
#include <QTableWidget>
#include <QInputDialog>

#include <DockManager.h>
#include <DockAreaWidget.h>
#include <DockWidget.h>

MainWindow::MainWindow(QWidget *t_parent) :
    QMainWindow(t_parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ads::CDockManager::setConfigFlag(ads::CDockManager::OpaqueSplitterResize, true);
    ads::CDockManager::setConfigFlag(ads::CDockManager::XmlCompressionEnabled, false);
    ads::CDockManager::setConfigFlag(ads::CDockManager::FocusHighlighting, true);
    m_dock_manager = new ads::CDockManager(this);

    // Set central widget
    QPlainTextEdit* plain_text_widget = new QPlainTextEdit();
    plain_text_widget->setPlaceholderText("This is the central editor. Enter your text here.");
    ads::CDockWidget* central_dock_widget = new ads::CDockWidget("CentralWidget");
    central_dock_widget->setWidget(plain_text_widget);
    auto* central_dock_area = m_dock_manager->setCentralWidget(central_dock_widget);
    central_dock_area->setAllowedAreas(ads::DockWidgetArea::OuterDockAreas);

    // create other dock widgets
    QTableWidget* table = new QTableWidget();
    table->setColumnCount(3);
    table->setRowCount(10);
    ads::CDockWidget* table_dock_widget = new ads::CDockWidget("Table 1");
    table_dock_widget->setWidget(table);
    table_dock_widget->setMinimumSizeHintMode(ads::CDockWidget::MinimumSizeHintFromDockWidget);
    table_dock_widget->resize(250, 150);
    table_dock_widget->setMinimumSize(200,150);
    auto table_area = m_dock_manager->addDockWidget(ads::DockWidgetArea::LeftDockWidgetArea, table_dock_widget);
    ui->menuView->addAction(table_dock_widget->toggleViewAction());

    table = new QTableWidget();
    table->setColumnCount(5);
    table->setRowCount(1020);
    table_dock_widget = new ads::CDockWidget("Table 2");
    table_dock_widget->setWidget(table);
    table_dock_widget->setMinimumSizeHintMode(ads::CDockWidget::MinimumSizeHintFromDockWidget);
    table_dock_widget->resize(250, 150);
    table_dock_widget->setMinimumSize(200,150);
    m_dock_manager->addDockWidget(ads::DockWidgetArea::BottomDockWidgetArea, table_dock_widget, table_area);
    ui->menuView->addAction(table_dock_widget->toggleViewAction());

    QTableWidget* properties_table = new QTableWidget();
    properties_table->setColumnCount(3);
    properties_table->setRowCount(10);
    ads::CDockWidget* properties_dock_widget = new ads::CDockWidget("Properties");
    properties_dock_widget->setWidget(properties_table);
    properties_dock_widget->setMinimumSizeHintMode(ads::CDockWidget::MinimumSizeHintFromDockWidget);
    properties_dock_widget->resize(250, 150);
    properties_dock_widget->setMinimumSize(200, 150);
    m_dock_manager->addDockWidget(ads::DockWidgetArea::RightDockWidgetArea, properties_dock_widget, central_dock_area);
    ui->menuView->addAction(properties_dock_widget->toggleViewAction());
    createPerspectiveUi();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createPerspectiveUi()
{
    m_save_perspective_action = new QAction("Create Perspective", this);
    connect(m_save_perspective_action, SIGNAL(triggered()), SLOT(savePerspective()));
    m_perspective_list_action = new QWidgetAction(this);
    m_perspective_combo_box = new QComboBox(this);
    m_perspective_combo_box->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    m_perspective_combo_box->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    connect(m_perspective_combo_box, SIGNAL(activated(const QString&)),
        m_dock_manager, SLOT(openPerspective(const QString&)));
    m_perspective_list_action->setDefaultWidget(m_perspective_combo_box);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(m_perspective_list_action);
    ui->toolBar->addAction(m_save_perspective_action);
}

void MainWindow::savePerspective()
{
    QString perspective_name = QInputDialog::getText(this, "Save Perspective", "Enter unique name:");
    if (perspective_name.isEmpty())
        return;
    m_dock_manager->addPerspective(perspective_name);
    QSignalBlocker blocker(m_perspective_combo_box);
    m_perspective_combo_box->clear();
    m_perspective_combo_box->addItems(m_dock_manager->perspectiveNames());
    m_perspective_combo_box->setCurrentText(perspective_name);
}

void MainWindow::closeEvent(QCloseEvent* t_event)
{
    m_dock_manager->deleteLater();
    QMainWindow::closeEvent(t_event);
}
