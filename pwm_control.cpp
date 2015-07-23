#include "pwm_control.h"

PWM_Control::PWM_Control(QWidget *parent)
	: QWidget(parent)
{
	create_pwm_ctrl();
}

void PWM_Control::create_pwm_ctrl()
{
	grid = new QGridLayout(this);

	psc = new QVExtSlider("psc", this);
	period = new QVExtSlider("period", this);
	duty = new QVExtSlider("duty", this);
	dzone = new QVExtSlider("dzone", this);
	fdiv = new QComboBox(this);
	ngen = new QCheckBox("neg.en", this);
	start = new QPushButton("start", this);

	tick_time = new QLabel("0", this);
	tick_freq = new QLabel("0", this);
	pwm_period = new QLabel("0", this);
	pwm_freq = new QLabel("0", this);
	pwm_pulse_time = new QLabel("0", this);
	pwm_dzone_time = new QLabel("0", this);

	QStringList list;
	list << "/1" << "/2" << "/4";
	fdiv->addItems(list);

	psc->setRange(0, 65535);
	period->setRange(0, 65535);
	dzone->setRange(0, 255);

	grid->addWidget(psc, 0, 0, 9, 1);
	grid->addWidget(period, 0, 1, 9, 1);
	grid->addWidget(duty, 0, 2, 9, 1);
	grid->addWidget(dzone, 0, 3, 9, 1);
	grid->addWidget(addSeparator(QFrame::VLine, this), 0, 4, 9, 1);

	grid->addWidget(new QLabel("Fck", this), 0, 5);
	grid->addWidget(fdiv, 0, 6);
	grid->addWidget(ngen, 1, 5);
	grid->addWidget(start, 2, 5);

	grid->addWidget(new QLabel("F(tim)   = "), 3, 5);
	grid->addWidget(new QLabel("T(tim)   = "), 4, 5);
	grid->addWidget(new QLabel("F(PWM)   = "), 5, 5);
	grid->addWidget(new QLabel("T(PWM)   = "), 6, 5);
	grid->addWidget(new QLabel("T(pulse) = "), 7, 5);
	grid->addWidget(new QLabel("T(dzone) = "), 8, 5);

	grid->addWidget(tick_time, 3, 6);
	grid->addWidget(tick_freq, 4, 6);
	grid->addWidget(pwm_period, 5, 6);
	grid->addWidget(pwm_freq, 6, 6);
	grid->addWidget(pwm_pulse_time, 7, 6);
	grid->addWidget(pwm_dzone_time, 8, 6);

	grid->setRowStretch(0, 1);

}

QFrame *PWM_Control::addSeparator(QFrame::Shape shape, QWidget *parent)
{
	QFrame *f = new QFrame(parent);
	f->setFrameShadow(QFrame::Sunken);
	f->setLineWidth(1);
	f->setMidLineWidth(0);
	f->setFrameShape(shape);
	return f;
}