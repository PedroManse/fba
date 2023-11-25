
void draw_full_line (
	const struct frame frame,
	const int y,
	const color write
) {
#ifndef RELEASE
	assert(in_frame(frame, y, 0));
#endif

	color *location = frame_pos(frame, y, 0);

	for (unsigned int i = 0; i < frame.cols; i++) {
		location[i] = write;
	}
}

void draw_full_column (
	const struct frame frame,
	const int x,
	const color write
) {

#ifndef RELEASE
	assert(in_frame(frame, 0, x));
#endif

	color *location = frame_pos(frame, 0, x);
	for (unsigned int y = 0; y < frame.rows; y++) {
		location[y*frame.skip] = write;
	}
}

void draw_line(
	const struct frame frame,
	const int y, const int startx,
	const int endx,
	const color write
) {
#ifndef RELEASE
	assert(in_frame(frame, y, startx));
	assert(in_frame(frame, y, endx));
#endif

	color *location = frame_pos(frame, y, 0);
	for (int x = startx; x < endx; x++) {
		location[x] = write;
	}
}

void draw_column(
	const struct frame frame,
	const int x, const int starty,
	const int endy,
	color write
) {
#ifndef RELEASE
	assert(in_frame(frame, starty, x));
	assert(in_frame(frame, starty, x));
#endif

	color *location = frame_pos(frame, 0, x);
	for (int y = starty; y < endy; y++) {
		location[y*frame.skip] = write;
	}
}

void draw_rectangle(
	const struct frame frame,
	const int topy, const int topx,
	const int boty, const int botx,
	const color write
) {
#ifndef RELEASE
	assert(in_frame(frame, topy, topx));
	assert(in_frame(frame, boty, botx));
#endif

	draw_line  (frame, topy, topx, botx, write);
	draw_line  (frame, boty, topx, botx, write);
	draw_column(frame, topx, topy, boty, write);
	draw_column(frame, botx, topy, boty, write);
}

void fill_rectangle(
	const struct frame frame,
	const int topy, const int topx,
	const int boty, const int botx,
	const color write
) {
#ifndef RELEASE
	assert(in_frame(frame, topy, topx));
	assert(in_frame(frame, boty, botx));
#endif

	for (int y = topy; y < boty; y++ ) {
		draw_line  (frame, y, topx, botx, write);
	}
}

static inline void _cord_draw(
	const struct frame frame,
	int y, int x,
	int draw_offset, const int draw_diameter,
	const color write
) {
	y-=draw_offset;
	x-=draw_offset;
#ifndef RELEASE
	assert(in_frame(frame, y, x));
#endif
	color *location = frame_pos(frame, y, x);
	for (int _y = 0; _y < draw_diameter; _y++) {
		for (int _x = 0; _x < draw_diameter; _x++) {
			location[_x+_y*frame.skip] = write;
		}
	}
}

void cord_draw(
	const struct frame frame,
	const int y, const int x,
	const unsigned int draw_diameter, color write
) {
#ifndef RELEASE
	assert(in_frame(frame, y, x));
#endif

	if (draw_diameter == 1) {
		*frame_pos(frame, y, x) = write;
	} else {
		_cord_draw(frame, y, x,
				(draw_diameter-(draw_diameter%2))/2,
				draw_diameter, write);
	}
}

void cord_t_draw(
	const struct frame frame,
	const cord pnt, const unsigned int draw_diameter,
	const color write
) {
	cord_draw( frame, pnt.y, pnt.x, draw_diameter, write);
}

void polar_draw(
	const struct frame frame,
	int y, int x,
	const long double radius, const long double angle,
	const int draw_diameter,
	const color write
) {
	cord diff = polar_cord(radius, angle);
	y+=diff.y;
	x+=diff.x;
	cord_draw(frame, y, x, draw_diameter, write);
}

void draw_free_line(
	const struct frame frame,
	const int starty, const int startx,
	const int endy, const int endx,
	const int p, // pixel count // 'precision'
	const int draw_diameter,
	const color write
) {

	// iterate p times, range = [0,1]
	for (long double t = 0; t <= 1; t+=1/(long double)p) {
		cord pnt = cord_lerp(starty, startx, endy, endx, t);
		cord_t_draw(frame, pnt, draw_diameter, write);
	}
}

void polar_fill(
	const struct frame frame,
	const int starty, const int startx,
	const long double radius, const long double angle,
	const int draw_diameter,
	const color write
) {
	cord diff = polar_cord(radius, angle);
	int endy = starty+diff.y;
	int endx = startx+diff.x;
	draw_free_line(frame,
			starty, startx,
			endy, endx,
			(int)distancel(
				starty, startx,
				endy, endx
			),
			draw_diameter,
			write
		);
}

void draw_circle(
	const struct frame frame,
	const int y, const int x, const int radius,
	const color write
) {
#ifndef RELEASE
	assert(in_frame(frame, y-radius, x-radius));
	assert(in_frame(frame, y+radius, x+radius));
#endif

	color *location = frame_pos(frame, y, x);
	const int sr = -(radius*radius);

	for (int _y = -radius; _y <= radius; _y++)
		for (int _x = -radius; _x <= radius; _x++)
			if ( abs(_y*_y + _x*_x + sr) <= radius)
				location[(y+_y)*frame.skip+x+_x] = write;
}

void fill_circle(
	const struct frame frame,
	const int y, const int x, const int radius,
	const color write
) {
#ifndef RELEASE
	assert(in_frame(frame, y-radius, x-radius));
	assert(in_frame(frame, y+radius, x+radius));
#endif

	color *location = frame_pos(frame, y, x);
	const int radsqr = radius*radius;

	for (int _y = -radius; _y <= radius; _y++)
		for (int _x = -radius; _x <= radius; _x++)
			if ( _y*_y + _x*_x -radsqr <= radius)
				location[(y+_y)*frame.skip+x+_x] = write;
}



