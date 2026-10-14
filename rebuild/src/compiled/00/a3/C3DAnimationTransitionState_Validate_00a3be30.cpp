// C3DAnimationTransitionState::Validate() -- virtual: validates the start state then the end state (both through C3DAnimationStateBase vtable slot 5; the second call is a sibling jump)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3BE30, length 20 bytes (the chunk's 64-byte span over-captures the int3 padding and the next function at 0x00A3BE50).
// Callees: C3DAnimationStateBase::Validate (vtable slot 5, `call/jmp [vt+0x14]`).
// Layout from engine/C3DAnimationTransitionState.h: StartState_Object +8, EndState_Object +0xc.
struct C3DAnimationStateBase {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void Validate();               // slot 5
};

struct C3DAnimationTransitionState {       // engine/C3DAnimationTransitionState.h offsets
    virtual void v00();
    unsigned long IVRefCount;              // +4
    C3DAnimationStateBase* StartState;     // +8
    C3DAnimationStateBase* EndState;       // +0xc
    float Progress;                        // +0x10
    virtual void Validate();
};

void C3DAnimationTransitionState::Validate()
{
    StartState->Validate();
    EndState->Validate();
}