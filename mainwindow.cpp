#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QAction>
#include <QApplication>
#include <QDebug>
#include "dialogrestartclock.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create the system tray icon and its context menu.
    trayIcon = new QSystemTrayIcon(this);
    // Provide a valid icon (ensure the resource exists or use a file path).
    trayIcon->setIcon(QIcon(":/icons/app_icon.png"));

    // Create the context menu.
    trayMenu = new QMenu(this);
    QAction *restartAction = new QAction("Restart Clock", this);
    QAction *stopAction = new QAction("Stop", this);
    QAction *quitAction = new QAction("Quit", this);

    trayMenu->addAction(restartAction);
    trayMenu->addAction(stopAction);
    trayMenu->addSeparator();
    trayMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();

    // Connect the menu actions.
    connect(restartAction, &QAction::triggered, this, &MainWindow::restartClock);
    connect(stopAction, &QAction::triggered, this, &MainWindow::stopClock);
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);

    // Connect tray icon activation (e.g. left-click) to show/hide window.
    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::trayIconActivated);

    // Create a QTimer for the countdown (1 hour = 3600000 ms).
    timer = new QTimer(this);
    timer->setInterval(3600000); // For testing, you can change this to something shorter (e.g., 10000 for 10 sec).
    // timer->setInterval(10000);
    timer->start();
    connect(timer, &QTimer::timeout, this, &MainWindow::showNotification);

    // Icon tray popup
    tooltipTimer = new QTimer(this);
    tooltipTimer->setInterval(1000);                    // update every second
    connect(tooltipTimer, &QTimer::timeout,
            this,        &MainWindow::updateTrayTooltip);
    tooltipTimer->start();

    // immediately set the first tooltip
    updateTrayTooltip();

    // Optionally, hide the main window immediately if you want the app to remain in tray.
    this->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::updateTrayTooltip()
{
    int msecsLeft = timer->remainingTime();
    if (msecsLeft < 0)
        msecsLeft = 0;              // timer inactive or just fired

    // convert to seconds (and minutes if you like):
    int secsLeft = msecsLeft / 1000;
    int mins  = secsLeft / 60;
    int secs  = secsLeft % 60;

    trayIcon->setToolTip(
        tr("Next break in %1:%2")
            .arg(mins)
            .arg(secs, 2, 10, QChar('0'))
        );
}

void MainWindow::restartClock()
{
    // Stop and restart the timer.
    timer->stop();
    timer->start();
    qDebug() << "Clock restarted.";
}

void MainWindow::stopClock()
{
    timer->stop();
    qDebug() << "Clock stopped.";
}

void MainWindow::showNotification()
{
    DialogRestartClock dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
         restartClock();  // Restart the timer if the user clicks Yes.
    }
    else {
        timer->stop();
        qDebug() << "Timer not restarted after notification.";
    }
}



void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger) {
        // For left-click, toggle main window visibility.
        if (this->isVisible())
            this->hide();
        else
            this->show();
    }
}
