#ifdef __cplusplus
extern "C" {
#endif
typedef enum { FASTPM_PAINTER_LINEAR,
               FASTPM_PAINTER_CUBIC,
               FASTPM_PAINTER_QUADRATIC,
               FASTPM_PAINTER_LANCZOS2,
               FASTPM_PAINTER_LANCZOS3,
               FASTPM_PAINTER_DB6,
               FASTPM_PAINTER_DB12,
               FASTPM_PAINTER_DB20,
               FASTPM_PAINTER_SYM6,
               FASTPM_PAINTER_SYM12,
               FASTPM_PAINTER_SYM20,

} PMeshPainterType;

typedef struct PMeshPainter PMeshPainter;

typedef double (*pmesh_kernelfunc)(double x);

struct PMeshPainter {
    PMeshPainterType type;
    int diffdir; /* -1 to not taking differences*/
    int support;
    int ndim;
    double scale[32]; /* scale from position to grid units */
    double translate[32]; /* translate in grid units */
    ptrdiff_t Nmesh[32]; /* periodicity */

    void * canvas;
    int canvas_dtype_elsize;
    ptrdiff_t size[32];
    ptrdiff_t strides[32];

    /* Private: */
    void   (*paint)(PMeshPainter * painter, double pos[], double weight);
    double (*readout)(PMeshPainter * painter, double pos[]);

    pmesh_kernelfunc kernel;
    pmesh_kernelfunc diff;

    double nativesupport; /* unscaled support */
    double vfactor; /* nativesupport / support */
    double shift;
    int left; /* offset to start the kernel, (support - 1) / 2*/
    int Npoints; /* (support) ** ndim */
};

void
pmesh_painter_init(PMeshPainter * painter);

void
pmesh_painter_paint(PMeshPainter * painter, double pos[], double weight);

double
pmesh_painter_readout(PMeshPainter * painter, double pos[]);

#ifdef __cplusplus
}
#endif
