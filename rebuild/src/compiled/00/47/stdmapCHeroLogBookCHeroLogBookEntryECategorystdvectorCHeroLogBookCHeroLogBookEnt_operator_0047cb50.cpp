// std::map<CHeroLogBook::CHeroLogBookEntry::ECategory, std::vector<CHeroLogBook::CHeroLogBookEntry> >::operator[](const ECategory& k) -- lower_bound, insert an empty entry vector when the category is missing, return the mapped vector
// Retail: Fable.exe (TLC, VC7.1) @ 0x0047CB50, 207 bytes, `ret 4`.
// Callees: vector<CHeroLogBookEntry>::vector(const vector&) @ 0x0047BB90 (the pair's second),
// _Rb_tree::insert_unique(iterator hint, const value_type&) @ 0x0047C8C0 (iterator by hidden pointer),
// std::__destroy_aux<CHeroLogBookEntry*>(first, last, const __false_type&) @ 0x0047BAF0 (both temporaries' ~vector),
// free @ 0x00BFEA14.
// Shape: STLport 4.x `T& operator[](const key_type& k) { iterator i = lower_bound(k); if (i == end() ||
// key_comp()(k, (*i).first)) i = insert(i, value_type(k, T())); return (*i).second; }` with lower_bound inlined
// (aligned loop, signed `jl`: the enum key); the by-value hint iterator has a user copy constructor (built in its
// argument slot), the insert's result and the destroy tags reuse the dead key-argument slot. The temporaries die in
// reverse order (the pair, then `T()`, whose null start/finish the compiler propagates: `xor edx,edx ; xor ecx,ecx`
// and no free). That propagation, and the start pointer kept in esi across the destroy call, need the callees in
// THIS TU and out of line (`auto_inline(off)`), as retail's are (0x0047BAF0, 0x0047BB90, 0x0047C500, 0x0047C8C0 all
// sit beside it): VC7.1 then knows the temporaries' addresses do not escape. With them only declared, it reloads
// the pair's start and the `T()` members after the calls (218 v 207 bytes). insert_unique is a stand-in body (its own
// row lies outside this chunk); the others follow the retail shapes. Speed TU (CHeroLogBook::LogEntries).
// Label correction: catalog "CHeroLogBook_InsertEntry" is a guess; this is the LogEntries map's operator[].
// Leaf: ??A?$map
#include <stdlib.h>
#include <new>

class CWideStringData;
class CWideString {
public:
    CWideStringData* PStringData;
    ~CWideString();                                         // 0x0099B510
};

class CHeroLogBook {
public:
    class CHeroLogBookEntry {
    public:
        enum ECategory { CATEGORY_NONE };
        CWideString Name;                                   // +0x00
        CWideString AbbreviatedName;                        // +0x04
        CWideString Content;                                // +0x08
    };
};

namespace std {
struct __false_type { __false_type() { } };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

#pragma auto_inline(off)
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)   // 0x0047BAF0
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
#pragma auto_inline(on)

template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
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
    vector(const vector& __x);                              // 0x0047BB90 for CHeroLogBookEntry
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                             const __false_type&);                         // 0x0047B710

// 0x0047BB90, defined in this TU (as retail's copy is) and kept out of line: the caller then knows its argument
// does not escape, so the `T()` temporary's null members survive the later calls.
#pragma auto_inline(off)
template <class _Tp> vector<_Tp>::vector(const vector& __x) : _Vector_base<_Tp>(allocator<_Tp>())
{
    unsigned int __n = (unsigned int)(__x._M_finish - __x._M_start);
    this->_M_start = (_Tp*)malloc(__n * sizeof(_Tp));
    this->_M_finish = __uninitialized_copy((const _Tp*)__x._M_start, (const _Tp*)__x._M_finish, this->_M_start,
                                           __false_type());
    this->_M_end_of_storage._M_data = this->_M_start + __n;
}
#pragma auto_inline(on)

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
protected:
    _Link_type _M_create_node(const _Value& __x)
    {
        _Link_type __tmp = (_Link_type)malloc(sizeof(_Rb_tree_node<_Value>));
        _Construct(&__tmp->_M_value_field, __x);
        return __tmp;
    }
private:
    iterator _M_insert(_Rb_tree_node_base* __x, _Rb_tree_node_base* __y, const _Value& __v,
                       _Rb_tree_node_base* __w = 0);                     // 0x0047C500
public:
    iterator insert_unique(iterator __position, const _Value& __v);     // 0x0047C8C0
};

#pragma auto_inline(off)
template <class _Key, class _Value, class _Compare>
typename _Rb_tree<_Key, _Value, _Compare>::iterator
_Rb_tree<_Key, _Value, _Compare>::_M_insert(_Rb_tree_node_base* __x_, _Rb_tree_node_base* __y_, const _Value& __v,
                                            _Rb_tree_node_base* __w_)
{
    _Link_type __w = (_Link_type)__w_;
    _Link_type __x = (_Link_type)__x_;
    _Link_type __y = (_Link_type)__y_;
    _Link_type __z;
    if (__y == this->_M_header._M_data ||
        (__w == 0 && (__x != 0 || _M_key_compare(__v.first, _S_key(__y))))) {
        __z = _M_create_node(__v);
        __y->_M_left = __z;
        if (__y == this->_M_header._M_data) {
            this->_M_header._M_data->_M_parent = __z;
            this->_M_header._M_data->_M_right = __z;
        }
        else if (__y == this->_M_header._M_data->_M_left)
            this->_M_header._M_data->_M_left = __z;
    }
    else {
        __z = _M_create_node(__v);
        __y->_M_right = __z;
        if (__y == this->_M_header._M_data->_M_right)
            this->_M_header._M_data->_M_right = __z;
    }
    __z->_M_parent = __y;
    __z->_M_left = 0;
    __z->_M_right = 0;
    _Rb_global<bool>::_Rebalance(__z, this->_M_header._M_data->_M_parent);
    ++this->_M_node_count;
    return iterator(__z);
}

// Stand-in for 0x0047C8C0 (the hinted insert; its own row is elsewhere): same TU and out of line, so the value
// reference does not escape.
template <class _Key, class _Value, class _Compare>
typename _Rb_tree<_Key, _Value, _Compare>::iterator
_Rb_tree<_Key, _Value, _Compare>::insert_unique(iterator __position, const _Value& __v)
{
    return _M_insert(0, __position._M_node, __v);
}
#pragma auto_inline(on)

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

template vector<CHeroLogBook::CHeroLogBookEntry>::vector(const vector<CHeroLogBook::CHeroLogBookEntry>&);
template class map<CHeroLogBook::CHeroLogBookEntry::ECategory, vector<CHeroLogBook::CHeroLogBookEntry> >;
}