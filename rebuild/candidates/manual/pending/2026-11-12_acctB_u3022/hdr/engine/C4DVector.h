#ifndef FABLE_ENGINE_C4DVECTOR_H
#define FABLE_ENGINE_C4DVECTOR_H
#include "fable_compiler.h"
// C4DVector (Ego_r TPI: public float X,Y,Z,W at 0/4/8/0xC, size 0x10). Ctors ??0C4DVector@@QAE@XZ, @MMMM@Z, @ABVC3DVector@@M@Z
// (FableWin); Initialise(const C3DVector&, float) ?Initialise@C4DVector@@QAEXABVC3DVector@@M@Z and ToC3DVector
// ?ToC3DVector@C4DVector@@QBE?AVC3DVector@@XZ (FableWin) are inline in retail (CParticleSimulation's particle accessors,
// 0x00AE0070 / 0x00AE0710 read Positions[i] with stride 0x10).
#include "engine/C3DVector.h"
#ifdef FABLE_RETAIL_C4DVECTOR
class CRGBFloatColour;
#endif
#ifdef FABLE_RETAIL_C4DVECTOR_RGBCOLOUR
class CRGBColour;
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
    // Retail branch (chunk 723b water passes; Codex pass 13 rows 0x00989CB0 / 0x0098ACF0 / 0x009DA9F0): Ego_r.pdb
    // ??0C4DVector@@QAE@ABVCRGBFloatColour@@@Z (X=R, Y=G, Z=B, W=A; defined in engine/CRGBFloatColour.h),
    // ?Initialise@C4DVector@@QAEXMMMM@Z and ??DC4DVector@@QBE?BV0@M@Z (component-wise scale; CShaderRenderManager::
    // UpdateObjectSpaceLighting 0x00989CB0 scales the attenuation pairs), all inline in retail (no out-of-line copy).
    inline C4DVector(const CRGBFloatColour& colour);
    void Initialise(float x, float y, float z, float w) { X = x; Y = y; Z = z; W = w; }
#if defined(FABLE_RETAIL_C4DVECTOR_NAMED_RESULT) || defined(FABLE_RETAIL_C4DVECTOR_NAMED_SCALE)
    // Retail opt-in (chunk 2044, row 0x00AA0090): FableWin.pdb's /Od copies of ??DC4DVector@@QBE?BV0@M@Z / ??GC4DVector@@QBE?BV0@ABV0@@Z /
    // ??HC4DVector@@QBE?BV0@ABV0@@Z are 38i / 42i / 42i (101 / 112 / 112 bytes), exactly a named result returned through the
    // copy (VC7.1 /Od of `return C4DVector(...)` is 30i / 34i / 34i); release bytes unchanged, extra IL folds away.
    const C4DVector operator*(float sc) const { C4DVector result(X*sc,Y*sc,Z*sc,W*sc); return result; }
#else
    const C4DVector operator*(float sc) const { return C4DVector(X*sc,Y*sc,Z*sc,W*sc); }
#endif
    // chunk 747 (row 0x00AA3C50 C3DKeyframe::BlendWith): FableWin.pdb ??GC4DVector@@QBE?BV0@ABV0@@Z / ??HC4DVector@@QBE?BV0@ABV0@@Z
    // and ?Interpolate@C4DVector@@QBE?BV1@ABV1@M@Z, inline in retail: BlendWith builds (v - *this) in a frame temporary, scales it
    // into a second one and adds *this (three temporaries, `fsub` / `fmul [t]` / `fadd`). Interpolate is __forceinline: written
    // plain inline, VC7.1 emits it out of line in that caller.
#if defined(FABLE_RETAIL_C4DVECTOR_NAMED_RESULT) || defined(FABLE_RETAIL_C4DVECTOR_NAMED_ADDSUB)
    const C4DVector operator-(const C4DVector& v) const { C4DVector result(X-v.X,Y-v.Y,Z-v.Z,W-v.W); return result; }
    const C4DVector operator+(const C4DVector& v) const { C4DVector result(X+v.X,Y+v.Y,Z+v.Z,W+v.W); return result; }
#else
    const C4DVector operator-(const C4DVector& v) const { return C4DVector(X-v.X,Y-v.Y,Z-v.Z,W-v.W); }
    const C4DVector operator+(const C4DVector& v) const { return C4DVector(X+v.X,Y+v.Y,Z+v.Z,W+v.W); }
#endif
    FABLE_FORCEINLINE const C4DVector Interpolate(const C4DVector& v, float t) const { return (v - *this) * t + *this; }
#endif
#ifdef FABLE_RETAIL_C4DVECTOR_ADDASSIGN
    // Retail opt-in (chunk 2005, row 0x00A4D920): FableWin.pdb ??YC4DVector@@QAEABV0@ABV0@@Z (expanded in retail).
    const C4DVector& operator+=(const C4DVector& v) { X += v.X; Y += v.Y; Z += v.Z; W += v.W; return *this; }
#endif
#ifdef FABLE_RETAIL_C4DVECTOR_ASSIGN3D
    // Retail opt-in (chunk 2005, row 0x00A4D920): FableWin.pdb ??4C4DVector@@QAEABV0@ABVC3DVector@@@Z (expanded in retail):
    // X/Y/Z copied, W cleared (every position path of the inlined keyframe interpolation stores W = 0, 0x00A4DDC7).
    const C4DVector& operator=(const C3DVector& v) { X = v.X; Y = v.Y; Z = v.Z; W = 0.0f; return *this; }
#endif
#ifdef FABLE_RETAIL_C4DVECTOR_RGBCOLOUR
    // Chunk 756 (row 0x00BB17D0): FableWin.pdb ??0C4DVector@@QAE@ABVCRGBColour@@@Z (FableWin 0x017D4970 / 0x02D149F0), out of line
    // in retail at 0x00BB17D0 (`ret 4`, returns this): each byte channel times 1/255, X=R, Y=G, Z=B, W=A.
    C4DVector(const CRGBColour& colour);
#endif
};
#endif
