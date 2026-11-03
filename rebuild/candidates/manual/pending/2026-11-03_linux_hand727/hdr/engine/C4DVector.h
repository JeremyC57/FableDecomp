#ifndef FABLE_ENGINE_C4DVECTOR_H
#define FABLE_ENGINE_C4DVECTOR_H
// C4DVector (Ego_r TPI: public float X,Y,Z,W at 0/4/8/0xC, size 0x10). Ctors ??0C4DVector@@QAE@XZ, @MMMM@Z, @ABVC3DVector@@M@Z
// (FableWin); Initialise(const C3DVector&, float) ?Initialise@C4DVector@@QAEXABVC3DVector@@M@Z and ToC3DVector
// ?ToC3DVector@C4DVector@@QBE?AVC3DVector@@XZ (FableWin) are inline in retail (CParticleSimulation's particle accessors,
// 0x00AE0070 / 0x00AE0710 read Positions[i] with stride 0x10).
#include "engine/C3DVector.h"
#ifdef FABLE_RETAIL_C4DVECTOR
class CRGBFloatColour;
#endif
class C4DVector {
public:
    float X,Y,Z,W;
    C4DVector() { }
    C4DVector(float x,float y,float z,float w) : X(x),Y(y),Z(z),W(w) { }
    C4DVector(const C3DVector& v,float w) : X(v.X),Y(v.Y),Z(v.Z),W(w) { }
    void Initialise(const C3DVector& vect,float w) { X=vect.X; Y=vect.Y; Z=vect.Z; W=w; }
    C3DVector ToC3DVector() const { return C3DVector(X,Y,Z); }
#ifdef FABLE_RETAIL_C4DVECTOR
    // Retail opt-in (chunk 723b, the water passes): Ego_r.pdb ??0C4DVector@@QAE@ABVCRGBFloatColour@@@Z (X=R, Y=G, Z=B, W=A;
    // defined in engine/CRGBFloatColour.h) and ?Initialise@C4DVector@@QAEXMMMM@Z, inline everywhere.
    inline C4DVector(const CRGBFloatColour& colour);
    void Initialise(float x, float y, float z, float w) { X = x; Y = y; Z = z; W = w; }
    // Chunk 727 (C3DAnimationManager, the partial-animation blend 0x00A9DE50): FableWin.pdb ??HC4DVector@@QBE?BV0@ABV0@@Z,
    // ??GC4DVector@@QBE?BV0@ABV0@@Z, ??DC4DVector@@QBE?BV0@M@Z and ?Interpolate@C4DVector@@QBE?BV1@ABV1@M@Z, inline in retail
    // (the difference, its scale by t and the sum each land in a 16-byte frame temporary, then the result is copied).
    __forceinline const C4DVector operator+(const C4DVector& v) const { return C4DVector(X + v.X, Y + v.Y, Z + v.Z, W + v.W); }
    __forceinline const C4DVector operator-(const C4DVector& v) const { return C4DVector(X - v.X, Y - v.Y, Z - v.Z, W - v.W); }
    __forceinline const C4DVector operator*(float s) const { return C4DVector(X * s, Y * s, Z * s, W * s); }
    __forceinline const C4DVector Interpolate(const C4DVector& v, float t) const { return *this + (v - *this) * t; }
#endif
};
#endif
