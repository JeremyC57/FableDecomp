// std::vector<ENavigatorType>::erase(ENavigatorType* first, ENavigatorType* last) -- STLport range erase: copy the tail down over [first, last), drop the vacated end, return first
// Leaf: ?erase@?$vector@W4ENavigatorType@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00672070, 52 bytes (+ int3 padding to 64), thiscall `ret 8`. Only caller: 0x00671793.
//   Context: CThingCreatureDef::Transfer (the 0x0067175D..0x00671793 run) sorts the def's NavigatorTypes vector and
//   erases the duplicates: sort 0x006720B0, adjacent_find 0x00671BB0, __unique_copy 0x00671BE0 (landed), erase 0x00672070.
// Label correction: the catalog "Vector_Erase" is descriptive.
// Shape: STLport verbatim `i = __copy_ptrs(last, finish, first, __false_type()); _Destroy(i, finish); finish = i; return
//   first;` -- the counted random-access __copy (a non-POD enum), the destination cursor in edx, the source as
//   `edi - first` + cursor. No pragma.
enum ENavigatorType { NAVIGATOR_TYPE_0 };                  // 4-byte enum (signed compares)
namespace std {
struct __false_type { };
struct random_access_iterator_tag { };
template <class _Tp> inline void _Destroy(_Tp*, _Tp*) { }
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
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* iterator;
    _Tp* _M_start;                              // +0x00
    _Tp* _M_finish;                             // +0x04
    _Tp* _M_end_of_storage;                     // +0x08
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};
template class vector<ENavigatorType>;
}