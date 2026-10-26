// std::_Rb_tree::erase(iterator position) -- (set<CCharString>) STLport 4.6 single-node erase: unlink the node through _Rebalance_for_erase (root / leftmost / rightmost kept), destroy its value, free it and decrement the count
// Leaf: ?erase@?$_Rb_tree@VCCharString@@V1@U?$_Identity@VCCharString@@@std@@U?$less@VCCharString@@@3@V?$allocator@VCCharString@@@3@@std@@QAEXU?$_Rb_tree_iterator@VCCharString@@U?$_Nonconst_traits@VCCharString@@@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2B780, 66 bytes (+ int3 padding to 80), thiscall `ret 4` (the iterator by value).
// Identity (proven): set<CCharString>; evidence: its only caller 0x00A264F7 (in 0x00A264C0) lower-cases a CCharString, looks it up with 0x004B9370 -- the landed set<CCharString>::_M_find (CFaction's AlliedFactions / EnemyFactions) -- on the same tree and erases the hit unless it is end()
// Label: the manifest's "StdMap_Erase" is propagated (family H065: six erase(iterator) bodies that differ only in the value
//   destructor and its node offset; retail has no identical-code folding, so each address is one instantiation).
//   STLport has two functions with this body per tree type -- `_Rb_tree::erase(iterator)` and the container's forwarding
//   `map/set::erase(iterator)` with the tree's erase inlined; these rows are modelled as the tree's.
// Callees: _Rb_global<bool>::_Rebalance_for_erase(z, root&, leftmost&, rightmost&) @ 0x0042A34E (__cdecl), CCharString::~CCharString 0x0099EAE0 on the value (+0x10),
//   free 0x00BFEA14 (allocator<_Node>::deallocate, null-tested: the node pointer only fed a `lea`).
extern "C" void __cdecl free(void* block);             // 0x00BFEA14
class CCharString {                                  // 4 bytes: shared string block
public:
    void* PStringData;
    ~CCharString();                                   // 0x0099EAE0
    bool operator<(const CCharString& other) const;
};

namespace std {

template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};

struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                               // +0x00
    _Base_ptr   _M_parent;                              // +0x04
    _Base_ptr   _M_left;                                // +0x08
    _Base_ptr   _M_right;                               // +0x0C
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};

template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Base_ptr __cdecl _Rebalance_for_erase(_Base_ptr __z, _Base_ptr& __root, _Base_ptr& __leftmost,
                                                  _Base_ptr& __rightmost);                    // 0x0042A34E
};
typedef _Rb_global<bool> _Rb_global_inst;

struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };

template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(const _Self& __it) { _M_node = __it._M_node; }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Value value_type;
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;

    _STLP_alloc_proxy<_Link_type, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                    // +0x04
    _Compare     _M_key_compare;                                   // +0x08

    void erase(iterator __position)
    {
        _Link_type __y = (_Link_type)_Rb_global_inst::_Rebalance_for_erase(__position._M_node,
                                                                            this->_M_header._M_data->_M_parent,
                                                                            this->_M_header._M_data->_M_left,
                                                                            this->_M_header._M_data->_M_right);
        _Destroy(&__y->_M_value_field);
        this->_M_header.deallocate(__y, 1);
        --_M_node_count;
    }
};
}   // namespace std

template void std::_Rb_tree<CCharString, CCharString, std::_Identity<CCharString >, std::less<CCharString >, std::allocator<CCharString > >::erase(
    std::_Rb_tree<CCharString, CCharString, std::_Identity<CCharString >, std::less<CCharString >, std::allocator<CCharString > >::iterator);