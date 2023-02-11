#include <gint/display.h>
#include <gint/keyboard.h>
#include "scenes.h"
#include "args.h"

static Parameters params = {
	.window.width = DWIDTH, .window.height = DHEIGHT,
	.gen = {
		.shape = CARRE,
		.enabled = true,
		.nb_points = 200,
		.rayon = 32,
		.concentration = 3,
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
