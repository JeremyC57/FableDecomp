// std::vector<std::pair<unsigned long, CSmallVector<NAnimationEvents::CEvent,32> > >::_M_range_insert<pointer>(iterator position, pointer first, pointer last, const forward_iterator_tag&) -- STLport 4.6 insert of the range [first, last) before position (in place when capacity allows, else reallocate)
// Leaf: ??$_M_range_insert@PAU?$pair@KV?$CSmallVector@VCEvent@NAnimationEvents@@$0CA@@@@std@@@?$vector@U?$pair@KV?$CSmallVector@VCEvent@NAnimationEvents@@$0CA@@@@std@@V?$allocator@U?$pair@KV?$CSmallVector@VCEvent@NAnimationEvents@@$0CA@@@@std@@@2@@std@@QAEXPAU?$pair@KV?$CSmallVector@VCEvent@NAnimationEvents@@$0CA@@@@1@00ABUforward_iterator_tag@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FC950, 476 bytes, thiscall `ret 10h`.
// Identity (proven): STLport 4.6 _vector.h `_M_range_insert(iterator, _ForwardIterator, _ForwardIterator, const forward_iterator_tag&)`
//   line for line (distance, capacity test, elems_after split with __uninitialized_copy / __copy_backward / copy, else advance /
//   two __uninitialized_copy / copy, else the reallocation: (max) length, allocate, three __uninitialized_copy, _M_clear, _M_set)
//   over the keyed animation-event element of the neighbouring _M_fill_insert 0x006FC7A0 and _M_insert_overflow 0x006FC6A0; every
//   __uninitialized_copy call (0x006FBAE0) takes the shifted _M_finish too, so _ForwardIterator is the non-const pointer. Callers:
//   the member-template insert(pos, first, last) thunk 0x006FD000 and two tail sites. The catalog label
//   "AnimEventVector_ReallocateInsert" is descriptive.
// Callees: __uninitialized_copy @ 0x006FBAE0, __copy_backward @ 0x006FB830, __copy @ 0x006FBC50 (copy's pointer path),
//   __destroy_aux @ 0x006FB8A0, malloc @ 0x00BFEA0E, free @ 0x00BFEA14.
// Shape: the tag-taking helpers are same-TU noinline __fastcall instantiations with their STLport bodies (as in 0x006FC7A0);
//   at landing the retail COMDATs win.
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                // 0x00BFEA14
void __cdecl operator delete[](void* block);              // 0x00BFEB1C
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

namespace NAnimationEvents {
class CEvent {                                            // Ego_r.pdb: 8 bytes
public:
    unsigned long Index : 30;
    unsigned long ExternallyImported : 1;
    unsigned long StopEvent : 1;
    float Time;
};
}
template <class T, int N> class CSmallVector {
public:
    T* Start;                                             // +0x00
    int Size;                                             // +0x04
    CSmallVector(const CSmallVector& other) : Start(0), Size(0) { Copy(other); }
    ~CSmallVector() { Clear(); }
    const CSmallVector& operator=(const CSmallVector& other) { Copy(other); return *this; }   // PDB ??4 returns ABV0
protected:
    void Clear()
    {
        delete[] Start;
        Start = 0;
        Size = 0;
    }
    void Copy(const CSmallVector& other);                 // 0x006FB5C0
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __p) : first(__p.first), second(__p.second) { }
    pair& operator=(const pair& __p) { first = __p.first; second = __p.second; return *this; }
};
struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

// Same-TU out-of-line instantiations (the STLport 4.6 bodies; retail 0x006FBAE0, 0x006FBB30, 0x006FB830, 0x006FBBD0).
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                                                  const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result, const random_access_iterator_tag&,
                                                                   _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                                   const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
#pragma auto_inline(on)
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}
template <class _InputIter, class _OutputIter>
inline _OutputIter copy(_InputIter __first, _InputIter __last, _OutputIter __result)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _RandomAccessIterator>
inline int distance(const _RandomAccessIterator& __first, const _RandomAccessIterator& __last)
{
    return __last - __first;
}
template <class _RandomAccessIterator, class _Distance>
inline void advance(_RandomAccessIterator& __i, _Distance __n)
{
    __i += __n;
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
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
    typedef _Tp value_type;
    typedef value_type* pointer;
    typedef value_type* iterator;
    typedef unsigned int size_type;
    typedef __false_type _TrivialAss;
    typedef __false_type _IsPODType;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    template <class _ForwardIterator>
    void _M_range_insert(iterator __position, _ForwardIterator __first, _ForwardIterator __last, const forward_iterator_tag&)
    {
        if (__first != __last) {
            size_type __n = distance(__first, __last);

            if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
                const size_type __elems_after = this->_M_finish - __position;
                pointer __old_finish = this->_M_finish;
                if (__elems_after > __n) {
                    __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, _IsPODType());
                    this->_M_finish += __n;
                    __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, _TrivialAss());
                    copy(__first, __last, __position);
                }
                else {
                    _ForwardIterator __mid = __first;
                    advance(__mid, __elems_after);
                    __uninitialized_copy(__mid, __last, this->_M_finish, _IsPODType());
                    this->_M_finish += __n - __elems_after;
                    __uninitialized_copy(__position, __old_finish, this->_M_finish, _IsPODType());
                    this->_M_finish += __elems_after;
                    copy(__first, __mid, __position);
                }
            }
            else {
                const size_type __old_size = size();
                const size_type __len = __old_size + (max)(__old_size, __n);
                pointer __new_start = this->_M_end_of_storage.allocate(__len);
                pointer __new_finish = __new_start;
                __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, _IsPODType());
                __new_finish = __uninitialized_copy(__first, __last, __new_finish, _IsPODType());
                __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, _IsPODType());
                _M_clear();
                _M_set(__new_start, __new_finish, __new_start + __len);
            }
        }
    }
};
typedef pair<unsigned long, CSmallVector<NAnimationEvents::CEvent, 32> > _KeyedEvents;
template void vector<_KeyedEvents>::_M_range_insert<_KeyedEvents*>(_KeyedEvents*, _KeyedEvents*, _KeyedEvents*, const forward_iterator_tag&);
}   // namespace std