// CCountedComponentHolder_004ae3e0::~CCountedComponentHolder_004ae3e0() -- destructor: destroy the vector of wide-string records at +0x1c, then release and clear the intrusive counted pointer at +0x18
// Leaf: ??1CCountedComponentHolder_004ae3e0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AE3E0, length 37 bytes, plain `ret`; no call or address references (an
// unreferenced COMDAT). The owning class is not recoverable (descriptive name); the manifest label
// `_Dest_val<allocator<pair<EAnimComponent,CIVCountedPointer<CAnimComponentBase>>>>` is propagated over the shape.
// Callees: 0x004AE040 = the out-of-line destructor of a vector of 0x18-byte records holding CWideStrings at +4/+8
// (its body: walk [start,finish) destroying both strings, then free(start)); the pointee's virtual slot 1 on zero count.
// Shape: members in reverse order -- the +0x1c vector first, then ~CIVCountedPointer inlined with the zero store inside
// the non-null arm. Bytes +0x00..+0x17 are trivially destructible.
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

struct CWideStringRecordVector {                             // vector<{?, CWideString, CWideString, ...}> (0x18-byte records)
    void* m_pStart;
    void* m_pFinish;
    void* m_pEndOfStorage;
    ~CWideStringRecordVector();                              // 0x004AE040
};

struct CCountedComponentHolder_004ae3e0 {
    unsigned char _pad00[0x18];                              // +0x00 trivially destructible members
    CIVCountedPointer<CAnimComponentBase> m_pComponent;      // +0x18
    CWideStringRecordVector m_records;                       // +0x1c
    ~CCountedComponentHolder_004ae3e0();
};

CCountedComponentHolder_004ae3e0::~CCountedComponentHolder_004ae3e0()
{
}