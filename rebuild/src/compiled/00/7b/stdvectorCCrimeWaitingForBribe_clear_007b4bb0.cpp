// std::vector<CCrimeWaitingForBribe>::clear() -- STLport clear: erase(begin(), end()) -- assign the (empty) tail down, destroy nothing, set the end to begin
// Leaf: ?clear@?$vector@VCCrimeWaitingForBribe@@V?$allocator@VCCrimeWaitingForBribe@@@std@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007b4bb0, 43 bytes, thiscall, plain `ret`.
// Identity (proven): Ego_r.pdb ?clear@?$vector@VCCrimeWaitingForBribe@@...@QAEXXZ; the landed CPersistContext::TransferVector<CCrimeWaitingForBribe> 0x007B5470 calls it in its reset arm.
// Callees: none.
// Shape (family L040): erase(first, last) inline with __copy_ptrs -> __copy over [last, finish) = [finish, finish):
//   VC7.1 keeps the two loads and the dead 8-byte copy loop, then stores the new finish.
enum EOpinionDeedType { };
class CCrimeWaitingForBribe {                              // Ego_r.pdb, 8 bytes
public:
    long DeedHandle;                                      // +0x00
    EOpinionDeedType Type;                                // +0x04
    CCrimeWaitingForBribe() : DeedHandle(0), Type(EOpinionDeedType(0)) { }
};
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
template void std::vector<CCrimeWaitingForBribe >::clear();