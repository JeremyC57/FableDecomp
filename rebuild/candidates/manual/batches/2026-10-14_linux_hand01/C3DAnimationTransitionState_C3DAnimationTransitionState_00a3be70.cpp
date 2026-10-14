// C3DAnimationTransitionState::C3DAnimationTransitionState(const CIVCountedPointer& start, const CIVCountedPointer& end, float progress)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3BE70, length 57 bytes. No callees.
// Layout from engine/C3DAnimationTransitionState.h (0x14 bytes): vptr, +4 IVRefCount (the
// CIVCountedPointeeBase root, see engine/CIVCountedPointeeBase.h), StartState_Object +8,
// EndState_Object +0xc, Progress +0x10. The root constructor sets the count to 1 before the
// derived vtable store (its own vtable store is dead and dropped); the two counted pointers are
// copy-constructed (store + addref) and the float is copied as a dword. ret 0xc.
struct CIVCountedPointeeBase {
    virtual void Destroy(int flags);       // slot 0
    virtual void Delete();                 // slot 1
    unsigned long IVRefCount;              // +4
    CIVCountedPointeeBase() : IVRefCount(1) { }
};

struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
};

struct C3DAnimationStateBase : CIVCountedPointeeBase {
    C3DAnimationStateBase() { }
};

struct C3DAnimationTransitionState : C3DAnimationStateBase {   // vtable 0x01265144
    CIVCountedPointer StartState;          // +8
    CIVCountedPointer EndState;            // +0xc
    float Progress;                        // +0x10
    C3DAnimationTransitionState(const CIVCountedPointer& start, const CIVCountedPointer& end, float progress);
};

C3DAnimationTransitionState::C3DAnimationTransitionState(const CIVCountedPointer& start, const CIVCountedPointer& end, float progress)
    : StartState(start), EndState(end), Progress(progress)
{
}
