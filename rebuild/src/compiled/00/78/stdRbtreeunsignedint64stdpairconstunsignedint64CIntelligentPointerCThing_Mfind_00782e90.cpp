// std::_Rb_tree<unsigned __int64, std::pair<const unsigned __int64, CIntelligentPointer<CThing>>, ...>::_M_find(const unsigned __int64& k) const -- STLport tree lookup: walk to the first node whose key is not less than k, return it if k is not less than its key, else the header (end)
// Leaf: ?_M_find@?$_Rb_tree@_KU?$pair@$$CB_KV?$CIntelligentPointer@VCThing@@@@@std@@U?$_Select1st@U?$pair@$$CB_KV?$CIntelligentPointer@VCThing@@@@@std@@@2@U?$less@_K@2@V?$allocator@U?$pair@$$CB_KV?$CIntelligentPointer@VCThing@@@@@std@@@2@@std@@QBEPAU?$_Rb_tree_node@U?$pair@$$CB_KV?$CIntelligentPointer@VCThing@@@@@std@@@2@AB_K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00782e90, 84 bytes, thiscall, `ret 4`.
// Identity (proven): the landed CTCBoastingCrowdControl::GetCrowdGroup 0x00783730 / OnKillCrowd 0x00783240 and the boast-removal erase 0x00783CC0 look UIDs up in Ego_r.pdb CTCBoastingCrowdControl::HeroCrowdThings (map<unsigned __int64, CIntelligentPointer<CThing> >); bound only as the stand-ins CHeroCrowdMap::find.
// Callees: none.
// Shape (family L044 = family L008's bytes): STLport 4.6 _tree.h _M_find verbatim with less<unsigned __int64> (the
//   64-bit compare `cmp hi ; jb/ja ; cmp lo`), the key reloaded from its reference for the final test.
class CThing;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                            // Ego_r.pdb: vptr + PData, 8 bytes
public:
    virtual ~CBaseIntelligentPointer();                   // 0x00A01C10
    CBaseObjectPointer* PData;                            // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
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
    _Link_type _M_root() const { return (_Link_type)this->_M_header._M_data->_M_parent; }
    _Rb_tree_node<_Value>* _M_find(const _Key& __k) const;
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
_Rb_tree_node<_Value>* _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_find(const _Key& __k) const
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

template class _Rb_tree<unsigned __int64, pair<const unsigned __int64, CIntelligentPointer<CThing> >, _Select1st<pair<const unsigned __int64, CIntelligentPointer<CThing> > >,
                        less<unsigned __int64>, allocator<pair<const unsigned __int64, CIntelligentPointer<CThing> > > >;
} // namespace std