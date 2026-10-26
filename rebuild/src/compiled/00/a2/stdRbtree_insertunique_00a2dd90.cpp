// std::_Rb_tree::insert_unique(const value_type& v) -- (map<CCharString, vector<CMeshDataBank::CLipSyncEntry> >) STLport unique insert over CCharString keys: walk from the root to the insertion parent, step the candidate back once when the last turn was left (unless it is the leftmost), insert unless the candidate's key equals v's; pair<iterator, bool> through the hidden pointer
// Leaf: ?insert_unique@?$_Rb_tree@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2DD90, 256 bytes, thiscall, `ret 8` (hidden pair<iterator, bool>
//   result pointer and &v).
// Identity: map<CCharString, vector<CMeshDataBank::CLipSyncEntry> > -- proven; evidence: its caller is the hinted insert_unique 0x00A2DEF0 of the landed map<CCharString, vector<CMeshDataBank::CLipSyncEntry> >::operator[] 0x00A2E290 (CMeshDataBank::InitialiseEntry), and its _M_insert 0x00A2DC90 allocates 0x20-byte nodes and copies the value with the vector<CLipSyncEntry> copy constructor 0x00A2B970.
// Label: the manifest's "std_set_insert_CCharString" is propagated across byte-identical bodies; this is STLport 4.6
//   `_Rb_tree::insert_unique(const value_type&)`.
// Callees: CBasicString<char>::operator< @ 0x00429950 (same-TU noinline body, behind the inline CCharString::operator<:
//   equal data pointers -> false, null this -> true, null other -> false), this tree's _M_insert(x, y, v, w) @ 0x00A2DC90
//   and its iterator's operator--() @ 0x00A2A550 (own COMDATs, declared only).
// Shape: STLport 4.6 _tree.c verbatim; the begin() branch inserts with `_M_insert(y, y, v)`, the other with
//   `_M_insert(x, y, v)`; pair and iterator have user constructors (hidden-pointer return, iterator copied in place).
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
class CMeshDataBank { public: class CLipSyncEntry { public: CCharString PhonemeName; unsigned long AnimationIndex; }; };
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
template <class _Tp, class _Alloc = allocator<_Tp> > class vector { public: _Tp* _M_start; _Tp* _M_finish; _Tp* _M_end_of_storage; };
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Tp> struct _Nonconst_traits { };

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;
    _Base_ptr _M_parent;
    _Base_ptr _M_left;
    _Base_ptr _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    bool operator==(const _Rb_tree_base_iterator& __y) const { return _M_node == __y._M_node; }
};
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    _Rb_tree_iterator(_Rb_tree_node<_Value>* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Self& __it) { _M_node = __it._M_node; }
    _Self& operator--();                               // 0x00A2A550 (own COMDAT)
};
template <class _Tp, class _Alloc> class _Rb_tree_base {
public:
    _STLP_alloc_proxy<_Rb_tree_node<_Tp>*, _Rb_tree_node<_Tp>, allocator<_Rb_tree_node<_Tp> > > _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }
    static _Link_type _S_left(_Link_type __x) { return (_Link_type)__x->_M_left; }
    static _Link_type _S_right(_Link_type __x) { return (_Link_type)__x->_M_right; }
    _Link_type _M_root() const { return (_Link_type)this->_M_header._M_data->_M_parent; }
    iterator begin() { return iterator((_Link_type)this->_M_header._M_data->_M_left); }
private:
    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const _Value& __v, _Base_ptr __w = 0);   // 0x00A2DC90
public:
    pair<iterator, bool> insert_unique(const _Value& __v);
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
pair<typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator, bool>
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_unique(const _Value& __v)
{
    _Link_type __y = this->_M_header._M_data;
    _Link_type __x = _M_root();
    bool __comp = true;
    while (__x != 0) {
        __y = __x;
        __comp = _M_key_compare(_KeyOfValue()(__v), _S_key(__x));
        __x = __comp ? _S_left(__x) : _S_right(__x);
    }
    iterator __j = iterator(__y);
    if (__comp)
        if (__j == begin())
            return pair<iterator, bool>(_M_insert(__y, __y, __v), true);
        else
            --__j;
    if (_M_key_compare(_S_key(__j._M_node), _KeyOfValue()(__v)))
        return pair<iterator, bool>(_M_insert(__x, __y, __v), true);
    return pair<iterator, bool>(__j, false);
}
typedef pair<const CCharString, vector<CMeshDataBank::CLipSyncEntry> > _TreeValue;
template pair<_Rb_tree<CCharString, _TreeValue, _Select1st<_TreeValue>, less<CCharString> >::iterator, bool>
    _Rb_tree<CCharString, _TreeValue, _Select1st<_TreeValue>, less<CCharString> >::insert_unique(const _TreeValue&);
}