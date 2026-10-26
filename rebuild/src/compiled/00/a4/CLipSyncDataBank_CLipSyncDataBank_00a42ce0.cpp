// CLipSyncDataBank::~CLipSyncDataBank() -- empty Entries, then the members (ResourceBank, Entries) and the CBankFile base
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A42CE0, length 192 bytes.
// AUDIO/LIP-SYNC LAYER (replacement work later): true class CLipSyncDataBank -- the body stores vtable 0x0129D3D4
// (rtti-vtables.tsv: CLipSyncDataBank) and its only caller is the deleting destructor 0x00A43FD0 (slot 0). The catalog
// label "C3DClothPrimitive::~C3DClothPrimitive" is a propagated byte twin.
// Callees: STLport __copy<CLipSyncDataBankEntry*>(first, last, result, const random_access_iterator_tag&, int*) @ 0x00A438E0
// (__fastcall; catalog "_Move<NGameText::CDataBankTextEntry*...>" twin), _Destroy(first, last, const __false_type&) @ 0x00A43A00
// (__fastcall), operator delete[]/free @ 0x00BFEA14, operator delete @ 0x00BFE9BC, CBankFile::~CBankFile @ 0x009D6490.
// Retail offsets (retail CBankFile is 0x164; PDB containers 4 bytes wider): Entries (CArray<CLipSyncDataBankEntry>, 12 bytes)
// +0x164, ResourceBank (CCountedPointer<CResourceBank>) +0x170 (PDB +0x1d8/+0x1e8).
// Shape: `Entries.clear()` as the inline STLport erase(begin(), end()) (the landed 0x005EC5C0 shape), then the member
// destructors in reverse order: the counted pointer (re-read release, zero stores) and the vector (range destroy, free).
// Leaf: ??1CLipSyncDataBank
struct SCountBlock {
    long m_references;                              // +0
    void (__fastcall* m_pfnDestroy)(void*);         // +4
    void* m_pObject;                                // +8
};
template<class T> class CCountedPointer { public:
    T* Object;                                      // +0
    SCountBlock* Count;                             // +4
    ~CCountedPointer()
    {
        if (Count) {
            --Count->m_references;
            if (Count->m_references == 0) {
                Count->m_pfnDestroy(Count->m_pObject);
                delete Count;
            }
        }
        Object = 0;
        Count = 0;
    }
};

struct random_access_iterator_tag { random_access_iterator_tag() {} };
struct __false_type { __false_type() {} };
extern "C" void __cdecl free(void* p);              // 0x00BFEA14

class CLipSyncDataBankEntry { public: void* Data; long m_04; };
CLipSyncDataBankEntry* __fastcall __copy(CLipSyncDataBankEntry* first, CLipSyncDataBankEntry* last, CLipSyncDataBankEntry* result,
                                         const random_access_iterator_tag& tag, int* distance);   // 0x00A438E0
void __fastcall _Destroy(CLipSyncDataBankEntry* first, CLipSyncDataBankEntry* last, const __false_type& trivial);   // 0x00A43A00

struct CEntryVector {                               // STLport vector<CLipSyncDataBankEntry> (12 bytes)
    CLipSyncDataBankEntry* _M_start;
    CLipSyncDataBankEntry* _M_finish;
    CLipSyncDataBankEntry* _M_end_of_storage;
    CLipSyncDataBankEntry* begin() { return _M_start; }
    CLipSyncDataBankEntry* end() { return _M_finish; }
    CLipSyncDataBankEntry* erase(CLipSyncDataBankEntry* first, CLipSyncDataBankEntry* last)
    {
        CLipSyncDataBankEntry* i = __copy(last, _M_finish, first, random_access_iterator_tag(), (int*)0);
        _Destroy(i, _M_finish, __false_type());
        _M_finish = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
    ~CEntryVector()
    {
        _Destroy(_M_start, _M_finish, __false_type());
        if (_M_start)
            free(_M_start);
    }
};

class CResourceBank;
namespace N_009d6490 { struct CBankFile {
    virtual ~CBankFile();                           // 0x009D6490
    unsigned char _body[0x160];                     // retail CBankFile is 0x164 bytes
}; } using N_009d6490::CBankFile;

class CLipSyncDataBank : public CBankFile { public:
    CEntryVector Entries;                           // +0x164
    CCountedPointer<CResourceBank> ResourceBank;    // +0x170
    virtual ~CLipSyncDataBank();
};

CLipSyncDataBank::~CLipSyncDataBank()
{
    Entries.clear();
}