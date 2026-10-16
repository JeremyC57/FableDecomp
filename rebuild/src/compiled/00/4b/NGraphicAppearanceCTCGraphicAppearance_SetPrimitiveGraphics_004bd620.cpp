// NGraphicAppearance::CTCGraphicAppearance::SetPrimitiveGraphics(CPrimitive*, const CEnginePrimitive&, unsigned short) -- send an engine primitive to the scene manager: for a tracked primitive with the appearance's persistence/colour-filter/occlusion/after-water flags, else as a one-off
// Leaf: SetPrimitiveGraphics
//
// Retail 0x004BD620, 112 bytes (`ret 0Ch`). Ego_r.pdb `?SetPrimitiveGraphics@CTCGraphicAppearance@
// NGraphicAppearance@@QAEXPAVCPrimitive@2@ABVCEnginePrimitive@@G@Z`. Callees: CTCGraphicAppearance::
// IsDrawingAfterWater() const @ 0x004BD3C0 (`this` still in ecx), the scene manager's virtual slots 23
// (+0x5c, primitive handle, primitive, flags) and 20 (+0x50, primitive only); the manager is +0x34 of
// the engine object the global @ 0x013B8A18 points to.
// Layout: retail CTCGraphicAppearance (PDB -0x18): ActivePersistenceFlags +0x52, the flag bit byte +0x59
// (bit 3 DisableOcclusionTesting, bit 4 DrawAfterWater), ByPassColorFilter +0x5b (signed char);
// CPrimitive's engine handle at +4.
// Shape: the flags are an `unsigned short` local (`xor esi,esi ; mov si ; or si`, the 32-bit `or`s, the
// `movzx` for the unsigned long parameter); each arm keeps its own `ret 0Ch`.
struct CEnginePrimitive;
struct CEnginePrimitiveHandle {                             // 8 bytes: vtable + counted data pointer
    void* _vtable;
    void* Data;
};

struct CEngineSceneManager {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void SetPrimitive(const CEnginePrimitive& primitive);   // slot 20 (+0x50)
    virtual void v21(); virtual void v22();
    virtual void SetPersistentPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, unsigned long flags);   // slot 23 (+0x5c)
};

struct CEngineGlobals {
    unsigned char        _pad00[0x34];
    CEngineSceneManager* PSceneManager;                     // +0x34
};
extern CEngineGlobals* g_pEngineGlobals;                    // 0x013B8A18

namespace NGraphicAppearance {

struct CPrimitive {
    void*                  _vtable;                         // +0
    CEnginePrimitiveHandle Handle;                          // +4
};

struct CTCGraphicAppearance {
    unsigned char  _pad00[0x52];
    unsigned short ActivePersistenceFlags;                  // +0x52
    unsigned char  _pad54[0x59 - 0x54];
    unsigned char  SelfIlluminating : 1;                    // +0x59 bit 0
    unsigned char  ForceResendEveryRenderFrame : 1;         //        bit 1
    unsigned char  ResendPrimsEveryRenderFrame : 1;         //        bit 2
    unsigned char  DisableOcclusionTesting : 1;             //        bit 3
    unsigned char  DrawAfterWater : 1;                      //        bit 4
    unsigned char  CachedStaticFocalPos : 1;
    unsigned char  RenderedLastFrame : 1;
    unsigned char  PrimitivesDrawn : 1;
    unsigned char  Alpha;                                   // +0x5a
    signed char    ByPassColorFilter;                       // +0x5b
    bool IsDrawingAfterWater() const;                       // 0x004BD3C0
    void SetPrimitiveGraphics(CPrimitive* primitive, const CEnginePrimitive& enginePrimitive, unsigned short flags);
};

void CTCGraphicAppearance::SetPrimitiveGraphics(CPrimitive* primitive, const CEnginePrimitive& enginePrimitive, unsigned short flags)
{
    if (primitive)
    {
        unsigned short allFlags = ActivePersistenceFlags | flags;
        if (ByPassColorFilter > 0)
            allFlags |= 0x800;
        if (DisableOcclusionTesting)
            allFlags |= 0x4000;
        if (IsDrawingAfterWater())
            allFlags |= 8;
        g_pEngineGlobals->PSceneManager->SetPersistentPrimitive(primitive->Handle, enginePrimitive, allFlags);
    }
    else
        g_pEngineGlobals->PSceneManager->SetPrimitive(enginePrimitive);
}

}