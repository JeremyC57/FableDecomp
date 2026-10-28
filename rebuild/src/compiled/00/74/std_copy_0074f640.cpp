// std::__copy(_RandomAccessIter first, _RandomAccessIter last, _OutputIter result, const random_access_iterator_tag&, _Distance*) -- STLport random-access copy: assign last - first elements forward, return the end of the output (std::pair<long, CCharString>)
// Leaf: ??$__copy@PAU?$pair@JVCCharString@@@std@@PAU12@H@std@@YIPAU?$pair@JVCCharString@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0074f640, 61 bytes, __fastcall (first in ecx, last in edx), `ret 0Ch`.
// Identity (proven): bound (reloc_bindings) ??$__copy@PAU?$pair@JVCCharString@@@std@@PAU12@H@std@@YIPAU?$pair@JVCCharString@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z; callers the landed vector<pair<long, CCharString> >::erase 0x0074FB20, CTCContainer::RemoveItem 0x008ED040 and CTCContainer::OnSerialise 0x008ED2B0.
// Callee: the second member's operator= 0x0099EFB0 (thiscall).
// Shape (family L049): n = (last - first) >> 3; per element the leading dword copied through eax, then one thiscall
//   assignment of the member at +4; returns the advanced result.
class CCharString {
public:
    const CCharString& operator=(const CCharString& other);      // 0x0099EFB0
private:
    void* PStringData;                                    // +0x00
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
template std::pair<long, CCharString>* __fastcall std::__copy(std::pair<long, CCharString>*, std::pair<long, CCharString>*, std::pair<long, CCharString>*, const std::random_access_iterator_tag&, int*);