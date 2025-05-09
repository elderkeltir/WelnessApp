#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QMenu>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showNotification();
    void restartClock();
    void stopClock();
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    void updateTrayTooltip();
    Ui::MainWindow *ui;
    QTimer *timer;                // 1hr countdown timer
    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    QTimer *tooltipTimer;
};

#endif // MAINWINDOW_H
