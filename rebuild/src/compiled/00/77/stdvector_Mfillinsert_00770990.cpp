// std::vector::_M_fill_insert(list<CWeaponTrailEntry>* pos, size_type n, const list<CWeaponTrailEntry>& x) -- STLport fill-insert of n copies of a weapon-trail point list at pos: shift in place when the capacity suffices, else grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00770990, length 272 bytes (thiscall, `ret 0Ch`).
// Identity: the catalog label "Vector_Insert_LinkedList_Conditional" is descriptive. The only caller is the landed
//   vector<list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry> >::resize 0x00770D60 (`insert(end(), n - size(), x)`);
//   CTCWeaponTrail::Trail's TU.
// Callees: list copy constructor 0x00770050 (x_copy), __uninitialized_copy 0x007705A0, __copy_backward_ptrs 0x00770600
//   (`*--result = *--last` through list::operator= 0x0076FF90), fill 0x00770200, __uninitialized_fill_n 0x007702B0,
//   ~list 0x004DB5BB (the ICF-shared list destructor), _M_insert_overflow 0x00770810 (this chunk).
// Shape: STLport 4.x vector.c `_M_fill_insert` verbatim: `if (n != 0) { if (size_type(_M_end_of_storage - _M_finish) >= n)
//   { _Tp x_copy = x; elems_after = _M_finish - pos; old_finish = _M_finish; if (elems_after > n) {
//   __uninitialized_copy(_M_finish - n, _M_finish, _M_finish, __false_type()); _M_finish += n;
//   __copy_backward_ptrs(pos, old_finish - n, old_finish, __false_type()); fill(pos, pos + n, x_copy); } else {
//   uninitialized_fill_n(_M_finish, n - elems_after, x_copy); _M_finish += n - elems_after; __uninitialized_copy(pos,
//   old_finish, _M_finish, __false_type()); _M_finish += elems_after; fill(pos, old_finish, x_copy); } } else
//   _M_insert_overflow(pos, x, __false_type(), n); }`. x_copy lives in the dead `x` slot; the out-of-line helpers have
//   same-TU noinline bodies that never read their tags, so every tag is homed on the `n` slot.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14

inline void* operator new(unsigned int, void* p) { return p; }

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

class CEnginePrimitiveWeaponTrail {
public:
    class CWeaponTrailEntry {                               // Ego_r.pdb, 28 bytes
    public:
        C3DVector Point0;                                   // +0x00
        C3DVector Point1;                                   // +0x0C
        float     Time;                                     // +0x18
    };
};

namespace std {

struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _T1, class _T2> inline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{                                                                                              // 0x00770350
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _ForwardIterator, class _Tp>
inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy(__first, __last, __first);
}

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

template <class _Tp> struct _List_iterator {                // _List_iterator<_Tp, _Nonconst_traits<_Tp> >
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
};

template <class _Tp> struct _List_const_iterator {          // _List_iterator<_Tp, _Const_traits<_Tp> >: implicit copy
    _List_node_base* _M_node;
    const _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_const_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_const_iterator& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_node<_Tp>           _Node;
    typedef _List_iterator<_Tp>       iterator;
    typedef _List_const_iterator<_Tp> const_iterator;

    _Node* _M_node;                                         // the alloc proxy's node pointer (4 bytes)

    list(const list& __x) : _M_node(0)
    {                                                                                          // 0x00770050
        _Node* __n = (_Node*)malloc(sizeof(_Node));
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node = __n;
        insert(begin(), __x.begin(), __x.end());
    }
    list& operator=(const list& __x);                                                          // 0x0076FF90
    ~list()
    {                                                                                          // 0x004DB5BB
        _List_node_base* __cur = _M_node->_M_next;
        while (__cur != _M_node) {
            _List_node_base* __tmp = __cur;
            __cur = __cur->_M_next;
            free(__tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
        if (_M_node != 0)
            free(_M_node);
    }
    iterator begin() { return iterator(_M_node->_M_next); }
    const_iterator begin() const { const_iterator __it; __it._M_node = _M_node->_M_next; return __it; }
    const_iterator end() const { const_iterator __it; __it._M_node = _M_node; return __it; }

    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = (_Node*)malloc(sizeof(_Node));
        new (&__tmp->_M_data) _Tp(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return iterator(__tmp);
    }
    template <class _InputIter>
    __declspec(noinline) void _M_insert_dispatch(iterator __pos, _InputIter __first, _InputIter __last,
                                                 const __false_type&)
    {                                                                                          // 0x0076FD40
        for (; __first != __last; ++__first)
            insert(__pos, *__first);
    }
    template <class _InputIter> void insert(iterator __pos, _InputIter __first, _InputIter __last)
    {
        _M_insert_dispatch(__pos, __first, __last, __false_type());
    }
};


template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                              // 0x007705A0
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{                                                                                              // 0x007702B0
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _BidirectionalIter1, class _BidirectionalIter2>
__declspec(noinline) _BidirectionalIter2 __fastcall __copy_backward_ptrs(_BidirectionalIter1 __first,
                                                                         _BidirectionalIter1 __last,
                                                                         _BidirectionalIter2 __result, const __false_type&)
{                                                                                              // 0x00770600
    for (int __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                                              // 0x00770200
    for (; __first != __last; ++__first)
        *__first = __val;
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    public:
    protected:
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    public:
    protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                                             // 0x00770810
    public:
    void _M_fill_insert(pointer __position, size_type __n, const _Tp& __x);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(pointer __position, size_type __n, const _Tp& __x)
{
    if (__n != 0) {
        if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const size_type __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __false_type());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                fill(__position, __position + __n, __x_copy);
            }
            else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, __false_type());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        }
        else
            _M_insert_overflow(__position, __x, __false_type(), __n);
    }
}

template void vector<list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry> >::_M_fill_insert(
    list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry>*, unsigned int,
    const list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry>&);

}