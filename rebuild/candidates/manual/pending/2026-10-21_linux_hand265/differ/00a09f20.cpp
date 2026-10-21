// CMeshBankManager::OpenMeshBank(const CCharString&, bool) -- return the already-open mesh bank of that name, else create, open and register a new one
// Grade: DIFFER(488v485) -- one instruction: the found-path add-ref on the returned copy is mov/inc/mov through ecx->esi; retail allocates esi directly and emits inc dword ptr [esi]. Everything else (loop, auto_inline(off) Compare, counted pointer ctor/assign/copy, inline push_back) matches
// Retail 0x00A09F20, 482 bytes, `ret 0xc` (hidden CCountedPointer<CMeshDataBank> return, name, unused bool).
// Callees: Compare 0x00411570 (__fastcall, the inline CCharString ==), operator new 0x00BFEA1A, CMeshDataBank ctor
//   0x00A27030 (0x460 bytes), CMeshDataBank vslot 1 Open(name, flags), CCharString() 0x0099E4B0, operator=
//   0x0099EFB0, copy ctor 0x0099EC30, ~CCharString 0x0099EAE0, malloc 0x00BFEA0E (list node), operator delete
//   0x00BFE9BC, the counted pointer's delete function 0x00428AE7. Global 0x013CA7B0 (bool) picks the open flags.
// Layout: MeshDataBanks (STLport list<pair<CCharString, CCountedPointer<CMeshDataBank> > >, head node pointer) at
//   +8; node {next, prev, name +8, counted pointer {Data +0xc, Info +0x10}} (0x14 bytes, malloc'd).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
void operator delete(void* p);                        // 0x00BFE9BC
inline void* operator new(unsigned int, void* where) { return where; }

struct CCharStringData {
    const char*   m_pData;
    unsigned long m_length;
};

#pragma auto_inline(off)
long __fastcall Compare_00411570(const char* a, const char* b)   // 0x00411570 (the landed body)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)

struct CCharString {
    CCharStringData* PStringData;
    CCharString();                                   // 0x0099E4B0
    CCharString(const CCharString& other);           // 0x0099EC30
    ~CCharString();                                  // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->m_length == other.PStringData->m_length)
            return Compare_00411570(PStringData->m_pData, other.PStringData->m_pData) ? false : true;
        return false;
    }
};

void __fastcall DeleteMeshDataBank(void* data);       // 0x00428AE7

struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
    CCPPointerInfo(void* p) : RefCount(1), DeleteFunc(DeleteMeshDataBank), Data(p) { }
};

template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(T* p) : Data(p), Info(p ? new CCPPointerInfo(p) : 0) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    ~CCountedPointer()
    {
        Release();
        Data = 0;
        Info = 0;
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        T* data = other.Data;
        CCPPointerInfo* info = other.Info;
        if (Info != info) {
            Release();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
    T* operator->() const { return Data; }
};

struct CMeshDataBank {
    CMeshDataBank();                                  // 0x00A27030
    virtual void v00();
    virtual void Open(const CCharString& name, unsigned long flags);   // vslot 1
    unsigned char _pad04[0x460 - 4];
};

extern bool g_13CA7B0;                                // 0x013CA7B0

struct CMeshBankEntry {
    CCharString                    first;
    CCountedPointer<CMeshDataBank> second;
};

struct CMeshBankNode {
    CMeshBankNode* next;
    CMeshBankNode* prev;
    CMeshBankEntry value;
};

struct CMeshBankManager {
    unsigned char  _pad00[8];
    CMeshBankNode* MeshDataBanks;                     // +0x08
    CCountedPointer<CMeshDataBank> OpenMeshBank(const CCharString& name, bool async);
};

CCountedPointer<CMeshDataBank> CMeshBankManager::OpenMeshBank(const CCharString& name, bool async)
{
    for (CMeshBankNode* node = MeshDataBanks->next; node != MeshDataBanks; node = node->next)
    {
        if (node->value.first == name)
            return node->value.second;
    }
    CCountedPointer<CMeshDataBank> pointer(new CMeshDataBank);
    unsigned long flags = 0;
    if (!g_13CA7B0)
        flags = 0xf8;
    pointer->Open(name, flags);
    CMeshBankEntry entry;
    entry.first = name;
    entry.second = pointer;
    CMeshBankNode* pos = MeshDataBanks;
    CMeshBankNode* node = (CMeshBankNode*)malloc(sizeof(CMeshBankNode));
    if (&node->value)
        new (&node->value) CMeshBankEntry(entry);
    CMeshBankNode* prev = pos->prev;
    node->next = pos;
    node->prev = prev;
    prev->next = node;
    pos->prev = node;
    return pointer;
}
