// Behaviour test for 009a7f80: src/009a7f80.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CBankFileManager::FindContainedBank(const CCharString& name, SBankEntryInfo* outInfo, CCountedPointer<CBankFile>* outBank)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CBankFileManager::FindContainedBank(const CCharString& name, SBankEntryInfo* outInfo, CCountedPointer<CBankFile>* outBank) -- resolve the name through the alias map, then search every open bank's entry index for it; on a hit copy the 20-byte entry record and the bank pointer out and return true
// Leaf: ?FindContainedBank@CBankFileManager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A7F80, length 270 bytes (`ret 0Ch`).
// Callees: map<CCharString,CCharString>::find @ 0x009AB5D0 (aliases), CCharString copy ctor @ 0x0099EC30,
//          map<CCharString,SBankEntryInfo>::find @ 0x009AB4F0 (a bank's index at +0x18), ~CCharString @ 0x0099EAE0,
//          operator delete @ 0x00BFE9BC (count blocks; the per-type deleter is called through the block, ecx = object).
// Layout (local): the manager's STLport list of CCountedPointer<CBankFile> at +0x10 (4-byte head), the alias map at +0x18.
// Shape: the bank copy releases with the short DecRefToZero form (block in esi), the out-pointer assignment with the
//          long re-reading form (CCountedPointer::operator= with the `Info != o.Info` guard). The alias resolution is an
//          inline by-value member (`if (alias != end) return alias->second; return name;`): each return copy-constructs
//          into the caller's local, so the local's address is formed before the branch and each arm pushes its own source
//          (`add eax,14h ; push eax ; jmp` / `push ebx`). A `?:` in the declaration, or a reference-returning resolver,
//          shares one `push eax` instead (1 byte longer).
void __cdecl operator delete(void* block);                 // 0x00BFE9BC

struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
};

struct SCountBlock {                                       // CCPPointerInfo: count, per-type deleter, object
    long  m_references;                                    // +0
    void (__fastcall* m_pfnDestroy)(void*);                // +4
    void* m_pObject;                                       // +8
};
__forceinline bool DecRefToZero(long& count) { return --count == 0; }

template <class T> struct CCountedPointer {                // {object, count block}
    T*           Data;                                     // +0
    SCountBlock* Info;                                     // +4
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->m_references; }
    ~CCountedPointer()
    {
        if (Info && DecRefToZero(Info->m_references)) {
            Info->m_pfnDestroy(Info->m_pObject);
            delete Info;
        }
    }
    CCountedPointer& operator=(const CCountedPointer& o)
    {
        if (Info != o.Info) {
            if (Info) {
                --Info->m_references;
                if (Info->m_references == 0) {
                    Info->m_pfnDestroy(Info->m_pObject);
                    delete Info;
                }
            }
            Data = o.Data;
            Info = o.Info;
            if (Info)
                ++Info->m_references;
        }
        return *this;
    }
    T* operator->() const { return Data; }
};

struct SBankEntryInfo {                                    // 20-byte per-entry record of a bank's index
    unsigned long Words[5];
};

template <class K, class V> struct CRetailMap {            // STLport map: header node, count, compare (12 bytes)
    struct Node { void* _M_links[4]; K first; V second; };
    Node*         _M_header;
    unsigned long _M_node_count;
    unsigned long _M_key_compare;
    Node* find(const K& key);                              // out of line
    Node* end() const { return _M_header; }
};

class CBankFile {
public:
    unsigned char _pad00[0x18];
    CRetailMap<CCharString, SBankEntryInfo> EntryIndex;   // +0x18 (find @ 0x009AB4F0)
};

struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
struct CBankListNode : _List_node_base { CCountedPointer<CBankFile> Bank; };

class CBankFileManager {
public:
    unsigned char      _pad00[0x10];
    CBankListNode*     Banks;                              // +0x10 STLport list<CCountedPointer<CBankFile> > head
    unsigned char      _pad14[4];
    CRetailMap<CCharString, CCharString> Aliases;          // +0x18 (find @ 0x009AB5D0)
    CCharString GetBankName(const CCharString& name)
    {
        CRetailMap<CCharString, CCharString>::Node* alias = Aliases.find(name);
        if (alias != Aliases.end())
            return alias->second;
        return name;
    }
    bool FindContainedBank(const CCharString& name, SBankEntryInfo* outInfo, CCountedPointer<CBankFile>* outBank);
};

bool CBankFileManager::FindContainedBank(const CCharString& name, SBankEntryInfo* outInfo, CCountedPointer<CBankFile>* outBank)
{
    CCharString bankName = GetBankName(name);
    for (CBankListNode* it = (CBankListNode*)Banks->_M_next; it != Banks; it = (CBankListNode*)it->_M_next) {
        CCountedPointer<CBankFile> bank(it->Bank);
        CRetailMap<CCharString, SBankEntryInfo>::Node* entry = bank->EntryIndex.find(bankName);
        if (entry != bank->EntryIndex.end()) {
            *outInfo = entry->second;
            *outBank = bank;
            return true;
        }
    }
    return false;
}

// ---- stubs and driver ----
#include <cstdlib>
static int g_copies = 0, g_dtors = 0, g_destroyed = 0;
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_copies; }
CCharString::~CCharString() { ++g_dtors; }
static CCharString MakeStr(const char* s) { CCharString* p = (CCharString*)calloc(1, sizeof(CCharString)); p->PStringData = (void*)s; return *p; }
template <> CRetailMap<CCharString, CCharString>::Node* CRetailMap<CCharString, CCharString>::find(const CCharString& key)
{
    Node* n = (Node*)_M_header->_M_links[1];   // the single entry hangs off the header's parent link
    if (n && strcmp((const char*)n->first.PStringData, (const char*)key.PStringData) == 0) return n;
    return _M_header;
}
template <> CRetailMap<CCharString, SBankEntryInfo>::Node* CRetailMap<CCharString, SBankEntryInfo>::find(const CCharString& key)
{
    Node* n = (Node*)_M_header->_M_links[1];
    if (n && strcmp((const char*)n->first.PStringData, (const char*)key.PStringData) == 0) return n;
    return _M_header;
}
static void __fastcall DestroyBank(void*) { ++g_destroyed; }
template <class K, class V> static void InitMap(CRetailMap<K, V>& m, const char* key, typename CRetailMap<K, V>::Node** entryOut)
{
    typedef typename CRetailMap<K, V>::Node Node;
    Node* header = (Node*)calloc(1, sizeof(Node));
    Node* entry = (Node*)calloc(1, sizeof(Node));
    entry->first.PStringData = (void*)key;
    header->_M_links[1] = entry;
    m._M_header = header; m._M_node_count = 1; m._M_key_compare = 0;
    *entryOut = entry;
}
int main()
{
    CBankFileManager* mgr = (CBankFileManager*)calloc(1, sizeof(CBankFileManager));
    CRetailMap<CCharString, CCharString>::Node* aliasNode;
    InitMap(mgr->Aliases, "Alias", &aliasNode);
    aliasNode->second.PStringData = (void*)"RealBank";
    CBankFile* bankA = (CBankFile*)calloc(1, sizeof(CBankFile));
    CBankFile* bankB = (CBankFile*)calloc(1, sizeof(CBankFile));
    CRetailMap<CCharString, SBankEntryInfo>::Node* entryA; CRetailMap<CCharString, SBankEntryInfo>::Node* entryB;
    InitMap(bankA->EntryIndex, "Other", &entryA);
    InitMap(bankB->EntryIndex, "RealBank", &entryB);
    for (int k = 0; k < 5; ++k) { entryA->second.Words[k] = 100 + k; entryB->second.Words[k] = 200 + k; }
    // list: head <-> nodeA <-> nodeB
    CBankListNode* head = (CBankListNode*)calloc(1, sizeof(CBankListNode));
    CBankListNode* nodeA = (CBankListNode*)calloc(1, sizeof(CBankListNode));
    CBankListNode* nodeB = (CBankListNode*)calloc(1, sizeof(CBankListNode));
    head->_M_next = nodeA; nodeA->_M_next = nodeB; nodeB->_M_next = head;
    head->_M_prev = nodeB; nodeB->_M_prev = nodeA; nodeA->_M_prev = head;
    SCountBlock* blockA = new SCountBlock; blockA->m_references = 1; blockA->m_pfnDestroy = DestroyBank; blockA->m_pObject = bankA;
    SCountBlock* blockB = new SCountBlock; blockB->m_references = 1; blockB->m_pfnDestroy = DestroyBank; blockB->m_pObject = bankB;
    nodeA->Bank.Data = bankA; nodeA->Bank.Info = blockA; nodeB->Bank.Data = bankB; nodeB->Bank.Info = blockB;
    mgr->Banks = head;
    SBankEntryInfo info; memset(&info, 0, sizeof(info));
    CCountedPointer<CBankFile>* out = (CCountedPointer<CBankFile>*)calloc(1, sizeof(CCountedPointer<CBankFile>));
    CCharString alias = MakeStr("Alias");
    int copies = g_copies, dtors = g_dtors;
    // aliased name resolves to RealBank, found in the second bank
    CHECK(mgr->FindContainedBank(alias, &info, out));
    CHECK(info.Words[0] == 200 && info.Words[4] == 204 && out->Data == bankB && out->Info == blockB);
    CHECK(blockB->m_references == 2 && blockA->m_references == 1 && g_destroyed == 0);
    CHECK(g_copies - copies == 1 && g_dtors - dtors == 1);
    // direct (unaliased) name found in the first bank; the old out bank is released
    CCharString other = MakeStr("Other");
    CHECK(mgr->FindContainedBank(other, &info, out));
    CHECK(info.Words[0] == 100 && out->Data == bankA && blockA->m_references == 2 && blockB->m_references == 1);
    // missing name: false, outputs untouched
    CCharString missing = MakeStr("Missing");
    info.Words[0] = 7;
    CHECK(!mgr->FindContainedBank(missing, &info, out));
    CHECK(info.Words[0] == 7 && out->Data == bankA && blockA->m_references == 2 && blockB->m_references == 1 && g_destroyed == 0);
    if (fails == 0) printf("009a7f80_TEST PASS\n");
    return fails;
}
