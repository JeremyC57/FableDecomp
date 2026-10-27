// std::map<long, unsigned long>::operator[](long const& __k) -- the value for k, inserting a zero value when missing
// Leaf: ??A?$map@JKU?$less@J@std@@V?$allocator@U?$pair@$$CBJK@std@@@2@@std@@QAEAAKABJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E9760, 115 bytes, thiscall, `ret 4`.
// Identity (proven): its insert is the landed _Rb_tree<long, pair<const long, unsigned long> >::insert_unique(iterator, const value_type&) 0x005E93E0 (bound); callers CTCQuestCompletionUI's QuestCard_SetupDialog 0x005E4860 / 0x005E49E0 / ActivateReal 0x005E5F30.
// Callees: _Rb_tree<...>::insert_unique(iterator, const value_type&) 0x005E93E0 (the iterator by value, the result through a hidden pointer).
// Shape (family M160, representative 0x005E9760): STLport 4.6 `iterator __i = lower_bound(__k); if (__i == end() ||
//   key_comp()(__k, (*__i).first)) __i = insert(__i, value_type(__k, _Tp())); return (*__i).second;` -- the M130 model with a
//   plain 4-byte value (the pair temporary in the 8-byte frame, the result homed in the dead k slot) and, as the landed
//   map<unsigned long, unsigned long>::operator[] 0x00553E60, a same-TU stand-in body for insert_unique. Speed TU.
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    _T1 first;
    _T2 second;
};
template <class _Pair> struct _Select1st { const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; } };
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool _M_color;
    _Base_ptr _M_parent;
    _Base_ptr _M_left;
    _Base_ptr _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Tp> struct _Nonconst_traits { };
template <class _Value, class _Traits> struct _Rb_tree_iterator {
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) { }
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
    _Rb_tree_node_base* _M_node;
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc> class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(((_Rb_tree_node<_Value>*)__x)->_M_value_field); }
    _Base_ptr _M_lower_bound(const _Key& __k) const
    {
        _Base_ptr __y = _M_header._M_data;
        _Base_ptr __x = _M_header._M_data->_M_parent;
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = __x->_M_left;
            else
                __x = __x->_M_right;
        return __y;
    }
    iterator lower_bound(const _Key& __x) { return iterator(_M_lower_bound(__x)); }
    iterator end() { return iterator(_M_header._M_data); }
    __declspec(noinline) iterator insert_unique(iterator __position, const _Value& __x);   // 0x005E93E0 (same-TU stand-in body below)
    _STLP_alloc_proxy<_Base_ptr, allocator<_Rb_tree_node<_Value> > > _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > > class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    iterator lower_bound(const _Key& __x) { return _M_t.lower_bound(__x); }
    iterator end() { return _M_t.end(); }
    _Compare key_comp() const { return _M_t._M_key_compare; }
    iterator insert(iterator __position, const value_type& __x) { return _M_t.insert_unique(__position, __x); }
    _Tp& operator[](const _Key& __k)
    {
        iterator __i = lower_bound(__k);
        if (__i == end() || key_comp()(__k, (*__i).first))
            __i = insert(__i, value_type(__k, _Tp()));
        return (*__i).second;
    }
    _Rep_type _M_t;
};
// Same-TU stand-in for _Rb_tree::insert_unique(iterator, const value_type&) (its own row, landed): kept out of line and
// reading only the key, as the landed map<unsigned long, unsigned long>::operator[] 0x00553E60 does, so VC7.1 schedules the
// pair temporary's second-word store after the argument pushes as retail does.
#pragma auto_inline(off)
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_unique(iterator __position, const _Value& __x)
{
    if (_M_key_compare(_KeyOfValue()(__x), _S_key(__position._M_node)))
        ++_M_node_count;
    return __position;
}
#pragma auto_inline(on)
template unsigned long& map<long, unsigned long >::operator[](long const&);
}