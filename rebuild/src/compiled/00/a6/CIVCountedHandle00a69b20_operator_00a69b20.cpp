// CIVCountedHandle_00a69b20::operator=(const CIVCountedHandle_00a69b20&) -- if the handles differ: clear this one out of line, then copy the counted pointer at +4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A69B20, length 80 bytes.
// Callees: ClearHandle @ 0x00A69AD0 (this, no args); CIVCountedPointeeBase slot 1 (Delete) at zero count.
// The manifest label (C3DAnimationBlendState::operator=) is propagated; the body is the assignment
// of a {vptr, CIVCountedPointer} handle. The outer compare guards the out-of-line Clear; the
// inner compare / release / adopt / addref is the inlined CIVCountedPointer::operator= (the same
// shape as 0x00710600), re-reading other's pointer after the virtual call.
class CIVCountedPointeeBase { public:
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

struct CIVCountedHandle_00a69b20 {
    virtual void v00();
    CIVCountedPointer Data;                // +4
    void ClearHandle();                    // 0x00A69AD0
    CIVCountedHandle_00a69b20& operator=(const CIVCountedHandle_00a69b20& other);
};

CIVCountedHandle_00a69b20& CIVCountedHandle_00a69b20::operator=(const CIVCountedHandle_00a69b20& other)
{
    if (Data.Object != other.Data.Object) {
        ClearHandle();
        Data = other.Data;
    }
    return *this;
}