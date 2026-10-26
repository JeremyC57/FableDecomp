// std::__copy(CCharString* first, CCharString* last, CCharString* result, const random_access_iterator_tag&, int*) -- STLport counted forward assignment of CCharString elements; returns the advanced result
// Leaf: ??$__copy@PAVCCharString@@PAV1@H@std@@YIPAVCCharString@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0048528F, 45 bytes, __fastcall (first ecx, last edx; result, the tag reference and the distance pointer on the stack), `ret 0Ch`; size-optimised TU (result advanced in place, `add dword ptr [esp+0Ch],4`).
// Identity (probable): std::__copy<CCharString*, CCharString*, int>; evidence: the 4-byte element is assigned with CCharString::operator= 0x0099EFB0 (landed); it is the only CCharString __copy in the image (byte scan), and its only reference is an out-of-line __copy_ptrs(first, last, result, __false_type) wrapper 0x004855F1 that nothing calls (a COMDAT kept from a TU whose caller was discarded), so the iterator constness is not pinned: mutable as in the erase-reached siblings (0x004DBDE1, 0x006557FB, 0x00655828, 0x00CD2BF1).
// Label: the manifest's "_Copy_impl<CArray<CRandomAppearanceMorph::CBodyPartMesh>*, ...>" is propagated (family H060: six out-of-line __copy loops, one per element).
// Callee: CCharString::operator= 0x0099EFB0 (the element assignment).
// Shape: STLport 4.6 _algobase.h `__copy(first, last, result, random_access_iterator_tag, Distance*)` as written (the landed CWideString
//   0x00433CEC / CArray<unsigned short> 0x00F3B8A0 bodies), `__fastcall` on the template definition.
#pragma optimize("s", on)
class CCharString {                                        // 4 bytes: the shared string data pointer
public:
    void* PStringData;
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
};

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
template CCharString* __fastcall __copy<CCharString*, CCharString*, int>(CCharString*, CCharString*, CCharString*, const random_access_iterator_tag&, int*);
}