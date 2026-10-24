// CBankFileManager::GetRetailBankFilePath(const CCharString& bank) -- resolve the bank alias, build BasePath + the bank's retail bank path, create that retail bank file the first time it is asked for, and return the path; an unknown bank throws CGenericException("<name> bank not found!")
// Bounds: int3-after-noreturn row, landed against the N+1 span: only the int3 VC7.1 emits after the final noreturn _CxxThrowException call (q.py strips it from the retail row); RELOCATION_MATCH against the 223-byte N+1 span read from Fable.exe (probe/n1.py). Land against an N+1 row (GOTCHAS: int3-after-noreturn rows LANDED).
// Leaf: ?GetRetailBankFilePath@CBankFileManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A7D40, 223 bytes (the int3 VC7.1 emits after the final noreturn
// _CxxThrowException call included; one more 0xCC pads the 224-byte row), thiscall `ret 8` (hidden CWideString result +
// bank). PDB (Ego_r): `public: class CWideString __fastcall CBankFileManager::GetRetailBankFilePath(class CCharString
// const &)` (QAE: not const -- it records the created bank). Catalog name "CBankFileManager_FindOrCreateBank" is
// descriptive. Caller 0x009CB9D4.
// Callees: BankAliasMap.find @ 0x009AB5D0 and Banks.find @ 0x009AB560 (out of line, return the node), CCharString copy
// ctor @ 0x0099EC30, operator+(const CWideString&, const CWideString&) @ 0x0099BE70 (__fastcall), CreatedRetailBanks
// .find -> _M_find @ 0x009AA4C0 and .insert -> insert_unique @ 0x009AAEB0 (this chunk), CreateRetailBank @ 0x009A7720
// (landed), CWideString copy ctor @ 0x0099B720 (the by-value return, no NRVO), ~CWideString @ 0x0099B510,
// ~CCharString @ 0x0099EAE0, operator+(const CCharString&, const char*) @ 0x0099F600, CCharString::operator const char*
// @ 0x0099E4C0, _CxxThrowException @ 0x00BFEB84 (ThrowInfo 0x013692F8: CGenericException : CExceptionBase, 1 byte).
// Layout: Ego_r.pdb CBankFileManager with retail containers: Banks +0x04, BankAliasMap +0x18, BasePath +0x24,
// CreatedRetailBanks +0x28; CBankInfo::RetailBankFilePath at node value +4 (node +0x18).
// Shape: the landed FindBank (0x009A7CA0) front end -- the key is a frame local copy-constructed through the two-return
// ResolveAlias helper -- then the path homed in the dead bank argument slot, `CreatedRetailBanks.find(path) ==
// CreatedRetailBanks.end()` (end() read before the call), and the pair<iterator, bool> insert result in the frame. No
// /EHsc in this TU: nothing is destroyed on the throw path.
class CWideString {
public:
    void* PStringData;                                      // +0x00
    CWideString(const CWideString& other);                  // 0x0099B720
    ~CWideString();                                         // 0x0099B510
    bool operator<(const CWideString& other) const;         // 0x0099AF70
};
CWideString __fastcall operator+(const CWideString& left, const CWideString& right);   // 0x0099BE70

class CCharString {
public:
    void* PStringData;                                      // +0x00
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    operator const char*() const;                           // 0x0099E4C0
};
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600

class CExceptionBase {
};
class CGenericException : public CExceptionBase {
public:
    CGenericException(const char* message) {}
};

class CBankFile;
struct CBankInfo {                                          // CBankFileManager::CBankInfo, 20 bytes (PDB)
    CWideString DevBankFilePath;                            // +0x00
    CWideString RetailBankFilePath;                         // +0x04
    CWideString DevHeaderFilePath;                          // +0x08
    CWideString RetailHeaderFilePath;                       // +0x0c
    CBankFile*  RegisteredBank;                             // +0x10
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
    SNode* find(const K& key);                              // Banks: 0x009AB560, BankAliasMap: 0x009AB5D0
    SNode* end() const { return m_pHead; }
};

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x00
    _T2 second;                                             // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
};
struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0c
};
template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
};
template <class _Key> class set {
public:
    typedef _Rb_tree_iterator<_Key> iterator;
    _Rb_tree_node_base* _M_header;                          // +0x00
    unsigned int _M_node_count;                             // +0x04
    unsigned int _M_key_compare;                            // +0x08
    _Rb_tree_node_base* _M_find(const _Key& __k) const;     // 0x009AA4C0
    pair<iterator, bool> insert_unique(const _Key& __v);    // 0x009AAEB0
    iterator end() { return iterator(_M_header); }
    iterator find(const _Key& __x) { return iterator(_M_find(__x)); }
    pair<iterator, bool> insert(const _Key& __x) { return insert_unique(__x); }
};
}

class CBankFileManager {
public:
    void*                                 _vfptr;               // +0x00 (CBaseClass)
    CRetailMap<CCharString, CBankInfo>    Banks;                // +0x04
    void*                                 RetailBanks;          // +0x10
    bool                                  UsingRetailBanks;     // +0x14
    CRetailMap<CCharString, CCharString>  BankAliasMap;         // +0x18
    CWideString                           BasePath;             // +0x24
    _STL::set<CWideString>                CreatedRetailBanks;   // +0x28
    unsigned long                         FileVersion;          // +0x34
    CCharString ResolveAlias(const CCharString& name)
    {
        CRetailMap<CCharString, CCharString>::SNode* alias = BankAliasMap.find(name);
        if (alias != BankAliasMap.end())
            return alias->second;
        return name;
    }
    void CreateRetailBank(const CWideString& path);         // 0x009A7720
    CWideString GetRetailBankFilePath(const CCharString& bank);
};

CWideString CBankFileManager::GetRetailBankFilePath(const CCharString& bank)
{
    CCharString key = ResolveAlias(bank);
    CRetailMap<CCharString, CBankInfo>::SNode* info = Banks.find(key);
    if (info != Banks.end()) {
        CWideString path = BasePath + info->second.RetailBankFilePath;
        if (CreatedRetailBanks.find(path) == CreatedRetailBanks.end()) {
            CreateRetailBank(path);
            CreatedRetailBanks.insert(path);
        }
        return path;
    }
    throw CGenericException(key + " bank not found!");
}