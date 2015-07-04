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
//	, ADC1_dial(new ADC_Dial("ADC1", this))
//	, ADC2_dial(new ADC_Dial("ADC2", this))
	, led_box(new QCheckBox("LED", this))
	, spi_start_btn(new QPushButton("start", this))
	, spi_data_label(new QLabel(this))
	, ADC1_control(new AnalogControl(this))
	, ADC2_control(new AnalogControl(this))
//	, slider(new QSlider(Qt::Vertical, this))
{
	setFont(QFont("Monospace", 10, -1, false));
	setLayout(grid);

//	slider->setRange(0, 4096.000);
	ADC1_control->setRange(0, 5000.000);
	ADC1_control->setTitle("ADC1");

	ADC2_control->setRange(0, 5000.000);
	ADC2_control->setTitle("ADC2");

	led_box->setDisabled(true);

	spi_start_btn->setDisabled(true);

//	ADC1_dial->setRange(0, 4096);
//	ADC2_dial->setRange(0, 4096);

//	grid->addWidget(ADC1_dial, 1, 0);
//	grid->addWidget(ADC2_dial, 1, 1);

	grid->addWidget(ADC1_control, 0, 0);
	grid->addWidget(ADC2_control, 0, 1);

	grid->addWidget(led_box, 1, 0);
	grid->addWidget(spi_start_btn, 1, 1);
	grid->addWidget(spi_data_label, 2, 0, 1, 2);
//	grid->addWidget(slider, 0, 2, 2, 1);

	adjustSize();
	setFixedSize(this->size());

//	connect(slider, SIGNAL(valueChanged(int)),
//		this, SLOT(sl_changed(int)));

	spi_device = new SPI_Thread();

	if (spi_device->getState() == false) return;

//	if (gpio_init() == -1) return;

//	if (spi0_unidir_poll_init(250,
//		SPI0_CHPA_BEGINN | SPI0_CPOL_HIGH) == -1) return;

	spi_start_btn->setDisabled(false);
	led_box->setDisabled(false);

	connect(led_box, SIGNAL(toggled(bool)),
		this, SLOT(toggle_led(bool)));

	connect(spi_device, SIGNAL(SPI_Tread_DataRDY(qreal,qreal)),
		this, SLOT(spi_device_value_rdy(qreal,qreal)));

	connect(spi_start_btn, SIGNAL(clicked()),
		this, SLOT(spi_start_btn_clicked()));

	connect(spi_device, SIGNAL(spi_thread_stopped()),
		this, SLOT(spi_dev_stopped()));

}

Window::~Window()
{
	delete spi_device;
//	gpio_deinit();
//	spi0_unidir_poll_deinit();
}

//void Window::sl_changed(int value)
//{
//	ADC1_control->setrot((qreal)value);
//	ADC2_control->setrot((qreal)value);
//}

//#define SINGLE_TRANSFER
//char in_data[4];
//char out_data[4] = {0xAA, 0xF0, 0xF1, 0xF2};
//unsigned short ADC_data[2];

/*!
 ********************************************************************
 * brief
 *
 ********************************************************************
 */
//void Window::spi_send_btn_clicked()
//{
//	printf("Send one byte to SPI0\n");

//#ifdef SINGLE_TRANSFER
//	unsigned char data = spi0_unidir_poll_transfer(0xAA);

//	spi_data_label->setText(QString("SPI Data In: 0x%1")
//		.arg(data, 2, 16, QLatin1Char('0')));
//#else
//	int error_code = spi0_unidir_poll_block_transfer(out_data, (char *)&ADC_data[0], 4);
//	if (error_code != 0) {
//		printf("SPI Data Transfer Error: %d\n"
//			"Device not response\n", error_code);
//		return;
//	}
//	QString str;
//	str.append("Rx:");

//	/*for (int i = 0; i < 2; i++) {
//		str.append(QString(" 0x%1")
//			.arg(ADC_data[i], 2, 16, QLatin1Char('0')));
//	}

//	spi_data_label->setText(str);
//	*/
//	ADC1_control->setrot((qreal)ADC_data[0]);
//	ADC2_control->setrot((qreal)ADC_data[1]);

//#endif
//}

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

void Window::spi_dev_stopped()
{
	spi_start_btn->setText("start");
	start_state = false;
}


