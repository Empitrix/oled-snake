#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <driver/gpio.h>
#include <ssd1306/ssd1306.h>

#include "lib/gpio.h"
#include "lib/display.h"
#include "lib/utils.h"
#include "lib/game.h"
#include "lib/rules.h"


// main block array
static struct BLOCK blocks[MAX_BLOCK] = {
	{DISPLAY_WIDTH / 2 + 1, DISPLAY_HEIGHT / 2 + 3, HEAD_B},
	{DISPLAY_WIDTH / 2 + 1, DISPLAY_HEIGHT / 2 + 3, BODY_B},
	{DISPLAY_WIDTH / 2 + 1, DISPLAY_HEIGHT / 2 + 3, BODY_B},
	{50, 50, POINT_B},
};

static struct BLOCK tmpb[MAX_BLOCK];  // temporary block array (dummy)


void app_main(void){

	// set the following GPIOs to INPUT-MODE
	set_pin_mode(GPIO_RIGHT, INPUT_PINMODE);
	set_pin_mode(GPIO_DOWN , INPUT_PINMODE);
	set_pin_mode(GPIO_UP   , INPUT_PINMODE);
	set_pin_mode(GPIO_LEFT , INPUT_PINMODE);

	// initialize OLED display and font
	const font_info_t *font = font_builtin_fonts[FONT_FACE_GLCD5x7];
	ssd1306_t dev = init_display(SCL_PIN, SDA_PIN, DISPLAY_WIDTH, DISPLAY_HEIGHT);


	// initialize display's buffer
	uint8_t fb[BUFFER_SIZE];
	memset(fb, 0, BUFFER_SIZE);


	// game rules initialization
	struct GMAE_RULES rules;
	rules.ix = 1;       // move to right
	rules.iy = 0;       // UP/DOWN (Y) direction
	rules.gpoints = 0;  // 0 points
	rules.running = 1;  // game is running ( not over yet )
	rules.gstep = 5;    // game steps
	rules.ln = 4;       // current number of blocks (HEAD, BODY, POINT)


	while(1){
		memset(fb, 0, BUFFER_SIZE);  // clear

		// get movements (update directions)
		update_idx(&rules.ix, &rules.iy, (int[4]){GPIO_RIGHT, GPIO_LEFT, GPIO_UP, GPIO_DOWN});

		/* UPDATE MOVEMENTs */
		blockcpy(tmpb, blocks, rules.ln);
		tmpb[0].x = blocks[0].x + (rules.ix * rules.gstep);  // head x
		tmpb[0].y = blocks[0].y + (rules.iy * rules.gstep);  // head y

		if(tmpb[0].x > DISPLAY_WIDTH || tmpb[0].x < 0 || tmpb[0].y > DISPLAY_HEIGHT || tmpb[0].y < 0){
			memset(fb, 0, BUFFER_SIZE);
			rules.running = 0;
		}

		// detect if the game is over
		if(rules.running == 0){
			game_over_fn(dev, font, fb, blocks, &rules);
			continue;
		}

		// Shift all of the block bodies to their previous positions
		int idx;
		for(idx = 1; idx < rules.ln; ++idx){
			if(tmpb[idx].type == BODY_B){
				tmpb[idx].x = blocks[idx - 1].x;
				tmpb[idx].y = blocks[idx - 1].y;
			}
		}
		blockcpy(blocks, tmpb, rules.ln);  // copy temporary block array to main one


		// snake hit the point (NEW POINT)
		if(blocks[0].x == blocks[idx - 1].x && blocks[0].y == blocks[idx - 1].y){
			rules.gpoints++;  // add 1 to points

			struct BLOCK np = get_point(blocks, DISPLAY_WIDTH, DISPLAY_HEIGHT, rules.gstep, rules.ln);

			blocks[idx - 1] = blocks[idx - 2];  // add one more block to body
			blocks[idx] = np;  // add the point block
			rules.ln++;
		} else {
			// the snake crossed itself
			if(any_crossed(blocks, blocks[0], rules.ln, 1) != 0){
				memset(fb, 0, BUFFER_SIZE);
				rules.running = 0;
			}
		}


		// draw blocks
		for(int i = 0; i < rules.ln; i++){
			if(blocks[i].type == POINT_B)
				ssd1306_draw_circle(&dev, fb, blocks[i].x + 2, blocks[i].y + 2, 2, OLED_COLOR_INVERT);  // draw point (r:2)
			else
				draw_box(&dev, fb, rules.gstep, blocks[i].x, blocks[i].y);  // draw the snake
		}

		ssd1306_load_frame_buffer(&dev, fb);  // load buffer into the display

		int d = 200 - rules.gpoints * 4;
		dlay((d  > 10) ? d : 10);

	}

}

