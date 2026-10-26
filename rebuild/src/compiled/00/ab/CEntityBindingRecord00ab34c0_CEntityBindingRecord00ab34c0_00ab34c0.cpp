// CEntityBindingRecord_00ab34c0::~CEntityBindingRecord_00ab34c0() -- destroy an entity-binding record: release its counted pointer at +0x18, destroy its vector of counted pointers at +0xc (elements, then storage), then its name string
// Leaf: ??1CEntityBindingRecord_00ab34c0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB34C0, length 95 bytes (+ int3 padding), plain `ret`. Callers: the
// non-virtual ??_G 0x00AB3620 and the CCountedPointer per-type deleter 0x00AB3640 (`if (p) { ~T; operator delete }`), whose
// address the record-building helpers 0x00AB1990 / 0x00AB1CC0 / 0x00AB1F30 (manifest "BuildEntityScriptBindings_*") store
// in a fresh count block -- so the record is non-polymorphic and created through CCountedPointer<T>(new T). Its class is
// not recoverable (descriptive name); the manifest label is propagated and wrong.
// Callees: CCPPointerInfo::DeleteFunc through the block, operator delete @ 0x00BFE9BC (the block), the element destroy
// loop 0x00AB2BD0 (`__fastcall` first/last + the iterator tag by reference: 8-byte CCountedPointer elements), free
// @ 0x00BFEA14 (the storage), CCharString::~CCharString @ 0x0099EAE0.
// Shape: members in reverse order; the vector's inline destructor is the landed CStringKeyedTable 0x006FD040 form (destroy
// range with a tag temporary in the high byte of the reserved `push ecx` slot, `lea ecx,[esp+7]`, then `if (start) free`);
// the frame slot keeps the last destructor a plain call.
void __cdecl operator delete(void* p);                       // 0x00BFE9BC
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14

class CCharString {
public:
    void* PStringData;
    ~CCharString();                                          // 0x0099EAE0
};

class CCPPointerInfo { public:                                    // Ego_r.pdb: RefCount +0, DeleteFunc +4, Data +8
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void* data);               // called with Data in ecx, nothing on the stack
    void* Data;
};
template<class T> class CCountedPointer { public:
    T*              Data;                                    // +0x00
    CCPPointerInfo* Info;                                    // +0x04
    void ReleaseInfo()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    ~CCountedPointer() { ReleaseInfo(); Data = 0; Info = 0; }
};

struct SNonscalarPtrIteratorTag { SNonscalarPtrIteratorTag() { } };

struct CBindingTarget;
void __fastcall DestroyRange(CCountedPointer<CBindingTarget>* first, CCountedPointer<CBindingTarget>* last,
                             const SNonscalarPtrIteratorTag& tag);   // 0x00AB2BD0

struct CBindingVector {                                      // STLport vector<CCountedPointer<CBindingTarget> >
    CCountedPointer<CBindingTarget>* _M_start;
    CCountedPointer<CBindingTarget>* _M_finish;
    CCountedPointer<CBindingTarget>* _M_end_of_storage;
    ~CBindingVector()
    {
        DestroyRange(_M_start, _M_finish, SNonscalarPtrIteratorTag());
        if (_M_start)
            free(_M_start);
    }
};

struct CBindingOwner;

struct CEntityBindingRecord_00ab34c0 {
    CCharString                    m_name;                   // +0x00
    long                           m_value04;                // +0x04
    long                           m_value08;                // +0x08
    CBindingVector                 m_bindings;               // +0x0c
    CCountedPointer<CBindingOwner> m_owner;                  // +0x18
    ~CEntityBindingRecord_00ab34c0();
};

CEntityBindingRecord_00ab34c0::~CEntityBindingRecord_00ab34c0()
{
}