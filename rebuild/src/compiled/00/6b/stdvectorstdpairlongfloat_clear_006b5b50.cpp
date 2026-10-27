// std::vector<std::pair<long, float>>::clear() -- STLport clear: erase(begin(), end()) -- assign the (empty) tail down, destroy nothing, set the end to begin
// Leaf: ?clear@?$vector@U?$pair@JM@std@@V?$allocator@U?$pair@JM@std@@@2@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006b5b50, 43 bytes, thiscall, plain `ret`.
// Identity (probable): its only caller CEnvironment::SetCameraThemes 0x006B3E60 walks the global vector at 0x013BA8EC (8-byte elements: the long theme index at +0 and the float weight at +4 it passes to NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::SetThemeWeight(long, float, bool) 0x00886D70) and then clears it; the landed vector<pair<long, float> > helpers 0x006B5A50 / 0x006B5ED0 serve the same global.
// Callees: none.
// Shape (family L040): erase(first, last) inline with __copy_ptrs -> __copy over [last, finish) = [finish, finish):
//   VC7.1 keeps the two loads and the dead 8-byte copy loop, then stores the new finish.
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                            // +0x00
    _T2 second;                                           // +0x04
};
}
namespace std {
struct __false_type { };
struct random_access_iterator_tag { };
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
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {                                             // STLport 4.6, 12 bytes
public:
    typedef _Tp* iterator;
    _Tp* _M_start;                                        // +0x00
    _Tp* _M_finish;                                       // +0x04
    _Tp* _M_end_of_storage;                               // +0x08
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
};
}
template void std::vector<std::pair<long, float> >::clear();