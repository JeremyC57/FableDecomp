// std::_Rb_tree<long, long, _Identity<long>, less<long>, allocator<long> >::_M_find(const long& k) const -- STLport set<long> lookup: lower-bound walk from the root, then the header (end) unless the found node's key equals k; returns the node pointer
// Leaf: ?_M_find@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00614A10, 56 bytes (+8 int3), `ret 4` (thiscall: the key reference).
// Callers: CTCVillage code at 0x0065914B (call 0x00659156) and CTCVillage::ReportCrime 0x0065CB6B (call 0x0065CDFD,
//   which compares the result with the header word: `PlayersCrimes.find(crime) == PlayersCrimes.end()`), both on the
//   tree at CTCVillage+0x64. The PDB orders PlayersCrimes (set<long>) after fifteen lists from +0x28; with STLport's
//   4-byte lists that is +0x28 + 15*4 = +0x64.
// Label correction: the catalog `RBTree_FindByKey` is behavioural; the body is STLport 4.5
//   `_M_find(k)` (the out-of-line worker behind `iterator find(k) { return iterator(_M_find(k)); }`, which is why the
//   result is a node pointer in eax rather than an iterator returned through a hidden pointer).
// Layout: STLport _Rb_tree: _M_header._M_data (the heap header node) +0, _M_node_count +4; node: _M_color +0,
//   _M_parent +4, _M_left +8, _M_right +0xC, _M_value_field (long, compared signed: `jl`) +0x10.
// Shape: speed TU (no pragma): the header in eax, the walk cursor in ecx, the candidate in edx, k read once into esi for
//   the loop and re-read for the final `k < key(y)` test.
namespace std {

template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};

template <class T> struct _Identity {
    const T& operator()(const T& x) const { return x; }
};

template <class T> struct allocator { };

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                            // +0x00
    _Base_ptr _M_parent;                           // +0x04
    _Base_ptr _M_left;                             // +0x08
    _Base_ptr _M_right;                            // +0x0C
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                         // +0x10
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
struct _STLP_alloc_proxy : public _MaybeReboundAlloc {
    _Value _M_data;
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Key key_type;

    _STLP_alloc_proxy<_Link_type, _Rb_tree_node<_Value>, _Alloc> _M_header;   // +0x00
    unsigned int _M_node_count;                                              // +0x04
    _Compare _M_key_compare;                                                 // +0x08

    _Link_type _M_root() const { return (_Link_type)_M_header._M_data->_M_parent; }
    static _Link_type _S_left(_Link_type x) { return (_Link_type)x->_M_left; }
    static _Link_type _S_right(_Link_type x) { return (_Link_type)x->_M_right; }
    static const _Key& _S_key(_Link_type x) { return _KeyOfValue()(x->_M_value_field); }

    _Rb_tree_node<_Value>* _M_find(const key_type& __k) const;
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
_Rb_tree_node<_Value>* _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_find(const key_type& __k) const
{
    _Link_type __y = this->_M_header._M_data;      // Last node which is not less than __k.
    _Link_type __x = _M_root();                    // Current node.

    while (__x != 0)
        if (!_M_key_compare(_S_key(__x), __k))
            __y = __x, __x = _S_left(__x);
        else
            __x = _S_right(__x);
    if (__y == this->_M_header._M_data || _M_key_compare(__k, _S_key(__y)))
        __y = this->_M_header._M_data;
    return __y;
}

template class _Rb_tree<long, long, _Identity<long>, less<long>, allocator<long> >;

}