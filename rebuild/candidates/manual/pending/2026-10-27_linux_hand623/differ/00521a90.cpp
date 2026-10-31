// CThingManager::SetNameForThing(unsigned __int64 uid, const CWideString& name) -- NPC name map: creates the entry with an empty script name when the uid is new, then stores the display name
// Grade: DIFFER(78v78) -- register colouring only: retail `lea eax` for the first operator[] key and `mov ecx,[name] ; push ecx ; lea edx ; mov ecx,esi ; push edx` for the second; ours `lea ecx` then `mov edx ; push edx ; lea eax ; push eax ; mov ecx,esi`
// Wave-25 chunk 623 retry (tried, inert): STLport map model (std::map, _Rb_tree_base_iterator free operator==, out-of-line PDB map::operator[]); find() == end() in one expression (end loaded before the call); named iterator (this source); the real STLport operator[] body as a same-TU noinline member (inert); inline DoesNPCNameEntryExistForThing test (worse)
// Leaf: ?SetNameForThing@CThingManager@@QAEX_KABVCWideString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00521A90, 78 bytes, `ret 0Ch`.
// Identity: Ego_r/FableWin.pdb ?SetNameForThing@CThingManager@@QAEX_KABVCWideString@@@Z.
// Callees: NPCNameMap _Rb_tree::_M_find 0x00527E00 (node pointer in eax; spelling of the landed GetNameForThing
//   0x0051F040), map::operator[] 0x0052B8B0 (PDB ??A?$map@_KU?$pair@VCCharString@@VCWideString@@@std@@...), twice,
//   CCharString::operator=(const char*) 0x0099EFE0, CWideString::operator=(const CWideString&) 0x0099B7D0.
// Layout: retail CThingManager::NPCNameMap (STLport map<unsigned __int64, pair<CCharString, CWideString> >, 12 bytes) at
//   +0xCC (Ego_r +0xF4); end() is the header node pointer `[esi]`.
// Shape: the find result is a named iterator (an inline `find(uid) == end()` loads the header before the call); each
//   assignment pushes its right-hand side before operator[] is evaluated.
class CCharString {
public:
    void* PData;
    const CCharString& operator=(const char* text);          // 0x0099EFE0
};
class CWideString {
public:
    void* PData;
    const CWideString& operator=(const CWideString& other);  // 0x0099B7D0
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
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
inline bool operator==(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y)
{ return __x._M_node == __y._M_node; }
template <class _Value> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
};
template <class _Key, class _Value, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_iterator<_Value> iterator;
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    _Compare            _M_key_compare;
    _Rb_tree_node_base* _M_find(const _Key& __k) const;     // 0x00527E00
    iterator find(const _Key& __k) { return iterator(_M_find(__k)); }
    iterator end() { return iterator(_M_header); }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator find(const _Key& __x) { return _M_t.find(__x); }
    iterator end() { return _M_t.end(); }
    _Tp& operator[](const _Key& __k);                       // 0x0052B8B0 (out of line)
};
}
class CThingManager {
public:
    unsigned char _Before[0xCC];
    std::map<unsigned __int64, std::pair<CCharString, CWideString> > NPCNameMap;   // +0xCC
    void SetNameForThing(unsigned __int64 uid, const CWideString& name);
};

void CThingManager::SetNameForThing(unsigned __int64 uid, const CWideString& name)
{
    std::map<unsigned __int64, std::pair<CCharString, CWideString> >::iterator it = NPCNameMap.find(uid);
    if (it == NPCNameMap.end())
        NPCNameMap[uid].first = "";
    NPCNameMap[uid].second = name;
}
