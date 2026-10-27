// C3DAnimationManager::BeginLoadingAnimation(const C3DAnimationStateBase& state) -- start loading every animation the state tree refers to (both sides of each transition)
// Leaf: ?BeginLoadingAnimation@C3DAnimationManager@@QAEXABVC3DAnimationStateBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A9DF50, 72 bytes, `ret 4`. Ego_r.pdb
//   `?BeginLoadingAnimation@C3DAnimationManager@@QAEXABVC3DAnimationStateBase@@@Z` (donor 0x00BE1A70, right after
//   IsAnimationLoaded 0x00BE1A10 = retail 0x00A9DEF0); it recurses on the start state (0x00A9DF6A) and the
//   C3DAnimationBlendState overload calls it (0x00A9F4F1 / 0x00A9F52B). The end-state recursion is a tail call that VC7.1
//   turns into the loop back to 0x00A9DF64.
// Callees: CMeshDataBank::PeekAnimation(unsigned long) const @ 0x00A242C0 (the only CMeshDataBank animation getter in both
//   PDB name sets; reloc_bindings carries eight stand-in spellings for the address), C3DAnimationInfo::IsLoaded() const
//   @ 0x00A23F50, C3DAnimationInfo::BeginLoading(bool) const @ 0x00A26730 (`?BeginLoading@C3DAnimationInfo@@QBEX_N@Z`).
// Layout: Ego_r.pdb -- C3DAnimationManager MeshBank +4; C3DAnimationState AnimationIndex +8; C3DAnimationTransitionState
//   StartState +8 / EndState +0xC (CIVCountedPointer<C3DAnimationStateBase>); GetType is virtual slot 2 (+8) and the
//   transition type is 2. engine/C3DAnimationTransitionState.h flattens the vptr, so the classes are local models.
class C3DAnimationInfo {
public:
    bool IsLoaded() const;                              // 0x00A23F50
    void BeginLoading(bool wait) const;                 // 0x00A26730
};

class CMeshDataBank {
public:
    const C3DAnimationInfo* PeekAnimation(unsigned long index) const;   // 0x00A242C0
};

template <class T> class CIVCountedPointer {            // retail 4 bytes
public:
    T* Object;                                          // +0x00
    T& operator*() const { return *Object; }
};

class C3DAnimationStateBase {                           // CIVCountedPointee: vptr +0, count +4
public:
    enum EType {                                        // enumerator names descriptive (values from the GetType slots:
        STATE_ANIMATION = 0,                            //   C3DAnimationState 0x00550880 returns 0,
        STATE_ALPHA = 1,                                //   C3DAnimationStateAlpha 0x0070FE20 returns 1,
        STATE_TRANSITION = 2                            //   C3DAnimationTransitionState 0x00710520)
    };
    virtual ~C3DAnimationStateBase();                   // slot 0
    virtual void Slot1();                               // slot 1 (not overridden; unnamed here)
    virtual EType GetType() const = 0;                  // slot 2 (+0x08)
    unsigned long RefCount;                             // +0x04
};

class C3DAnimationState : public C3DAnimationStateBase {
public:
    long  AnimationIndex;                               // +0x08
    float AnimationTime;                                // +0x0C
    unsigned long GetAnimationIndex() const { return AnimationIndex; }
};

class C3DAnimationTransitionState : public C3DAnimationStateBase {
public:
    CIVCountedPointer<C3DAnimationStateBase> StartState;   // +0x08
    CIVCountedPointer<C3DAnimationStateBase> EndState;     // +0x0C
    float                                    Progress;     // +0x10
    const C3DAnimationStateBase& PeekStartState() const { return *StartState; }
    const C3DAnimationStateBase& PeekEndState() const { return *EndState; }
};

class C3DAnimationManager {
public:
    virtual ~C3DAnimationManager();
    CMeshDataBank* MeshBank;                            // +0x04
    bool IsAnimationLoaded(const C3DAnimationStateBase& state) const;
    void BeginLoadingAnimation(const C3DAnimationStateBase& state);
};

void C3DAnimationManager::BeginLoadingAnimation(const C3DAnimationStateBase& state)
{
    if (state.GetType() == C3DAnimationStateBase::STATE_TRANSITION) {
        const C3DAnimationTransitionState& transition = static_cast<const C3DAnimationTransitionState&>(state);
        BeginLoadingAnimation(transition.PeekStartState());
        BeginLoadingAnimation(transition.PeekEndState());
    }
    else {
        const C3DAnimationState& animation = static_cast<const C3DAnimationState&>(state);
        MeshBank->PeekAnimation(animation.GetAnimationIndex())->BeginLoading(false);
    }
}