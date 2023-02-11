#include <gint/display.h>
#include <gint/keyboard.h>
#include "scenes.h"
#include "args.h"
#define MARGIN 10

static Parameters params = {
	.window.width = DWIDTH, .window.height = DHEIGHT,
	.gen = {
		.shape = CERCLE,
		.margin = MARGIN,
		.enabled = true,
		.nb_points = 300,
		.rayon = 32,
		.concentration = 1,
		.progressif = true,
		.animation = true,
	},
	.inception = false
};

int main(void) {
	dclear(C_WHITE);
	SCN_polygon_simple(params);
	// dtext(1, 1, C_BLACK, "Sample fxSDK add-in.");
	dupdate();

	return 1;
}
