#include <gint/display.h>
#include <math.h>
#include "graphics.h"
#include "convexhull.h"


/**
 * @brief Affiche les points de la liste sur la fenêtre
 * 
 * @param points Adresse de la liste de points
 * @param color Couleur des points
 */
void GFX_plot_points(ListPoint* points, color_t color) {
    Vertex* entry;
    CIRCLEQ_FOREACH(entry, points, entries) {
        dpixel(
            entry->p->x,
            entry->p->y,
            color
        );
    }
}

color_t GFX_map_color(int n) {
    static color_t colors[] = {
        C_BLACK, C_LIGHT, C_DARK,
    };
    n %= 3;

    return colors[n];
}

/**
 * @brief Dessine une enveloppe convexe pleine
 * 
 * @param convex Adresse de l'enveloppe convexe
 */
/*
void GFX_draw_filled_polygon(ConvexHull* convex) {
    int axis_x[convex->current_len];
    int axis_y[convex->current_len];

    GFX_PolygonList_to_xy_array(convex, axis_x, axis_y);
    MLV_draw_filled_polygon(
        axis_x, axis_y, convex->current_len,
        convex->color
    );
    GFX_plot_points((ListPoint*) &convex->poly, convex->color);
}
*/

/**
 * @brief Dessine un polygone par des lignes (non rempli)
 * 
 * @param convex Adresse de l'enveloppe convexe
 */
void GFX_draw_lines_polygon(ConvexHull* convex) {
    Vertex* vtx;
    CIRCLEQ_FOREACH_REVERSE(vtx, &(convex->poly), entries) {
        Vertex* vtx1 = CIRCLEQ_LOOP_NEXT(&convex->poly, vtx, entries);
        dline(
            vtx->p->x, vtx->p->y,
            vtx1->p->x, vtx1->p->y,
            convex->color
        );
    }
}

/**
 * @brief Liste ConvexHull en double tableau.
 * 
 * @param convex Instance de ConvexHull
 * @param x_axis Axe x de destination
 * @param y_axis Axe y de destination
 */
/*
void GFX_PolygonList_to_xy_array(ConvexHull* convex, int* x_axis, int* y_axis) {
    Vertex* vtx;
    int i = 0;
    CIRCLEQ_FOREACH(vtx, &(convex->poly), entries) {
        x_axis[i] = vtx->p->x;
        y_axis[i] = vtx->p->y;
        ++i;
    }
}
*/

/**
 * @brief Dessine une liste de polygônes convexes, pleins.
 * 
 * @param convexs Adresse de la liste ConvexHulls
 */
/*
void GFX_draw_filled_polygons(ListConvexHull* convexs) {
    ConvexHullEntry* convex_entry;

    CIRCLEQ_FOREACH(convex_entry, convexs, entries) {
        GFX_draw_filled_polygon(
            convex_entry->convex
        );
    }
}
*/


/**
 * @brief Dessine une liste de polygônes convexes par
 * des lignes (non remplis).
 * 
 * @param convexs Adresse de la liste ConvexHulls
 */
void GFX_draw_lines_polygons(ListConvexHull* convexs) {
    ConvexHullEntry* convex_entry;

    CIRCLEQ_FOREACH(convex_entry, convexs, entries) {
        GFX_draw_lines_polygon(
            convex_entry->convex
        );
    }
}


void GFX_animate_convex(ConvexHull* convex, ListPoint* reste) {
    dclear(C_WHITE);
    
    GFX_plot_points(reste, C_BLACK);
    GFX_plot_points((ListPoint*) &(convex->poly), C_BLACK);
    GFX_draw_lines_polygon(convex);

    // MLV_wait_milliseconds(1);
    dupdate();
}


void GFX_animate_ListConvexHull(ListConvexHull* convexs) {
    dclear(C_WHITE);

    GFX_draw_lines_polygons(convexs);

    // Un sleep ici pour le moteur de gris ?
    // MLV_wait_milliseconds(1);
    dupdate();
}


/*
void GFX_draw_triangle(Point a, Point b, Point c, MLV_Color color) {
    const int marge = 5;
    MLV_draw_line(a.x, a.y, b.x, b.y, color);
    MLV_draw_text(a.x, a.y - marge, "P0", MLV_COLOR_BLACK);
    MLV_draw_line(b.x, b.y, c.x, c.y, color);
    MLV_draw_text(b.x, b.y - marge, "P1", MLV_COLOR_BLACK);
    MLV_draw_line(c.x, c.y, a.x, a.y, color);
    MLV_draw_text(c.x, c.y - marge, "P2", MLV_COLOR_BLACK);
}
*/

/*
void GFX_draw_debug_triangle_direction(ConvexHull* convex, Point a, Point b, Point c) {
    fprintf(stderr,
        "Triangle( Point(%.1lf, %.1lf), Point(%.1lf, %.1lf), Point(%.1lf, %.1lf) )\n",
        a.x, a.y, b.x, b.y, c.x, c.y
    );
    int direct = IS_DIRECT_TRIANGLE(a, b, c);
    fprintf(stderr, "Direction : %d\n", direct);
    if (MLV_get_keyboard_state(MLV_KEYBOARD_LSHIFT) == MLV_PRESSED)
        return;
    MLV_Color color = direct? MLV_COLOR_GREEN : MLV_COLOR_RED;
    MLV_clear_window(MLV_COLOR_WHITE);
    GFX_draw_lines_polygon(convex);
    GFX_plot_points((ListPoint*) &convex->poly, convex->color);
    GFX_draw_triangle(a, b, c, color);
    MLV_actualise_window();
    MLV_wait_keyboard(NULL, NULL, NULL);
}
*/
