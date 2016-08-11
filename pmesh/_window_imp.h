#ifdef __cplusplus
extern "C" {
#endif
typedef enum { FASTPM_PAINTER_LINEAR, FASTPM_PAINTER_CUBIC, FASTPM_PAINTER_QUADRATIC, FASTPM_PAINTER_LANCZOS} FastPMPainterType;

typedef struct FastPMPainter FastPMPainter;

typedef double (*fastpm_kernelfunc)(double x, double invh);

struct FastPMPainter {
    FastPMPainterType type;
    int diffdir; /* -1 to not taking differences*/
    int support;
    int ndim;
    double scale[32]; /* scale from position to grid units */
    ptrdiff_t translate[32]; /* translate in grid units */
    ptrdiff_t Nmesh[32]; /* periodicity */

    void * canvas;
    int canvas_dtype_elsize;
    ptrdiff_t size[32];
    ptrdiff_t strides[32];

    /* Private: */
    void   (*paint)(FastPMPainter * painter, double pos[], double weight);
    double (*readout)(FastPMPainter * painter, double pos[]);

    fastpm_kernelfunc kernel;
    fastpm_kernelfunc diff;

    double hsupport;
    double invh;
    int left; /* offset to start the kernel, (support - 1) / 2*/
    int Npoints; /* (support) ** ndim */
};

void
fastpm_painter_init(FastPMPainter * painter);

void
fastpm_painter_paint(FastPMPainter * painter, double pos[], double weight);

double
fastpm_painter_readout(FastPMPainter * painter, double pos[]);

#ifdef __cplusplus
}
#endif