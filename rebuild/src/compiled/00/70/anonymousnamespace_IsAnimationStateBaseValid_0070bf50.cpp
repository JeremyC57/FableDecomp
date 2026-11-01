// `anonymous namespace'::IsAnimationStateBaseValid(const CMeshDataBank& bank, const C3DAnimationStateBase& state) -- an animation state is valid when its animation (plain or alpha state) is loaded in the bank, or, for a transition state, when both its start and end states are valid; any other state type is valid
// Leaf: ?IsAnimationStateBaseValid@?A
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070BF50, 104 bytes, `__fastcall` (bank in ecx, state in edx), plain `ret`.
// Name: Ego_r.pdb / FableWin.pdb ?IsAnimationStateBaseValid@?A0x22a65229@@YI_NABVCMeshDataBank@@ABVC3DAnimationStateBase@@@Z
//   (a file-local helper of the CAnimateThing TU, next to IsAnimStateBlendValid). Callers: itself twice (0x0070BF97 / 0x0070BFA5)
//   and the landed IsAnimStateBlendValid 0x0070C770 (0x0070C785 / 0x0070C7D0), which declares this row under the same spelling.
// Callees: C3DAnimationStateBase::GetType (vtable slot 2, +8), CMeshDataBank::PeekAnimation(unsigned long) const 0x00A242C0
//   (FableWin ?PeekAnimation@CMeshDataBank@@QBEPBVC3DAnimationInfo@@K@Z), C3DAnimationInfo::IsLoaded() const 0x00A23F50
//   (?IsLoaded@C3DAnimationInfo@@QBE_NXZ, bound so in reloc_bindings).
// Shape: three separate type arms, as FableWin.pdb's GFDebugDynamicCast<const C3DAnimationStateBase, T> instantiations for
//   C3DAnimationState, C3DAnimationStateAlpha AND C3DAnimationTransitionState show (the debug build casts in each arm; release
//   code is the plain static_cast). The type-0 and type-1 arms are byte-identical, so VC7.1 cross-jumps the first into the
//   second: `test eax,eax ; je <type-1 arm>` -- the arm that looked like an `||` condition laid out in place. Each arm reads the
//   index into a named `const unsigned long` local (PeekAnimation's parameter type): only then do both arms load it into the
//   dead `state` register (esi) and merge completely (a named pointer or an inline read gives ecx/eax and a partial merge,
//   110 bytes). `if (!loaded) return false;` returns the tested al directly; the transition arm's two failures share one
//   `xor al,al` epilogue.
// Layout: engine/CIVCountedPointeeBase.h (retail form: vptr, slot 1 Delete, IVRefCount +4), engine/C3DAnimationInfo.h and
//   engine/CMeshDataBank.h (Layout-derived to declare the two callees); local models for the animation-state classes (no
//   engine headers): C3DAnimationStateBase : CIVCountedPointeeBase (first own virtual GetType at slot 2); C3DAnimationState AnimationIndex +8 / AnimationTime +0xC; C3DAnimationStateAlpha adds AlphaWeight
//   +0x10; C3DAnimationTransitionState StartState +8 / EndState +0xC (CIVCountedPointer<C3DAnimationStateBase>) / Progress
//   +0x10 -- the members the landed IsAnimStateBlendValid also models. EType enumerator names are not in the PDB name lists:
//   the arms compare the values 0 / 1 / 2 (the C3DAnimationState / C3DAnimationStateAlpha / C3DAnimationTransitionState
//   GetType overrides).
#include "rebuild_abi.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CIVCountedPointeeBase.h"
#define C3DAnimationInfo C3DAnimationInfoLayout
#include "engine/C3DAnimationInfo.h"
#undef C3DAnimationInfo
#define CMeshDataBank CMeshDataBankLayout
#include "engine/CMeshDataBank.h"
#undef CMeshDataBank
class C3DAnimationInfo : public C3DAnimationInfoLayout {
public:
    bool IsLoaded() const;                                       // 0x00A23F50
};
class CMeshDataBank : public CMeshDataBankLayout {
public:
    const C3DAnimationInfo* PeekAnimation(unsigned long index) const;   // 0x00A242C0
};
class C3DAnimationStateBase : public CIVCountedPointeeBase {    // slots 0/1 are CIVCountedPointeeBase's dtor / Delete
public:
    enum EType { };
    virtual EType GetType() const;                               // slot 2
};
class C3DAnimationState : public C3DAnimationStateBase {
public:
    long  AnimationIndex;                                        // +0x08
    float AnimationTime;                                         // +0x0C
};
class C3DAnimationStateAlpha : public C3DAnimationState {
public:
    float AlphaWeight;                                           // +0x10
};
template <class T> class CIVCountedPointer {
public:
    T* Object;
    T& operator*() const { return *Object; }
};
class C3DAnimationTransitionState : public C3DAnimationStateBase {
public:
    CIVCountedPointer<C3DAnimationStateBase> StartState;         // +0x08
    CIVCountedPointer<C3DAnimationStateBase> EndState;           // +0x0C
    float Progress;                                              // +0x10
};

namespace {
bool __fastcall IsAnimationStateBaseValid(const CMeshDataBank& bank, const C3DAnimationStateBase& state)
{
    if (state.GetType() == 0) {                                  // C3DAnimationState
        const unsigned long index = static_cast<const C3DAnimationState&>(state).AnimationIndex;
        if (!bank.PeekAnimation(index)->IsLoaded())
            return false;
    }
    else if (state.GetType() == 1) {                             // C3DAnimationStateAlpha
        const unsigned long index = static_cast<const C3DAnimationStateAlpha&>(state).AnimationIndex;
        if (!bank.PeekAnimation(index)->IsLoaded())
            return false;
    }
    else if (state.GetType() == 2) {                             // C3DAnimationTransitionState
        const C3DAnimationTransitionState* transition = static_cast<const C3DAnimationTransitionState*>(&state);
        if (!IsAnimationStateBaseValid(bank, *transition->StartState))
            return false;
        if (!IsAnimationStateBaseValid(bank, *transition->EndState))
            return false;
    }
    return true;
}
}

// Keeps the file-local function emitted (its retail callers are in the same TU); not part of the row.
bool __fastcall IsAnimationStateBaseValid_Use(const CMeshDataBank& bank, const C3DAnimationStateBase& state)
{
    return IsAnimationStateBaseValid(bank, state);
}