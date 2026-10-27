// std::map<NUISystem::CComponent*, NUISystem::CList*>::operator[](NUISystem::CComponent* const& __k) -- the value for k, inserting a zero value when missing
// Leaf: ??A?$map@PAVCComponent@NUISystem@@PAVCList@2@U?$less@PAVCComponent@NUISystem@@@std@@V?$allocator@U?$pair@QAVCComponent@NUISystem@@PAVCList@2@@std@@@5@@std@@QAEAAPAVCList@NUISystem@@ABQAVCComponent@3@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B3076, 69 bytes, thiscall, `ret 4`, ebp frame.
// Identity (proven): its lower bound 0x005B1DA0 is bound ?_M_lower_bound@?$_Rb_tree@PAVCComponent@NUISystem@@U?$pair@QAVCComponent@NUISystem@@PAVCList@2@@std@@...; callers CTCInventoryTrade::ConstructNormalShopList / ConstructActual{Sell,Wanted,Buy}InventoryPC / SellItemPC / BuyItem -- Ego_r.pdb CTCInventoryTrade::{Sell,Buy,Wanted}ListsByCategory are map<NUISystem::CComponent*, NUISystem::CList*>.
// Callees: _Rb_tree<...>::_M_lower_bound(const key_type&) const 0x005B1DA0, map::insert(iterator, const value_type&) 0x005B2E8A
//   (out of line; the iterator by value, the result through a hidden pointer).
// Shape (family M152, representative 0x005B3076): STLport 4.6 `iterator __i = lower_bound(__k); if (__i == end() ||
//   key_comp()(__k, (*__i).first)) __i = insert(__i, value_type(__k, _Tp())); return (*__i).second;` -- the pair temporary in
//   the 8-byte frame, the hint iterator copy-constructed in its argument slot, the result homed in the dead k slot.
//   `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace NUISystem { class CComponent; class CList; }
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
    _Base_ptr _M_lower_bound(const _Key& __k) const;      // 0x005B1DA0
    iterator lower_bound(const _Key& __x) { return iterator(_M_lower_bound(__x)); }
    iterator end() { return iterator(_M_header._M_data); }
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
    iterator insert(iterator __position, const value_type& __x);   // 0x005B2E8A
    _Tp& operator[](const _Key& __k)
    {
        iterator __i = lower_bound(__k);
        if (__i == end() || key_comp()(__k, (*__i).first))
            __i = insert(__i, value_type(__k, _Tp()));
        return (*__i).second;
    }
    _Rep_type _M_t;
};
template NUISystem::CList*& map<NUISystem::CComponent*, NUISystem::CList* >::operator[](NUISystem::CComponent* const&);
}