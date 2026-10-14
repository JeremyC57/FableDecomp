// CIVCountedHolder_00710600::operator=(const CIVCountedHolder_00710600&) -- copies the intrusive counted pointer at +4 (release old, adopt new, addref) and returns *this
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00710600, length 59 bytes.
// Callees: CIVCountedPointeeBase vtable slot 1 (Delete, `call [eax+4]`) when the count reaches zero.
// The manifest label (C3DAnimationBlendState::operator=) is a propagated name: the body is the
// memberwise assignment of a {vptr, CIVCountedPointer} object (the same 8-byte layout as
// C3DAnimationBlendState / CEnginePrimitiveHandle, see engine/CIVCountedPointeeBase.h for the
// pointee: vptr + IVRefCount at +4). The inlined CIVCountedPointer::operator= skips when both
// hold the same object, releases the old one (`dec [ecx+4] ; jne`, virtual slot 1 at zero, then a
// null store), re-reads other's pointer (the argument is a reference, so it is reloaded after the
// virtual call), stores it and bumps the count. The vptr is not copied.
struct CIVCountedPointeeBase {
    virtual void Destroy(int flags);       // slot 0
    virtual void Delete();                 // slot 1: `call [eax+4]`
    unsigned long IVRefCount;              // +4
};

struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;         // +0
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

struct CIVCountedHolder_00710600 {
    virtual void v00();
    CIVCountedPointer Data;                // +4
    CIVCountedHolder_00710600& operator=(const CIVCountedHolder_00710600& other);
};

CIVCountedHolder_00710600& CIVCountedHolder_00710600::operator=(const CIVCountedHolder_00710600& other)
{
    Data = other.Data;
    return *this;
}
