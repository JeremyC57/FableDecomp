// std::__copy(_RandomAccessIter first, _RandomAccessIter last, _OutputIter result, const random_access_iterator_tag&, _Distance*) -- STLport random-access copy: assign last - first elements forward, return the end of the output (std::pair<float, CCharString>)
// Leaf: ??$__copy@PBU?$pair@MVCCharString@@@std@@PAU12@H@std@@YIPAU?$pair@MVCCharString@@@0@PBU10@0PAU10@ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00718d10, 61 bytes, __fastcall (first in ecx, last in edx), `ret 0Ch`.
// Identity (probable): its callers are vector<pair<float, CCharString> >::operator= 0x00719B30 (twice; the landed CSubtitledMessage::operator= 0x00719E80 applies it to this+0x24 = CSubtitledMessage::AnimationTags, Ego_r.pdb CVectorMap<float, CCharString>) and the __copy_ptrs front end 0x00719040; the operator= passes const sources, so the first iterator is const (one COMDAT may serve both); bound only as the stand-in ?CopyRange@@YIPAUCElement8b@@....
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
template std::pair<float, CCharString>* __fastcall std::__copy(const std::pair<float, CCharString>*, const std::pair<float, CCharString>*, std::pair<float, CCharString>*, const std::random_access_iterator_tag&, int*);