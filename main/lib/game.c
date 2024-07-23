#include <ssd1306/ssd1306.h>
#include "./utils.h"
#include "rules.h"
#include <math.h>


/* randint: return a random number from given range */
int randint(int start, int end){
	return (rand() % (end - start + 1)) + start;
}


int any_crossed(struct BLOCK blocks[], struct BLOCK block, int ln, int start){
	for(int i = start; i < ln; ++i){
		if(block.x == blocks[i].x && block.y == blocks[i].y)
			return 1;
	}
	return 0;
}


struct BLOCK get_point_block(struct BLOCK blocks[], int dw, int dh, int step){

	int nw = randint(step, (dw - 3) - step);
	int nh = randint(step, (dh - 4) - step);

	nw += step - (nw % (step));
	nh += step - (nh % (step));

	return (struct BLOCK){nw, nh, POINT_B};
}


struct BLOCK get_point(struct BLOCK blocks[], int dw, int dh, int step, int ln){
	struct BLOCK pb;
	do {
		pb = get_point_block(blocks, dw, dh, step);
	} while(any_crossed(blocks, pb, ln, 0));
	return pb;
}



void game_over_fn(ssd1306_t dev, const font_info_t *font, uint8_t *fb, struct BLOCK blocks[MAX_BLOCK], struct GMAE_RULES *rules){

	char msgp[100];

	sprintf(msgp, "GAME OVER   %d", rules->gpoints);
	ssd1306_draw_string(&dev, fb, font, 10, 10, msgp, OLED_COLOR_WHITE, OLED_COLOR_BLACK);
	ssd1306_draw_string(&dev, fb, font, 0, 35, "PRESS <DOWN> TO PLAY", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
	ssd1306_draw_string(&dev, fb, font, 0, 45, "AGAIN.", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
	ssd1306_load_frame_buffer(&dev, fb);

	for(;;){

		// pressed down button
		if(gpio_get_level(GPIO_DOWN)){
			memset(blocks, 0, MAX_BLOCK);

			// update snake's body
			blocks[0] = (struct BLOCK){DISPLAY_WIDTH / 2 + 1, DISPLAY_HEIGHT / 2 + 3, HEAD_B};
			blocks[1] = (struct BLOCK){DISPLAY_WIDTH / 2 + 1, DISPLAY_HEIGHT / 2 + 3, BODY_B};
			blocks[2] = (struct BLOCK){DISPLAY_WIDTH / 2 + 1, DISPLAY_HEIGHT / 2 + 3, BODY_B};

			// get a random point block
			blocks[3] = get_point(blocks, DISPLAY_WIDTH, DISPLAY_HEIGHT, rules->gstep, rules->ln);

			rules->ix = 1;  // move to right
			rules->iy = 0;
			rules->ln = 4;
			rules->gpoints = 0;
			rules->running = 1;

			// count down
			for(int i = 3; i > 0; --i){
				memset(fb, 0, BUFFER_SIZE);

				char msg[100];
				sprintf(msg, "%d", i);

				// show text in the middle of the display
				ssd1306_draw_string(&dev, fb, font,
					(DISPLAY_WIDTH / 2) - ((int)strlen(msg) * 5),
					DISPLAY_HEIGHT / 2,
					msg,
					OLED_COLOR_WHITE,
					OLED_COLOR_BLACK);

				// update buffer
				ssd1306_load_frame_buffer(&dev, fb);
				dlay(1000);  // wait for 1 second
			}

			break;
		}

		dlay(200);
	}
}
