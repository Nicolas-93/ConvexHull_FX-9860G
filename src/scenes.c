#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <gint/keyboard.h>
#include "scenes.h"
#include "graphics.h"
#include "convexhull.h"
#include "generation.h"


void SCN_polygon_simple(Parameters params) {
    ConvexHull* convex = CVH_init_convexhull();
    ListPoint points, reste;
    CIRCLEQ_INIT(&points);
    CIRCLEQ_INIT(&reste);
    key_event_t ev;
    // Point* point;

    if (params.gen.enabled) {
        GEN_choose_generation(params, &points);
        CVH_points_to_ConvexHull(
            &points, convex, &reste,
            params.gen.animation? GFX_animate_convex : NULL
        );
    }
    while (1) {
        GFX_animate_convex(convex, &reste);
        ev = getkey();
        if (ev.key == KEY_EXE) {
            break;
        }
    }

    CVH_free_convexhull(convex);
    CVH_free_vertex_list(&reste, false);
    CVH_free_vertex_list(&points, true);

    return;
}

/*
void SCN_polygon_inception(Parameters params) {
    ListConvexHull convexs;
    ListPoint points;
    CIRCLEQ_INIT(&convexs);
    CIRCLEQ_INIT(&points);
    MLV_Ev ev;
    Point* point;

    if (params.gen.enabled) {
        GEN_choose_generation(params, &points);
        CVH_points_to_ListConvexHull(
            &points, &convexs, 
            params.gen.animation ? GFX_animate_ListConvexHull : NULL
        );
    }

    while (1) {
        GFX_animate_ListConvexHull(&convexs);
        ev = SCN_wait_ev();
        if (ev.type == MLV_KEY) {
            if (ev.key_btn == MLV_KEYBOARD_ESCAPE) {
                CVH_free_vertex_list(&points, true);
                CVH_free_ListConvexHull(&convexs);
                break;
            }
        }
        else if (IS_CLICK(ev)) {
            point = CVH_add_user_point(&points, MOUSE_EV_TO_POINT(ev));
            CVH_add_inception_recursif(&convexs, convexs.cqh_first, point);
        }
    }
}*/
