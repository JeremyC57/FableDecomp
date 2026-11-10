#if defined(FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_ADD)
#ifndef FABLE_ENGINE_CENGINESUBPRIMITIVERENDERER_H
#define FABLE_ENGINE_CENGINESUBPRIMITIVERENDERER_H
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CEngineComponent.h"
#include "engine/CArray.h"
#include "engine/CEngineRenderEnums.h"
class CEngineSubPrimitiveBase;
class CEnginePrimitiveManager;
class CEngineSubPrimitiveRenderer:public CBaseClassNonCopyable,public CEngineComponent {
public:
 CEngineSubPrimitiveRenderer();
 virtual unsigned long PeekSceneFilterFlags()const;
 void AddSubPrimitive(CEngineSubPrimitiveBase*,float,unsigned char,unsigned long,CEnginePrimitiveManager*);
#ifdef FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_ADDTYPED
 // Nested opt-in (chunk 1014, row 0x00B84720): FableWin.pdb ?AddSubPrimitive@CEngineSubPrimitiveRenderer@@QAEXW4ESubPrimitiveType@@
 // PAVCEngineSubPrimitiveBase@@MPAVCEnginePrimitiveManager@@@Z (FableWin 0x02DB2510, right after ??0CSortElement@...@QAE@KPAVCEngineSubPrimitiveBase@@@Z
 // 0x02DB2490); CSortElement is Ego_r.pdb's {SortValue, SubPrimitive} (8 bytes) with FableWin's (sort value, sub-primitive) constructor.
 void AddSubPrimitive(ESubPrimitiveType type,CEngineSubPrimitiveBase* subPrimitive,float distance,CEnginePrimitiveManager* manager);   // 0x00B84720
 class CSortElement {public:unsigned long Key;void* Primitive;
  CSortElement(unsigned long sortValue,CEngineSubPrimitiveBase* subPrimitive):Key(sortValue),Primitive(subPrimitive){}};
#else
 class CSortElement {public:unsigned long Key;void* Primitive;};
#endif
#ifdef FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_CTOR
 // Nested opt-in (chunk 1016, row 0x00B84470 ??0CEngineSubPrimitiveRenderer@@QAE@XZ): FableWin.pdb ??0CSubPrimitiveList@CEngineSubPrimitiveRenderer@@QAE@XZ
 // (inline in retail: the renderer's vector-constructor loop stores 0 to the CArray words, RenderPass and PreRenderPass, false to the
 // two pass flags and true to Sorted / Enable, 0x00B844A0..0x00B844B7) and Ego_r.pdb / FableWin.pdb ?SetMaxSortDistance@CEngineSubPrimitiveRenderer@@QAEXM@Z
 // (out of line at 0x00B84290: MaxSortDistance = d, SortDistanceMultiplier = 2^30 / d; the constructor expands it with 10000, folding
 // the quotient to 0x47D1B717). No layout change.
 class CSubPrimitiveList {public:CArray<CSortElement> Elements;ERenderPassType RenderPass,PreRenderPass;bool BoundingVolumePass,RequiresStableSort,Sorted,Enable;
  CSubPrimitiveList():RenderPass(RENDER_PASS_NULL),PreRenderPass(RENDER_PASS_NULL),BoundingVolumePass(false),RequiresStableSort(false),Sorted(true),Enable(true){}};
 void SetMaxSortDistance(float distance){MaxSortDistance=distance;SortDistanceMultiplier=1073741824.0f/distance;}
private:
 // FableWin.pdb ?GetList@CEngineSubPrimitiveRenderer@@AAEAAVCSubPrimitiveList@1@W4ESubPrimitiveType@@@Z (private; inline in retail).
 CSubPrimitiveList& GetList(ESubPrimitiveType type){return SubPrimitiveLists[type];}
public:
#else
 class CSubPrimitiveList {public:CArray<CSortElement> Elements;ERenderPassType RenderPass,PreRenderPass;bool BoundingVolumePass,RequiresStableSort,Sorted,Enable;};
#endif
private:
 float MaxSortDistance,SortDistanceMultiplier;
 CSubPrimitiveList SubPrimitiveLists[15];
 CArray<CSortElement> SortBuffer;
 bool EnableSolidPass,EnableAlphaPass;
};
FABLE_STATIC_ASSERT(sizeof(CEngineSubPrimitiveRenderer)==400);
#endif
#else
#ifndef FABLE_ENGINE_CENGINESUBPRIMITIVERENDERER_H
#define FABLE_ENGINE_CENGINESUBPRIMITIVERENDERER_H
#ifdef FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER
// Retail branch (chunk 731, 2026-11-03): RTTI .?AVCEngineSubPrimitiveRenderer@@ (vtable 0x012A3A28) : CBaseClassNonCopyable (+0x0), CEngineComponent (+0x4); sizeof 0x190 from
// CEngine::Initialise 0x00B26360 (`new` of 0x190 bytes before the constructor call). Constructor ??0CEngineSubPrimitiveRenderer@@QAE@XZ 0x00B84470 and
// the PeekSceneFilterFlags override (?PeekSceneFilterFlags@CEngineSubPrimitiveRenderer@@UBEKXZ): Ego_r.pdb / FableWin.pdb. Own members are
// not modelled yet (one opaque block; the generated Ego_r model below lists their names).
#ifndef FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINECOMPONENT
#endif
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CEngineComponent.h"
#ifdef FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_RADIXSORT
#include "engine/CArray.h"
#endif
class CEngineInit;
class CEngineSubPrimitiveRenderer : public CBaseClassNonCopyable, public CEngineComponent {
public:
    CEngineSubPrimitiveRenderer();             // 0x00B84470 ??0CEngineSubPrimitiveRenderer@@QAE@XZ
    virtual unsigned long PeekSceneFilterFlags() const;
#ifdef FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_RADIXSORT
    // Nested opt-in (chunk 762, row 0x00B847D0 RadixSort): CSortElement (8 bytes, key first; Ego_r.pdb
    // CEngineSubPrimitiveRenderer::CSortElement) and SortBuffer, the CArray<CSortElement> scratch at +0x180 (Ego_r.pdb name;
    // RadixSort reads [this+0x180]/[this+0x184]).
    class CSortElement {
    public:
        unsigned long Key;                      // +0x00
        void*         Primitive;                // +0x04
        CSortElement() { }
    };
private:
    void RadixSort(CArray<CSortElement>& elements);   // 0x00B847D0 ?RadixSort@CEngineSubPrimitiveRenderer@@AAEXAAV?$CArray@VCSortElement@CEngineSubPrimitiveRenderer@@@@@Z
    unsigned char _pad_0x10[0x170];             // +0x10 members not modelled yet
    CArray<CSortElement> SortBuffer;            // +0x180
    unsigned char _pad_0x18c[0x4];              // +0x18c EnableSolidPass, EnableAlphaPass, padding
};
#else
private:
    unsigned char _pad_0x10[0x180];             // +0x10 members not modelled yet (Ego_r.pdb names; retail STLport sizes)
};
#endif
FABLE_STATIC_ASSERT(sizeof(CEngineSubPrimitiveRenderer) == 0x190);
#else // generated flattened Ego_r layout (the landed default, unchanged)
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CEngineSubPrimitiveRenderer = 0x1d0.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CEngineSubPrimitiveRenderer = 0x190.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

#pragma pack(push, 1)
class CEngineSubPrimitiveRenderer {
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char _pad_0x04[0x4];                            // +0x04
    unsigned char _pad_0x08[0x4];                            // +0x08
    unsigned char _pad_0x0c[0x4];                            // +0x0c
    float         MaxSortDistance;                           // +0x10
    float         SortDistanceMultiplier;                    // +0x14
    unsigned char SubPrimitiveLists[0x168];                  // +0x18 CSubPrimitiveList[15]
    unsigned char SortBuffer[0xc];                           // +0x180 CArray<CEngineSubPrimitiveRenderer::CSortElement>
    bool          EnableSolidPass;                           // +0x18c
    bool          EnableAlphaPass;                           // +0x18d
    unsigned char _pad_0x18e[0x2];                           // +0x18e
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CEngineSubPrimitiveRenderer) == 0x190);
FABLE_STATIC_ASSERT(offsetof(CEngineSubPrimitiveRenderer, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CEngineSubPrimitiveRenderer, MaxSortDistance) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CEngineSubPrimitiveRenderer, SortDistanceMultiplier) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CEngineSubPrimitiveRenderer, SubPrimitiveLists) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CEngineSubPrimitiveRenderer, SortBuffer) == 0x180);
FABLE_STATIC_ASSERT(offsetof(CEngineSubPrimitiveRenderer, EnableSolidPass) == 0x18c);
FABLE_STATIC_ASSERT(offsetof(CEngineSubPrimitiveRenderer, EnableAlphaPass) == 0x18d);

#endif // FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER
#endif // FABLE_ENGINE_CENGINESUBPRIMITIVERENDERER_H

#endif
