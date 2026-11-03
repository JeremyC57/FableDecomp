#ifndef FABLE_ENGINE_CRGBFLOATCOLOUR_H
#define FABLE_ENGINE_CRGBFLOATCOLOUR_H
#include "rebuild_abi.h"
// Original Ego_r TPI0x2cd2; native colours at1ac/1bc, four float fields each.
class CRGBFloatColour {
public:
    float R,G,B,A;
    CRGBFloatColour();
    CRGBFloatColour(float r,float g,float b,float a) : R(r),G(g),B(b),A(a) {}
    void Initialise(float r,float g,float b,float a) { R=r;G=g;B=b;A=a; }
};
FABLE_STATIC_ASSERT(sizeof(CRGBFloatColour)==16);
#endif
