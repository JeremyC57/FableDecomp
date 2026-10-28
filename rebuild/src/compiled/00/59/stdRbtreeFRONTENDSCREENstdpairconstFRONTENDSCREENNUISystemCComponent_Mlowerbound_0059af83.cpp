// std::_Rb_tree<FRONTEND_SCREEN, std::pair<const FRONTEND_SCREEN, NUISystem::CComponent*>, ...>::_M_lower_bound(const FRONTEND_SCREEN& k) const -- STLport lower bound: walk to the first node whose key is not less than k; return it, or the header when none
// Leaf: ?_M_lower_bound@?$_Rb_tree@W4FRONTEND_SCREEN@@U?$pair@$$CBW4FRONTEND_SCREEN@@PAVCComponent@NUISystem@@@std@@U?$_Select1st@U?$pair@$$CBW4FRONTEND_SCREEN@@PAVCComponent@NUISystem@@@std@@@3@U?$less@W4FRONTEND_SCREEN@@@3@V?$allocator@U?$pair@$$CBW4FRONTEND_SCREEN@@PAVCComponent@NUISystem@@@std@@@3@@std@@QBEPAU?$_Rb_tree_node@U?$pair@$$CBW4FRONTEND_SCREEN@@PAVCComponent@NUISystem@@@std@@@2@ABW4FRONTEND_SCREEN@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059af83, 37 bytes, thiscall, `ret 4`; size-optimised TU (unaligned address).
// Identity (proven): bound (owner map) ?_M_lower_bound@?$map@W4FRONTEND_SCREEN@@PAVCComponent@NUISystem@@...; callers the landed map<FRONTEND_SCREEN, CComponent*> tree helpers 0x0059B098 / 0x0059B32A and map::operator[] 0x0059B5D7.
// Callees: none.
// Shape (family L016): STLport 4.6 _tree.c _M_lower_bound verbatim under `#pragma optimize("s", on)`; signed key (`jl`).
#pragma optimize("s", on)
enum FRONTEND_SCREEN { };
namespace NUISystem { class CComponent; }
namespace std {

template <class _Tp> class allocator { };

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};

} // namespace std

namespace std {

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};

struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};

template <class _Tp, class _Alloc> class _Rb_tree_base {
public:
    _STLP_alloc_proxy<_Rb_tree_node<_Tp>*, _Rb_tree_node<_Tp>, allocator<_Rb_tree_node<_Tp> > > _M_header;
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
    static const _Key& _S_key(_Link_type __x) { return _KeyOfValue()(__x->_M_value_field); }
    static _Link_type _S_left(_Link_type __x) { return (_Link_type)__x->_M_left; }
    static _Link_type _S_right(_Link_type __x) { return (_Link_type)__x->_M_right; }
    protected:
    _Link_type _M_root() const { return (_Link_type)this->_M_header._M_data->_M_parent; }
    public:
    private:
    _Rb_tree_node<_Value>* _M_lower_bound(const _Key& __k) const;
    public:
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
_Rb_tree_node<_Value>* _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_lower_bound(const _Key& __k) const
{
    _Link_type __y = this->_M_header._M_data;      // last node which is not less than __k
    _Link_type __x = _M_root();                    // current node
    while (__x != 0)
        if (!_M_key_compare(_S_key(__x), __k))
            __y = __x, __x = _S_left(__x);
        else
            __x = _S_right(__x);
    return __y;
}
template class _Rb_tree<FRONTEND_SCREEN, pair<const FRONTEND_SCREEN, NUISystem::CComponent* >, _Select1st<pair<const FRONTEND_SCREEN, NUISystem::CComponent* > >,
                        less<FRONTEND_SCREEN>, allocator<pair<const FRONTEND_SCREEN, NUISystem::CComponent* > > >;
} // namespace std