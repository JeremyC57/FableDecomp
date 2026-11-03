#ifndef FABLE_ENGINE_C4DVECTOR_H
#define FABLE_ENGINE_C4DVECTOR_H
#include "rebuild_abi.h"
// Original Ego_r TPI0xc3e2; native vector stride16 and field stores in0098acf0.
class C4DVector {
public:
    C4DVector();
    C4DVector(float x,float y,float z,float w) : X(x),Y(y),Z(z),W(w) {}
    float X,Y,Z,W;
};
FABLE_STATIC_ASSERT(sizeof(C4DVector)==16);
#endif
