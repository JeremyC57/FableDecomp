// std::vector<C3DVector>::_M_range_insert<C3DVector*>(C3DVector* pos, C3DVector* first, C3DVector* last, const forward_iterator_tag&) -- STLport vector insert(pos, first, last) of 12-byte vectors: shift the tail in place when capacity allows, else reallocate to size + max(size, n) and copy prefix, range and suffix
// Leaf: ??$_M_range_insert@PAVC3DVector@@@?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@QAEXPAVC3DVector@@00ABUforward_iterator_tag@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A70CB0, 461 bytes, thiscall `ret 10h` (pos, first, last, the tag reference).
// Identity: the one caller is the landed CArray::DeallocateUnusedMemory 0x00A725D0, called from
//   CPhysicsMesh::GetMeshDataFromFileData at 0x00A6CFEB on this+0x3C = CPhysicsMesh::TotalPoints (Ego_r.pdb
//   CArray<C3DVector>; retail offset with STLport's 0x10 CVectorMap and 0xC CArray); 12-byte stride (`0x2AAAAAAB ; sar 1`);
//   the copy callee 0x0074CA20 is __copy<C3DVector*,C3DVector*,int>. Template argument (proven): _ForwardIterator =
//   C3DVector*. Retail calls one __uninitialized_copy target (0x0066DDF0) from all five placement copies and one __copy
//   target (0x0074CA20) from both range copies; STLport 4.6 produces that only when the range iterator is the vector's
//   own pointer (the caller inserts a non-const CArray's begin()/end()), and retail does not fold identical code. Catalog label
//   "Vector_InsertRange_PreallocatedElements" is descriptive.
// Callees (COMDATs the linker kept from other TUs of the same instantiation; __fastcall): __uninitialized_copy(.., const
//   __false_type&) 0x0066DDF0 (x5), __copy_backward(.., const random_access_iterator_tag&, int*) 0x0075B5B0,
//   __copy(.., const random_access_iterator_tag&, int*) 0x0074CA20 (x2), malloc 0x00BFEA0E, free 0x00BFEA14.
// Shape: the landed 0x009B5310 STLport 4.6 member-template `_M_range_insert` verbatim; C3DVector has the memberwise user
//   copy constructor FableWin.pdb names (so __false_type traits) and a trivial destructor (the _Destroy loop vanishes).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);            // 0x00BFEA14

inline void* operator new(unsigned int, void* p) { return p; }

class C3DVector {                                     // 12 bytes (Ego_r.pdb X, Y, Z)
public:
    float X;                                          // +0x00
    float Y;                                          // +0x04
    float Z;                                          // +0x08
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }   // FableWin.pdb ??0C3DVector@@QAE@ABV0@@Z
};

namespace std {
struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                 // 0x0066DDF0
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)
{                                                                                 // 0x0075B5B0
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _RandomAccessIter, class _OutputIter, class _Distance>
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                                                   _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{                                                                                 // 0x0074CA20
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
#pragma auto_inline(on)

template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _InputIter, class _OutputIter>
inline _OutputIter copy(_InputIter __first, _InputIter __last, _OutputIter __result)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _RandomAccessIterator, class _Distance>
inline void distance(const _RandomAccessIterator& __first, const _RandomAccessIterator& __last, _Distance& __n)
{
    __n += __last - __first;
}

template <class _RandomAccessIterator, class _Distance>
inline void advance(_RandomAccessIterator& __i, _Distance __n)
{
    __i += __n;
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef __false_type _IsPODType;
    typedef __false_type _TrivialAss;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }

    template <class _ForwardIterator>
    void _M_range_insert(iterator __position, _ForwardIterator __first, _ForwardIterator __last,
                         const forward_iterator_tag&)
    {
        if (__first != __last) {
            size_type __n = 0;
            distance(__first, __last, __n);

            if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
                const size_type __elems_after = this->_M_finish - __position;
                pointer __old_finish = this->_M_finish;
                if (__elems_after > __n) {
                    __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, _IsPODType());
                    this->_M_finish += __n;
                    __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, _TrivialAss());
                    copy(__first, __last, __position);
                } else {
                    _ForwardIterator __mid = __first;
                    advance(__mid, __elems_after);
                    __uninitialized_copy(__mid, __last, this->_M_finish, _IsPODType());
                    this->_M_finish += __n - __elems_after;
                    __uninitialized_copy(__position, __old_finish, this->_M_finish, _IsPODType());
                    this->_M_finish += __elems_after;
                    copy(__first, __mid, __position);
                }
            } else {
                const size_type __old_size = size();
                const size_type __len = __old_size + (max)(__old_size, __n);
                pointer __new_start = this->_M_end_of_storage.allocate(__len);
                pointer __new_finish = __new_start;
                __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, _IsPODType());
                __new_finish = __uninitialized_copy(__first, __last, __new_finish, _IsPODType());
                __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, _IsPODType());
                _Destroy(this->_M_start, this->_M_finish);
                this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
                this->_M_start = __new_start;
                this->_M_finish = __new_finish;
                this->_M_end_of_storage._M_data = __new_start + __len;
            }
        }
    }
};

template void vector<C3DVector>::_M_range_insert<C3DVector*>(C3DVector*, C3DVector*, C3DVector*,
                                                             const forward_iterator_tag&);
}