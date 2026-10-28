// std::__copy(_RandomAccessIter first, _RandomAccessIter last, _OutputIter result, const random_access_iterator_tag&, _Distance*) -- STLport random-access copy: assign last - first elements forward, return the end of the output (std::pair<CCharString, long>)
// Leaf: ??$__copy@PAU?$pair@VCCharString@@J@std@@PAU12@H@std@@YIPAU?$pair@VCCharString@@J@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006d3b50, 59 bytes, __fastcall (first in ecx, last in edx), `ret 0Ch`.
// Identity (proven): bound (reloc_bindings) as ??$__copy@PAU?$pair@VCCharString@@J@std@@PAU12@H@std@@YI...; its element is 8 bytes and its assignment is pair's implicit operator= -- CCharString::operator= 0x0099EFB0 on first, then the long second; callers 0x006D4509 (a landed vector erase) and 0x006D404C.
// Callee: the element's member assignment 0x0099EFB0 (thiscall).
// Shape (family L031): n = (last - first) >> 3; per element one thiscall assignment of the leading member, then the
//   trailing scalar copied through al/eax; returns the advanced result.
class CCharString {
public:
    const CCharString& operator=(const CCharString& other);      // 0x0099EFB0
private:
    void* PData;                                          // +0x00
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                            // +0x00
    _T2 second;                                           // +0x04
};
}
namespace std {
struct random_access_iterator_tag { };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
}
template std::pair<CCharString, long>* __fastcall std::__copy(std::pair<CCharString, long>*, std::pair<CCharString, long>*, std::pair<CCharString, long>*, const std::random_access_iterator_tag&, int*);