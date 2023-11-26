
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>

typedef struct {
	uint32_t y, x;
} cord;
typedef struct {
	long double y, x;
} cordl;
#define decord(point) (point.y, point.x)

typedef struct {
	// radius, angle
	long double r, a;
}	polar;
#define depolar(plr) (plr.r, plr.a)

// ARGB
typedef uint32_t color;
#define COL(R, G, B) (color)( (R<<16) + (G<<8) + B )

struct sprite {
	uint32_t width;
	uint32_t heigth;
	color *content;
};

struct frame {
	color* fbmem;
	unsigned int bpp; // bytes per pixel // should be 4
	unsigned int xoff, yoff; // x, y offset
	unsigned int skip; // @+skip = from y to y+1 (in bytes)
	unsigned int rows, cols; // row,col count
	unsigned long int screensize; //
	int fd;
	char* tty;
};

struct frame InitFb() {
	int fbfd = 0;
	color *fbp = 0;
	long int screensize = 0;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	// Open the file for reading and writing
	fbfd = open("/dev/fb0", O_RDWR);
	if (fbfd == -1) {
		perror("InitFb: cannot open framebuffer device");
		exit(1);
	}
	//printf("The framebuffer device was opened successfully.\n");
	// read  fixed screen information
	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
		perror("InitFb: can't read fixed information");
		exit(2);
	}
	//  variable screen information
	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
		perror("InitFb: can't read variable information");
		exit(3);
	}
	if (vinfo.bits_per_pixel != 8*4) {
		fprintf(stderr, "InitFb: screen not RGBA");
		exit(4);
	}
	//printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
	// Figure out the size of the screen in bytes
	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
	// Map the device to memory
	fbp = (color *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if ( *(int*)fbp == -1) {
		perror("InitFb: failed to map framebuffer device to memory");
		exit(5);
	}

	struct frame jar = {
		.fd = fbfd,
		.fbmem = fbp,
		.bpp = vinfo.bits_per_pixel/8,
		.xoff = vinfo.xoffset*vinfo.bits_per_pixel/8,
		.yoff = vinfo.yoffset*vinfo.bits_per_pixel/8,
		.skip = finfo.line_length/(vinfo.bits_per_pixel/8),// from y to y+1
		.screensize = screensize,
		.rows = (screensize / finfo.line_length),
		.cols = finfo.line_length/(vinfo.bits_per_pixel/8)-10,
		.tty = ttyname(STDIN_FILENO),
	};
	return jar;
}

#include "maths.h"
#include "draw.h"

