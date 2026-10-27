// std::_Rb_tree::_M_upper_bound(const CCharString& k) const -- (_Rb_tree<CCharString, pair<const CCharString, std::pair<long, long>> >) the first node whose key is greater than k, or the header
// Leaf: ?_M_upper_bound@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@3@@std@@QBEPAU_Rb_tree_node_base@2@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007564F0, 67 bytes, thiscall, `ret 4`; returns the node pointer.
// Identity (proven): its callers are the equal_range 0x00756B60, erase-by-key 0x00756C60 and insert 0x00756790 of the CTCHeroRememberCrimes TU's map<CCharString, pair<long, long> > (the landed _M_find 0x00756350 / TransferMap 0x00756D00 of that tree: Ego_r.pdb CTCHeroRememberCrimes::RegionNamesToCrimeDurationAndFine).
// Callees: CBasicString<char>::operator< 0x00429950 (through the inline CCharString::operator<: equal data pointers -> false,
//   null k -> true, null node key -> false).
// Shape (family M084, representative 0x004B9D50): STLport 4.6 _tree.c `_M_upper_bound`: `__y = header; __x = root; while (__x)
//   if (_M_key_compare(__k, _S_key(__x))) __y = __x, __x = _S_left(__x); else __x = _S_right(__x); return __y;` -- k's data
//   pointer loaded once (edi) -- VC7.1 hoists it only when it can see that CBasicString<char>::operator< writes no memory,
//   so that callee's body is compiled in this TU (noinline, as the landed _M_find 0x00756350 of the same family); speed TU.
template <class T> class CBasicString {
public:
    __forceinline long Compare(const CBasicString& other) const
    {
        const T* b = other.m_pData;
        const T* a = m_pData;
        for (;;) {
            if (*a == 0 && *b == 0)
                return 0;
            if (*a < *b)
                return -1;
            if (*a > *b)
                return 1;
            ++a;
            ++b;
        }
    }
    bool operator<(const CBasicString& other) const;
    T* m_pData;
};
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CBasicString<T>::operator<(const CBasicString& other) const    // 0x00429950
{
    return Compare(other) < 0;
}
#pragma auto_inline(on)
class CCharStringData {
public:
    CBasicString<char> Data;
};
class CCharString {
public:
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData)
            return false;
        if (PStringData == 0)
            return true;
        if (other.PStringData == 0)
            return false;
        return PStringData->Data < other.PStringData->Data;
    }
    CCharStringData* PStringData;
};
namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
template <class _T1, class _T2> struct pair { typedef _T1 first_type; _T1 first; _T2 second; };
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
template <class _Pair> struct _Select1st { const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; } };
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool _M_color;                                        // +0x00
    _Base_ptr _M_parent;                                  // +0x04
    _Base_ptr _M_left;                                    // +0x08
    _Base_ptr _M_right;                                   // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                       // +0x04
    _Compare _M_key_compare;                                          // +0x08
    _Base_ptr _M_root() const { return this->_M_header._M_data->_M_parent; }
    static _Base_ptr& _S_left(_Base_ptr __x) { return __x->_M_left; }
    static _Base_ptr& _S_right(_Base_ptr __x) { return __x->_M_right; }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(((_Node*)__x)->_M_value_field); }
    _Base_ptr _M_upper_bound(const _Key& __k) const;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
_Rb_tree_node_base* _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_upper_bound(const _Key& __k) const
{
    _Rb_tree_node_base* __y = this->_M_header._M_data;
    _Rb_tree_node_base* __x = _M_root();
    while (__x != 0)
        if (_M_key_compare(__k, _S_key(__x)))
            __y = __x, __x = _S_left(__x);
        else
            __x = _S_right(__x);
    return __y;
}
typedef pair<const CCharString, std::pair<long, long> > _V;
template class _Rb_tree<CCharString, _V, _Select1st<_V>, less<CCharString>, allocator<_V> >;
}