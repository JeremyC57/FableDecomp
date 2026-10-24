// `anonymous namespace'::IsAnimationStateBaseValid(const CMeshDataBank& bank, const C3DAnimationStateBase& state) -- an animation state is valid when its animation is loaded (plain and alpha states) or when both ends of a transition are valid
// Grade: DIFFER(104v104) -- block layout only: retail lays the type 0/1 IsLoaded block straight after `cmp eax,1 ; jne` (index loaded through esi), VC7.1 here moves it behind the shared true/false tails (`je` twice, index through ecx); tried if/else-if with `||`, early `return true/false` per block, inverted `!= 0 && != 1` with the IsLoaded test in the else, `? true : false` return
// Leaf: ?IsAnimationStateBaseValid@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070BF50, length 104 bytes (+ int3 padding). `__fastcall`: bank in ecx, state in
//         edx, plain `ret`. Ego_r.pdb: `?IsAnimationStateBaseValid@?A0x22a65229@@YI_NABVCMeshDataBank@@ABVC3DAnimationStateBase@@@Z`
//         (a file-local helper of the CAnimateThing TU; recursive).
// Callees: the state's virtual GetType() (slot 2: 0 = C3DAnimationState, 1 = C3DAnimationStateAlpha -- landed 0x0070FE20 --,
//          2 = C3DAnimationTransitionState), CMeshDataBank::PeekAnimation(unsigned long) const @ 0x00A242C0 (manifest
//          "CTCInventoryAbilities::GetAbilityLevel" is propagated), C3DAnimationInfo::IsLoaded() const @ 0x00A23F50, itself.
// Identity: the manifest label "IsAnimationStateBaseValid" is right (PDB); its other caller is IsAnimStateBlendValid 0x0070C770.
// Shape: an if / else-if chain that re-calls the virtual GetType() for every test (`== 0 || == 1`, then `== 2`), each
//   failing test returning false and one `return true` at the end (VC7.1 returns the IsLoaded() result register on the
//   false path and shares `mov al,1`); the transition's two ends are tested start first.
// Layout: local polymorphic models -- engine/C3DAnimationState.h and C3DAnimationTransitionState.h are flat PDB structs
//   (vptr as a plain word); retail offsets used: AnimationIndex +0x08 (C3DAnimationState), StartState +0x08 / EndState
//   +0x0C (C3DAnimationTransitionState, CIVCountedPointer<C3DAnimationStateBase> = the object pointer).
class C3DAnimationInfo {
public:
    bool IsLoaded() const;                              // 0x00A23F50
};
class CMeshDataBank {
public:
    const C3DAnimationInfo* PeekAnimation(unsigned long index) const;   // 0x00A242C0
};

class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase();                   // slot 0
    long RefCount;                                      // +0x04
};
class C3DAnimationStateBase : public CIVCountedPointeeBase {
public:
    virtual void Validate();                            // slot 1
    virtual long GetType() const;                       // slot 2
};
class C3DAnimationState : public C3DAnimationStateBase {
public:
    long  AnimationIndex;                               // +0x08
    float AnimationTime;                                // +0x0C
};
template <class T> class CIVCountedPointer {
public:
    T* Object;
    T& operator*() const { return *Object; }
};
class C3DAnimationTransitionState : public C3DAnimationStateBase {
public:
    CIVCountedPointer<C3DAnimationStateBase> StartState;   // +0x08
    CIVCountedPointer<C3DAnimationStateBase> EndState;     // +0x0C
    float Progress;                                     // +0x10
};

namespace {
bool __fastcall IsAnimationStateBaseValid(const CMeshDataBank& bank, const C3DAnimationStateBase& state);
}

bool __fastcall IsAnimationStateBaseValid_Use(const CMeshDataBank& bank, const C3DAnimationStateBase& state)
{
    return IsAnimationStateBaseValid(bank, state);
}

namespace {
bool __fastcall IsAnimationStateBaseValid(const CMeshDataBank& bank, const C3DAnimationStateBase& state)
{
    if (state.GetType() == 0 || state.GetType() == 1) {
        if (!bank.PeekAnimation(static_cast<const C3DAnimationState&>(state).AnimationIndex)->IsLoaded())
            return false;
    }
    else if (state.GetType() == 2) {
        const C3DAnimationTransitionState& transition = static_cast<const C3DAnimationTransitionState&>(state);
        if (!IsAnimationStateBaseValid(bank, *transition.StartState) || !IsAnimationStateBaseValid(bank, *transition.EndState))
            return false;
    }
    return true;
}
}
