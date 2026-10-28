// std::vector::_M_range_insert(iterator position, ForwardIterator first, ForwardIterator last, const forward_iterator_tag&) -- (vector<pair<unsigned long, long> >, ForwardIterator = pair*) STLport 4.6 range insert: shift the tail inside the spare capacity, else reallocate to size + max(size, n) and copy head, range and tail
// Leaf: ??$_M_range_insert@PAU?$pair@KJ@std@@@?$vector@U?$pair@KJ@std@@V?$allocator@U?$pair@KJ@std@@@2@@std@@QAEXPAU?$pair@KJ@1@00ABUforward_iterator_tag@1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A39610, 389 bytes, thiscall `ret 10h` (position, first, last, tag).
// Identity (proven): STLport 4.6 _vector.h member template vector<_Tp>::_M_range_insert(iterator, _ForwardIterator,
//   _ForwardIterator, const forward_iterator_tag&) (public QAE). Element pair<unsigned long, long> (8 bytes,
//   CCRCSymbolMap::SLONGMap's CArray data): its callers are the landed CArray<pair<unsigned long, long> >::
//   DeallocateMemory 0x00A39950 / DeallocateUnusedMemory 0x00A39A00 (`temp.insert(temp.begin(), begin(), end())`).
//   _ForwardIterator is the NON-const pair*: every __uninitialized_copy here -- the head and tail (pair*) and the
//   inserted range alike -- calls the single COMDAT 0x009B25C0, bound
//   `__uninitialized_copy<pair<unsigned long, long>*, pair<unsigned long, long>*>`; a const_pointer range would be a
//   second instantiation. The landed 0x00A39A00 binds this row as the PBU (const) spelling: NAME_FIXES.tsv.
//   Catalog label "Vector_Insert_Pair_Float_Attitude" is propagated.
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14, __uninitialized_copy<pair*, pair*>(.., const __false_type&) 0x009B25C0,
//   __copy_backward_ptrs<pair*, pair*>(.., const __false_type&) 0x009B3100, copy<pair*, pair*> 0x00A39460.
// Shape: STLport 4.6 text verbatim (distance / advance inline, _M_clear's trivial destroy loop vanishes). The three
//   helpers get same-TU noinline STAND-IN bodies (only their calls matter; the copy helpers are simplified loops):
//   declared only, VC7.1 places n, elems_after and the tags in other frame slots (389v389, slot numbers only).
//   At landing the retail COMDATs win.
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

namespace std {
struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
__forceinline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) { __destroy_aux(__first, __last, __false_type()); }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                   // 0x009B25C0
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _InputIter, class _OutputIter>
__declspec(noinline) _OutputIter __fastcall __copy_backward_ptrs(_InputIter __first, _InputIter __last,
                                                                 _OutputIter __result, const __false_type&)
{                                                                                   // 0x009B3100
    for (int __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _InputIter, class _OutputIter>
__declspec(noinline) _OutputIter __fastcall copy(_InputIter __first, _InputIter __last, _OutputIter __result)
{                                                                                   // 0x00A39460
    for (int __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
#pragma auto_inline(on)

template <class _RandomAccessIterator>
inline int distance(const _RandomAccessIterator& __first, const _RandomAccessIterator& __last)
{
    return __last - __first;
}
template <class _RandomAccessIterator, class _Distance>
inline void advance(_RandomAccessIterator& __i, _Distance __n) { __i += __n; }

template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    typedef __false_type _TrivialAss;
    typedef __false_type _IsPODType;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
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
public:
    template <class _ForwardIterator>
    void _M_range_insert(iterator __position,
                         _ForwardIterator __first,
                         _ForwardIterator __last,
                         const forward_iterator_tag &)
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
} // namespace std

template void std::vector<std::pair<unsigned long, long> >::_M_range_insert(std::pair<unsigned long, long>*,
    std::pair<unsigned long, long>*, std::pair<unsigned long, long>*, const std::forward_iterator_tag&);