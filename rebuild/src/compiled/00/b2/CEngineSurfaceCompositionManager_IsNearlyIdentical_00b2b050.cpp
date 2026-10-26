// CEngineSurfaceCompositionManager::IsNearlyIdentical(const CEngineTextureMorph& a, const CEngineTextureMorph& b, float tolerance) -- two texture morphs match when they blend the same textures in the same order with diffuse and specular colours each within `tolerance`
// Leaf: ?IsNearlyIdentical@CEngineSurfaceCompositionManager_Methods@@QBE_NABVCEngineTextureMorph@@0M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B2B050, 197 bytes (+ int3 padding), `ret 0Ch`; Ego_r.pdb names it
//   ?IsNearlyIdentical@CEngineSurfaceCompositionManager@@AAE_NABVCEngineTextureMorph@@0M@Z.
// Callees: the colour-distance helper 0x00B2AF50 (catalogued CalcColourDifference; a `__fastcall` member that ignores `this`: the
//   Euclidean distance between the four byte channels of two CRGBColour, `ret 8`).
// Shape: CEngineTextureMorph (Ego_r.pdb: OriginalTextureIndex +0, MorphID +4, Blends +8, a 12-byte retail STLport vector
//   of 16-byte CCompositeBlendDesc {TextureIndex, BlendType, DiffuseColour +8, SpecularColour +0xC}). VC folds
//   `a.Blends.size() != b.Blends.size()` into `xor` of the two byte lengths tested against 0xFFFFFFF0. The count is
//   read once after that test; each pass names the two blends (a's, then b's), which makes b's blend the walked pointer
//   and a's the a-b offset (unnamed indexing walks a). The index lives in the dead `a` argument slot, the offset in the
//   dead `b` slot; `this` is spilled into the `push ecx` frame word for the two helper calls; `dist > tolerance` is
//   `fcomp ; test ah,41h ; je` to the false exit. BlendType is not compared. The helper's body is defined above the
//   caller under `auto_inline(off)` (the retail neighbour order): it is pure, so the pointers and the count stay in
//   registers across the calls.
#include "engine/CEngineSurfaceCompositionManager.h"

extern "C" double __cdecl sqrt(double x);
#pragma intrinsic(sqrt)

class CRGBColour { public: unsigned char B, G, R, A; };
class CCompositeBlendDesc { public:
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
class CEngineTextureMorph { public:
    long                         OriginalTextureIndex;   // +0x0
    long                         MorphID;                // +0x4
    vector<CCompositeBlendDesc>  Blends;                 // +0x8
};

struct CEngineSurfaceCompositionManager_Methods : CEngineSurfaceCompositionManager {
    float CalcColourDifference(const CRGBColour& a, const CRGBColour& b) const;   // 0x00B2AF50
    bool IsNearlyIdentical(const CEngineTextureMorph& a, const CEngineTextureMorph& b, float tolerance) const;
};

#pragma auto_inline(off)
// Same-TU body of the helper (0x00B2AF50 sits directly above): seeing that it only reads lets VC keep both blend
// arrays' pointers and the count in registers across the calls. Not part of the graded symbol.
float CEngineSurfaceCompositionManager_Methods::CalcColourDifference(const CRGBColour& a, const CRGBColour& b) const
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
    unsigned int count = a.Blends.size();
    for (unsigned int i = 0; i < count; ++i) {
        const CCompositeBlendDesc& blendA = a.Blends[i];
        const CCompositeBlendDesc& blendB = b.Blends[i];
        if (blendA.TextureIndex != blendB.TextureIndex)
            return false;
        if (CalcColourDifference(blendA.DiffuseColour, blendB.DiffuseColour) > tolerance)
            return false;
        if (CalcColourDifference(blendA.SpecularColour, blendB.SpecularColour) > tolerance)
            return false;
    }
    return true;
}