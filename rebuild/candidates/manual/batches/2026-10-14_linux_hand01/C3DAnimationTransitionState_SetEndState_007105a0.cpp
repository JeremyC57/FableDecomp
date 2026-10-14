// C3DAnimationTransitionState::SetEndState(const CIVCountedPointer<C3DAnimationStateBase>&) -- replaces EndState (+0xc): release the old state, adopt and addref the new one
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007105A0, length 55 bytes.
// Callees: CIVCountedPointeeBase vtable slot 1 (Delete, `call [eax+4]`) when the count reaches zero.
// Layout from engine/C3DAnimationTransitionState.h: StartState_Object +0x8, EndState_Object +0xc,
// Progress +0x10 (the header is flat, so the counted pointer is modelled locally over the same
// offsets). The argument is a reference to the counted pointer (`mov eax,[edi]`), re-read after
// the virtual call. Same inlined CIVCountedPointer::operator= as 0x00710600.
struct CIVCountedPointeeBase {
    virtual void Destroy(int flags);       // slot 0
    virtual void Delete();                 // slot 1
    unsigned long IVRefCount;              // +4
};

struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        CIVCountedPointeeBase* old = Object;   // held in a register across the release (retail `dec [ecx+4]`)
        if (old != other.Object) {
            if (old) {
                if (--old->IVRefCount == 0)
                    old->Delete();
                Object = 0;
            }
            Object = other.Object;             // re-read: the argument is a reference
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
};

struct C3DAnimationTransitionState {       // engine/C3DAnimationTransitionState.h offsets
    virtual void v00();
    unsigned long IVRefCount;              // +4 (CIVCountedPointeeBase)
    CIVCountedPointer StartState;          // +8
    CIVCountedPointer EndState;            // +0xc
    float Progress;                        // +0x10
    void SetEndState(const CIVCountedPointer& state);
};

void C3DAnimationTransitionState::SetEndState(const CIVCountedPointer& state)
{
    EndState = state;
}
