#ifndef FABLE_ENGINE_FONT2DVERTEX_H
#define FABLE_ENGINE_FONT2DVERTEX_H
#ifdef FABLE_RETAIL_FONT2DVERTEX_PASS28
#include "D3DXVECTOR4.h"
// Layout: Ego_r FONT2DVERTEX and FableWin InitFont2DVertex independently give p at 0, color at 16, tu at 20 and tv at 24.
struct FONT2DVERTEX {
    D3DXVECTOR4 p;
    unsigned long color;
    float tu;
    float tv;
};
#endif
#endif
