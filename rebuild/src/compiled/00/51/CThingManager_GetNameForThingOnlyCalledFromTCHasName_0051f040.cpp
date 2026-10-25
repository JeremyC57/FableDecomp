// CThingManager::GetNameForThing_OnlyCalledFromTCHasName(unsigned __int64 uid) -- returns a copy of the display name (CWideString) the NPC name map holds for the thing's UID
// Leaf: ?GetNameForThing_OnlyCalledFromTCHasName@CThingManager@@QAE?AVCWideString@@_K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051F040, 38 bytes, thiscall, CWideString through the hidden pointer, `ret 0xC`
//   (hidden pointer + the 8-byte UID).
// Identity: FableWin.pdb / Ego_r.pdb ?GetNameForThing_OnlyCalledFromTCHasName@CThingManager@@QAE?AVCWideString@@_K@Z;
//   its one caller is CTCHasName::GetNPCShortName 0x007F49CD (after the DoesNPCNameEntryExistForThing test 0x0051F010).
// Callees: NPCNameMap's _Rb_tree::_M_find(const unsigned __int64&) const 0x00527E00 (out of line, node pointer in eax;
//   the unsigned high-then-low key compare), CWideString copy constructor 0x0099B720.
// Layout: retail CThingManager::NPCNameMap (map<unsigned __int64, pair<CCharString, CWideString> >) at +0xCC (Ego_r has
//   it at +0xF4 with 16-byte maps); node value +0x10 = { UID +0x10, CCharString +0x18, CWideString +0x1C }.
// Shape: `return (*NPCNameMap.find(uid)).second.second;` -- no end() test (the caller checks first); find's iterator
//   folds to the node pointer, the UID goes by reference (its own argument slot), and the hidden result pointer is
//   loaded after the call.
// Label correction: catalogued as `vector<bool>::push_back` (propagated); the body is
//   CThingManager::GetNameForThing_OnlyCalledFromTCHasName.
class CCharString {
public:
    void* PData;                                            // CBasicString<char>* (4 bytes)
    ~CCharString();                                         // 0x0099EAE0
};

class CWideString {
public:
    void* PData;                                            // CBasicString<unsigned short>* (4 bytes)
    CWideString(const CWideString& other);                  // 0x0099B720
    ~CWideString();                                         // 0x0099B510
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _Tp> class allocator {
public:
    allocator() { }
};

struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0C
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};

template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
};

template <class _Key, class _Value, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_iterator<_Value> iterator;
    _Rb_tree_node_base* _M_header;                          // +0x00
    unsigned int        _M_node_count;                      // +0x04
    _Compare            _M_key_compare;                     // +0x08

    _Rb_tree_node_base* _M_find(const _Key& __k) const;     // 0x00527E00 for the NPC name map
    iterator find(const _Key& __k) { return iterator(_M_find(__k)); }
};

template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator find(const _Key& __x) { return _M_t.find(__x); }
};
}

class CThingManager {
public:
    unsigned char _Before[0xCC];                            // +0x00 .. +0xCB (see the Layout note)
    std::map<unsigned __int64, std::pair<CCharString, CWideString> > NPCNameMap;   // +0xCC

    CWideString GetNameForThing_OnlyCalledFromTCHasName(unsigned __int64 uid);
};

CWideString CThingManager::GetNameForThing_OnlyCalledFromTCHasName(unsigned __int64 uid)
{
    return (*NPCNameMap.find(uid)).second.second;
}