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
#ifndef SPI_THREAD_H
#define SPI_THREAD_H

#include <QThread>

#include <stdio.h>

#include "bcm2835/bcm2835.h"
#include "bcm2835/gpio.h"
#include "bcm2835/spi0.h"

/*!
 ********************************************************************
 * brief
 *
 ********************************************************************
 */
class SPI_Thread : public QThread
{
	Q_OBJECT
public:
	explicit SPI_Thread();

	~SPI_Thread();

	void run();

	int SPI_Thread_Init(void);

	void SPI_Thread_DeInit(void);

	bool getState(void) {return spi_state;}

	void spi_stop_thread(void) {
		thread_state = false;
	}

	void spi_thread_start(void) {
		thread_state = true;
		this->start();
	}

	bool get_thread_state(void) {return thread_state;}

signals:
	void SPI_Tread_DataRDY(qreal v1, qreal v2);
	void spi_thread_stopped(void);

private:
	bool spi_state;
	bool thread_state;

	unsigned short ADC_data[2];
	char out_data[4];

	int reset_spi_device(void);
};

#endif // SPI_THREAD_H