#ifndef __H_GENERATION__
#define __H_GENERATION__

#include <stdbool.h>
#include "circleq.h"
#include "args.h"
#include "convexhull.h"


typedef struct {
    Point p;
    double dist;
} PointDistance;

double rand_double(double n);
double uniform(double a, double b);
int random_bool();
int random_direction();

void GEN_choose_generation(Parameters params, ListPoint* points);
Point GEN_formule_rectangle_uniforme(
    int offset_x, int offset_y,
    int i, int nb_points, int w_rmax, int h_rmax, double concentration
);
Point GEN_formule_carre_croissant(
    int offset_x, int offset_y,
    int i, int nb_points, int w_rmax, int h_rmax, double concentration
);
Point GEN_formule_cercle(
    int offset_x, int offset_y,
    int i, int nb_points, int w_rmax, int h_rmax, double concentration
);

int GEN_points_formule(
    ListPoint* points,
    int offset_x, int offset_y,
    int nb_points, int w_rmax, int h_rmax, double concentration, bool tri,
    Point (*formule) (int, int, int, int, int, int, double)
);
double GEN_distance(double ax, double ay, double bx, double by);
int GEN_compare_point_distance(const void* a, const void* b);
void GEN_sort_tab_PointDistance_to_ListPoint(PointDistance* tab_points, int size, ListPoint* points);

#endif
