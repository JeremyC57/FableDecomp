// CRegionConnectionRecord_00aecb80::~CRegionConnectionRecord_00aecb80() -- destroy a region-connection record: its two strings, its counted pointer at +0x38, its vector of {id, counted node} pairs at +0x28, then the string-keyed table at +0
// Leaf: ??1CRegionConnectionRecord_00aecb80
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AECB80, length 111 bytes (+ int3), plain `ret`. Callers: the non-virtual
// ??_G 0x00AECC10 and the CCountedPointer per-type deleter 0x00AECCA0, stored by Reset 0x00AECCC0 -- a non-polymorphic
// record created through CCountedPointer<T>(new T). The landed ~CStringKeyedTable 0x006FD040 names this function as the
// record destructor holding that table at +0. Its class is not recoverable (descriptive name); the manifest label
// (_Dest_val<allocator<CMapInfo>>) is propagated and wrong.
// Callees: CCharString::~CCharString @ 0x0099EAE0 (x2), CCPPointerInfo::DeleteFunc through the block, operator delete
// @ 0x00BFE9BC (the block), the element destroy loop 0x00AE9D50 (`__fastcall` first/last + tag by reference; 12-byte
// pair<long, CCountedPointer<CRegionConnectionNode>> elements -- manifest label), free @ 0x00BFEA14,
// CStringKeyedTable_006fd040::~CStringKeyedTable_006fd040 @ 0x006FD040.
// Shape: members in reverse order; the vector's inline destructor is the landed 0x006FD040 form (tag temporary in the high
// byte of the reserved `push ecx` slot, `lea ecx,[esp+7]`, then `if (start) free`); the table's dtor stays a plain call.
void __cdecl operator delete(void* p);                       // 0x00BFE9BC
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14

class CCharString {
public:
    void* PStringData;
    ~CCharString();                                          // 0x0099EAE0
};

struct CCPPointerInfo {                                    // Ego_r.pdb: RefCount +0, DeleteFunc +4, Data +8
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void* data);               // called with Data in ecx, nothing on the stack
    void* Data;
};
template<class T> struct CCountedPointer {
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

struct CRegionConnectionNode;
struct SNodeEntry {                                          // pair<long, CCountedPointer<CRegionConnectionNode> >
    long                                   first;
    CCountedPointer<CRegionConnectionNode> second;
};
void __fastcall DestroyRange(SNodeEntry* first, SNodeEntry* last, const SNonscalarPtrIteratorTag& tag);   // 0x00AE9D50

struct CNodeEntryVector {                                    // STLport vector<SNodeEntry>
    SNodeEntry* _M_start;
    SNodeEntry* _M_finish;
    SNodeEntry* _M_end_of_storage;
    ~CNodeEntryVector()
    {
        DestroyRange(_M_start, _M_finish, SNonscalarPtrIteratorTag());
        if (_M_start)
            free(_M_start);
    }
};

struct CStringKeyedTable_006fd040 {                          // polymorphic (CBase root), 0x20 bytes
    virtual void v00();
    unsigned char _body[0x1c];
    ~CStringKeyedTable_006fd040();                           // 0x006FD040
};

struct CConnectionOwner;

struct CRegionConnectionRecord_00aecb80 {
    CStringKeyedTable_006fd040        m_table;               // +0x00
    long                              m_value20;             // +0x20
    long                              m_value24;             // +0x24
    CNodeEntryVector                  m_nodes;               // +0x28
    long                              m_value34;             // +0x34
    CCountedPointer<CConnectionOwner> m_owner;               // +0x38
    long                              m_value40;             // +0x40
    CCharString                       m_name;                // +0x44
    CCharString                       m_scriptName;          // +0x48
    ~CRegionConnectionRecord_00aecb80();
};

CRegionConnectionRecord_00aecb80::~CRegionConnectionRecord_00aecb80()
{
}