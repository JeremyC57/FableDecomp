// _STL::map::operator[](const CCharString& k) -- STLport subscript of map<CCharString, vector<CCharString> >: lower_bound, insert (k, empty vector) when absent, return the mapped vector
// Leaf: ??A?$map@VCCharString@@V?$vector@VCCharString@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007AEBE0, 224 bytes, __fastcall `ret 4`.
// Identity: the value type is pair<const CCharString, vector<CCharString> > (landed _Rb_tree::_M_erase @ 0x007AE0E0 and
//   vector<CCharString> copy ctor @ 0x007ACEA0 of the same tree); 41 call sites (CGameScriptInterface::SetGossip
//   0x008A9E90, CTCAIScratchpad 0x007ABBD0, ...). The manifest's CArray<CMeshDataBank::CLipSyncEntry> is propagated.
// Callees: _M_lower_bound @ 0x007ACC00 (node pointer in eax), CBasicString<char>::operator< @ 0x00429950, CCharString copy
//   ctor @ 0x0099EC30 / dtor @ 0x0099EAE0, vector<CCharString> copy ctor @ 0x007ACEA0, _Rb_tree::insert_unique(iterator,
//   const value_type&) @ 0x007AE870 (result through the dead `k` slot), free @ 0x00BFEA14.
// Shape: STLport 4.6 map::operator[] verbatim. The default vector is three zero stores and its destructor folds away;
//   the value_type temporary's ~vector destroys [start, finish) and frees the null-tested start, then the key dies.
//   Same-TU stand-in (accepted precedent 0x0075D2B0): the vector copy constructor's body sits in this TU under
//   auto_inline(off); seeing that it only reads its argument lets VC7.1 fold the default temporary's ~vector, as
//   retail's whole-program knowledge did (declared-only, the temporary is re-read and freed: 255v224).
template <class C> class CBasicString {
public:
    bool operator<(const CBasicString& other) const;     // 0x00429950
};

class CCharString {
public:
    CBasicString<char>* PData;                           // +0x00
    CCharString();                                       // 0x0099E4B0
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
    bool operator<(const CCharString& other) const
    {
        if (other.PData == PData)
            return false;
        if (PData == 0)
            return true;
        if (other.PData == 0)
            return false;
        return *PData < *other.PData;
    }
};
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

namespace _STL {

template <class _Tp> inline void _Destroy(_Tp* __p) { __p->~_Tp(); }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    for (; __first != __last; ++__first)
        _Destroy(__first);
}

template <class _Tp> inline _Tp* __uninitialized_copy(const _Tp* __first, const _Tp* __last, _Tp* __result)
{
    for (; __first != __last; ++__first, ++__result)
        new (__result) _Tp(*__first);
    return __result;
}

template <class _Tp> class vector {                      // STLport 4.6 vector, malloc allocator
public:
    _Tp* _M_start;                                       // +0
    _Tp* _M_finish;                                      // +4
    _Tp* _M_end_of_storage;                              // +8
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    vector(const vector<_Tp>& __x);                      // 0x007ACEA0 (body below)
    ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start != 0)
            free(_M_start);
    }
};


// Same-TU copy of the out-of-line copy constructor 0x007ACEA0 (landed shape: allocate exactly x.size() slots, then copy the
// elements); under auto_inline(off) it stays a call, and VC7.1 sees that it only reads `x`, so the default vector
// temporary's destructor folds away as in retail (retail's whole-program knowledge).
#pragma auto_inline(off)
template <class _Tp> vector<_Tp>::vector(const vector<_Tp>& __x)
{
    unsigned int __n = (unsigned int)(__x._M_finish - __x._M_start);
    _M_start = 0;
    _M_finish = 0;
    _M_end_of_storage = 0;
    _M_start = __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0;
    _M_finish = _M_start;
    _M_end_of_storage = _M_start + __n;
    _M_finish = __uninitialized_copy((const _Tp*)__x._M_start, (const _Tp*)__x._M_finish, _M_start);
}
#pragma auto_inline(on)

} // namespace _STL
using _STL::vector;

namespace _STL {

template <class _T1, class _T2> struct pair {             // STLport 4.6 (member templates on)
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) {}
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};

template <class _Tp> inline _Tp __default_constructed(_Tp*) { return _Tp(); }   // _STLP_DEFAULT_CONSTRUCTED

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : _Rb_tree_node_base {
    _Value _M_value_field;                               // +0x10
};
template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) {}
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) {}
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
};

template <class _Key, class _Tp, class _Compare = less<_Key> > class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree_node_base* _M_header;                       // +0x00
    unsigned int        _M_node_count;                   // +0x04
    _Compare            _M_key_compare;                  // +0x08
    _Rb_tree_node_base* _M_lower_bound(const _Key& __k) const;          // 0x007ACC00
    iterator insert_unique(iterator __position, const value_type& __x);  // 0x007AE870 (_Rb_tree::insert_unique)
    iterator insert(iterator __position, const value_type& __x) { return insert_unique(__position, __x); }
    iterator lower_bound(const _Key& __k) { return iterator(_M_lower_bound(__k)); }
    iterator end() { return iterator(_M_header); }
    _Compare key_comp() const { return _M_key_compare; }
    _Tp& operator[](const _Key& __k)
    {
        iterator __i = lower_bound(__k);
        if (__i == end() || key_comp()(__k, (*__i).first))
            __i = insert(__i, value_type(__k, __default_constructed((_Tp*)0)));
        return (*__i).second;
    }
};

template vector<CCharString> & map<CCharString, vector<CCharString> >::operator[](const CCharString&);

} // namespace _STL