// std::deque<C2DVector>::erase(iterator first, iterator last) -- STLport deque range erase: the whole deque is a clear(); otherwise shift the shorter side over the gap (copy_backward the front part up, or copy the back part down), destroy the vacated elements, free their node buffers and move the start or finish; returns start + elems_before
// Leaf: ?erase@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0F040, 663 bytes, thiscall `ret 24h` (hidden result pointer, first, last; the
//   16-byte iterators by value). Single caller 0x00A0FCE8 (the deque's resize). The catalog label
//   "RenderList_ProcessIteration" is a guess: the landed deque<C2DVector> model 0x00A0F350 already names this address
//   deque::erase(first, last).
// Callees: clear() @ 0x00A0EAA0 -> 0x00A0EA90 (landed), copy_backward(first, last, result) @ 0x00A0EF90 (landed,
//   /Gr `__fastcall`, hidden result in ecx), copy(first, last, result) @ 0x00A0EB40 (landed), the iterator's
//   operator+(int) const @ 0x00A0E980 and operator-(int) const @ 0x00A0EA30 (landed; out of line in this function),
//   _Destroy(first, last) @ 0x0048D490 (landed, COMDAT from another TU), _M_destroy_nodes(nstart, nfinish)
//   @ 0x0048C820 (shared COMDAT, landed as a free-function twin).
// Model: the landed deque<C2DVector> TU model (0x00A0F350; _M_map / _M_map_size as _STLP_alloc_proxy, see 0x00A0EE50).
// Shape: STLport 4.6 `erase(first, last)` verbatim (copy / copy_backward are same-TU noinline stand-ins: with
//   declarations only VC7.1 gives their unused results and new_start / new_finish a fresh 16-byte frame slot instead
//   of retail's dead `last` argument slot): the full-range test compares _M_cur only (operator==), n and
//   elems_before are the inline _M_subtract, `(size() - n) / 2` is a signed division. The iterator's operator+ /
//   operator- are same-TU noinline bodies here (retail calls them out of line from this function, while the smaller
//   _M_fill_insert 0x00A0FAA0 inlines them).
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);   // 0x00BFEAE6
inline void* operator new(unsigned int, void* p) { return p; }

class C2DVector {                                           // 8 bytes
public:
    float X, Y;
    C2DVector() { }
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) { }
};

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}

inline void* __copy_trivial_backward(const void* __first, const void* __last, void* __result)
{
    const int _Num = (const char*)__last - (const char*)__first;
    return (_Num > 0) ? memmove((char*)__result - _Num, __first, _Num) : __result;
}

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp*) { }
template <class _Tp> inline void _Destroy(_Tp*, _Tp*) { }

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp> struct _Const_traits { typedef const _Tp& reference; typedef const _Tp* pointer; };

template <class _Tp> struct _Deque_iterator_base {          // 16 bytes
    enum _Constants { _blocksize = 128, __buffer_size = (sizeof(_Tp) < (unsigned int)_blocksize ? ((unsigned int)_blocksize / sizeof(_Tp)) : 1) };
    typedef int difference_type;
    typedef _Tp** _Map_pointer;
    typedef _Deque_iterator_base<_Tp> _Self;

    _Tp*        _M_cur;                                     // +0x00
    _Tp*        _M_first;                                   // +0x04
    _Tp*        _M_last;                                    // +0x08
    _Map_pointer _M_node;                                   // +0x0c

    _Deque_iterator_base() : _M_cur(0), _M_first(0), _M_last(0), _M_node(0) { }
    difference_type _M_subtract(const _Self& __x) const
    {
        return difference_type(__buffer_size) * (_M_node - __x._M_node - 1) + (_M_cur - _M_first) + (__x._M_last - __x._M_cur);
    }
    void _M_increment()
    {
        if (++_M_cur == _M_last) {
            _M_set_node(_M_node + 1);
            _M_cur = _M_first;
        }
    }
    void _M_decrement()
    {
        if (_M_cur == _M_first) {
            _M_set_node(_M_node - 1);
            _M_cur = _M_last;
        }
        --_M_cur;
    }
    void _M_advance(difference_type __n);                   // 0x00A0E2A0
    void _M_set_node(_Map_pointer __new_node)
    {
        _M_last = (_M_first = *(_M_node = __new_node)) + difference_type(__buffer_size);
    }
};

template <class _Tp, class _Traits> struct _Deque_iterator : public _Deque_iterator_base<_Tp> {
    typedef typename _Traits::reference reference;
    typedef int difference_type;
    typedef _Deque_iterator<_Tp, _Traits> _Self;
    typedef _Deque_iterator<_Tp, _Nonconst_traits<_Tp> > _Nonconst_self;

    _Deque_iterator() { }
    _Deque_iterator(const _Nonconst_self& __x) : _Deque_iterator_base<_Tp>(__x) { }

    reference operator*() const { return *this->_M_cur; }
    difference_type operator-(const _Self& __x) const { return this->_M_subtract(__x); }
    _Self& operator++() { this->_M_increment(); return *this; }
    _Self& operator--() { this->_M_decrement(); return *this; }
    _Self& operator+=(difference_type __n) { this->_M_advance(__n); return *this; }
    __declspec(noinline) _Self operator+(difference_type __n) const   // 0x00A0E980
    {
        _Self __tmp = *this;
        return __tmp += __n;
    }
    _Self& operator-=(difference_type __n) { return *this += -__n; }
    __declspec(noinline) _Self operator-(difference_type __n) const   // 0x00A0EA30
    {
        _Self __tmp = *this;
        return __tmp -= __n;
    }
    reference operator[](difference_type __n) const { return *(*this + __n); }
};

template <class _Tp, class _Traits1, class _Traits2>
inline bool __fastcall operator==(const _Deque_iterator<_Tp, _Traits1>& __x, const _Deque_iterator<_Tp, _Traits2>& __y) { return __x._M_cur == __y._M_cur; }
template <class _Tp, class _Traits1, class _Traits2>
inline bool __fastcall operator!=(const _Deque_iterator<_Tp, _Traits1>& __x, const _Deque_iterator<_Tp, _Traits2>& __y) { return __x._M_cur != __y._M_cur; }

template <class _Tp, class _Alloc = allocator<_Tp> > class deque {
public:
    typedef _Tp value_type;
    typedef unsigned int size_type;
    typedef int difference_type;
    typedef _Tp** _Map_pointer;
    typedef _Deque_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _Deque_iterator<_Tp, _Const_traits<_Tp> > const_iterator;

    iterator     _M_start;                                  // +0x00
    iterator     _M_finish;                                 // +0x10
    _STLP_alloc_proxy<_Map_pointer, _Tp*, allocator<_Tp*> > _M_map;   // +0x20
    _STLP_alloc_proxy<size_type, _Tp, _Alloc> _M_map_size;            // +0x24

    static size_type buffer_size() { return (size_type)_Deque_iterator_base<_Tp>::__buffer_size; }
    size_type size() const { return this->_M_finish - this->_M_start; }

    void push_front(const value_type& __t)
    {
        if (this->_M_start._M_cur != this->_M_start._M_first) {
            _Construct(this->_M_start._M_cur - 1, __t);
            --this->_M_start._M_cur;
        }
        else
            _M_push_front_aux_v(__t);
    }
    void pop_back()
    {
        if (this->_M_finish._M_cur != this->_M_finish._M_first) {
            --this->_M_finish._M_cur;
            _Destroy(this->_M_finish._M_cur);
        }
        else
            _M_pop_back_aux();
    }
    iterator erase(iterator __first, iterator __last);
    void insert(iterator __pos, size_type __n, const value_type& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const value_type& __x);
    void clear();                                                                        // 0x00A0EA90
    void _M_destroy_nodes(_Map_pointer __nstart, _Map_pointer __nfinish);              // 0x0048C820

    void _M_fill_insert(iterator __pos, size_type __n, const value_type& __x);          // 0x00A0FAA0
    void _M_push_front_aux_v(const value_type& __t);                                     // 0x00A0F3B0
    void _M_pop_back_aux();
    void _M_reallocate_map(size_type __nodes_to_add, bool __add_at_front);              // 0x00A0EE50
    void _M_reserve_map_at_back(size_type __nodes_to_add = 1)
    {
        if (__nodes_to_add + 1 > this->_M_map_size._M_data - (this->_M_finish._M_node - this->_M_map._M_data))
            _M_reallocate_map(__nodes_to_add, false);
    }
    void _M_reserve_map_at_front(size_type __nodes_to_add = 1)
    {
        if (__nodes_to_add > size_type(this->_M_start._M_node - this->_M_map._M_data))
            _M_reallocate_map(__nodes_to_add, true);
    }
    iterator _M_reserve_elements_at_front(size_type __n)
    {
        size_type __vacancies = this->_M_start._M_cur - this->_M_start._M_first;
        if (__n > __vacancies)
            _M_new_elements_at_front(__n - __vacancies);
        return this->_M_start - difference_type(__n);
    }
    iterator _M_reserve_elements_at_back(size_type __n)
    {
        size_type __vacancies = (this->_M_finish._M_last - this->_M_finish._M_cur) - 1;
        if (__n > __vacancies)
            _M_new_elements_at_back(__n - __vacancies);
        return this->_M_finish + difference_type(__n);
    }
    void _M_insert_aux(iterator __pos, size_type __n, const value_type& __x);          // 0x00A0F4B0
    void _M_new_elements_at_front(size_type __new_elements);                            // 0x00A0F300
    void _M_new_elements_at_back(size_type __new_elements);                             // 0x00A0F350
};

template <class _ForwardIterator>
void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last);                        // 0x0048D490
#pragma auto_inline(off)
template <class _InputIter, class _OutputIter>
__declspec(noinline) _OutputIter __fastcall copy(_InputIter __first, _InputIter __last, _OutputIter __result)
{                                                                                 // 0x00A0EB40
    for (int __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
__declspec(noinline) _BidirectionalIter2 __fastcall copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                                  _BidirectionalIter2 __result)
{                                                                                 // 0x00A0EF90
    for (int __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
#pragma auto_inline(on)

template <class _Tp, class _Alloc>
typename deque<_Tp, _Alloc>::iterator deque<_Tp, _Alloc>::erase(iterator __first, iterator __last)
{
    if (__first == this->_M_start && __last == this->_M_finish) {
        clear();
        return this->_M_finish;
    } else {
        difference_type __n = __last - __first;
        difference_type __elems_before = __first - this->_M_start;
        if (__elems_before < difference_type(this->size() - __n) / 2) {
            copy_backward(this->_M_start, __first, __last);
            iterator __new_start = this->_M_start + __n;
            _Destroy(this->_M_start, __new_start);
            this->_M_destroy_nodes(this->_M_start._M_node, __new_start._M_node);
            this->_M_start = __new_start;
        } else {
            copy(__last, this->_M_finish, __first);
            iterator __new_finish = this->_M_finish - __n;
            _Destroy(__new_finish, this->_M_finish);
            this->_M_destroy_nodes(__new_finish._M_node + 1, this->_M_finish._M_node + 1);
            this->_M_finish = __new_finish;
        }
        return this->_M_start + __elems_before;
    }
}

template deque<C2DVector, allocator<C2DVector> >::iterator
deque<C2DVector, allocator<C2DVector> >::erase(iterator, iterator);
}