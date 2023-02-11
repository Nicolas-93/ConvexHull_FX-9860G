/* GNU's CIRCLE_Q implementation */

#ifndef __CIRCLE_Q__
#define __CIRCLE_Q__

#define CIRCLEQ_HEAD(name, type)                                               \
    struct name {                                                              \
        struct type *cqh_first; /* first element */                            \
        struct type *cqh_last;  /* last element */                             \
    }

#define CIRCLEQ_HEAD_INITIALIZER(head)                                         \
    { (void *)&head, (void *)&head }

#define CIRCLEQ_ENTRY(type)                                                    \
    struct {                                                                   \
        struct type *cqe_next; /* next element */                              \
        struct type *cqe_prev; /* previous element */                          \
    }

/*
 * Circular queue functions.
 */
#define CIRCLEQ_INIT(head)                                                     \
    do {                                                                       \
        (head)->cqh_first = (void *)(head);                                    \
        (head)->cqh_last = (void *)(head);                                     \
    } while (/*CONSTCOND*/ 0)

#define CIRCLEQ_INSERT_AFTER(head, listelm, elm, field)                        \
    do {                                                                       \
        (elm)->field.cqe_next = (listelm)->field.cqe_next;                     \
        (elm)->field.cqe_prev = (listelm);                                     \
        if ((listelm)->field.cqe_next == (void *)(head))                       \
            (head)->cqh_last = (elm);                                          \
        else                                                                   \
            (listelm)->field.cqe_next->field.cqe_prev = (elm);                 \
        (listelm)->field.cqe_next = (elm);                                     \
    } while (/*CONSTCOND*/ 0)

#define CIRCLEQ_INSERT_BEFORE(head, listelm, elm, field)                       \
    do {                                                                       \
        (elm)->field.cqe_next = (listelm);                                     \
        (elm)->field.cqe_prev = (listelm)->field.cqe_prev;                     \
        if ((listelm)->field.cqe_prev == (void *)(head))                       \
            (head)->cqh_first = (elm);                                         \
        else                                                                   \
            (listelm)->field.cqe_prev->field.cqe_next = (elm);                 \
        (listelm)->field.cqe_prev = (elm);                                     \
    } while (/*CONSTCOND*/ 0)

#define CIRCLEQ_INSERT_HEAD(head, elm, field)                                  \
    do {                                                                       \
        (elm)->field.cqe_next = (head)->cqh_first;                             \
        (elm)->field.cqe_prev = (void *)(head);                                \
        if ((head)->cqh_last == (void *)(head))                                \
            (head)->cqh_last = (elm);                                          \
        else                                                                   \
            (head)->cqh_first->field.cqe_prev = (elm);                         \
        (head)->cqh_first = (elm);                                             \
    } while (/*CONSTCOND*/ 0)

#define CIRCLEQ_INSERT_TAIL(head, elm, field)                                  \
    do {                                                                       \
        (elm)->field.cqe_next = (void *)(head);                                \
        (elm)->field.cqe_prev = (head)->cqh_last;                              \
        if ((head)->cqh_first == (void *)(head))                               \
            (head)->cqh_first = (elm);                                         \
        else                                                                   \
            (head)->cqh_last->field.cqe_next = (elm);                          \
        (head)->cqh_last = (elm);                                              \
    } while (/*CONSTCOND*/ 0)

#define CIRCLEQ_REMOVE(head, elm, field)                                       \
    do {                                                                       \
        if ((elm)->field.cqe_next == (void *)(head))                           \
            (head)->cqh_last = (elm)->field.cqe_prev;                          \
        else                                                                   \
            (elm)->field.cqe_next->field.cqe_prev = (elm)->field.cqe_prev;     \
        if ((elm)->field.cqe_prev == (void *)(head))                           \
            (head)->cqh_first = (elm)->field.cqe_next;                         \
        else                                                                   \
            (elm)->field.cqe_prev->field.cqe_next = (elm)->field.cqe_next;     \
    } while (/*CONSTCOND*/ 0)

#define CIRCLEQ_FOREACH(var, head, field)                                      \
    for ((var) = ((head)->cqh_first); (var) != (const void *)(head);           \
         (var) = ((var)->field.cqe_next))

#define CIRCLEQ_FOREACH_REVERSE(var, head, field)                              \
    for ((var) = ((head)->cqh_last); (var) != (const void *)(head);            \
         (var) = ((var)->field.cqe_prev))

/*
 * Circular queue access methods.
 */
#define CIRCLEQ_EMPTY(head) ((head)->cqh_first == (void *)(head))
#define CIRCLEQ_FIRST(head) ((head)->cqh_first)
#define CIRCLEQ_LAST(head) ((head)->cqh_last)
#define CIRCLEQ_NEXT(elm, field) ((elm)->field.cqe_next)
#define CIRCLEQ_PREV(elm, field) ((elm)->field.cqe_prev)

#define CIRCLEQ_LOOP_NEXT(head, elm, field)                                    \
    (((elm)->field.cqe_next == (void *)(head)) ? ((head)->cqh_first)           \
                                               : (elm->field.cqe_next))
#define CIRCLEQ_LOOP_PREV(head, elm, field)                                    \
    (((elm)->field.cqe_prev == (void *)(head)) ? ((head)->cqh_last)            \
                                               : (elm->field.cqe_prev))

/**
 * @brief Définit un élément comme tête de la liste
 */
#define CIRCLEQ_SET_AS_FIRST(head, listelm, field)                             \
    do {                                                                       \
        (head)->cqh_last->field.cqe_next = (head)->cqh_first;                  \
        (head)->cqh_first->field.cqe_prev = (head)->cqh_last;                  \
                                                                               \
        (head)->cqh_first = (listelm);                                         \
        (head)->cqh_last = (listelm)->field.cqe_prev;                          \
                                                                               \
        (head)->cqh_first->field.cqe_prev = (void *)(head);                    \
        (head)->cqh_last->field.cqe_next = (void *)(head);                     \
    } while (0)

/**
 * @brief Déplace les cellules de head1 dans head2.
 *
 */
#define CIRCLEQ_MOVE_TO(head1, head2, field)                                   \
    do {                                                                       \
        *(head2) = *(head1);                                                   \
        (head2)->cqh_first->field.cqe_prev = (void *)(head2);                  \
        (head2)->cqh_last->field.cqe_next = (void *)(head2);                   \
    } while (0)

#endif
