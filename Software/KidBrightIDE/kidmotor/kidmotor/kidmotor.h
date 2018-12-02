#ifndef __KIDMOTOR_H__
#define __KIDMOTOR_H__

/*
  DEV By IOXhop : www.ioxhop.com
  Sonthaya Nongnuch : fb.me/maxthai
*/

#include "driver.h"
#include "device.h"
#include "i2c-dev.h"
#include "driver/uart.h"
#include "kidbright32.h"

enum {
	CH_A = 0,
	CH_B = 1
};

class KidMotor : public Device {
	private:		
		enum {
			s_detect,
			s_write,
			s_wait,
			s_error
		} state;
		TickType_t tickcnt, polling_tickcnt;
		
		uint8_t dirSave[2] = { 0, 0 };
		uint8_t pwmSeve[2] = { 255, 255 };
		bool syncData[2] = { false, false };
		
		uint8_t updateCh = 255;
		uint8_t errWriteCount = 0;

	public:
		// constructor
		KidMotor(int bus_ch, int dev_addr) ;
		
		// override
		void init(void);
		void process(Driver *drv);
		int prop_count(void);
		bool prop_name(int index, char *name);
		bool prop_unit(int index, char *unit);
		bool prop_attr(int index, char *attr);
		bool prop_read(int index, char *value);
		bool prop_write(int index, char *value);
		
		// method
		void setSpeed(uint8_t ch, uint8_t pwm) ;
		void setMotor(uint8_t ch, uint8_t dir) ;
		
};

#endif
