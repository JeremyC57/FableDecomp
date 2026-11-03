// std::_Rb_tree::find<_KT>(const _KT& k) -- map<long, long> lookup (STLport 4.6.1 _tree.h 458-459: `template <class _KT> iterator find(const _KT& __x) { return iterator(_M_find(__x)); }`)
// Leaf: ??$find@J@?$_Rb_tree@JU?$pair@$$CBJJ@std@@U?$_Select1st@U?$pair@$$CBJJ@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJJ@std@@@2@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBJJ@std@@U?$_Nonconst_traits@U?$pair@$$CBJJ@std@@@2@@1@ABJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005937E1, thiscall, `ret 8` (hidden iterator result + key reference).
// Identity (owner decision 2026-11-03, D1: STLport find / _M_find are member templates): Binder: CGameScriptInterface::DeregisterTimer 0x006E7690 (landed) calls it as the non-const find of map<long, long> ScriptTimers; its _M_find is 0x00592F92 (same instantiation, landed).
//   Was the generated stand-in "CRegion::SetMiniMapRegionExitTextOffsetX (extern C __stdcall forwarder)"; retail has no identical-code folding, so this
//   address is one instantiation (value type and constness from the binders above).
// Shape: size TU; `_M_find` stays out of line (call 0x00592F92 with `this` still in ecx), the iterator is built in the hidden result.
#pragma optimize("s", on)

namespace std {
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    _T1 first;
    _T2 second;
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                     // +0x00
    _Base_ptr _M_parent;                                    // +0x04
    _Base_ptr _M_left;                                      // +0x08
    _Base_ptr _M_right;                                     // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};

struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
template <class _Tp> struct _Nonconst_traits { };
template <class _Tp> struct _Const_traits { };
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator(_Rb_tree_node<_Value>* __x) { _M_node = __x; }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Key key_type;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    typedef _Rb_tree_iterator<_Value, _Const_traits<_Value> > const_iterator;
protected:
    _STLP_alloc_proxy<_Link_type, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                    // +0x04
    _Compare     _M_key_compare;                                   // +0x08

    _Link_type& _M_root() const { return (_Link_type&)_M_header._M_data->_M_parent; }
    static _Link_type& _S_left(_Base_ptr __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }
public:
    template <class _KT> iterator find(const _KT& __x) { return iterator(_M_find(__x)); }
    template <class _KT> const_iterator find(const _KT& __x) const { return const_iterator(_M_find(__x)); }
private:
    template <class _KT> _Rb_tree_node<_Value>* _M_find(const _KT& __k) const;
};

typedef _Rb_tree<long, pair<const long, long >, _Select1st<pair<const long, long > >, less<long >, allocator<pair<const long, long > > > _Tree;
template _Tree::iterator _Tree::find<long >(const long&);
}
