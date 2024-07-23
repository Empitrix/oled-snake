enum PIN_MODE {
	INPUT_PINMODE,
	OUTPUT_PINMODE
};


void set_pin_mode(int pin, enum PIN_MODE mode){
	gpio_config_t io_conf;

	if(mode == OUTPUT_PINMODE){

		io_conf.intr_type = GPIO_INTR_DISABLE;          // disable interrupt
		io_conf.mode = GPIO_MODE_OUTPUT;                // set as output mode
		io_conf.pin_bit_mask = (1ULL<<pin);             // bit mask of the pins that you want to set
		io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;   // disable pull-down mode
		io_conf.pull_up_en = GPIO_PULLUP_DISABLE;       // disable pull-up mode

	} else {

		io_conf.intr_type = GPIO_INTR_HIGH_LEVEL;           // interrupt of rising edge
		io_conf.mode = GPIO_MODE_INPUT;                  // set as input mode
		io_conf.pin_bit_mask = (1ULL<<pin);              // bit mask of the pins, use GPIO4
		// io_conf.pull_up_en = GPIO_PULLUP_DISABLE;        // enable pull-up mode
		io_conf.pull_up_en = 0;
		io_conf.pull_down_en = 0;
		// io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;    // enable pull-up mode
	}

	// gpio_set_pull_mode(pin, GPIO_PULLDOWN_ONLY);
	// gpio_set_level(pin, 0);
	gpio_config(&io_conf);                             // configure GPIO with the given settings
}
