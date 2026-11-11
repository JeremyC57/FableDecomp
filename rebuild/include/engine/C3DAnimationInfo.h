#ifndef FABLE_ENGINE_C3DANIMATIONINFO_H
#define FABLE_ENGINE_C3DANIMATIONINFO_H
#ifdef FABLE_RETAIL_C3DANIMATIONINFO
// Retail opt-in (chunk 761, rows 0x00A26830 BuildSharedAnimation / 0x00A26AA0 Preload): the polymorphic retail class. RTTI
// .?AVC3DAnimationInfo@@ : CMeshDataBankEntry (vtable 0x0129CD64: slot 0 ??_G 0x00A2BD70, 2 Sizeof 0x00A265B0, 3 Reset 0x00A24D40);
// the nested C3DAnimationInfo::CAnimationData : CResource (vtable 0x0129CE50; 0x3C bytes = BeginLoading's `push 3Ch` allocation:
// the 12-byte STLport map Animations +0x1C, SharedAnimation +0x28, FileData +0x30 (BeginLoading assigns `lea ecx,[edi+30h]`),
// AnimInfo +0x38). Members and access: Ego_r.pdb (?BeginLoading@C3DAnimationInfo@@QBEX_N@Z, the private const
// ?BuildSharedAnimation@C3DAnimationInfo@@ABEXXZ, ?Preload@..QBEXXZ, ?TouchAnimation@..QBEXXZ, ?IsLoading@..QBE_NXZ (out of line at
// 0x00A23F30 in its own TU, expanded here: `mov eax,[ecx+0Ch] ; test ; mov ecx,[eax+30h] ; test`), and
// ?BuildSharedAnimation@CAnimationData@C3DAnimationInfo@@QAEXXZ 0x00A24D70). Data is the mutable CIVCountedPointer the const
// members load. Needs FABLE_RETAIL_DEF_CLASS_BASE, FABLE_RETAIL_CIVCOUNTEDPOINTER, FABLE_RETAIL_CMESHDATABANKENTRY (+ its macros).
#include "engine/CMeshDataBankEntry.h"
#include "engine/CResource.h"
#include "engine/CIVCountedPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
class C3DAnimation2;
class CBankFileAsyncData;
#ifdef FABLE_RETAIL_C3DANIMATIONINFO_BUILD
// Retail opt-in (Codex pass19): original Ego_r/FableWin types and donor calls; native animation rows 00a24d70/00a26ad0. See NAMING_EVIDENCE.md.
#include "stlport/fable_map.h"
class C3DAnimationStats;class C3DMesh2;class C3DAnimFileXLoader;
class C3DAnimationInfo : public CMeshDataBankEntry {
public:
 C3DAnimationInfo(const C3DAnimationInfo&);
 C3DAnimationInfo(unsigned long,unsigned long,CMeshDataBank*);
 virtual ~C3DAnimationInfo();
 CCountedPointer<const C3DAnimation2> GetAnimation(const C3DMesh2&)const;
 CCountedPointer<const C3DAnimationStats> GetAnimationStats()const;
 bool IsDeltaAnimation()const;bool IsCombinationAnimation()const;
 float GetDuration()const;float GetNonLoopingDuration()const;float GetMovementDistance()const;
 C3DVector GetMovementVector()const;float GetRotation()const;
 void TouchAnimation()const;
 bool IsLoading()const{return !Data.IsNull()&&!Data->FileData.IsNull();}
 bool IsLoaded()const{return !Data.IsNull()&&!Data->SharedAnimation.IsNull();}
 void BeginLoading(bool)const;void Preload()const;
 virtual unsigned long Sizeof()const;
private:
 virtual void Reset();
 void SetDuration(float);void SetNonLoopingDuration(float);void SetMovementVector(const C3DVector&);
 bool RecoverAnimationMemory(unsigned long)const;
 C3DAnimFileXLoader& GetAnimLoader()const{return *GetBank()->AnimLoader;}
 typedef std::map<unsigned long,CCountedPointer<C3DAnimation2> > TAnimationMap;
public:
 class CAnimationData:public CResource {
 public:
  CAnimationData(const CAnimationData&);CAnimationData(const C3DAnimationInfo*);
  virtual ~CAnimationData();virtual bool IsResourceLoaded()const;virtual void OnResourceLoaded();
  void BuildSharedAnimation();bool CanUnload()const;virtual bool UnloadResource();void UpdateSize();unsigned long Sizeof()const;
  TAnimationMap Animations;
  CCountedPointer<C3DAnimation2> SharedAnimation;
  CCountedPointer<CBankFileAsyncData> FileData;
  const C3DAnimationInfo* AnimInfo;
  CAnimationData& operator=(const CAnimationData&);
 };
private:
 void BuildSharedAnimation()const {
  if(!IsLoading())BeginLoading(true);
  Data->RemoveFromResourceList();Data->BuildSharedAnimation();
 }
 CCountedPointer<C3DAnimation2> BuildAnimationInstance(const C3DMesh2&)const;
 mutable CIVCountedPointer<CAnimationData> Data;
 float Duration,NonLoopingDuration;C3DVector MovementVector;float Rotation;
public:C3DAnimationInfo& operator=(const C3DAnimationInfo&);
};
#else
class C3DAnimationInfo : public CMeshDataBankEntry {
public:
    class CAnimationData : public CResource {
    public:
        void BuildSharedAnimation();                                     // 0x00A24D70
        unsigned char                        Animations[0xc];            // +0x1c map<unsigned long, CCountedPointer<C3DAnimation2> >
        CCountedPointer<C3DAnimation2>       SharedAnimation;            // +0x28
        CCountedPointer<CBankFileAsyncData>  FileData;                   // +0x30
        C3DAnimationInfo*                    AnimInfo;                   // +0x38
    };
    C3DAnimationInfo(unsigned long index, unsigned long, CMeshDataBank* bank);
    virtual ~C3DAnimationInfo();
    bool IsLoading() const { return !Data.IsNull() && Data->FileData.Data; }
    void BeginLoading(bool) const;                                       // 0x00A26730
    void TouchAnimation() const;                                         // 0x00A26A50
    void Preload() const;                                                // 0x00A26AA0
private:
    void BuildSharedAnimation() const;                                   // 0x00A26830
    mutable CIVCountedPointer<CAnimationData> Data;                      // +0x0c
    float                                     Duration;                  // +0x10
    float                                     NonLoopingDuration;        // +0x14
    C3DVector                                 MovementVector;            // +0x18
    float                                     Rotation;                  // +0x24
};
#endif
FABLE_STATIC_ASSERT(sizeof(C3DAnimationInfo) == 0x28);
FABLE_STATIC_ASSERT(sizeof(C3DAnimationInfo::CAnimationData) == 0x3c);
#else
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof C3DAnimationInfo = 0x28.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CIVCountedPointeeBase;

#pragma pack(push, 1)
class C3DAnimationInfo {
public:
#ifdef FABLE_RETAIL_C3DANIMATIONINFO_LOADING
    // Retail opt-in (agentNF9, the CThingCreatureBase preload rows 0x00663140 / 0x006631C0 / 0x00663610): Ego_r.pdb / FableWin.pdb
    // ?TouchAnimation@C3DAnimationInfo@@QBEXXZ (out of line at 0x00A26A50) and ?BeginLoading@C3DAnimationInfo@@QBEX_N@Z (0x00A26730),
    // public const, as the retail model above declares them. Declarations only, in the generated layout: no layout change.
    void TouchAnimation() const;                                         // 0x00A26A50
    void BeginLoading(bool) const;                                       // 0x00A26730
#endif
    void*                  __vftable;                        // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char          _pad_0x04[0x4];                   // +0x04
    unsigned char          _pad_0x08[0x4];                   // +0x08
    CIVCountedPointeeBase* Data_Object;                      // +0x0c CIVCountedPointer<C3DAnimationInfo::CAnimationData>::Object
    float                  Duration;                         // +0x10
    float                  NonLoopingDuration;               // +0x14
    float                  MovementVector_X;                 // +0x18 C3DVector::X
    float                  MovementVector_Y;                 // +0x1c C3DVector::Y
    float                  MovementVector_Z;                 // +0x20 C3DVector::Z
    float                  Rotation;                         // +0x24
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(C3DAnimationInfo) == 0x28);
FABLE_STATIC_ASSERT(offsetof(C3DAnimationInfo, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(C3DAnimationInfo, Data_Object) == 0xc);
FABLE_STATIC_ASSERT(offsetof(C3DAnimationInfo, Duration) == 0x10);
FABLE_STATIC_ASSERT(offsetof(C3DAnimationInfo, NonLoopingDuration) == 0x14);
FABLE_STATIC_ASSERT(offsetof(C3DAnimationInfo, MovementVector_X) == 0x18);
FABLE_STATIC_ASSERT(offsetof(C3DAnimationInfo, MovementVector_Y) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(C3DAnimationInfo, MovementVector_Z) == 0x20);
FABLE_STATIC_ASSERT(offsetof(C3DAnimationInfo, Rotation) == 0x24);

#endif // FABLE_RETAIL_C3DANIMATIONINFO

#endif // FABLE_ENGINE_C3DANIMATIONINFO_H
