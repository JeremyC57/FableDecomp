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