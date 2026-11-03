// std::_Rb_tree::find<_KT>(const _KT& k) const -- map<EWorldMapNameGraphic, unsigned long> lookup (STLport 4.6.1 _tree.h 458-459: `template <class _KT> const_iterator find(const _KT& __x) const { return const_iterator(_M_find(__x)); }`)
// Leaf: ??$find@W4EWorldMapNameGraphic@@@?$_Rb_tree@W4EWorldMapNameGraphic@@U?$pair@$$CBW4EWorldMapNameGraphic@@K@std@@U?$_Select1st@U?$pair@$$CBW4EWorldMapNameGraphic@@K@std@@@3@U?$less@W4EWorldMapNameGraphic@@@3@V?$allocator@U?$pair@$$CBW4EWorldMapNameGraphic@@K@std@@@3@@std@@QBE?AU?$_Rb_tree_iterator@U?$pair@$$CBW4EWorldMapNameGraphic@@K@std@@U?$_Const_traits@U?$pair@$$CBW4EWorldMapNameGraphic@@K@std@@@2@@1@ABW4EWorldMapNameGraphic@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006BD0F0, thiscall, `ret 8` (hidden iterator result + key reference).
// Identity (owner decision 2026-11-03, D1: STLport find / _M_find are member templates): Binder: CRegion::GetWorldMapNameGraphic 0x006BC380 (landed) calls it as the const find of NUISystem::CUILocaleGraphicsDef::WorldMapRegionName, map<EWorldMapNameGraphic, unsigned long>; keys compare signed (`jl` / `jge`). Replaces the CUnprovenKey/Value_006bd0f0 placeholders.
//   Was the generated stand-in "_Rb_tree<CUnprovenKey_006bd0f0, ...>::find"; retail has no identical-code folding, so this
//   address is one instantiation (value type and constness from the binders above).
// Shape: `const_iterator(_M_find(__x))` with _M_find inlined: walk keeping the last node not less than k, then the header when nothing was
//   found or k < key(y). Mapped types are opaque (find never reads them).
enum EWorldMapNameGraphic { };
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
    template <class _KT> _Rb_tree_node<_Value>* _M_find(const _KT& __k) const
    {
        _Link_type __y = _M_header._M_data;                 // last node which is not less than k
        _Link_type __x = _M_root();                         // current node
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = _S_left(__x);
            else
                __x = _S_right(__x);
        if (__y == _M_header._M_data || _M_key_compare(__k, _S_key(__y)))
            __y = _M_header._M_data;
        return __y;
    }
};

typedef _Rb_tree<EWorldMapNameGraphic, pair<const EWorldMapNameGraphic, unsigned long >, _Select1st<pair<const EWorldMapNameGraphic, unsigned long > >, less<EWorldMapNameGraphic >, allocator<pair<const EWorldMapNameGraphic, unsigned long > > > _Tree;
template _Tree::const_iterator _Tree::find<EWorldMapNameGraphic >(const EWorldMapNameGraphic&) const;
}
