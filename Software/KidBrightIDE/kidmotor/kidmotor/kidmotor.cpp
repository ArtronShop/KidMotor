#ifndef __KIDMOTOR_CPP__
#define __KIDMOTOR_CPP__

#include "kidmotor.h"

/*
  DEV By IOXhop : www.ioxhop.com
  Sonthaya Nongnuch : fb.me/maxthai
*/

KidMotor::KidMotor(int bus_ch, int dev_addr) {
	channel = 0;
	address = dev_addr;
	polling_ms = 100;
}

void KidMotor::init(void) {
	// clear initialized flag
	this->initialized = false;
	
	// Start initialized
	this->state = s_detect;
	
	// Reset speed of I2C to 100kHz
	i2c_config_t conf;

    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = CHAIN_SDA_GPIO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = CHAIN_SCL_GPIO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 100E3; // Set speed to 100kHz

	i2c_param_config(I2C_NUM_1, &conf);
}

int KidMotor::prop_count(void) {
	// not supported
	return 0;
}

bool KidMotor::prop_name(int index, char *name) {
	// not supported
	return false;
}

bool KidMotor::prop_unit(int index, char *unit) {
	// not supported
	return false;
}

bool KidMotor::prop_attr(int index, char *attr) {
	// not supported
	return false;
}

bool KidMotor::prop_read(int index, char *value) {
	// not supported
	return false;
}

bool KidMotor::prop_write(int index, char *value) {
	// not supported
	return false;
}
// --------------------------------------

// Start here
void KidMotor::process(Driver *drv) {
	I2CDev *i2c = (I2CDev *)drv;
	
	switch (this->state) {
		case s_detect:
			// detect i2c device
			if (i2c->detect(this->channel, this->address) == ESP_OK) {
				// clear error flag
				this->error = false;
				// set initialized flag
				this->initialized = true;
				
				// Reset module
				this->syncData[CH_A] = true;
				this->syncData[CH_B] = true;
				this->state = s_write;
			} else {
				this->state = s_error;
			}
			break;

		case s_write:
			this->updateCh = 255;
			if (this->syncData[CH_A]) {
				this->updateCh = CH_A;
			} else if (this->syncData[CH_B]) {
				this->updateCh = CH_B;
			}
			if (this->updateCh != 255) {
				uint8_t data[] = { 
					(uint8_t)((this->dirSave[updateCh]<<4)|updateCh),
					this->pwmSeve[updateCh]
				};
				if (i2c->write(this->channel, this->address, data, sizeof data) == ESP_OK) {
					this->errWriteCount = 0;
					this->syncData[updateCh] = false;
				} else {
					this->errWriteCount++;
					if (this->errWriteCount > 10) {
						this->errWriteCount = 0;
						this->state = s_error;
					}
				}
			}
			break;
		
		case s_wait:
			if (this->error) {
				// wait polling_ms timeout
				if (is_tickcnt_elapsed(this->tickcnt, this->polling_ms)) {
					this->state = s_detect;
				}
			}
			break;

		case s_error:
			// set error flag
			this->error = true;
			// clear initialized flag
			this->initialized = false;
			// get current tickcnt
			this->tickcnt = get_tickcnt();
			// goto wait and retry with detect state
			this->state = s_wait;
			break;

	}
}

// Method
void KidMotor::setMotor(uint8_t ch, uint8_t dir) {
  this->dirSave[ch] = dir;
  this->syncData[ch] = true;
}

void KidMotor::setSpeed(uint8_t ch, uint8_t pwm) {
  this->pwmSeve[ch] = pwm;
  this->syncData[ch] = true;
}

#endif
