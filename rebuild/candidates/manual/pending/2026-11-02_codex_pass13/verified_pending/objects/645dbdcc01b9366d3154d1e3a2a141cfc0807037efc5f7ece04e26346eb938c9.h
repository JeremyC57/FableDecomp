#ifndef FABLE_ENGINE_C4DVECTOR_H
#define FABLE_ENGINE_C4DVECTOR_H
#include "rebuild_abi.h"
#ifdef FABLE_RETAIL_SHADER_LIGHTING
#include "engine/C3DVector.h"
#endif
// Original Ego_r TPI0xc3e2; native vector stride16 and field stores in0098acf0.
class C4DVector {
public:
#ifdef FABLE_RETAIL_SHADER_LIGHTING
    C4DVector() {}
    void Initialise(const C3DVector& vect,float w) {X=vect.X;Y=vect.Y;Z=vect.Z;W=w;}
    void Initialise(float x,float y,float z,float w) {X=x;Y=y;Z=z;W=w;}
    C4DVector(const C3DVector& v,float w):X(v.X),Y(v.Y),Z(v.Z),W(w){}
    const C4DVector operator*(float sc) const {return C4DVector(X*sc,Y*sc,Z*sc,W*sc);}
#else
    C4DVector();
#endif
    C4DVector(float x,float y,float z,float w) : X(x),Y(y),Z(z),W(w) {}
    float X,Y,Z,W;
};
FABLE_STATIC_ASSERT(sizeof(C4DVector)==16);
#endif
