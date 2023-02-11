#include <gint/display.h>
#include <gint/gray.h>
#include <gint/keyboard.h>
#include "scenes.h"
#include "args.h"

static Parameters params = {
	.window.width = DWIDTH, .window.height = DHEIGHT,
	.gen = {
		.shape = RECTANGLE,
		.margin = 10,
		.enabled = true,
		.nb_points = 250,
		.concentration = 1,
		.progressif = true,
		.animation = true,
	},
	.inception = true,
};

int main(void) {
	dclear(C_WHITE);
	dgray(DGRAY_ON);

	SCN_polygon_inception(params);

	dupdate();
	dgray(DGRAY_OFF);

	return 1;
}
