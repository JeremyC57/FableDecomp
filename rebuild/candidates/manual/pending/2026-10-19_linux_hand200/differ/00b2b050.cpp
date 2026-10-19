// Grade: DIFFER(ours v retail) -- 208v197: register roles of the two morphs are swapped (ours holds a in edi and walks a's blends; retail holds a in eax, walks b's blends and indexes a by the a-b offset), so the prologue loads and the loop set-up differ; tried named element references and swapped size-compare operands
// CEngineSurfaceCompositionManager::IsNearlyIdentical(const CEngineTextureMorph& a, const CEngineTextureMorph& b, float tolerance) -- two texture morphs match when they blend the same textures in the same order with diffuse and specular colours each within `tolerance`
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B2B050, 197 bytes (+ int3 padding), `ret 0Ch`.
// Callees: the colour-distance helper 0x00B2AF50 (catalogued "$E2"; a `__thiscall` member that ignores `this`: the
//   Euclidean distance between the four byte channels of two CRGBColour, `ret 8`).
// Shape: CEngineTextureMorph (Ego_r.pdb: OriginalTextureIndex +0, MorphID +4, Blends +8, a 12-byte retail STLport vector
//   of 16-byte CCompositeBlendDesc {TextureIndex, BlendType, DiffuseColour +8, SpecularColour +0xC}). VC folds
//   `a.Blends.size() != b.Blends.size()` into `xor` of the two byte lengths tested against 0xFFFFFFF0. The index is
//   unsigned (`jb`/`jbe` against the size_type count) and lives in the dead `a` argument slot; `this` is spilled into the
//   `push ecx` frame word for the two helper calls; `dist > tolerance` is `fcomp ; test ah,41h ; je` to the false exit.
//   BlendType is not compared. The helper's body is defined above the caller under `auto_inline(off)` (the retail
//   neighbour order): it is pure, so the starts, the a-b offset and the count stay in registers across the calls.
// Leaf: IsNearlyIdentical
#include "engine/CEngineSurfaceCompositionManager.h"

extern "C" double __cdecl sqrt(double x);
#pragma intrinsic(sqrt)

struct CRGBColour { unsigned char B, G, R, A; };
struct CCompositeBlendDesc {
    unsigned long TextureIndex;                     // +0x0
    long          BlendType;                        // +0x4 ECompositeBlendType
    CRGBColour    DiffuseColour;                    // +0x8
    CRGBColour    SpecularColour;                   // +0xC
};
template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    const T& operator[](unsigned int index) const { return _M_start[index]; }
};
struct CEngineTextureMorph {
    long                         OriginalTextureIndex;   // +0x0
    long                         MorphID;                // +0x4
    vector<CCompositeBlendDesc>  Blends;                 // +0x8
};

struct CEngineSurfaceCompositionManager_Methods : CEngineSurfaceCompositionManager {
    float GetColourDistance(const CRGBColour& a, const CRGBColour& b) const;   // 0x00B2AF50
    bool IsNearlyIdentical(const CEngineTextureMorph& a, const CEngineTextureMorph& b, float tolerance) const;
};

#pragma auto_inline(off)
// Same-TU body of the helper (0x00B2AF50 sits directly above): seeing that it only reads lets VC keep both blend
// arrays' pointers and the count in registers across the calls. Not part of the graded symbol.
float CEngineSurfaceCompositionManager_Methods::GetColourDistance(const CRGBColour& a, const CRGBColour& b) const
{
    float dr = (float)((long)a.R - (long)b.R);
    float dg = (float)((long)a.G - (long)b.G);
    float db = (float)((long)a.B - (long)b.B);
    float da = (float)((long)a.A - (long)b.A);
    return (float)sqrt(da * da + db * db + dg * dg + dr * dr);
}
#pragma auto_inline(on)

bool CEngineSurfaceCompositionManager_Methods::IsNearlyIdentical(const CEngineTextureMorph& a, const CEngineTextureMorph& b, float tolerance) const
{
    if (a.Blends.size() != b.Blends.size())
        return false;
    for (unsigned int i = 0; i < a.Blends.size(); ++i) {
        if (a.Blends[i].TextureIndex != b.Blends[i].TextureIndex)
            return false;
        if (GetColourDistance(a.Blends[i].DiffuseColour, b.Blends[i].DiffuseColour) > tolerance)
            return false;
        if (GetColourDistance(a.Blends[i].SpecularColour, b.Blends[i].SpecularColour) > tolerance)
            return false;
    }
    return true;
}
