#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "generation.h"
#include "convexhull.h"
#include "utils.h"

#define PI 3.14159265358979323846

/**
 * @brief Renvoie un nombre aléatoire entre 0 et n.
 * 
 * @param n 
 * @return double 
 */
inline double rand_double(double n) {
    return (double)rand() / (double)(RAND_MAX / n);
}

/**
 * @brief Renvoie un nombre aléatoire entre a et b.
 * 
 * @param a 
 * @param b 
 * @return double 
 */
inline double uniform(double a, double b) {
    return a + (b - a) * rand_double(1);
}
/**
 * @brief Renvoie un booléen aléatoire.
 * 
 * @return int 
 */
inline int random_bool() {
    return rand() > (RAND_MAX / 2);
}

/**
 * @brief Renvoie aléatoirement un entier négatif
 * ou positif.
 * 
 * @return int 
 */
inline int random_direction() {
    return random_bool() == 0 ? -1 : 1;
}

/**
 * @brief Fonction de comparaison de distance entre deux points.
 * 
 * @param ax 
 * @param ay 
 * @param bx 
 * @param by 
 * @return double 
 */
inline double GEN_distance(double ax, double ay, double bx, double by) {
    double comp_x = (bx - ax);
    double comp_y = (by - ay);
    return comp_x * comp_x + comp_y * comp_y;
}

inline int GEN_compare_point_distance(const void* a, const void* b) {
    return ((PointDistance*) a)->dist - ((PointDistance*) b)->dist;
}

/**
 * @brief Génère un point dans un cercle.
 * 
 * @param largeur Largeur de la fenêtre.
 * @param hauteur Hauteur de la fenêtre.
 * @param i Itérateur de la boucle sur nb_points.
 * @param nb_points Nombre de points à générer.
 * @param r_max Rayon du cercle.
 * @param concentration
 * Cas :
 * - Si < 0 : Les points vont être générés à l'extérieur du cercle,
 * en tendant vers ce dernier
 * - Si = 0 : Les points seront sur le cercle
 * - Si > 0 et < 1 : Les points vont être générés à l'intérieur du cercle,
 * en tendant vers ce dernier
 * - Si = 1 : Les points seront distribués uniformément
 * - Si > 1 : Les points tendront vers le centre du cercle
 * @return Point Point généré
 */
Point GEN_formule_cercle(
    int offset_x, int offset_y,
    int i, int nb_points, int w_rmax, int h_rmax, double concentration
) {
    Point p;
    double rnd, rayon, angle;
    int r_max = MIN(w_rmax, h_rmax);
    rnd = rand_double(1);
    rayon = sqrt(pow(rnd, concentration));
    angle = 2 * PI * rand_double(1);
    rayon *= r_max;
    p.x = rayon * cos(angle) + offset_x;
    p.y = rayon * sin(angle) + offset_y;

    return p;
}

/**
 * @brief Génère un point dans un carré uniforme.
 * 
 * @return Point 
 */
Point GEN_formule_rectangle_uniforme(
    int offset_x, int offset_y,
    int i, int nb_points, int w_rmax, int h_rmax, double concentration
) {
    Point p;
    p.x = uniform(-w_rmax, w_rmax) + offset_x;
    p.y = uniform(-h_rmax, h_rmax) + offset_y;

    return p;
}

/**
 * @brief Génère un point d'un carré croissant, en utilisant
 * l'itérateur de la boucle de la fonction appelante. 
 * 
 * @return Point 
 */
Point GEN_formule_carre_croissant(
    int offset_x, int offset_y,
    int i, int nb_points, int w_rmax, int h_rmax, double concentration
) {
    Point p;
    double dist_inc = ((double) h_rmax / (double) nb_points) * i;
    double a_x = -dist_inc, a_y = -dist_inc, b_x = dist_inc, b_y = dist_inc;

    p.x = uniform(a_x, b_x) + offset_x;
    p.y = uniform(a_y, b_y) + offset_y;

    return p;
}

/**
 * @brief Génère un ensemble de points sur un disque, selon
 * un mode génération fourni par une fonction en paramètre.
 * 
 * @param points Adresse de la liste de points où les points seront stockés.
 * @param largeur Largeur de la fenêtre.
 * @param hauteur Hauteur de la fenêtre.
 * @param nb_points Nombre de points à générer.
 * @param r_max Rayon de l'ensemble.
 * @param concentration 
 * @param tri Spécifie si la liste doit être triée en fonction
 * de la distance des points par rapport à l'origine du cercle.
 * @param formule Fonction à appeler pour générer un point d'un ensemble.
 * @return 0 si une erreur est survenue pendant l'allocation des points,
 * 1 sinon.
 */
int GEN_points_formule(
    ListPoint* points,
    int offset_x, int offset_y,
    int nb_points, int w_rmax, int h_rmax, double concentration, bool tri,
    Point (*formule) (int, int, int, int, int, int, double)
) {
    PointDistance* tab_points = NULL;
    CIRCLEQ_INIT(points);
    if (tri) {
        tab_points = malloc(nb_points * sizeof(PointDistance));
        if (!tab_points)
            return 0;
    }

    for (int i = 0; i < nb_points; ++i) {
        Point p = formule(
            offset_x, offset_y,
            i, nb_points,
            w_rmax, h_rmax,
            concentration
        );

        if (tri) {
            double dist = GEN_distance(p.x, p.y, offset_x, offset_y);
            PointDistance p_dist = {p, dist};
            tab_points[i] = p_dist;
        }
        else {
            Vertex* new_entry = CVH_new_vertex(p);
            if (!new_entry)
                return 0;
            CIRCLEQ_INSERT_TAIL(points, new_entry, entries);
        }
    }
    if (tri) {
        GEN_sort_tab_PointDistance_to_ListPoint(tab_points, nb_points, points);
    }
    return 1;
}

/**
 * @brief Trie un tableau de PointDistance, et le copie dans une ListePoint.
 * 
 * @param tab_points Tableau de PointDistance
 * @param size Taille du tableau
 * @param points Adresse de la ListePoint où seront copiés les points
 */
void GEN_sort_tab_PointDistance_to_ListPoint(PointDistance* tab_points, int size, ListPoint* points) {
    qsort(tab_points, size, sizeof(PointDistance), GEN_compare_point_distance);
    for (int i = 0; i < size; ++i) {
        Vertex* new_entry = CVH_new_vertex(tab_points[i].p);
        CIRCLEQ_INSERT_TAIL(points, new_entry, entries);
    }
    free(tab_points);
}

/**
 * @brief Renvoie une liste de points générée selon
 * les paramètres demandés par l'utilisateur.
 * 
 * @param params Paramètres
 * @param points Liste de points de destination
 */
void GEN_choose_generation(Parameters params, ListPoint* points) {
    CIRCLEQ_INIT(points);

    Point (*formule) (int, int, int, int, int, int, double);
    if (params.gen.shape == CERCLE) {
        formule = GEN_formule_cercle;
    }
    /* La génération croissante, ne nous permet pas de
    générer des points uniformément, comme la fonction cercle */
    else if (params.gen.concentration != 1) {
        formule = GEN_formule_carre_croissant;
    } else {
        formule = GEN_formule_rectangle_uniforme;
    }
    srand(time(NULL));
    GEN_points_formule(points,
        params.window.width / 2, params.window.height / 2,
        params.gen.nb_points,
        (params.window.width - params.gen.margin) / 2, (params.window.height - params.gen.margin) / 2,
        params.gen.concentration, params.gen.progressif,
        formule
    );
    /*printf(
        "Animation : %d, Inception : %d, Tri : %d\n",
        params.gen.animation, params.inception, params.gen.progressif
    );*/
}
