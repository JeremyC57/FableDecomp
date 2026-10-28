// std::vector<C2DLineF>::_M_range_insert<C2DLineF*>(iterator pos, C2DLineF* first, C2DLineF* last, const forward_iterator_tag&) -- STLport forward-range insert of 16-byte lines: in place when the spare capacity holds the range (shift the tail up, then copy the range in), otherwise allocate size + max(size, n), copy head, range and tail into it and adopt it
// Leaf: ??$_M_range_insert@PAVC2DLineF@@@?$vector@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@QAEXPAVC2DLineF@@00ABUforward_iterator_tag@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A74AC0, 376 bytes, thiscall `ret 10h` (four arguments).
// Identity (proven): the landed CArray<C2DLineF>::DeallocateUnusedMemory 0x00A74EA0 calls this row through vector<C2DLineF>'s
//   insert(pos, first, last) and binds exactly this symbol; its callees __copy_backward<C2DLineF*, C2DLineF*, int> 0x00A74560 and
//   copy<C2DLineF*, C2DLineF*> 0x00A74840 are landed C2DLineF instances.
// Template argument (proven): _ForwardIterator = C2DLineF*. The caller inserts a non-const CArray's begin()/end(), which
//   STLport 4.6's member-template insert forwards unchanged; every placement copy then is __uninitialized_copy<C2DLineF*,
//   C2DLineF*> and both range copies copy<C2DLineF*, C2DLineF*> -- retail calls exactly one target for each (0x00A13490,
//   0x00A74840). The const_iterator form would instantiate two copy helpers, and retail does not fold identical code.
// Callees: __uninitialized_copy<C2DLineF*, C2DLineF*> 0x00A13490 (every placement copy; __fastcall first/last + result + tag), __copy_backward 0x00A74560 (the tail shift), copy 0x00A74840 (the range
//   assignment), malloc 0x00BFEA0E / free 0x00BFEA14.
// Shape: STLport 4.6 vector.c `_M_range_insert(..., const forward_iterator_tag&)` verbatim (no EH); the tags are homed on
//   live argument slots because the tag-taking copy bodies are same-TU noinline bodies that never read them.
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }
class C2DVector { public: float X, Y; };
class C2DLineF {                                           // Ego_r.pdb, 16 bytes
public:
    C2DVector Start;                                       // +0x00
    C2DVector End;                                         // +0x08
};
namespace std {
struct __false_type { __false_type() { } };
struct forward_iterator_tag { forward_iterator_tag() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
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
template <class _InputIter, class _OutputIter>
__declspec(noinline) _OutputIter __fastcall copy(_InputIter __first, _InputIter __last, _OutputIter __result)
{
    for (int __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
#pragma auto_inline(on)
template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

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
    typedef const _Tp* const_iterator;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    void _M_clear()
    {
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    template <class _ForwardIterator>
    void _M_range_insert(iterator __position, _ForwardIterator __first, _ForwardIterator __last, const forward_iterator_tag&);
};
template <class _Tp, class _Alloc> template <class _ForwardIterator>
void vector<_Tp, _Alloc>::_M_range_insert(iterator __position, _ForwardIterator __first, _ForwardIterator __last, const forward_iterator_tag&)
{
    if (__first != __last) {
        size_type __n = __last - __first;
        if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            const size_type __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __false_type());
                this->_M_finish += __n;
                copy_backward(__position, __old_finish - __n, __old_finish);
                copy(__first, __last, __position);
            }
            else {
                _ForwardIterator __mid = __first;
                __mid += __elems_after;
                __uninitialized_copy(__mid, __last, this->_M_finish, __false_type());
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, __false_type());
                this->_M_finish += __elems_after;
                copy(__first, __mid, __position);
            }
        }
        else {
            const size_type __old_size = size();
            const size_type __len = __old_size + (max)(__old_size, __n);
            pointer __new_start = this->_M_end_of_storage.allocate(__len);
            pointer __new_finish = __new_start;
            __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
            __new_finish = __uninitialized_copy(__first, __last, __new_finish, __false_type());
            __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
            _M_clear();
            _M_set(__new_start, __new_finish, __new_start + __len);
        }
    }
}
}
template void std::vector<C2DLineF>::_M_range_insert<C2DLineF*>(C2DLineF*, C2DLineF*, C2DLineF*, const std::forward_iterator_tag&);