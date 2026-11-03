// CTCAIScratchpad::GetGossip(CCharString category) -- static: the gossip-line vector of a category, or null when the category is not in the gossip map
// Leaf: ?GetGossip@CTCAIScratchpad@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AE510, 66 bytes, static __fastcall with the CCharString by value
//   on the stack (`ret 4`, ecx/edx unused), destroyed by the callee on both paths.
// Identity: the manifest's "std::map::_Find_or_default_construct" is descriptive. Ego_r.pdb has three byte-shaped
//   twins in this order: ?GetGossip@CTCAIScratchpad@@SIPBV?$vector@VCCharString@@...@VCCharString@@@Z,
//   ?GetGossipVillages@... and ?GetGossipFactions@...; retail keeps the order: 0x008AE510 (map 0x013BAE44),
//   0x008AE740 (map 0x013BAE50), 0x008AE970 (map 0x013BAE5C), each followed by its Add* sibling over the same map.
//   The three static maps (the dynamic initialisers 0x0121F160 / 0x0121F1A0 / 0x0121F1E0) are not in the PDB name
//   set: descriptive names.
// Callees: _Rb_tree::_M_find(const key&) 0x008AC200 (node pointer in eax, compared with the header word),
//   map<CCharString, vector<CCharString> >::operator[] 0x007AEBE0 (landed), ~CCharString 0x0099EAE0.
// Shape: `iterator it = map.find(k); if (it == map.end()) return 0; return &map[k];` -- STLport find() is the inline
//   wrapper of the out-of-line _M_find; the NAMED iterator puts the call before the end() load (`cmp eax,[map]`;
//   the one-expression `find(k) == end()` loads the header first, into esi); the by-value argument dies on each
//   path (the `lea ecx,[esp+4]` hoisted above the branch).
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;                           // +0x00
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
};

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _Tp* _M_end_of_storage;                                 // +0x08
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const;
};

struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) {}
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) {}
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
};

template <class _Pair> struct _Select1st { };          // STLport 4.6 _function.h (map's KeyOfValue)
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                               // +0x10
};
// STLport 4.6.1 _tree.h: find / _M_find are member templates (owner decision 2026-11-03), _M_find private.
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc> class _Rb_tree {
public:
    typedef _Rb_tree_iterator<_Value> iterator;
    _Rb_tree_node_base* _M_header;                       // +0x00
    unsigned int        _M_node_count;                   // +0x04
    _Compare            _M_key_compare;                  // +0x08
    template <class _KT> iterator find(const _KT& __k) { return iterator(_M_find(__k)); }
    iterator end() { return iterator(_M_header); }
private:
    template <class _KT> _Rb_tree_node<_Value>* _M_find(const _KT& __k) const;  // 0x008AC200
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > > class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator find(const _Key& __k) { return _M_t.find(__k); }
    iterator end() { return _M_t.end(); }
    _Tp& operator[](const _Key& __k);                    // 0x007AEBE0
};

} // namespace std

typedef std::map<CCharString, std::vector<CCharString> > CGossipMap;

class CTCAIScratchpad {
public:
    static CGossipMap Gossip;                               // 0x013BAE44 (descriptive)
    static const std::vector<CCharString>* __fastcall GetGossip(CCharString category);
};

const std::vector<CCharString>* __fastcall CTCAIScratchpad::GetGossip(CCharString category)
{
    CGossipMap::iterator it = Gossip.find(category);
    if (it == Gossip.end())
        return 0;
    return &Gossip[category];
}