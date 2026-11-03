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
    // Retail branch (chunk 723b water passes; Codex pass 13 rows 0x00989CB0 / 0x0098ACF0 / 0x009DA9F0): Ego_r.pdb
    // ??0C4DVector@@QAE@ABVCRGBFloatColour@@@Z (X=R, Y=G, Z=B, W=A; defined in engine/CRGBFloatColour.h),
    // ?Initialise@C4DVector@@QAEXMMMM@Z and ??DC4DVector@@QBE?BV0@M@Z (component-wise scale; CShaderRenderManager::
    // UpdateObjectSpaceLighting 0x00989CB0 scales the attenuation pairs), all inline in retail (no out-of-line copy).
    inline C4DVector(const CRGBFloatColour& colour);
    void Initialise(float x, float y, float z, float w) { X = x; Y = y; Z = z; W = w; }
    const C4DVector operator*(float sc) const { return C4DVector(X*sc,Y*sc,Z*sc,W*sc); }
    // Chunk 733 (CUserProfileManager::RenderUIBackground 0x004075C0): FableWin.pdb ??HC4DVector@@QBE?BV0@ABV0@@Z and
    // ??GC4DVector@@QBE?BV0@ABV0@@Z, the component-wise sum and difference (const members returning const C4DVector, inline).
    const C4DVector operator+(const C4DVector& v) const { return C4DVector(X+v.X,Y+v.Y,Z+v.Z,W+v.W); }
    const C4DVector operator-(const C4DVector& v) const { return C4DVector(X-v.X,Y-v.Y,Z-v.Z,W-v.W); }
#endif
};
#endif
