// CGameScriptInterface::DeregisterTimer(long timer) -- forget a script timer: erase its entry from the timer map when it is registered
// Leaf: ?DeregisterTimer@CGameScriptInterface@@UAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E7690, 74 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb ?DeregisterTimer@CGameScriptInterface@@UAEXJ@Z (virtual, one long); referenced only from
//   the CGameScriptInterface vtable (0x0126106C); the body erases from Ego_r.pdb CGameScriptInterface::ScriptTimers
//   (map<long,long>, retail +0x3C) -- the catalog label already names it.
// Callees: std::_Rb_tree<long, pair<const long,long>, _Select1st, less<long>, allocator>::find(const long&) 0x005937E1
//   (out of line, iterator through the hidden pointer; it calls _M_find 0x00592F92 -- probable: map::find inlines into it;
//   the landed 0x005937E1 source is a misidentified extern "C" stdcall, see NOTES "Review at landing"),
//   _Rb_global<bool>::_Rebalance_for_erase 0x0042A34E (cdecl), free 0x00BFEA14 (allocator::deallocate, null-tested).
// Shape: STLport 4.6 `map::erase(key)`-style lookup: find, then `if (it != end()) erase(it)` with erase(iterator) inline
//   (rebalance, destroy, deallocate, --node count); the header node is read once.
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Dummy> class _Rb_global {
public:
    static _Rb_tree_node_base* __cdecl _Rebalance_for_erase(_Rb_tree_node_base* __z, _Rb_tree_node_base*& __root,
                                                            _Rb_tree_node_base*& __leftmost,
                                                            _Rb_tree_node_base*& __rightmost);              // 0x0042A34E
};
typedef _Rb_global<bool> _Rb_global_inst;
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
inline bool operator!=(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node != __y._M_node; }
template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { _M_node = 0; }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> >& __it) { _M_node = __it._M_node; }
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
protected:
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
protected:
    unsigned int _M_node_count;
    _Compare     _M_key_compare;
public:
    iterator end() { return iterator(this->_M_header._M_data); }
    iterator find(const _Key& __k);                                                                   // 0x005937E1
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
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator end() { return _M_t.end(); }
    iterator find(const _Key& __x) { return _M_t.find(__x); }
    void erase(iterator __position) { _M_t.erase(__position); }
};
}

class CGameScriptInterface {
public:
    virtual void v00();
    unsigned char             _pad04[0x3C - 0x04];
    std::map<long, long>      ScriptTimers;              // +0x3C
    virtual void DeregisterTimer(long timer);
};

void CGameScriptInterface::DeregisterTimer(long timer)
{
    std::map<long, long>::iterator it = ScriptTimers.find(timer);
    if (it != ScriptTimers.end())
        ScriptTimers.erase(it);
}