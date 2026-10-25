// std::map::map(const map& x) -- (map<long, CCountedPointer<CEngineSceneMap> >) STLport copy constructor with the _Rb_tree copy inline: zero the header proxy, malloc the 0x1C-byte header node, then either the empty initialisation (root 0, leftmost = rightmost = header) or the out-of-line _M_copy of x's root with leftmost / rightmost found by walking it; copy the node count
// Leaf: ??0?$map@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B97DE0, 161 bytes (+ int3 padding), thiscall, `ret 4`, returns this.
// Identity: map<long, CCountedPointer<CEngineSceneMap> > -- proven; evidence: the inner map of Ego_r.pdb CEngineScene::Maps (map<long, map<long, CCountedPointer<CEngineSceneMap> > >): the CEngineScene TU indexes the outer map (operator[] 0x00B97B60 from 0x00B93F53..0x00B94404) and its pair copies (0x00B98100, 0x00B981D0, 0x00B98290, 0x00B98320) copy the long key and call this constructor on +4; 0x1C-byte nodes; its _M_copy 0x00B97980 copies the key and the counted pointer (inc of its count).
// Label: the manifest's "std_set_init_RedBlackTree" is propagated across byte-identical bodies (the eight rows differ only in the header
//   node size and the _M_copy call); FableWin.pdb names the map copy constructors (and no _Rb_tree copy constructor),
//   so the tree copy is inline in this one.
// Callees: malloc @ 0x00BFEA0E (_Rb_tree_base: allocate(1) of the header node), _Rb_tree::_M_copy(_Link_type x, _Link_type p)
//   @ 0x00B97980 (thiscall, `ret 8`).
// Body: STLport 4.6 `map(const map& x) : _M_t(x._M_t) {}` with `_Rb_tree(const _Rb_tree& x) :
//   _Rb_tree_base(x.get_allocator()), _M_node_count(0), _M_key_compare(x._M_key_compare) { if (x._M_root() == 0)
//   _M_empty_initialize(); else { _S_color(header) = red; _M_root() = _M_copy(x._M_root(), header); _M_leftmost() =
//   _S_minimum(_M_root()); _M_rightmost() = _S_maximum(_M_root()); } _M_node_count = x._M_node_count; }` -- the red
//   colour store of both branches is hoisted above the test. VC7.1 keeps the tree copy constructor out of line under
//   plain `inline` (a 19-byte forwarder, graded), so it is `__forceinline` here.
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t size);           // 0x00BFEA0E
template <class T> class CCountedPointer {          // 8 bytes: data, shared count block
public:
    T* Data;
    void* Info;
};
class CEngineSceneMap;
namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    template <class _Tp1> allocator(const allocator<_Tp1>&) { }
    _Tp* allocate(size_t __n) { return (_Tp*)malloc(__n * sizeof(_Tp)); }
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
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
    _Tp* allocate(size_t __n) { return _MaybeReboundAlloc::allocate(__n); }
};
struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                               // +0x00
    _Base_ptr   _M_parent;                              // +0x04
    _Base_ptr   _M_left;                                // +0x08
    _Base_ptr   _M_right;                               // +0x0C
    static _Base_ptr _S_minimum(_Base_ptr __x) { while (__x->_M_left != 0) __x = __x->_M_left; return __x; }
    static _Base_ptr _S_maximum(_Base_ptr __x) { while (__x->_M_right != 0) __x = __x->_M_right; return __x; }
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    typedef allocator<_Node> _M_node_allocator_type;
    typedef _Alloc allocator_type;
    allocator_type get_allocator() const { return allocator_type((const _M_node_allocator_type&)_M_header); }
    _Rb_tree_base(const allocator_type& __a) : _M_header(_M_node_allocator_type(__a), (_Node*)0)
    {
        _M_header._M_data = _M_header.allocate(1);
    }
    _STLP_alloc_proxy<_Node*, _Node, _M_node_allocator_type> _M_header;   // +0x00
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_base<_Value, _Alloc> _Base;
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef size_t size_type;
    enum { _S_rb_tree_red = false, _S_rb_tree_black = true };
    size_type _M_node_count;                            // +0x04
    _Compare  _M_key_compare;                           // +0x08
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
    static _Rb_tree_node_base::_Color_type& _S_color(_Base_ptr __x) { return __x->_M_color; }
    static _Link_type _S_minimum(_Link_type __x) { return (_Link_type)_Rb_tree_node_base::_S_minimum(__x); }
    static _Link_type _S_maximum(_Link_type __x) { return (_Link_type)_Rb_tree_node_base::_S_maximum(__x); }
    _Link_type _M_copy(_Link_type __x, _Link_type __p);   // 0x00B97980
    void _M_empty_initialize()
    {
        _S_color(this->_M_header._M_data) = _S_rb_tree_red;   // used to distinguish header from root in iterator.operator++
        _M_root() = 0;
        _M_leftmost() = this->_M_header._M_data;
        _M_rightmost() = this->_M_header._M_data;
    }
    __forceinline _Rb_tree(const _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>& __x)   // inline in every retail set / map copy
        : _Base(__x.get_allocator()), _M_node_count(0), _M_key_compare(__x._M_key_compare)
    {
        if (__x._M_root() == 0)
            _M_empty_initialize();
        else {
            _S_color(this->_M_header._M_data) = _S_rb_tree_red;
            _M_root() = _M_copy(__x._M_root(), this->_M_header._M_data);
            _M_leftmost() = _S_minimum(_M_root());
            _M_rightmost() = _S_maximum(_M_root());
        }
        _M_node_count = __x._M_node_count;
    }
};
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {
public:
    typedef _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _Rep_type;
    _Rep_type _M_t;
    set(const set<_Key, _Compare, _Alloc>& __x) : _M_t(__x._M_t) { }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    _Rep_type _M_t;
    map(const map<_Key, _Tp, _Compare, _Alloc>& __x) : _M_t(__x._M_t) { }
};
template map<long, CCountedPointer<CEngineSceneMap> >::map(const map<long, CCountedPointer<CEngineSceneMap> >&);
}