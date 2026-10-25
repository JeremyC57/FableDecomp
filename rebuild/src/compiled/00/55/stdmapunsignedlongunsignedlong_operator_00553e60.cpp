// std::map<unsigned long, unsigned long>::operator[](const unsigned long& k) -- STLport subscript: lower_bound, insert (k, 0) when absent, return the mapped word
// Leaf: ??A?$map@KK
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00553E60, 115 bytes, thiscall, `ret 4`, returns &(*it).second.
// Identity: the manifest's "std::map::insert" is propagated. The landed callers C3DMesh2::RemapBoneIndices 0x00A8B820
//   and C3DPrimitive2::RemapBoneIndices 0x00A90F00 declare `unsigned long& operator[](const unsigned long&)` @ 0x00553E60
//   (bone-index remap); NUISystem::CMorphingSprite::ProcessChangeState shares the folded body (16 callers).
// Callees: _Rb_tree::insert_unique(iterator, const value_type&) 0x005537F0 (the iterator argument built in its slot,
//   the iterator result returned through the dead `k` argument slot).
// Shape: STLport 4.6 map::operator[] verbatim with `_M_lower_bound` INLINE (the `jb` walk keeps the header in esi,
//   the candidate in edx and the key in ebx); `__default_constructed((_Tp*)0)` gives the late `mov [esp+1Ch],0`
//   store of the pair temporary's second word after the argument pushes -- only when insert_unique has a same-TU
//   out-of-line body that reads the value (declaration-only schedules the store one push earlier).
namespace _STL {

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
};

template <class _Tp> inline _Tp __default_constructed(_Tp*) { return _Tp(); }   // _STLP_DEFAULT_CONSTRUCTED

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : _Rb_tree_node_base {
    _Value _M_value_field;                               // +0x10
};
template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) {}
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) {}
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
};

template <class _Key, class _Tp, class _Compare = less<_Key> > class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree_node<value_type> _Node;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree_node_base* _M_header;                       // +0x00
    unsigned int        _M_node_count;                   // +0x04
    _Compare            _M_key_compare;                  // +0x08

    static const _Key& _S_key(_Rb_tree_node_base* __x) { return ((_Node*)__x)->_M_value_field.first; }
    _Rb_tree_node_base* _M_lower_bound(const _Key& __k) const
    {
        _Rb_tree_node_base* __y = _M_header;             // last node which is not less than __k
        _Rb_tree_node_base* __x = _M_header->_M_parent;  // the root
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = __x->_M_left;
            else
                __x = __x->_M_right;
        return __y;
    }
    __declspec(noinline) iterator insert_unique(iterator __position, const value_type& __x);  // 0x005537F0
    iterator insert(iterator __position, const value_type& __x) { return insert_unique(__position, __x); }
    iterator lower_bound(const _Key& __k) { return iterator(_M_lower_bound(__k)); }
    iterator end() { return iterator(_M_header); }
    _Compare key_comp() const { return _M_key_compare; }
    _Tp& operator[](const _Key& __k)
    {
        iterator __i = lower_bound(__k);
        if (__i == end() || key_comp()(__k, (*__i).first))
            __i = insert(__i, value_type(__k, __default_constructed((_Tp*)0)));
        return (*__i).second;
    }
};

// Same-TU stand-in for _Rb_tree::insert_unique(iterator, const value_type&) 0x005537F0 (its own row): kept out of line
// and reading the value, so VC7.1 schedules the pair temporary's late second-word store as retail does.
#pragma auto_inline(off)
template <class _Key, class _Tp, class _Compare>
typename map<_Key, _Tp, _Compare>::iterator map<_Key, _Tp, _Compare>::insert_unique(iterator __position, const value_type& __x)
{
    if (_M_key_compare(__x.first, _S_key(__position._M_node)))
        ++_M_node_count;
    return __position;
}
#pragma auto_inline(on)

template unsigned long& map<unsigned long, unsigned long>::operator[](const unsigned long&);

} // namespace _STL