#ifndef FABLE_ENGINE_C4DVECTOR_H
#define FABLE_ENGINE_C4DVECTOR_H
// Layout: original complete PDB fields; native sequence stride/offsets agree.
#include "engine/C3DVector.h"
class C4DVector { public:float X,Y,Z,W; C4DVector(){}
 C4DVector(float x,float y,float z,float w):X(x),Y(y),Z(z),W(w){}
 C4DVector(const C3DVector& v,float w):X(v.X),Y(v.Y),Z(v.Z),W(w){}
#if defined(FABLE_RETAIL_ANIMATION_RETRY) || defined(FABLE_RETAIL_PARTICLE_PROGRAM_TRANSFORM)
 // Original lib_4d_vector.hpp22, public (const C3DVector&,float).
 void Initialise(const C3DVector& vect,float w){X=vect.X;Y=vect.Y;Z=vect.Z;W=w;}
#endif
 C3DVector ToC3DVector()const{return C3DVector(X,Y,Z);}
};
#endif
