/*!
 ********************************************************************
 * file
 * author	kaljan
 * version	0.0.0
 * date
 * brief
 *
 ********************************************************************
 */
#include "window.h"

Window::Window(QWidget *parent)
	: QWidget(parent)
	, start_state(false)
	, grid(new QGridLayout(this))
	, led_box(new QCheckBox("LED", this))
	, spi_start_btn(new QPushButton("start", this))
	, spi_data_label(new QLabel(this))
	, ADC1_control(new AnalogControl(this))
	, ADC2_control(new AnalogControl(this))
	, single_transfer_btn(new QPushButton("send", this))
{
	setFont(QFont("Monospace", 10, -1, false));
	setLayout(grid);

	ADC1_control->setRange(0, 5000.000);
	ADC1_control->setTitle("ADC1");

	ADC2_control->setRange(0, 5000.000);
	ADC2_control->setTitle("ADC2");

	led_box->setDisabled(true);

	spi_start_btn->setDisabled(true);
	single_transfer_btn->setDisabled(true);

	grid->addWidget(ADC1_control, 0, 0);
	grid->addWidget(ADC2_control, 0, 1);

	grid->addWidget(led_box, 1, 0);
	grid->addWidget(spi_start_btn, 1, 1);
	grid->addWidget(single_transfer_btn, 2, 0);
	grid->addWidget(spi_data_label, 3, 0, 1, 2);


	adjustSize();
	setFixedSize(this->size());

	spi_device = new SPI_Thread();

	if (spi_device->getState() == false) return;

//	spi_start_btn->setDisabled(false);
	led_box->setDisabled(false);
	single_transfer_btn->setDisabled(false);

	connect(led_box, SIGNAL(toggled(bool)),
		this, SLOT(toggle_led(bool)));

//	connect(spi_device, SIGNAL(SPI_Tread_DataRDY(qreal,qreal)),
//		this, SLOT(spi_device_value_rdy(qreal,qreal)));

//	connect(spi_start_btn, SIGNAL(clicked()),
//		this, SLOT(spi_start_btn_clicked()));

//	connect(spi_device, SIGNAL(spi_thread_stopped()),
//		this, SLOT(spi_dev_stopped()));

	connect(single_transfer_btn, SIGNAL(clicked()),
		this, SLOT(single_transfer_btn_clicked()));

}

Window::~Window()
{
	delete spi_device;
}


/*!
 ********************************************************************
 * brief
 *
 ********************************************************************
 */
void Window::toggle_led(bool t)
{
	if (t == false) {
		bcm2835_GPIO->GPCLR0 = GPIO_GPSET0_GP26;
	} else {
		bcm2835_GPIO->GPSET0 = GPIO_GPCLR0_GP26;
	}
}

/*!
 ********************************************************************
 * brief
 *
 ********************************************************************
 */
void Window::spi_start_btn_clicked()
{
	if (start_state == false) {
		spi_start_btn->setText("stop");
		start_state = true;
		spi_device->spi_thread_start();
	} else {
		spi_start_btn->setText("start");
		start_state = false;
		spi_device->spi_stop_thread();
	}

}

/*!
 ********************************************************************
 * brief
 *
 ********************************************************************
 */
void Window::spi_device_value_rdy(qreal v1, qreal v2)
{
	ADC1_control->setrot(v1);
	ADC2_control->setrot(v2);
}

/*!
 ********************************************************************
 * brief
 *
 ********************************************************************
 */
void Window::spi_dev_stopped()
{
	spi_start_btn->setText("start");
	start_state = false;
}

char spi_out_data[4] = {0xAA, 0xF0, 0xF1, 0xF2};
unsigned short ADC_values[2];

void Window::single_transfer_btn_clicked()
{
	GPIO_MARK1_SET
	spi0_unidir_poll_block_transfer(
		(const char *)(&spi_out_data[0]),
		(char *)(&ADC_values[0]), 4
		);
	GPIO_MARK1_CLR

	ADC1_control->setrot((qreal)ADC_values[0]);
	ADC2_control->setrot((qreal)ADC_values[1]);


}
