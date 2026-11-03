#ifndef FABLE_ENGINE_C4DVECTOR_H
#define FABLE_ENGINE_C4DVECTOR_H
// C4DVector (Ego_r TPI: public float X,Y,Z,W at 0/4/8/0xC, size 0x10). Ctors ??0C4DVector@@QAE@XZ, @MMMM@Z, @ABVC3DVector@@M@Z
// (FableWin); Initialise(const C3DVector&, float) ?Initialise@C4DVector@@QAEXABVC3DVector@@M@Z and ToC3DVector
// ?ToC3DVector@C4DVector@@QBE?AVC3DVector@@XZ (FableWin) are inline in retail (CParticleSimulation's particle accessors,
// 0x00AE0070 / 0x00AE0710 read Positions[i] with stride 0x10).
#include "engine/C3DVector.h"
class C4DVector {
public:
    float X,Y,Z,W;
    C4DVector() { }
    C4DVector(float x,float y,float z,float w) : X(x),Y(y),Z(z),W(w) { }
    C4DVector(const C3DVector& v,float w) : X(v.X),Y(v.Y),Z(v.Z),W(w) { }
    void Initialise(const C3DVector& vect,float w) { X=vect.X; Y=vect.Y; Z=vect.Z; W=w; }
    C3DVector ToC3DVector() const { return C3DVector(X,Y,Z); }
};
#endif
