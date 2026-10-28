// std::_Rb_tree<unsigned __int64, std::pair<const unsigned __int64, std::pair<CCharString, CWideString>>, ...>::_M_find(const unsigned __int64& k) const -- STLport map lookup: walk to the first node whose key is not less than k, return it if k is not less than its key, else the header (end)
// Leaf: ?_M_find@?$_Rb_tree@_KU?$pair@$$CB_KU?$pair@VCCharString@@VCWideString@@@std@@@std@@U?$_Select1st@U?$pair@$$CB_KU?$pair@VCCharString@@VCWideString@@@std@@@std@@@2@U?$less@_K@2@V?$allocator@U?$pair@$$CB_KU?$pair@VCCharString@@VCWideString@@@std@@@std@@@2@@std@@QBEPAU_Rb_tree_node_base@2@AB_K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00527e00, 84 bytes, thiscall, `ret 4`.
// Identity (proven): bound ?_M_find@?$_Rb_tree@_KU?$pair@$$CB_KU?$pair@VCCharString@@VCWideString@@@std@@@std@@...QBEPAU_Rb_tree_node_base@2@AB_K@Z (the landed caller CThingManager::GetNameForThing_OnlyCalledFromTCHasName 0x0051F040 looks up Ego_r.pdb CThingManager::NPCNameMap, map<unsigned __int64, pair<CCharString, CWideString> >); also called by CThingManager::SetNameForThing 0x00521A90; this row keeps that binding's _Rb_tree_node_base* return.
// Callees: none.
// Shape (family L008, model the landed _M_find 0x00756350): STLport 4.6 _tree.h _M_find verbatim with less<unsigned __int64>
//   (the 64-bit compare `cmp hi ; jb/ja ; cmp lo`), the key reloaded from its reference for the final test.
class CCharString {
public:
    void* PStringData;                                    // +0x00
};
class CWideString {
public:
    void* PStringData;                                    // +0x00
};
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
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
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
    _Rb_tree_node_base* _M_find(const _Key& __k) const;
    public:
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
_Rb_tree_node_base* _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_find(const _Key& __k) const
{
    _Link_type __y = this->_M_header._M_data;      // last node which is not less than __k
    _Link_type __x = _M_root();                    // current node
    while (__x != 0)
        if (!_M_key_compare(_S_key(__x), __k))
            __y = __x, __x = _S_left(__x);
        else
            __x = _S_right(__x);
    if (__y == this->_M_header._M_data || _M_key_compare(__k, _S_key(__y)))
        __y = this->_M_header._M_data;
    return __y;
}

template class _Rb_tree<unsigned __int64, pair<const unsigned __int64, std::pair<CCharString, CWideString> >, _Select1st<pair<const unsigned __int64, std::pair<CCharString, CWideString> > >,
                        less<unsigned __int64>, allocator<pair<const unsigned __int64, std::pair<CCharString, CWideString> > > >;
} // namespace std