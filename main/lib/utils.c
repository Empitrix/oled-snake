#include "rules.h"

/* Game RULES */
struct GMAE_RULES {
	int ix, iy;   // X, Y directions (-1, 0, 1)
	int ln;       // snake length
	int gpoints;  // number of points
	int running;  // game running (game over)
	int gstep;    // game steps (SCALE)
};


// block types (HEAD, BODY, POINT)
enum BLOCK_TYPE {
	BODY_B,
	HEAD_B,
	POINT_B
};

struct BLOCK {
	int x, y;              // coordinates
	enum BLOCK_TYPE type;  // type
};


/* copy all of the blocks in 't' to 's' (S <- T) */
void blockcpy(struct BLOCK dst[], struct BLOCK src[], int ln){
	memset(dst, 0, MAX_BLOCK);
	int i;
	for(i = 0; i < ln; ++i){
		dst[i] = src[i];
	}
}


/* update_idx: update game movement & direction */
void update_idx(int *ix, int *iy, int pins[4]){

	// RIGHT
	if(gpio_get_level(pins[0]) && *ix != -1){
		*ix = 1;
		*iy = 0;
		return;
	}

	// DOWN
	if(gpio_get_level(pins[3]) && *iy != -1){
		*iy = 1;
		*ix = 0;
		return;
	}

	// LEFT
	if(gpio_get_level(pins[1]) && *ix != 1){
		*ix = -1;
		*iy = 0;
		return;
	}

	// UP
	if(gpio_get_level(pins[2]) && *iy != 1){
		*iy = -1;
		*ix = 0;
		return;
	}

}

/* dlay: delay for given milliseconds */
void dlay(int ms){
	vTaskDelay(ms / portTICK_PERIOD_MS);
}
