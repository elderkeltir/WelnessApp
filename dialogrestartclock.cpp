#include "dialogrestartclock.h"
#include "ui_dialogrestartclock.h"

DialogRestartClock::DialogRestartClock(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogRestartClock)
{
    ui->setupUi(this);

    ui->buttonBox->setEnabled(false);
    connect(ui->agreeCheckBox, &QCheckBox::toggled,
            this, [this](bool checked) {
        ui->buttonBox->setEnabled(checked);
    });

    adjustSize();               // make it snug to its contents
    setFixedSize(size());       // now forbid any resize
}

DialogRestartClock::~DialogRestartClock()
{
    delete ui;
}
