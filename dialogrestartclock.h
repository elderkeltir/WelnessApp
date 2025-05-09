#ifndef DIALOGRESTARTCLOCK_H
#define DIALOGRESTARTCLOCK_H

#include <QDialog>

namespace Ui {
class DialogRestartClock;
}

class DialogRestartClock : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRestartClock(QWidget *parent = nullptr);
    ~DialogRestartClock();

private:
    Ui::DialogRestartClock *ui;
};

#endif // DIALOGRESTARTCLOCK_H
