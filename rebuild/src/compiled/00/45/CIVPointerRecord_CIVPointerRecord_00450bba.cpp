// CIVPointerRecord::~CIVPointerRecord() -- destructor of a record whose only non-trivial member is a CIVCountedPointer at +0x0c
// Leaf: ??0?$pair@
// Retail: Fable.exe @ 0x00450BBA, 26 bytes; catalog label "_Dest_val<allocator<pair<EAnimComponent,CIVCountedPointer<..>>>>" is
// wrong (that helper takes the allocator and pointer as arguments; this is a thiscall with `ret`). Callers: 0x00451504
// (`add ecx,4 ; jmp` -- an owner's dtor destroying its member at +4) and the folded vector-deleting dtor 0x00451C83.
// Body: CIVCountedPointer release -- `if (p) { p->Release(); p = 0; }` with Release = `if (--IVRefCount == 0) slot1()`.
// Layout: +0x00..+0x0b trivially destructible data (unknown, folded body); +0x0c CIVCountedPointer<T> (one pointer).
// Opt: size TU (`and dword ptr [esi+0Ch],0`).
#pragma optimize("s", on)
class CIVCountedPointeeBase { public:
    virtual ~CIVCountedPointeeBase();
    virtual void DeleteThis();                          // slot 1 (+4)
    unsigned long IVRefCount;                           // +0x04
    __forceinline void Release() { if (--IVRefCount == 0) DeleteThis(); }
};
template <class T> class CIVCountedPointer { public:
    T* Object;
    __forceinline ~CIVCountedPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
};
struct CIVPointerRecord {
    long Data[3];                                       // +0x00
    CIVCountedPointer<CIVCountedPointeeBase> Pointer;   // +0x0c
    ~CIVPointerRecord();
};
CIVPointerRecord::~CIVPointerRecord() {}