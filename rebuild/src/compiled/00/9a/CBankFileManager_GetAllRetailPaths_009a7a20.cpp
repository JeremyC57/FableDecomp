// CBankFileManager::GetAllRetailPaths(set<CWideString>& paths) -- collect the full retail bank file path (base path + retail path) of every registered bank that has one
// Leaf: ?GetAllRetailPaths@CBankFileManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A7A20, 152 bytes, thiscall `ret 4`. PDB (Ego_r): `private: void __fastcall
// CBankFileManager::GetAllRetailPaths(set<CWideString>&)`. Catalog name "RBTree_ForEachNode" (descriptive). No static
// callers (reached from a sibling member through a computed call, or dead in retail).
// Callees: CWideString::GetLength() const @ 0x0099B220, operator+(const CWideString&, const CWideString&) @ 0x0099BE70
// (__fastcall: result in ecx, BasePath in edx, the retail path pushed), set<CWideString>::insert -> _M_t.insert_unique
// @ 0x009AAEB0 (this chunk), ~CWideString @ 0x0099B510.
// Layout: Ego_r.pdb CBankFileManager (Banks map<CCharString, CBankInfo> +0x04, BasePath +0x24); CBankInfo (PDB, 20 bytes):
// DevBankFilePath +0, RetailBankFilePath +4, DevHeaderFilePath +8, RetailHeaderFilePath +0xC, RegisteredBank +0x10.
// Shape: `for (it = Banks.begin(); it != Banks.end(); ++it) if (it->second.RetailBankFilePath.GetLength())
// paths.insert(BasePath + it->second.RetailBankFilePath);` -- STLport's _M_increment is inlined, end() (the header) is
// re-read at the bottom test; the operator+ temporary is homed in the dead `paths` argument slot and insert's
// pair<iterator, bool> result lands in the 8-byte frame.
class CWideString {
public:
    void* PStringData;                                      // +0x00
    CWideString(const CWideString& other);                  // 0x0099B720
    ~CWideString();                                         // 0x0099B510
    long GetLength() const;                                 // 0x0099B220
};
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);   // 0x0099BE70

class CCharString {
public:
    void* PStringData;                                      // +0x00
};
class CBankFile;

namespace std {
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

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

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};

template <class _Dummy> struct _Rb_global {
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* _M_node)
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Rb_tree_node_base* __y = _M_node->_M_parent;
            while (_M_node == __y->_M_right) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            if (_M_node->_M_right != __y)
                _M_node = __y;
        }
        return _M_node;
    }
};

template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Value* operator->() const { return &(operator*()); }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global<bool>::_M_increment(_M_node); return *this; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
};

template <class _Value> class _Rb_tree {
public:
    typedef _Rb_tree_iterator<_Value> iterator;
    _Rb_tree_node_base* _M_header;                          // +0x00 (header node: parent = root, left = leftmost)
    unsigned int _M_node_count;                             // +0x04
    unsigned int _M_key_compare;                            // +0x08 (empty comparator, padded)
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
    pair<iterator, bool> insert_unique(const _Value& __v);  // 0x009AAEB0 for set<CWideString>
};

template <class _Key, class _Tp> class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef typename _Rb_tree<value_type>::iterator iterator;
    _Rb_tree<value_type> _M_t;
    iterator begin() { return _M_t.begin(); }
    iterator end() { return _M_t.end(); }
};

template <class _Key> class set {
public:
    typedef typename _Rb_tree<_Key>::iterator iterator;
    _Rb_tree<_Key> _M_t;
    pair<iterator, bool> insert(const _Key& __x) { return _M_t.insert_unique(__x); }
};
}

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CBankFileManager : public CBaseClass {                // vtable 0x0129B018
public:
    class CBankInfo {                                       // PDB: 20 bytes
    public:
        CWideString DevBankFilePath;                        // +0x00
        CWideString RetailBankFilePath;                     // +0x04
        CWideString DevHeaderFilePath;                      // +0x08
        CWideString RetailHeaderFilePath;                   // +0x0c
        CBankFile*  RegisteredBank;                         // +0x10
    };
    std::map<CCharString, CBankInfo> Banks;                // +0x04
    void*                             RetailBanks;          // +0x10 (list<CCountedPointer<NBankFileManager::CRetailBank> >)
    bool                              UsingRetailBanks;     // +0x14
    std::map<CCharString, CCharString> BankAliasMap;       // +0x18
    CWideString                       BasePath;             // +0x24
    std::set<CWideString>            CreatedRetailBanks;   // +0x28
    unsigned long                     FileVersion;          // +0x34
private:
    void GetAllRetailPaths(std::set<CWideString>& paths);
};

void CBankFileManager::GetAllRetailPaths(std::set<CWideString>& paths)
{
    for (std::map<CCharString, CBankInfo>::iterator it = Banks.begin(); it != Banks.end(); ++it) {
        if (it->second.RetailBankFilePath.GetLength())
            paths.insert(BasePath + it->second.RetailBankFilePath);
    }
}