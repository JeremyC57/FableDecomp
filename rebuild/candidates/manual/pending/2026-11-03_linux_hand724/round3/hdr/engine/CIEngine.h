#ifndef FABLE_ENGINE_CIENGINE_H
#define FABLE_ENGINE_CIENGINE_H
#ifdef FABLE_RETAIL_CIENGINE
// Retail opt-in (chunk 725): the polymorphic engine interface. RTTI .?AVCIEngine@@ : CInitBaseClass : CBaseClass, vtable
// 0x012A0D24 (implementation CEngine 0x012A0F3C). Slots 0-16 below; the rest are not modelled yet. Names: FableWin.pdb
// CIEngine / CEngine members, placed by the retail vtables (CIEngine's own bodies 0x00B232D0.. and CEngine's overrides,
// catalog-named) and their `ret N`: 2 Initialise 0x00B232D0, 3 Release 0x00B232E0, 4 SetCamera (CEngine 0x00B23B50,
// ret 4), 5 SetWindow (CEngine 0x00B23B80, ret 4; FableWin declares it right after SetCamera), 6
// SetEnvironmentalEffectsAlwaysUpdate 0x00B232F0, 7 SetWorld 0x00B23300, 9 WorldFrameUpdate 0x00B23310, 10
// PausedWorldFrameUpdate 0x00B23320, 14 SetVideoOptions 0x00B23330, 15 PreChangeResolution / 16 PostChangeResolution
// (_purecall here; CEngine 0x00B2AA20 / 0x00B2AA30 = FableWin ?PreChangeResolution@CEngine@@UAE_NXZ /
// ?PostChangeResolution@CEngine@@UAE_NKKK@Z; called by CDisplayEngine 0x00434A10 / 0x00434A20). Slots 8, 11, 12, 13 are
// pure here; FableWin.pdb names them on CEngine in this order (ret sizes agree): 8 Render(const CEngineInterpolationInfo&,
// unsigned long) 0x00B27D90 (ret 8), 11 ConvertScreenPosToWorldPos 0x00B23CF0 (ret 8, catalog agrees), 12
// ConvertScreenPosToWorldPosAtViewDistance 0x00B24400 (ret 0xC), 13 ConvertWorldPosToScreenPos 0x00B242D0 (ret 8).
// Slots 17-22 (chunk 724b, CIEngine bodies, FableWin ?<name>@CIEngine@@ spellings): 17 Refresh 0x00B23340, 18
// CheckPrimitiveVisibility(unsigned __int64) 0x00B23350, 19 GetPrimitiveDrawDistanceFactor() const 0x00B23360, 20
// AddPrimitive(const CEnginePrimitive&) 0x00B23370 (NCameraModes::DrawSquare 0x00875FB0 calls [vtbl+0x50]), 21 ClipSphere
// 0x00B23380, 22 ClipBox 0x00B23390. Later slots are not modelled.
// Layout: CInitBaseClass (vptr, Valid +4), Active +8; sizeof 0xC (Ego_r).
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CInitBaseClass.h"
class C2DBoxF;
class C2DVector;
class C3DBoundingBox;
class C3DBoundingSphere;
class CEngineInterpolationInfo;
class CEnginePrimitive;
class C3DVector;
class CCamera;
class CEngineInit;
class CEngineVideoOptions;
class CIDrawWorld;
class CEnginePrimitiveHandle;
class CEnginePrimitiveAnimation;
class CEnginePrimitivePosition;
class CIEngine : public CInitBaseClass {
public:
    virtual ~CIEngine();                                                   //  0 ??_G 0x00B23770
    // slot 1: CInitBaseClass::Validate 0x0099A340 (not declared by CInitBaseClass.h; modelled here at its slot)
    virtual void Validate() const;                                        //  1
    virtual bool Initialise(const CEngineInit&);                          //  2 0x00B232D0
    virtual void Release();                                               //  3 0x00B232E0
    virtual void SetCamera(const CCamera*) = 0;                           //  4
    virtual void SetWindow(const C2DBoxF*) = 0;                           //  5
    virtual void SetEnvironmentalEffectsAlwaysUpdate(bool);               //  6 0x00B232F0
    virtual void SetWorld(const CIDrawWorld*);                            //  7 0x00B23300
    virtual void Render(const CEngineInterpolationInfo&, unsigned long) = 0;   //  8 (CEngine 0x00B27D90, ret 8)
    virtual void WorldFrameUpdate(bool);                                  //  9 0x00B23310
    virtual void PausedWorldFrameUpdate();                                // 10 0x00B23320
    virtual bool ConvertScreenPosToWorldPos(const C2DVector&, C3DVector&) const = 0;   // 11 (CEngine 0x00B23CF0, ret 8)
    virtual bool ConvertScreenPosToWorldPosAtViewDistance(float, const C2DVector&, C3DVector&) const = 0;   // 12 (CEngine 0x00B24400, ret 0xC)
    virtual bool ConvertWorldPosToScreenPos(const C3DVector&, C2DVector&) const = 0;   // 13 (CEngine 0x00B242D0, ret 8)
    virtual bool SetVideoOptions(const CEngineVideoOptions&);             // 14 0x00B23330
    virtual bool PreChangeResolution() = 0;                               // 15 (CEngine 0x00B2AA20)
    virtual bool PostChangeResolution(unsigned long, unsigned long, unsigned long) = 0;   // 16 (CEngine 0x00B2AA30)
    virtual void Refresh();                                               // 17 0x00B23340
    virtual bool CheckPrimitiveVisibility(unsigned __int64);              // 18 0x00B23350
    virtual float GetPrimitiveDrawDistanceFactor() const;                 // 19 0x00B23360
    virtual void AddPrimitive(const CEnginePrimitive&);                   // 20 0x00B23370
    virtual bool ClipSphere(const C3DBoundingSphere&) const;              // 21 0x00B23380
    virtual bool ClipBox(const C3DBoundingBox&) const;                    // 22 0x00B23390
    // chunk 724c: slots 23..30 (CIEngine vtable bodies 0x00B233A0..; FableWin ?<name>@CIEngine@@UAE... spellings, whose argument
    // counts 0x00B1F200 confirms: AddChildPrimitive 3, SetChildPrimitive 4 and 5). SetPersistentPrimitive (23) is the catalog
    // name of CEngine's slot-23 body 0x00B23BC0; its signature is not modelled.
    virtual void SetPersistentPrimitive();                                // 23
    virtual void SetPrimitiveAnimation(CEnginePrimitiveHandle&, const CEnginePrimitiveAnimation&);   // 24 0x00B233A0
    virtual void SetPrimitivePosition(CEnginePrimitiveHandle&, const CEnginePrimitivePosition&);     // 25 0x00B233B0
    virtual unsigned long AddChildPrimitive(CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned long);   // 26 0x00B233C0
    // MSVC emits an overload set in reverse declaration order: the 4-argument form lands in slot 28, the 5-argument in 27.
    virtual void SetChildPrimitive(CEnginePrimitiveHandle&, unsigned long, const CEnginePrimitive&, unsigned long);   // 28 0x00B233D0
    virtual void SetChildPrimitive(CEnginePrimitiveHandle&, unsigned long, CEnginePrimitiveHandle&, const CEnginePrimitive&,
        unsigned long);                                                   // 27 0x00B233E0
    virtual void GetChildPrimitiveHandle(CEnginePrimitiveHandle&, unsigned long, CEnginePrimitiveHandle&);   // 29 0x00B233F0
    virtual void ClearChildPrimitive(CEnginePrimitiveHandle&, unsigned long);                         // 30 0x00B23400
    bool Active;                                                          // +0x08
};
FABLE_STATIC_ASSERT(sizeof(CIEngine) == 0xc);
#else
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CIEngine = 0xc.
// Retail evidence: 3 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

#pragma pack(push, 1)
class CIEngine {
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char _pad_0x04[0x4];                            // +0x04
    bool          Active;                                    // +0x08
    unsigned char _pad_0x09[0x3];                            // +0x09
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CIEngine) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CIEngine, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CIEngine, Active) == 0x8);

#endif // FABLE_RETAIL_CIENGINE
#endif // FABLE_ENGINE_CIENGINE_H
