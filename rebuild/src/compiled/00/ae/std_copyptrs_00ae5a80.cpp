// std::__copy_ptrs(const C3DVertexBlend2* first, const C3DVertexBlend2* last, C3DVertexBlend2* result, const __false_type&) -- STLport copy over non-trivially-assigned pointers: assign last - first elements forward (the random-access __copy inlined), return the end of the output
// Leaf: ??$__copy_ptrs@PBVC3DVertexBlend2@@PAV1@@std@@YIPAVC3DVertexBlend2@@PBV1@0PAV1@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ae5a80, 38 bytes, __fastcall (first in ecx, last in edx, result + tag on the stack), `ret 8`.
// Identity (probable): both calls come from vector<C3DVertexBlend2>::operator= 0x00AE6690 (family L046, bound ??4?$vector@VC3DVertexBlend2@@...); 8-byte stride = sizeof(C3DVertexBlend2).
// Callees: none.
// Shape (family L045): n = (last - first) >> 3; the element's trivial assignment is a two-dword block copy through esi
//   (MSVC copies trivially-assignable classes bitwise, padding included); the source walks as result + (first - result).
class C3DVertexBlend2 {                                    // Ego_r.pdb, 8 bytes
public:
    unsigned long VertexIndex;                            // +0x00
    float BlendValue;                                     // +0x04
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
_OutputIter __fastcall __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
}
template C3DVertexBlend2* __fastcall std::__copy_ptrs(const C3DVertexBlend2*, const C3DVertexBlend2*, C3DVertexBlend2*, const std::__false_type&);