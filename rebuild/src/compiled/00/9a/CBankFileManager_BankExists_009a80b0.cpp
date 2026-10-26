// CBankFileManager::BankExists(const CCharString& bank) -- resolve the bank alias, then report whether the bank is known: in retail-bank mode whether any open retail bank's entry index holds it, otherwise whether it is a registered bank
// Leaf: ?BankExists@CBankFileManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A80B0, 145 bytes, thiscall `ret 4`. PDB (Ego_r): `public: bool __fastcall
// CBankFileManager::BankExists(class CCharString const &) const`. Catalog name "GetLanguageData" is wrong (the callee-canon
// label file already says BankExists). Callers: eight sites 0x00415099..0x00415236 and 0x009D9B5A.
// Callees: BankAliasMap.find @ 0x009AB5D0, CCharString copy ctor @ 0x0099EC30, a retail bank's entry-index find @ 0x009AB4F0
// (map at +0x18 of the bank; as the landed FindContainedBank 0x009A7F80), Banks.find @ 0x009AB560, ~CCharString @ 0x0099EAE0.
// Layout: retail CBankFileManager: Banks +0x04, RetailBanks +0x10 (STLport list<CCountedPointer<NBankFileManager::
// CRetailBank> > head node; the node holds the pointer at +8), UsingRetailBanks +0x14, BankAliasMap +0x18.
// Shape: same-TU noinline bodies for the three map finds and CBasicString<char>::operator< (VC7.1 then caches the
// list head across the calls), each find result a named local compared with end() after the call; the two-return
// ResolveAlias helper (as FindBank 0x009A7CA0: each arm pushes its own copy source), the retail
// branch walks the list through the counted pointer's operator-> without copying it; separate `return true` /
// `return false` exits (mov al,1 / xor al,al), the false exit laid right after the loop.
class CBasicStringChar {                                    // CBasicString<char> (13 bytes packed; only compared here)
public:
    char* PData;                                            // +0x00
    __forceinline long Compare(const CBasicStringChar& other) const
    {
        const char* b = other.PData;
        const char* a = PData;
        for (;;) {
            if (*a == 0 && *b == 0)
                return 0;
            if (*a < *b)
                return -1;
            if (*a > *b)
                return 1;
            ++a;
            ++b;
        }
    }
    bool operator<(const CBasicStringChar& other) const;    // 0x00429950 (body below, out of line)
};
#pragma auto_inline(off)
__declspec(noinline) bool CBasicStringChar::operator<(const CBasicStringChar& other) const
{
    return Compare(other) < 0;
}
#pragma auto_inline(on)
class CCharStringData { public:                                    // shared string block (Data, then NoRefs at +0x0D)
    CBasicStringChar Data;                                  // +0x00
};

class CCharString {
public:
    CCharStringData* PStringData;                           // +0x00
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData)
            return false;
        if (PStringData == 0)
            return true;
        if (other.PStringData == 0)
            return false;
        return PStringData->Data < other.PStringData->Data;
    }
};

template <class K, class V>
struct CRetailMap {                                         // STLport map<K, V>: {header, count, compare}
    struct SNode {
        unsigned char m_colour;
        SNode* m_pParent;
        SNode* m_pLeft;
        SNode* m_pRight;
        K first;                                            // +0x10
        V second;                                           // +0x14
    };
    SNode* m_pHead;                                         // +0x00
    unsigned long m_size;                                   // +0x04
    unsigned long m_compare;                                // +0x08
    SNode* find(const K& key) const;                        // out of line per instantiation (bodies below)
    SNode* end() const { return m_pHead; }
};

// The map finds (0x009AB4F0 / 0x009AB560 / 0x009AB5D0, STLport _M_find over CCharString keys) are COMDATs of this TU:
// with their bodies visible (kept out of line) VC7.1 knows they only read, so the list head stays in ebx across them.
#pragma auto_inline(off)
template <class K, class V>
__declspec(noinline) typename CRetailMap<K, V>::SNode* CRetailMap<K, V>::find(const K& k) const
{
    SNode* y = m_pHead;
    SNode* x = m_pHead->m_pParent;
    while (x != 0)
        if (!(x->first < k))
            y = x, x = x->m_pLeft;
        else
            x = x->m_pRight;
    if (y == m_pHead || k < y->first)
        y = m_pHead;
    return y;
}
#pragma auto_inline(on)

struct SBankEntryInfo {                                     // 20-byte per-entry record of a retail bank's index
    unsigned long Words[5];
};
struct SBankInfo {                                          // CBankFileManager::CBankInfo, 20 bytes
    void* Paths[4];
    void* RegisteredBank;
};

namespace NBankFileManager {
class CRetailBank {
public:
    unsigned char _pad00[0x18];
    CRetailMap<CCharString, SBankEntryInfo> EntryIndex;     // +0x18 (find @ 0x009AB4F0)
};
}

template <class T> class CCountedPointer { public:                 // {object, count block}
    T*    Data;                                             // +0x00
    void* Info;                                             // +0x04
    T* operator->() const { return Data; }
};

template <class T> struct CRetailList {                     // STLport list<T>: the header node pointer
    struct SNode {
        SNode* _M_next;                                     // +0x00
        SNode* _M_prev;                                     // +0x04
        T      _M_data;                                     // +0x08
    };
    struct const_iterator {
        const SNode* _M_node;
        const_iterator(const SNode* node) : _M_node(node) {}
        const T& operator*() const { return _M_node->_M_data; }
        const_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
        bool operator!=(const const_iterator& o) const { return _M_node != o._M_node; }
    };
    SNode* _M_node;                                         // +0x00
    const_iterator begin() const { return const_iterator(_M_node->_M_next); }
    const_iterator end() const { return const_iterator(_M_node); }
};

class CBankFileManager {
public:
    void*                                                   _vfptr;            // +0x00 (CBaseClass)
    CRetailMap<CCharString, SBankInfo>                      Banks;             // +0x04
    CRetailList<CCountedPointer<NBankFileManager::CRetailBank> > RetailBanks;  // +0x10
    bool                                                    UsingRetailBanks;  // +0x14
    CRetailMap<CCharString, CCharString>                    BankAliasMap;      // +0x18
    CCharString ResolveAlias(const CCharString& name) const
    {
        CRetailMap<CCharString, CCharString>::SNode* alias = BankAliasMap.find(name);
        if (alias != BankAliasMap.end())
            return alias->second;
        return name;
    }
    bool BankExists(const CCharString& bank) const;
};

bool CBankFileManager::BankExists(const CCharString& bank) const
{
    CCharString key = ResolveAlias(bank);
    if (UsingRetailBanks) {
        for (CRetailList<CCountedPointer<NBankFileManager::CRetailBank> >::const_iterator it = RetailBanks.begin();
             it != RetailBanks.end(); ++it) {
            CRetailMap<CCharString, SBankEntryInfo>::SNode* entry = (*it)->EntryIndex.find(key);
            if (entry != (*it)->EntryIndex.end())
                return true;
        }
    }
    else {
        CRetailMap<CCharString, SBankInfo>::SNode* info = Banks.find(key);
        if (info != Banks.end())
            return true;
    }
    return false;
}