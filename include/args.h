#ifndef H_ARGS
#define H_ARGS

enum Shape {RECTANGLE, CERCLE};
typedef struct {
    struct {
        enum Shape shape;
        int margin;
        int enabled;
        int nb_points;
        int rayon;
        double concentration;
        int progressif;
        int animation;
    } gen;
    struct {
        int width;
        int height;
    } window;
    int inception;
} Parameters;

#endif
