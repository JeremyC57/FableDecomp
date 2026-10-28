// std::vector<ENavigatorType>::_M_allocate_and_copy<const ENavigatorType*>(unsigned int n, const ENavigatorType* first, const ENavigatorType* last) -- STLport growth helper: allocate n navigator types and copy-construct [first, last) into the block, return the block
// Leaf: ??$_M_allocate_and_copy@PBW4ENavigatorType@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006715A0, 72 bytes (+ int3 padding to 80), thiscall `ret 0Ch` (`this` never read:
//   the allocator is empty). Only caller: vector<ENavigatorType>::operator= @ 0x006714B0 (call at 0x006714E2), which
//   CThingCreatureDef::operator= 0x00671160 calls for NavigatorTypes.
// Callees: malloc @ 0x00BFEA0E (allocate inline, `n != 0` test).
// Label correction: the catalog "Vector_MallocAndCopy" is descriptive; this is STLport 4.x `_M_allocate_and_copy`.
// Shape: `result = allocate(n); __uninitialized_copy(first, last, result, __false_type()); return result;` -- an enum is
//   not POD to STLport's __type_traits, so the copy is the placement loop with the null test (the cursor strength-reduced
//   to `result - first` + first). No pragma (speed TU: `add esp,4` after malloc).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }
enum ENavigatorType { NAVIGATOR_TYPE_0 };                  // 4-byte enum: STLport treats it as non-POD (__false_type)

namespace std {
struct __false_type { };
struct random_access_iterator_tag { };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp*, _Tp*) { }

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _RandomAccessIter, class _OutputIter, class _Distance>
inline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                          const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _Tp>
inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
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
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    protected:
    template <class _ForwardIter>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIter __first, _ForwardIter __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
    public:
    protected:
    void _M_clear();                                       // 0x006715F0
    public:
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);
};
}

template ENavigatorType* std::vector<ENavigatorType>::_M_allocate_and_copy<const ENavigatorType*>(unsigned int, const ENavigatorType*, const ENavigatorType*);