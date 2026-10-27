// std::vector::erase(CReverbEnvironmentEntry* __first, CReverbEnvironmentEntry* __last) -- (vector<CReverbEnvironmentEntry>) close the gap by assignment, drop the tail, return first
// Leaf: ?erase@?$vector@VCReverbEnvironmentEntry@@V?$allocator@VCReverbEnvironmentEntry@@@std@@@std@@QAEPAVCReverbEnvironmentEntry@@PAV3@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046BFE0, 66 bytes, thiscall, `ret 8` (size-optimised module, frame pointer).
// Identity (proven): its only caller is the resize 0x0046BF98 of vector<CReverbEnvironmentEntry> (whose _M_fill_insert 0x0046C022 is landed); 84-byte stride.
// Shape (family M054, representative 0x0046AD17): STLport `pointer __i = __copy_ptrs(__last, _M_finish, __first, __false_type());
//   _Destroy(__i, _M_finish); _M_finish = __i; return __first;` -- __copy's `for (n = last - first; n > 0; --n)` record
//   assignment loop inline (`__forceinline`); trivially destructible records leave _Destroy empty.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CReverbEnvironmentEntry { public: float Params[21]; };   // Ego_r.pdb, 84 bytes
namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__forceinline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _InputIter, class _OutputIter>
__forceinline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _Tp> __forceinline void _Destroy(_Tp*, _Tp*) { }
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    _Tp* erase(_Tp* __first, _Tp* __last)
    {
        _Tp* __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};
template class vector<CReverbEnvironmentEntry>;
}