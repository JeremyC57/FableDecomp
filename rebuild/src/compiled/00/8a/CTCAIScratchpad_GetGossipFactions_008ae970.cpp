// CTCAIScratchpad::GetGossipFactions(CCharString category) -- static: the gossip-faction vector of a category, or null when the category is not in the gossip-faction map
// Leaf: ?GetGossipFactions@CTCAIScratchpad@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AE970, 66 bytes, static __fastcall with the CCharString by value
//   on the stack (`ret 4`, ecx/edx unused), destroyed by the callee on both paths.
// Identity: the manifest's "std::map::_Find_or_default_construct" is descriptive. Ego_r.pdb has three byte-shaped
//   twins in this order: ?GetGossip@CTCAIScratchpad@@SIPBV?$vector@VCCharString@@...@VCCharString@@@Z,
//   ?GetGossipVillages@... and ?GetGossipFactions@...; retail keeps the order: 0x008AE510 (map 0x013BAE44),
//   0x008AE740 (map 0x013BAE50), 0x008AE970 (map 0x013BAE5C), each followed by its Add* sibling over the same map.
//   The three static maps (the dynamic initialisers 0x0121F160 / 0x0121F1A0 / 0x0121F1E0) are not in the PDB name
//   set: descriptive names.
// Callees: _Rb_tree::_M_find(const key&) 0x008AC200 (node pointer in eax, compared with the header word),
//   map<CCharString, vector<CCharString> >::operator[] 0x007AEBE0 (landed), ~CCharString 0x0099EAE0.
// Twin of CTCAIScratchpad::GetGossip 0x008AE510 (same body over the third map).
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

template <class _Key, class _Tp, class _Compare = less<_Key> > class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree_node_base* _M_header;                       // +0x00
    unsigned int        _M_node_count;                   // +0x04
    _Compare            _M_key_compare;                  // +0x08

    _Rb_tree_node_base* _M_find(const _Key& __k) const;  // 0x008AC200 (_Rb_tree::_M_find)
    iterator find(const _Key& __k) { return iterator(_M_find(__k)); }
    iterator end() { return iterator(_M_header); }
    _Tp& operator[](const _Key& __k);                    // 0x007AEBE0
};

} // namespace std

typedef std::map<CCharString, std::vector<CCharString> > CGossipMap;

class CTCAIScratchpad {
public:
    static CGossipMap GossipFactions;                       // 0x013BAE5C (descriptive)
    static const std::vector<CCharString>* __fastcall GetGossipFactions(CCharString category);
};

const std::vector<CCharString>* __fastcall CTCAIScratchpad::GetGossipFactions(CCharString category)
{
    CGossipMap::iterator it = GossipFactions.find(category);
    if (it == GossipFactions.end())
        return 0;
    return &GossipFactions[category];
}