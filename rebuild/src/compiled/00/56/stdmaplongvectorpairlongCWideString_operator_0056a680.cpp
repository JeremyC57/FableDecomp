// std::map<long, vector<pair<long, CWideString> > >::operator[](const long& k) -- STLport map index: lower_bound, insert an empty entry vector when the key is absent, return the mapped vector
// Leaf: ??A?$map
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056A680, 188 bytes, thiscall `ret 4`. The scoreboard map whose tree erase is
//   0x00569A80 (this chunk) and whose destructors are the landed 0x0056A320 / 0x0056A5C0. The catalog label
//   "Scoreboard_LookupOrCreateEntry" is descriptive.
// Callees: vector<pair<long, CWideString> >::vector(const vector&) @ 0x00566CD0 (the value_type's second member),
//   _Rb_tree::insert_unique(iterator, const value_type&) @ 0x0056A370 (iterator through the hidden pointer),
//   CWideString::~CWideString() @ 0x0099B510 and free @ 0x00BFEA14 (the value_type temporary's vector).
// Layout: STLport map {header node*, node count, comparator}; node colour +0, parent +4, left +8, right +0xC,
//   key +0x10, vector +0x14 (start/finish/end); 8-byte elements {long, CWideString}.
// Shape: the landed STLport operator[] (0x0047CB50): inline lower_bound (signed `jl`), `if (i == end() ||
//   key_comp()(k, i->first)) i = insert(i, value_type(k, _Tp())); return i->second;` -- the iterator by value
//   through its user copy ctor (`push ecx ; mov eax,esp`), the result homed in the dead `k` slot, the
//   duplicated return tail. The temporaries die in reverse order: the pair's vector inline (8-byte stride,
//   ~CWideString on elem+4, then free), then `_Tp()`, whose null members VC7.1 propagates (`xor edx,edx` held
//   from the lower_bound walk to the three stores; no destroy loop and no free). That propagation, and the
//   start/finish/result kept in edi/ebx/ebp across the destroy calls, need the copy ctor and insert_unique in
//   THIS TU and out of line, as retail's are (0x00566CD0, 0x0056A370 sit beside it): the temporaries'
//   addresses then do not escape. Use `__declspec(noinline)` here: `#pragma auto_inline(off)` (the 0x0047CB50
//   lever) also un-inlined the pair ctor in this TU (200 v 188). Both are stand-in bodies (rows elsewhere).
#include <stdlib.h>
#include <new>

class CWideStringData;
class CWideString {
public:
    CWideStringData* PStringData;
    ~CWideString();                                         // 0x0099B510
};

namespace std {

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp> class _STLP_alloc_proxy : public allocator<_Tp> {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const allocator<_Tp>& __a, _Value __p) : allocator<_Tp>(__a), _M_data(__p) { }
};

template <class _Tp> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp> _M_end_of_storage;         // +0x08
    _Vector_base(const allocator<_Tp>& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            free(_M_start);
    }
};

template <class _Tp> class vector : public _Vector_base<_Tp> {
public:
    explicit vector(const allocator<_Tp>& __a = allocator<_Tp>()) : _Vector_base<_Tp>(__a) { }
    __declspec(noinline) vector(const vector& __x);         // 0x00566CD0
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result);

// Stand-in for 0x00566CD0, defined in this TU and kept out of line: its argument does not escape.
template <class _Tp> vector<_Tp>::vector(const vector& __x) : _Vector_base<_Tp>(allocator<_Tp>())
{
    unsigned int __n = (unsigned int)(__x._M_finish - __x._M_start);
    this->_M_start = (_Tp*)malloc(__n * sizeof(_Tp));
    this->_M_finish = __uninitialized_copy((const _Tp*)__x._M_start, (const _Tp*)__x._M_finish, this->_M_start);
    this->_M_end_of_storage._M_data = this->_M_start + __n;
}

template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x00
    _T2 second;                                             // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair& __p) : first(__p.first), second(__p.second) { }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0c
};

template <class _Dummy> struct _Rb_global {
    static void __cdecl _Rebalance(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root);   // 0x0042971D
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};

template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) { }
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
};

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _Key, class _Value, class _Compare> class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value> iterator;

    _STLP_alloc_proxy<_Link_type, _Rb_tree_node<_Value> > _M_header;   // +0x00
    unsigned int _M_node_count;                                         // +0x04
    _Compare _M_key_compare;                                            // +0x08

    static const _Key& _S_key(_Rb_tree_node_base* __x) { return ((_Link_type)__x)->_M_value_field.first; }
    _Link_type _M_root() const { return (_Link_type)this->_M_header._M_data->_M_parent; }
    iterator end() { return iterator(this->_M_header._M_data); }
    _Link_type _M_lower_bound(const _Key& __k) const
    {
        _Link_type __y = this->_M_header._M_data;           // last node not less than __k
        _Link_type __x = _M_root();
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = (_Link_type)__x->_M_left;
            else
                __x = (_Link_type)__x->_M_right;
        return __y;
    }
    iterator lower_bound(const _Key& __k) { return iterator(_M_lower_bound(__k)); }
    __declspec(noinline) iterator insert_unique(iterator __position, const _Value& __v);   // 0x0056A370
};

// Stand-in for 0x0056A370 (the hinted insert; its own row lies outside this chunk): same TU and out of line,
// so the value reference does not escape.
template <class _Key, class _Value, class _Compare>
typename _Rb_tree<_Key, _Value, _Compare>::iterator
_Rb_tree<_Key, _Value, _Compare>::insert_unique(iterator __position, const _Value& __v)
{
    _Link_type __z = (_Link_type)malloc(sizeof(_Rb_tree_node<_Value>));
    _Construct(&__z->_M_value_field, __v);
    __position._M_node->_M_left = __z;
    __z->_M_parent = __position._M_node;
    __z->_M_left = 0;
    __z->_M_right = 0;
    _Rb_global<bool>::_Rebalance(__z, this->_M_header._M_data->_M_parent);
    ++this->_M_node_count;
    return iterator(__z);
}

template <class _Key, class _Tp, class _Compare = less<_Key> > class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Compare> _Rep_type;
    typedef typename _Rep_type::iterator iterator;

    _Rep_type _M_t;

    iterator lower_bound(const _Key& __x) { return _M_t.lower_bound(__x); }
    iterator end() { return _M_t.end(); }
    _Compare key_comp() const { return _M_t._M_key_compare; }
    iterator insert(iterator __position, const value_type& __x) { return _M_t.insert_unique(__position, __x); }
    _Tp& operator[](const _Key& __k)
    {
        iterator __i = lower_bound(__k);
        if (__i == end() || key_comp()(__k, (*__i).first))
            __i = insert(__i, value_type(__k, _Tp()));
        return (*__i).second;
    }
};

template vector<pair<long, CWideString> >::vector(const vector<pair<long, CWideString> >&);
template class map<long, vector<pair<long, CWideString> > >;
}