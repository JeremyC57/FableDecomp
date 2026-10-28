// std::__copy(std::pair<C3DVector, CCharString>* first, std::pair<C3DVector, CCharString>* last, std::pair<C3DVector, CCharString>* result, const random_access_iterator_tag&, int*) -- STLport counted forward assignment of pair<C3DVector, CCharString> elements; returns the advanced result
// Leaf: ??$__copy@PAU?$pair@VC3DVector@@VCCharString@@@std@@PAU12@H@std@@YIPAU?$pair@VC3DVector@@VCCharString@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00655828, 45 bytes, __fastcall (first ecx, last edx; result, the tag reference and the distance pointer on the stack), `ret 0Ch`; size-optimised TU (result advanced in place, `add dword ptr [esp+0Ch],0x10`).
// Identity (proven): std::__copy<std::pair<C3DVector, CCharString>*, std::pair<C3DVector, CCharString>*, int>; evidence: the 16-byte element's assignment 0x00654EB6 (landed pair<C3DVector, CCharString>::operator=: a 12-byte block copy, then CCharString::operator=) belongs to the player-GUI TU, whose only Ego_r.pdb vector of that element is CDrawMiniMap::RegionExits; the live caller 0x00656837 is that vector's erase(first, last): mutable iterators.
// Label: the manifest's "_Copy_impl<CArray<CRandomAppearanceMorph::CBodyPartMesh>*, ...>" is propagated (family H060: six out-of-line __copy loops, one per element).
// Callee: pair<C3DVector, CCharString>::operator= 0x00654EB6 (the element assignment).
// Shape: STLport 4.6 _algobase.h `__copy(first, last, result, random_access_iterator_tag, Distance*)` as written (the landed CWideString
//   0x00433CEC / CArray<unsigned short> 0x00F3B8A0 bodies), `__fastcall` on the template definition.
#pragma optimize("s", on)
class CCharString {                                        // 4 bytes: the shared string data pointer
public:
    void* PStringData;
    const CCharString& operator=(const CCharString& other);       // 0x0099EFB0
};
class C3DVector {                                          // 12 bytes
public:
    float X, Y, Z;
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair& operator=(const pair& other);                     // the implicit assignment, out of line
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
template std::pair<C3DVector, CCharString>* __fastcall __copy<std::pair<C3DVector, CCharString>*, std::pair<C3DVector, CCharString>*, int>(std::pair<C3DVector, CCharString>*, std::pair<C3DVector, CCharString>*, std::pair<C3DVector, CCharString>*, const random_access_iterator_tag&, int*);
}