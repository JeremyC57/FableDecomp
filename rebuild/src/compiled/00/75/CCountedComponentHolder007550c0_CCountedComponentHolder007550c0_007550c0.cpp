// CCountedComponentHolder_007550c0::~CCountedComponentHolder_007550c0() -- destructor whose only non-trivial member is the intrusive counted pointer at +0x3c: release it and clear it
// Leaf: ??1CCountedComponentHolder_007550c0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007550C0, length 29 bytes, plain `ret`; no call or address references (an
// unreferenced COMDAT). The owning class is not recoverable from the bytes or the neighbours (descriptive name); the
// manifest label `_Dest_val<allocator<pair<EAnimComponent,CIVCountedPointer<CAnimComponentBase>>>>` is propagated over the
// shape -- the landed 0x004C34A0 is the same release at +4. Pointee type CAnimComponentBase is the label's guess.
// Callees: the pointee's virtual slot 1 (+4, its self-delete) when its IVRefCount (+4) drops to zero.
// Shape: ~CIVCountedPointer inlined -- the object read into ecx, the count decrement, the slot-1 call on zero and the zero
// store INSIDE the non-null arm (`if (Object) { Release(); Object = 0; }`, GOTCHAS 0070cad0); `mov`, not `and`, so not a
// size TU. Bytes +0x00..+0x3b are trivially destructible (no code).
class CIVCountedPointeeBase { public:
    virtual ~CIVCountedPointeeBase();
    virtual void DeleteThis();                               // slot 1 (+4)
    unsigned long IVRefCount;                                // +0x04
};
class CAnimComponentBase : public CIVCountedPointeeBase { public: };

template<class T> class CIVCountedPointer {
public:
    T* Object;
    ~CIVCountedPointer()
    {
        T* object = Object;
        if (object) {
            if (--object->IVRefCount == 0)
                object->DeleteThis();
            Object = 0;
        }
    }
};

struct CCountedComponentHolder_007550c0 {
    unsigned char _pad00[0x3c];                              // +0x00 trivially destructible members
    CIVCountedPointer<CAnimComponentBase> m_pComponent;      // +0x3c
    ~CCountedComponentHolder_007550c0();
};

CCountedComponentHolder_007550c0::~CCountedComponentHolder_007550c0()
{
}