#include "fba.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#ifndef __useconds_t
#define __useconds_t unsigned int
#endif

#ifndef usleep
int usleep(__useconds_t);
#endif

int fsleep (long double t) {
	return usleep((int) lroundl(1000000.0 * t));
}

void sprite_deloc(struct sprite *sprite) {
	free(sprite->content);
	free(sprite);
}

struct sprite *sprite_read(char *filename) {
	struct sprite *sprite = malloc(sizeof(*sprite));
	int fd = open(filename, O_RDONLY);
	assert(read(fd,
		(uint32_t*)sprite,
		sizeof(uint32_t)*2)
	);
	color *content_buffer = calloc(
		sprite->width * sprite->heigth,
		sizeof(color)
	);
	assert(read(fd, content_buffer,
		sizeof(color) * sprite->width * sprite->heigth
	));
	sprite->content = content_buffer;
	close(fd);
	return sprite;
}

int main() {
	struct frame frame = InitFb();

	struct sprite *sprite = sprite_read("a.font");

	sprite_draw(frame, 100, 100, sprite);

	sprite_deloc(sprite);

// growing ball
//	for (int r = 1; r<100; r++) {
//		fill_circle(frame,
//			100, 100, r,
//			0xa0a0a0
//		);
//		fsleep(.3);
//	}

//	draw_free_line(
//		frame, 100, 100,
//		150, 150, 100, 0XFFFFFF
//	);
//	polar_fill(
//		frame, 100, 100,
//		75, 45, 1, 0XFF7F00
//	);

//	const int t = 4;
//	int r = 90;
//	cord_draw(frame, 100, 100, t+2, 0X000000);
//	for (int a = 0; a<360; a++) {
//		polar_draw(frame, 100, 100, r, a, t+2, 0X000000);
//	}
//
//	cord_draw(frame, 100, 100, t, 0X00FF7F);
//	for (long double a = 0; a<60; a+=0.01) {
//		polar_fill(frame, 100, 100, r, a-120, t, 0XFFFF00);
//		polar_fill(frame, 100, 100, r, a-240, t, 0XFFFF00);
//		polar_fill(frame, 100, 100, r, a-360, t, 0XFFFF00);
//	}
//	for (long double a = 0; a<60; a+=0.005) {
//		polar_fill(frame, 100, 100, r/7, a-120, t, 0);
//		polar_fill(frame, 100, 100, r/7, a-240, t, 0);
//		polar_fill(frame, 100, 100, r/7, a-360, t, 0);
//	}


//	const int t = 4;
//	int r = 90;
//	long double _r = 90;
//	cord_draw(frame, 100, 100, t+2, 0X000000);
//	for (int a = 0; a<360; a++) {
//		polar_draw(frame, 100, 100, r, a, t+2, 0X000000);
//	}
//
//	cord_draw(frame, 100, 100, t, 0X00FF7F);
//	for (int a = 0; a<36000; a++) {
//		//polar_draw(frame, 100, 100, r, a-120, t+2, 0X000000);
//		//polar_draw(frame, 100, 100, r, a-240, t+2, 0X000000);
//		//polar_draw(frame, 100, 100, r, a-360, t+2, 0X000000);
//
//		polar_draw(frame, 100, 100, r, a-120, t, 0XFF0000);
//		polar_draw(frame, 100, 100, r, a-240, t, 0X00FF00);
//		polar_draw(frame, 100, 100, r, a-360, t, 0X0000FF);
//		_r-=0.001L;
//		r = (int)_r;
//
//		fsleep(0.0018);
//	}

//	fill_rectangle( frame, 100, 100, 200, 200, 0xFF0000 );
//	fill_rectangle( frame, 200, 200, 300, 300, 0x00FF00 );
//	fill_rectangle( frame, 200, 100, 300, 200, 0x0000FF );
//	fill_rectangle( frame, 100, 200, 200, 300, 0x7F7F7F );

//	const int y = 10;
//	const int x = 10;
//	const int cwidth = 8;
//	const int cheigth = 16;
//	for (int _y = 0; _y<10; _y++) {
//		for (int _x = 0; _x<10; _x++) {
//			const int cx = (x+_x)*cwidth;
//			const int cy = (y+_y)*cheigth;
//
//			draw_column(frame, cx,       cy, cy+cheigth, 0XFF7F00);
//			draw_column(frame, cx+cwidth,cy, cy+cheigth, 0XFF7F00);
//
//			draw_line(frame, cy,         cx, cx+cwidth, 0XFF7F00);
//			draw_line(frame, cy+cheigth, cx, cx+cwidth, 0XFF7F00);
//		}
//	}

}
