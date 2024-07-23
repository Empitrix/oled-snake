#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <ssd1306/ssd1306.h>
#include <driver/i2c.h>
#include <esp_err.h>

#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#define SCL_PIN 5
#define SDA_PIN 4

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64


struct DISPLAY {
	ssd1306_t *dev;
};

ssd1306_t init_display(int scl, int sda, int width, int height){

	int i2c_master_port = I2C_NUM_0;

	i2c_config_t conf;
	conf.mode = I2C_MODE_MASTER;

	conf.sda_io_num = GPIO_NUM_4;
	conf.scl_io_num = GPIO_NUM_5;

	conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
	conf.scl_pullup_en = GPIO_PULLUP_ENABLE;

	conf.clk_stretch_tick = 300;

	ESP_ERROR_CHECK(i2c_driver_install(i2c_master_port, conf.mode));
	ESP_ERROR_CHECK(i2c_param_config(i2c_master_port, &conf));

	// init ssd1306
	ssd1306_t dev = {
		.i2c_port = I2C_NUM_0,
		.i2c_addr = SSD1306_I2C_ADDR_0,
		.screen = SSD1306_SCREEN,
		.width = DISPLAY_WIDTH,
		.height = DISPLAY_HEIGHT
	};


	ssd1306_init(&dev);
	ssd1306_set_whole_display_lighting(&dev, false);

	return dev;
}



void draw_box(const ssd1306_t *dev, uint8_t *fb, int size, int x, int y){
	for(int i = 0; i < size; ++i)
		for(int j = 0; j < size; ++j)
			ssd1306_draw_pixel(dev, fb, x + i, y + j, OLED_COLOR_WHITE);
}
