#include <QApplication>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QIcon>
#include <QMenu>
#include <QAction>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        qWarning("System tray is not available!");
        return 1;
    }

    QSystemTrayIcon trayIcon;
    trayIcon.setIcon(QIcon::fromTheme("dialog-information")); // or use your own icon
    trayIcon.setVisible(true);

    QTimer notificationTimer;
    QTimer tooltipTimer;

    auto showReminder = [&trayIcon]() {
        trayIcon.showMessage(
            "Нагадування",
            "Встань, походи, випий води і присядь 10 разів!",
            QSystemTrayIcon::Information,
            10000 // show for 10 seconds
            );
    };

    // Start the timer to fire every 60 minutes (60 * 60 * 1000 ms)
    notificationTimer.start(60 * 60 * 1000);
    QObject::connect(&notificationTimer, &QTimer::timeout, &app, showReminder);

    tooltipTimer.start(1000); // Update every second
    QObject::connect(&tooltipTimer, &QTimer::timeout, &app, [&]() {
        int remainingMs = notificationTimer.remainingTime();
        if (remainingMs > 0) {
            int minutes = remainingMs / 60000;
            int seconds = (remainingMs % 60000) / 1000;
            trayIcon.setToolTip(QString("Наступне повідомлення через %1:%2")
                                    .arg(minutes, 2, 10, QLatin1Char('0'))
                                    .arg(seconds, 2, 10, QLatin1Char('0')));
        } else {
            trayIcon.setToolTip("Очікування...");
        }
    });


    // Show immediately on startup
    showReminder();

    // === Tray Icon Menu ===
    QMenu menu;

    QAction stopAction("Stop");
    QAction restartAction("Restart");
    QAction quitAction("Quit");

    QObject::connect(&stopAction, &QAction::triggered, &app, [&]() {
        notificationTimer.stop();
    });

    QObject::connect(&restartAction, &QAction::triggered, &app, [&]() {
        if (!notificationTimer.isActive()) {
            notificationTimer.start(60 * 60 * 1000);
            showReminder();
        }
    });

    QObject::connect(&quitAction, &QAction::triggered, &app, &QApplication::quit);

    menu.addAction(&stopAction);
    menu.addAction(&restartAction);
    menu.addSeparator();
    menu.addAction(&quitAction);

    trayIcon.setContextMenu(&menu);

    return app.exec();
}
