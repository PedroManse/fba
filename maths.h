#include <math.h>
#define PI 3.141592L

// p = 0..1
long double lerp(
		long double a,
		long double b,
		long double p
) {
	return a * (1-p) + (b*p);
}

color* frame_pos ( struct frame jar, int y, int x ) {
	return jar.fbmem + (y+jar.yoff)*jar.skip + (jar.xoff+x);
}

bool in_frame(struct frame jar, int y, int x) {
	return ( (unsigned)y < jar.rows && (unsigned)x < jar.cols );
}

//d=√ ( (x 2 - x 1 )² + (y 2 - y 1 )²).
long double distancel(
	int y1, int x1,
	int y2, int x2
) {
	return sqrtl((long double)(
		(x2-x1)*(x2-x1)
		+
		(y2-y1)*(y2-y1)
	));
}

long double ldistancel(
	long double y1, long double x1,
	long double y2, long double x2
) {
	return sqrtl(
		(x2-x1)*(x2-x1)
		+
		(y2-y1)*(y2-y1)
	);
}

cord polar_cord(long double radius, long double angle) {
	cord pnt;
	long double a = angle*(PI/180);
	pnt.x = radius*cosl(a);
	pnt.y = radius*sinl(a);
	//printf("%i:%i, %i\n", pnt.x, pnt.y, pnt.x - pnt.y );
	return pnt;
}

cord polar_t_cord (polar plr) {
	cord pnt;
	long double a = plr.a*(PI/180);
	pnt.x = plr.r*cosl(a);
	pnt.y = plr.r*sinl(a);
	return pnt;
}

cord cord_lerp(
	int y1, int x1,
	int y2, int x2,
	long double p
) {
	cord pnt;
	pnt.y = lerp(y1, y2, p);
	pnt.x = lerp(x1, x2, p);
	return pnt;
}

cord cord_t_lerp(
	cord c1, cord c2, int p
) {
	cord pnt;
	pnt.y = lerp(c1.y, c2.y, p);
	pnt.x = lerp(c1.x, c2.x, p);
	return pnt;
}

long double angle_constraint(long double a) {
	while (a < 360L) {
		a-=360L;
	}
	return a;
}

polar *ipolar_rot (polar *plr, long double a) {
	plr->a=angle_constraint(plr->a+a);
	return plr;
}

polar polar_rot (polar plr, long double a) {
	plr.a=angle_constraint(plr.a+a);
	return plr;
}

