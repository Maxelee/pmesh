#define SETUP_KERNEL_CIC(Nd) \
    int d; \
    double XYZ[Nd]; \
    int IJK0[Nd], IJK1[Nd]; \
    double V1[Nd], V0[Nd]; \
\
    for(d = 0; d < Nd; d ++) { \
        XYZ[d] = pos[d]* painter->scale[d] + painter->translate[d]; \
        /* without floor, -1 < X < 0 is mapped to I=0 */ \
        IJK0[d] = (int) floor(XYZ[d]); \
        IJK1[d] = IJK0[d] + 1; \
    }; \
\
    for(d = 0; d < Nd; d ++) { \
        if(painter->order[d] == 0) { \
            V1[d] = XYZ[d] - IJK0[d]; \
            V0[d] = 1. - V1[d]; \
        } else { \
            V1[d] = painter->scale[d]; \
            V0[d] = - painter->scale[d]; \
        } \
    } \
\
    /* Do periodic wrapup in all directions. */ \
    /*  Buffer particles are copied from adjacent nodes */ \
    for(d = 0; d < Nd; d ++) { \
        if(painter->Nmesh[d] == 0) continue; \
        while(UNLIKELY(IJK0[d] < 0)) IJK0[d] += painter->Nmesh[d]; \
        while(UNLIKELY(IJK0[d] >= painter->Nmesh[d])) IJK0[d] -= painter->Nmesh[d]; \
        while(UNLIKELY(IJK1[d] < 0)) IJK1[d] += painter->Nmesh[d]; \
        while(UNLIKELY(IJK1[d] >= painter->Nmesh[d])) IJK1[d] -= painter->Nmesh[d]; \
    } \

static void
mkname(_cic_tuned_paint3) (PMeshPainter * painter, double pos[], double weight, double hsml)
{
    FLOAT * canvas = painter->canvas;

    SETUP_KERNEL_CIC(3);

    V0[0] *= weight;
    V1[0] *= weight;

    ACCESS3(_WRtPlus3, 0, 0, 0);
    ACCESS3(_WRtPlus3, 0, 0, 1);
    ACCESS3(_WRtPlus3, 0, 1, 0);
    ACCESS3(_WRtPlus3, 0, 1, 1);
    ACCESS3(_WRtPlus3, 1, 0, 0);
    ACCESS3(_WRtPlus3, 1, 0, 1);
    ACCESS3(_WRtPlus3, 1, 1, 0);
    ACCESS3(_WRtPlus3, 1, 1, 1);
}

static double
mkname(_cic_tuned_readout3) (PMeshPainter * painter, double pos[], double hsml)
{
    FLOAT * canvas = painter->canvas;

    SETUP_KERNEL_CIC(3);

    double value = 0;

    value += ACCESS3(_REd3, 0, 0, 0);
    value += ACCESS3(_REd3, 0, 0, 1);
    value += ACCESS3(_REd3, 0, 1, 0);
    value += ACCESS3(_REd3, 0, 1, 1);
    value += ACCESS3(_REd3, 1, 0, 0);
    value += ACCESS3(_REd3, 1, 0, 1);
    value += ACCESS3(_REd3, 1, 1, 0);
    value += ACCESS3(_REd3, 1, 1, 1);
    return value;
}

static void
mkname(_cic_tuned_paint2) (PMeshPainter * painter, double pos[], double weight, double hsml)
{
    FLOAT * canvas = painter->canvas;

    SETUP_KERNEL_CIC(2);

    V0[0] *= weight;
    V1[0] *= weight;

    ACCESS2(_WRtPlus2, 0, 0);
    ACCESS2(_WRtPlus2, 0, 1);
    ACCESS2(_WRtPlus2, 1, 0);
    ACCESS2(_WRtPlus2, 1, 1);
}

static double
mkname(_cic_tuned_readout2) (PMeshPainter * painter, double pos[], double hsml)
{
    FLOAT * canvas = painter->canvas;

    SETUP_KERNEL_CIC(2);

    double value = 0;

    value += ACCESS2(_REd2, 0, 0);
    value += ACCESS2(_REd2, 0, 1);
    value += ACCESS2(_REd2, 1, 0);
    value += ACCESS2(_REd2, 1, 1);
    return value;
}

static void
mkname(_cic_tuned_paint1) (PMeshPainter * painter, double pos[], double weight, double hsml)
{
    FLOAT * canvas = painter->canvas;

    SETUP_KERNEL_CIC(1);

    V0[0] *= weight;
    V1[0] *= weight;

    ACCESS1(_WRtPlus1, 0);
    ACCESS1(_WRtPlus1, 1);
}

static double
mkname(_cic_tuned_readout1) (PMeshPainter * painter, double pos[], double hsml)
{
    FLOAT * canvas = painter->canvas;

    SETUP_KERNEL_CIC(1);

    double value = 0;

    value += ACCESS1(_REd1, 0);
    value += ACCESS1(_REd1, 1);
    return value;
}


static int
mkname(_getfastmethod_cic) (PMeshPainter * painter, PMeshWindowInfo * window, paintfunc * fastpaint, readoutfunc * fastreadout)
{
    if(window->support != 2) return 0;

    if(painter->ndim == 1) {
        *fastpaint = mkname(_cic_tuned_paint1); \
        *fastreadout = mkname(_cic_tuned_readout1); \
        return 1;
    } 

    if(painter->ndim == 2) {
        *fastpaint = mkname(_cic_tuned_paint2); \
        *fastreadout = mkname(_cic_tuned_readout2); \
        return 1;
    } 
    if(painter->ndim == 3) {
        *fastpaint = mkname(_cic_tuned_paint3); \
        *fastreadout = mkname(_cic_tuned_readout3); \
        return 1;
    }
    return 0;
}

#undef SETUP_KERNEL_CIC

