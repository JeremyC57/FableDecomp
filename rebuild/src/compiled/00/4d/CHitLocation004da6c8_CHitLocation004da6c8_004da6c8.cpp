// CHitLocation_004da6c8::~CHitLocation_004da6c8() -- release the two counted pointers at +8 and +4 (the count at the pointee's +4 decremented, virtual slot 1 at zero, the word cleared)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DA6C8, length 47 bytes.
// The chunk label "CHitLocation::~CHitLocation" names a class without a vtable store here; named by address. Shape: the size-optimised module (`and dword ptr [esi+8],0`), no vtable store.
#pragma optimize("s", on)
class CIVCountedPointeeBase { public:
    virtual void Destroy(int flags);               // slot 0
    virtual void Delete();                         // slot 1
    unsigned long IVRefCount;                      // +4
};
struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    __forceinline void Release()
    {
        CIVCountedPointeeBase* old = Object;
        if (old) {
            if (--old->IVRefCount == 0)
                old->Delete();
            Object = 0;
        }
    }
    __forceinline ~CIVCountedPointer() { Release(); }
};

struct CHitLocation_004da6c8 {
    long             m_0;
    CIVCountedPointer m_pointer4;                  // +0x04
    CIVCountedPointer m_pointer8;                  // +0x08
    ~CHitLocation_004da6c8();
};

CHitLocation_004da6c8::~CHitLocation_004da6c8()
{
}