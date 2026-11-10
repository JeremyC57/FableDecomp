// Retail opt-in (Codex 2026-11-10): complete original types from both PDBs and native field/virtual offsets; see 00b396e0 NAMING_EVIDENCE.md.
#ifndef FABLE_ENGINE_CENGINEINTERNALPRIMITIVEBASE_H
#define FABLE_ENGINE_CENGINEINTERNALPRIMITIVEBASE_H
#if defined(FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED)
#include "engine/CEnginePersistentPrimitiveBase.h"
#include "engine/CEnginePrimitiveEnums.h"
#include "engine/CEngineFadeDistance.h"
#include "engine/CEngineSceneGridPos.h"
class CEngineSceneGridCell;
class CEnginePrimitive;
class CEngineCamera;
class CEnginePrimitiveAnimation;
class CEnginePrimitivePosition;
class CEngineMeshEffect;
class CEngineInternalPrimitiveOcclusionBase;
class C3DVector;
class CMatrix3x4;
class C3DBoundingBox;
class C3DBoundingSphere;
template<class T>class CIVCountedPointer;
class CEngineInternalPrimitiveBase : public CEnginePersistentPrimitiveBase {
public:
 CEngineInternalPrimitiveBase(long,unsigned long);
 virtual ~CEngineInternalPrimitiveBase();
 virtual void Update(const CEnginePrimitive&,unsigned long);
 virtual unsigned long AddChildPrimitive(const CEnginePrimitive&,unsigned long);
 virtual void SetChildPrimitive(unsigned long,CEnginePrimitiveHandle&,const CEnginePrimitive&,unsigned long);
 virtual void SetChildPrimitive(unsigned long,const CEnginePrimitive&,unsigned long);
 virtual void GetChildPrimitiveHandle(unsigned long,CEnginePrimitiveHandle&);
 virtual void ClearChildPrimitive(unsigned long);
 virtual void ClearAllChildPrimitives();
 virtual CEnginePersistentPrimitiveBase* GetChildPrimitive(unsigned long);
 virtual CEngineFadeDistance GetFadeDistance()const;
 virtual void PostRender();
 virtual bool GetWorldPosition(C3DVector&);
 virtual bool GetBoundingBoxTransform(CMatrix3x4&);
 virtual bool GetBoundingBoxWorldSpace(C3DBoundingBox&);
 virtual bool GetBoundingSphereWorldSpace(C3DBoundingSphere&);
 virtual bool GetBoundingSphereRadiusWorldSpace(float&);
 virtual bool IsInsideFrustum(const CEngineCamera&,bool);
 virtual bool GetObjectTransform(CMatrix3x4&)const;
 virtual bool SetObjectTransform(const CMatrix3x4&);
 virtual void SetAnimation(const CEnginePrimitiveAnimation&);
 virtual void SetPosition(const CEnginePrimitivePosition&);
 virtual void SetMeshEffect(const CIVCountedPointer<CEngineMeshEffect>&);
 virtual bool GetMeshEffect(CIVCountedPointer<CEngineMeshEffect>&)const;
 virtual CEngineInternalPrimitiveOcclusionBase* GetAsOcclusionPrimitive();
 virtual void CollectStats(std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<const unsigned long,unsigned long> > >&,EEnginePrimitiveStatIndex,unsigned long)const;
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH
 void SetNonPersistent(bool non_persistent){NonPersistent=non_persistent;}
#endif
 void SetEnable(bool enable){Enable=enable;}
 unsigned long EnginePrimitiveFlags;
 float AlphaMipBias;
 CEngineFadeDistance FadeDistance;
 unsigned long OcclusionLayerBitMask;
 CEngineSceneGridCell* GridCell;
 CEngineSceneGridPos GridPos;
 unsigned char Alpha,AlphaRef,PreloadStat;
 EEngineInterpolationSet InterpolationSet;
protected:
 virtual void NotifyReleased(const CEnginePrimitiveHandle&);
 virtual bool WasRendered()const;
 unsigned long RenderLayerMask;
private:
 unsigned char Enable:1;
 unsigned char NonPersistent:1;
 unsigned char OverrideFadeDistance:1;
 unsigned char PostRenderEnable:1;
 unsigned short LastFrameRendered;
 CEngineInternalPrimitiveBase* NextPrimitive;
 CEngineInternalPrimitiveBase** RefPrimitive;
 CEngineInternalPrimitiveBase* NextLayerMask;
 CEngineInternalPrimitiveBase** RefLayerMask;
};
FABLE_STATIC_ASSERT(sizeof(CEngineInternalPrimitiveBase)==72);
#elif defined(FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CLASS)
// Retail opt-in (chunk 892, row 0x00BE1C10 CEngineInternalPrimitive2DViewport::Update): the polymorphic class, so a derived internal
// primitive can declare its own virtuals over it. RTTI .?AVCEngineInternalPrimitiveBase@@ : CEnginePersistentPrimitiveBase :
// CIVCountedPointeeBase (ghidra_out/rtti_bases.tsv); the own members in Ego_r.pdb order, names and offsets (the flattened model
// below, typed: CEngineFadeDistance / CEngineSceneGridPos as their PDB members, the bitfield group as its uchar:1 word).
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#endif
#ifndef FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#endif
#include "rebuild_abi.h"
#include "engine/CEnginePersistentPrimitiveBase.h"
class CEngineSceneGridCell;
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_OCCLUSION
#include "engine/EEngineInterpolationSet.h"
#include "engine/CEngineFadeDistance.h"
#include "engine/CEngineSceneGridPos.h"
#else
enum EEngineInterpolationSet;
#endif
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CURRENTINTERPOLATIONINFO
class CEngineInterpolationInfo;
#endif
class CEngineInternalPrimitiveBase : public CEnginePersistentPrimitiveBase {
public:
    virtual ~CEngineInternalPrimitiveBase();
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_OCCLUSION
    virtual void CollectStats(std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<const unsigned long,unsigned long> > >&,EEnginePrimitiveStatIndex,unsigned long)const;
    void SetGridCell(CEngineSceneGridCell* cell){GridCell=cell;}
    void AddToList(CEngineInternalPrimitiveBase** list);
#endif
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CURRENTINTERPOLATIONINFO
    // Nested opt-in (chunk 899, row 0x01224900): the static data member Ego_r.pdb / FableWin.pdb name through
    // `dynamic initializer for 'CEngineInternalPrimitiveBase::CurrentInterpolationInfo'' (the 112-byte CEngineInterpolationInfo
    // @ 0x0143B8E0 that CEnginePrimitiveRenderer::FrameStart 0x00B32660 refreshes each frame). Access is not recorded (public).
    static CEngineInterpolationInfo CurrentInterpolationInfo;   // 0x0143B8E0
#endif
    unsigned long                  EnginePrimitiveFlags;           // +0x0c
    float                          AlphaMipBias;                   // +0x10
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_OCCLUSION
    CEngineFadeDistance FadeDistance;
#else
    float                          FadeDistance_FadeBeginDistance; // +0x14 CEngineFadeDistance::FadeBeginDistance
    float                          FadeDistance_MaxDrawDistance;   // +0x18 CEngineFadeDistance::MaxDrawDistance
#endif
    unsigned long                  OcclusionLayerBitMask;          // +0x1c
    CEngineSceneGridCell*          GridCell;                       // +0x20
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_OCCLUSION
    CEngineSceneGridPos GridPos;
#else
    signed char                    GridPos_Level;                  // +0x24 CEngineSceneGridPos::Level
    unsigned char                  GridPos_X;                      // +0x25 CEngineSceneGridPos::X
    unsigned char                  GridPos_Y;                      // +0x26 CEngineSceneGridPos::Y
#endif
    unsigned char                  Alpha;                          // +0x27
    unsigned char                  AlphaRef;                       // +0x28
    unsigned char                  PreloadStat;                    // +0x29
    EEngineInterpolationSet        InterpolationSet;               // +0x2c
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_OCCLUSION
protected:
#endif
    unsigned long                  RenderLayerMask;                // +0x30
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_OCCLUSION
private:
    unsigned char Enable:1,NonPersistent:1,OverrideFadeDistance:1;
#endif
    unsigned char                  PostRenderEnable : 1;           // +0x34
    unsigned short                 LastFrameRendered;              // +0x36
    CEngineInternalPrimitiveBase*  NextPrimitive;                  // +0x38
    CEngineInternalPrimitiveBase** RefPrimitive;                   // +0x3c
    CEngineInternalPrimitiveBase*  NextLayerMask;                  // +0x40
    CEngineInternalPrimitiveBase** RefLayerMask;                   // +0x44
};
FABLE_STATIC_ASSERT(sizeof(CEngineInternalPrimitiveBase) == 0x48);
#else
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CEngineInternalPrimitiveBase = 0x48.
// Retail evidence: 1 trusted landed function(s); reconciliation 2 conflict(s) in 1 file(s) vs 0 agreeing; PINNED by 3 retyped file(s) despite conflicts.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CEngineSceneGridCell;

#pragma pack(push, 1)
class CEngineInternalPrimitiveBase {
public:
    void*                         __vftable;                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char                 _pad_0x04[0x4];            // +0x04
    unsigned char                 _pad_0x08[0x4];            // +0x08
    unsigned long                 EnginePrimitiveFlags;      // +0x0c
    float                         AlphaMipBias;              // +0x10
    float                         FadeDistance_FadeBeginDistance; // +0x14 CEngineFadeDistance::FadeBeginDistance
    float                         FadeDistance_MaxDrawDistance; // +0x18 CEngineFadeDistance::MaxDrawDistance
    unsigned long                 OcclusionLayerBitMask;     // +0x1c
    CEngineSceneGridCell*         GridCell;                  // +0x20
    signed char                   GridPos_Level;             // +0x24 CEngineSceneGridPos::Level
    unsigned char                 GridPos_X;                 // +0x25 CEngineSceneGridPos::X
    unsigned char                 GridPos_Y;                 // +0x26 CEngineSceneGridPos::Y
    unsigned char                 Alpha;                     // +0x27
    unsigned char                 AlphaRef;                  // +0x28
    unsigned char                 PreloadStat;               // +0x29
    unsigned char                 _pad_0x2a[0x2];            // +0x2a
    unsigned char                 InterpolationSet[0x4];     // +0x2c EEngineInterpolationSet
    unsigned long                 RenderLayerMask;           // +0x30
    unsigned char                 PostRenderEnable[0x2];     // +0x34 bitfield group: uchar:1
    unsigned short                LastFrameRendered;         // +0x36
    CEngineInternalPrimitiveBase* NextPrimitive;             // +0x38
    void*                         RefPrimitive;              // +0x3c CEngineInternalPrimitiveBase * *
    CEngineInternalPrimitiveBase* NextLayerMask;             // +0x40
    void*                         RefLayerMask;              // +0x44 CEngineInternalPrimitiveBase * *
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CEngineInternalPrimitiveBase) == 0x48);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, EnginePrimitiveFlags) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, AlphaMipBias) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, FadeDistance_FadeBeginDistance) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, FadeDistance_MaxDrawDistance) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, OcclusionLayerBitMask) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, GridCell) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, GridPos_Level) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, GridPos_X) == 0x25);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, GridPos_Y) == 0x26);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, Alpha) == 0x27);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, AlphaRef) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, PreloadStat) == 0x29);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, InterpolationSet) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, RenderLayerMask) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, PostRenderEnable) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, LastFrameRendered) == 0x36);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, NextPrimitive) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, RefPrimitive) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, NextLayerMask) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, RefLayerMask) == 0x44);

#endif // FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CLASS
#endif // FABLE_ENGINE_CENGINEINTERNALPRIMITIVEBASE_H
