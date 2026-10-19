// Grade: DIFFER(186v197) -- loop induction/register allocation: retail derives three element pointers (esi = &b[i], edi = &a[i].SpecularColour, ebx = &b[i].SpecularColour) and a frame delta a - b, the index in the dead `a` slot and the count in ebp; ours keeps the index in ebp and one delta-plus-pointer pair (tried indexed vector access, walking pointers, cached base pointers with indexing, declaration order)
// CEngineSurfaceCompositionManager::IsNearlyIdentical(const CEngineTextureMorph&, const CEngineTextureMorph&, float) -- two texture morphs are nearly identical when they have the same number of blends, each pair uses the same texture and both their diffuse and specular colours differ by no more than the tolerance
// Leaf: IsNearlyIdentical
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B2B050, 197 bytes (+ int3 padding), `ret 0Ch`. Mangled
//   ?IsNearlyIdentical@CEngineSurfaceCompositionManager@@AAE_NABVCEngineTextureMorph@@0M@Z (private).
// Callees: CEngineSurfaceCompositionManager::CalcColourDifference(const CRGBColour&, const CRGBColour&) 0x00B2AF50 (float; the
//   manifest label "$E2" is wrong, Ego_r.pdb names ?CalcColourDifference@CEngineSurfaceCompositionManager@@AAEMABVCRGBColour@@0@Z).
// Layout (Ego_r.pdb, retail agrees): CEngineTextureMorph {OriginalTextureIndex, MorphID, Blends +8 (first +8, last +0xc)};
//   CCompositeBlendDesc (16 bytes) {TextureIndex, BlendType, DiffuseColour +8, SpecularColour +0xc}. The manager's own members
//   are not read (engine/CEngineSurfaceCompositionManager.h describes the rest of `this`).
// Shape: the size test is `a.size() != b.size()` on 16-byte elements (`xor ; test 0FFFFFFF0h`); retail keeps the loop index
//   in the dead `a` argument slot, the count in ebp and strength-reduces the element addresses into b[i], b[i]+0xc, a[i]+0xc
//   pointers plus a frame delta; `d > tolerance` is `test ah,41h ; je`.
#include "engine/CEngineSurfaceCompositionManager.h"

struct CRGBColour { unsigned char B, G, R, A; };

struct CCompositeBlendDesc {                           // 16 bytes
    unsigned long TextureIndex;                        // +0x00
    long          BlendType;                           // +0x04 (ECompositeBlendType)
    CRGBColour    DiffuseColour;                       // +0x08
    CRGBColour    SpecularColour;                      // +0x0c
};
struct CBlendVector {                                  // vector<CCompositeBlendDesc> (retail 12 bytes)
    CCompositeBlendDesc* _M_start;
    CCompositeBlendDesc* _M_finish;
    CCompositeBlendDesc* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const CCompositeBlendDesc& operator[](unsigned long i) const { return _M_start[i]; }
};
struct CEngineTextureMorph {
    long         OriginalTextureIndex;                 // +0x00
    long         MorphID;                              // +0x04
    CBlendVector Blends;                               // +0x08
};

struct CEngineSurfaceCompositionManagerMethods : CEngineSurfaceCompositionManager {
    float CalcColourDifference(const CRGBColour& a, const CRGBColour& b);   // 0x00B2AF50
    bool IsNearlyIdentical(const CEngineTextureMorph& a, const CEngineTextureMorph& b, float tolerance);
};

bool CEngineSurfaceCompositionManagerMethods::IsNearlyIdentical(const CEngineTextureMorph& a, const CEngineTextureMorph& b, float tolerance)
{
    if (a.Blends.size() != b.Blends.size())
        return false;
    unsigned long count = a.Blends.size();
    const CCompositeBlendDesc* blendA = a.Blends._M_start;
    const CCompositeBlendDesc* blendB = b.Blends._M_start;
    for (unsigned long i = 0; i < count; ++i) {
        if (blendA[i].TextureIndex != blendB[i].TextureIndex)
            return false;
        if (CalcColourDifference(blendA[i].DiffuseColour, blendB[i].DiffuseColour) > tolerance)
            return false;
        if (CalcColourDifference(blendA[i].SpecularColour, blendB[i].SpecularColour) > tolerance)
            return false;
    }
    return true;
}
