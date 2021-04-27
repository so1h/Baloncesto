/* ----------------------------------------------------------------------- */
/*                               ll_s_plot.h                               */
/* ----------------------------------------------------------------------- */
/*            rutina de interfaz de la llamada al sistema plot             */
/* ----------------------------------------------------------------------- */

#ifndef LL_S_PLOT_H
#define LL_S_PLOT_H
                          
/* 00 --> nada, 01 --> activar, 10 --> desactivar, 11 --> no usado         */
                          
#define PLOT_ON     (0x00000001 <<  0)                    /* para plot on  */
#define PLOT_ON_    (0x00000001 <<  1)                    /* para plot off */
#define PLOT_MEM    (0x00000001 <<  2)                    /* para plot mem */
#define PLOT_MEM_   (0x00000001 <<  3)                    /* para plot e9  */

int ll_s_plot ( unsigned flags, unsigned sched ) ;             

#endif /* LL_S_PLOT_H */
