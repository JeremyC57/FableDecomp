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
class C3DAnimationState;
class C3DAnimationStateBase;
template <class T> class CIVCountedPointer;
class C3DAnimationBlendState : public CBaseClass {
public:
    C3DAnimationBlendState();                                            // 0x004C2DD0
    C3DAnimationBlendState(const C3DAnimationBlendState&);
    virtual ~C3DAnimationBlendState();                                   // 0x004C2DF0
    void SetBaseAnimation(const C3DAnimationState& state);               // 0x00A3C260
    void SetBaseAnimation(const CIVCountedPointer<C3DAnimationStateBase>& state);
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
    class CSharedData;
    CIVCountedPointer<CSharedData> Data;
#else
    void* Data;                                                          // +0x04 CIVCountedPointer<C3DAnimationBlendState::CSharedData>
#endif
};
FABLE_STATIC_ASSERT(sizeof(C3DAnimationBlendState) == 8);
#endif
