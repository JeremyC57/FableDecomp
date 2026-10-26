// CHitLocation::~CHitLocation() -- release the two counted pointers at +0xc and +8 (the count at the pointee's +4 decremented, virtual slot 1 at zero, the word cleared), then the inlined CIVCountedPointeeBase destructor stores its vtable (0x012354D0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00710900, length 59 bytes.
// Shape: `this` in esi, the two releases with `mov dword ptr [esi+N],0`, the base vtable last.
class CIVCountedPointeeBase { public:
    virtual void Destroy(int flags);               // slot 0
    virtual void Delete();                         // slot 1
    unsigned long IVRefCount;                      // +4
    ~CIVCountedPointeeBase() { }                   // inlined: the vtable store
};
struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    void Release()
    {
        CIVCountedPointeeBase* old = Object;
        if (old) {
            if (--old->IVRefCount == 0)
                old->Delete();
            Object = 0;
        }
    }
    ~CIVCountedPointer() { Release(); }
};

struct __declspec(novtable) CHitLocation : CIVCountedPointeeBase {   // vtable 0x012354D0 is the base's: no vtable of its own
    CIVCountedPointer m_pointer8;                  // +0x08
    CIVCountedPointer m_pointerc;                  // +0x0c
    ~CHitLocation();
};

CHitLocation::~CHitLocation()
{
}