// std::_Rb_tree::_M_find(const key_type& k) const -- (multimap<CCharString, float>) STLport exact-key lookup over CCharString keys: walk from the root keeping the last node whose key is not less than k, then return it unless it is the header or k is less than its key (then the header, i.e. end())
// Leaf: ?_M_find@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@M@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@M@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@M@std@@@3@@std@@QBEPAU?$_Rb_tree_node@U?$pair@$$CBVCCharString@@M@std@@@2@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A54310, 112 bytes (+ int3 padding), thiscall, `ret 4`; returns the node pointer.
// Identity: `multimap<CCharString, float>`; evidence: its callers 0x00A4EAC0 and 0x00A4EAE0 run it on this+0x20 and read the found float (node +0x14); they are C3DAnimationStats' event queries (callers CManager::AnimHasEmbeddedEvent 0x006F90D0, GetEmbeddedEventTime 0x006F9130, C3DMeshObject::GetEventTime 0x00A36DF0), and Ego_r C3DAnimationStats +32 is TimingEvents, multimap<CCharString, float>.
//   Family F012 (17 byte-identical bodies, one COMDAT per tree type): the manifest label "BinarySearchTree_Find" is behavioural.
//   STLport 4.6 _tree.h `_Rb_tree::_M_find` (the out-of-line node lookup behind set/map/multimap find; map and multimap
//   share one `_Rb_tree<K, pair<const K, V>, _Select1st<...>, less<K>, allocator<...> >` instantiation).
// Callee: CBasicString<char>::operator< 0x00429950 (thiscall `ret 4`), reached through the inline CCharString::operator<
//   (equal data pointers -> false, null this -> true, null other -> false), once per step of the walk (node key vs k)
//   and once for the final test (k vs the candidate's key). Its same-TU noinline body under auto_inline(off) keeps
//   VC7.1's register allocation as retail (header in ebp, candidate in esi, node in edi, k in ebx).
// Node layout: color +0, parent +4, left +8, right +0xC, key at +0x10 (first word of the value); tree: header node
//   pointer +0, node count +4, comparator +8.
// Value: pair<const CCharString, float> (_Select1st); only the key is read, the mapped type
//   only names the instantiation (map and multimap share it).
template <class T> class CBasicString {
public:
    T* m_pData;
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
};
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CBasicString<T>::operator<(const CBasicString& other) const    // 0x00429950
{
    return Compare(other) < 0;
}
#pragma auto_inline(on)

class CCharStringData { public:
    CBasicString<char> Data;
};

class CCharString {
public:
    CCharStringData* PStringData;
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

template class _Rb_tree<CCharString, pair<const CCharString, float>, _Select1st<pair<const CCharString, float> >, less<CCharString>,
                        allocator<pair<const CCharString, float> > >;

} // namespace std