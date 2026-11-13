#ifndef FABLE_ENGINE_C3DANIMATIONBLENDSTATE_H
#define FABLE_ENGINE_C3DANIMATIONBLENDSTATE_H
// New shared header (chunk 749, row 0x005502E0 NUISystem::CMesh::Draw): Ego_r.pdb C3DAnimationBlendState, sizeof 8
// (RTTI .?AVC3DAnimationBlendState@@ : CBaseClass): Data +0x4 (CIVCountedPointer<C3DAnimationBlendState::CSharedData>, opaque
// here). Name set: FableWin.pdb ??0..QAE@XZ (out of line at 0x004C2DD0), ??0..QAE@ABV0@@Z, ??1..UAE@XZ (out of line at
// 0x004C2DF0), ?SetBaseAnimation@C3DAnimationBlendState@@QAEXABVC3DAnimationState@@@Z (0x00A3C260) and the
// CIVCountedPointer<C3DAnimationStateBase> overload.
#include "engine/CBaseClass.h"
#ifdef FABLE_RETAIL_C3DANIMATIONBLENDSTATE_TYPED_DATA
#include "engine/CIVCountedPointer.h"
#endif
#ifdef FABLE_RETAIL_C3DANIMATIONBLENDSTATE_SHAREDDATA
#include "engine/CIVCountedPointeeBase.h"
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DAnimationStateAlpha.h"
class C3DSkeletalMorph;
#endif
class C3DAnimationState;
class C3DAnimationStateBase;
template <class T> class CIVCountedPointer;
class C3DAnimationBlendState : public CBaseClass {
public:
#ifndef FABLE_RETAIL_C3DANIMATIONINTERPOLATOR_IMPLICIT
    C3DAnimationBlendState();                                            // 0x004C2DD0
    C3DAnimationBlendState(const C3DAnimationBlendState&);
    virtual ~C3DAnimationBlendState();                                   // 0x004C2DF0
#else
    // Opt-in (row 0x004BD6E0 CTCGraphicAppearance::SetPrimitive): the compiler-generated constructor and destructor, which retail
    // expands in place there (0x004BD719..0x004BD73F: ~CBaseClass ctor, vptr 0x01239838, Data=0; 0x004BD9C4..0x004BD9EA: Data
    // released, ~CBaseClass, no vptr re-store), as the landed implicit 0x004C2DD0 / 0x004C2DF0 rows model them.
#endif
    void SetBaseAnimation(const C3DAnimationState& state);               // 0x00A3C260
    void SetBaseAnimation(const CIVCountedPointer<C3DAnimationStateBase>& state);
#if defined(FABLE_RETAIL_C3DANIMATIONBLENDSTATE_SKELETALMORPH) && defined(FABLE_RETAIL_C3DANIMATIONBLENDSTATE_SHAREDDATA)
    // Retail opt-in (chunk Q32, row 0x00BD2A90): FableWin.pdb ?HasSkeletalMorph@C3DAnimationBlendState@@QBE_NXZ /
    // ?PeekSkeletalMorph@C3DAnimationBlendState@@QBEABVC3DSkeletalMorph@@XZ, inline in retail (0x00BD2A96..0x00BD2AAF reads
    // Data then Data->SkeletalMorph.Data at +0x18 in place).
    bool HasSkeletalMorph() const { return !Data.IsNull() && !Data->SkeletalMorph.IsNull(); }
    const C3DSkeletalMorph& PeekSkeletalMorph() const { return *Data->SkeletalMorph; }
#endif
#ifdef FABLE_RETAIL_C3DANIMATIONBLENDSTATE_CREATEINTERPOLATE
    // Opt-in (chunk 898, row 0x00A3BF10): Ego_r.pdb ?CreateInterpolate@C3DAnimationBlendState@@CI?AV?$CIVCountedPointer@VC3DAnimationStateBase@@@@ABVC3DAnimationStateBase@@0M@Z
    // (private static __fastcall; retail 0x00A3BF10, callers itself and SetInterpolate 0x00A3C3B0). Declaration only.
private:
    static CIVCountedPointer<C3DAnimationStateBase> __fastcall CreateInterpolate(const C3DAnimationStateBase& a,
                                                                                 const C3DAnimationStateBase& b, float t);
public:
#endif
#ifdef FABLE_RETAIL_C3DANIMATIONBLENDSTATE_TYPED_DATA
private:
#ifdef FABLE_RETAIL_C3DANIMATIONBLENDSTATE_SHAREDDATA
    // Nested opt-in (agentFAM06, row 0x00A3C2D0 SetBaseAnimation(const CIVCountedPointer<C3DAnimationStateBase>&)): Ego_r.pdb
    // C3DAnimationBlendState::CSharedData (a CIVCountedPointeeBase; BaseAnimation +0x08, CombinationBlends +0x0C, SkeletalMorph
    // after it: retail 0x20 bytes, the CArray being a 12-byte STLport vector) and the private Ego_r.pdb / FableWin.pdb
    // ?MakeOwnCopyOfData@C3DAnimationBlendState@@AAEXXZ (0x00A3C1A0). Needs FABLE_RETAIL_DEF_CLASS_BASE.
    class CSharedData : public CIVCountedPointeeBase {
    public:
        CIVCountedPointer<C3DAnimationStateBase> BaseAnimation;              // +0x08
        CArray<C3DAnimationStateAlpha> CombinationBlends;                     // +0x0C
        CCountedPointer<const C3DSkeletalMorph> SkeletalMorph;               // +0x18
    };
    void MakeOwnCopyOfData();                                            // 0x00A3C1A0
#else
    class CSharedData;
#endif
    CIVCountedPointer<CSharedData> Data;
#if defined(FABLE_RETAIL_C3DANIMATIONBLENDSTATE_SHAREDDATA) && defined(FABLE_RETAIL_C3DANIMATIONBLENDSTATE_ACCESSORS)
    // Nested opt-in (chunk 2044, rows 0x00AA0090 / 0x00A9F540): the PDBs' inline const accessors
    // ?PeekBaseAnimation@C3DAnimationBlendState@@QBEABVC3DAnimationStateBase@@XZ, ?GetCombinationBlendCount@C3DAnimationBlendState@@QBEKXZ
    // (landed 0x0070C770's body: Data null-tested, then the 20-byte-stride size) and
    // ?PeekCombinationBlend@C3DAnimationBlendState@@QBEABVC3DAnimationStateAlpha@@K@Z (FableWin.pdb). No layout change.
public:
    const C3DAnimationStateBase& PeekBaseAnimation() const { return *Data->BaseAnimation; }
    unsigned long GetCombinationBlendCount() const { return Data.IsNull() ? 0 : Data->CombinationBlends.size(); }
    const C3DAnimationStateAlpha& PeekCombinationBlend(unsigned long index) const { return Data->CombinationBlends[index]; }
#endif
#else
    void* Data;                                                          // +0x04 CIVCountedPointer<C3DAnimationBlendState::CSharedData>
#endif
};
FABLE_STATIC_ASSERT(sizeof(C3DAnimationBlendState) == 8);
#endif
