// std::__copy_ptrs(const _D3DVERTEXELEMENT9* first, const _D3DVERTEXELEMENT9* last, _D3DVERTEXELEMENT9* result, const __false_type&) -- STLport copy over non-trivially-assigned pointers: assign last - first elements forward (the random-access __copy inlined), return the end of the output
// Leaf: ??$__copy_ptrs@PBU_D3DVERTEXELEMENT9@@PAU1@@std@@YIPAU_D3DVERTEXELEMENT9@@PBU1@0PAU1@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a5de10, 38 bytes, __fastcall (first in ecx, last in edx, result + tag on the stack), `ret 8`.
// Identity (proven): both calls come from vector<_D3DVERTEXELEMENT9>::operator= 0x00A5E2E0 (family L046, CVertexShaderInput::D3DDeclaration); 8-byte stride = sizeof(_D3DVERTEXELEMENT9).
// Callees: none.
// Shape (family L045): n = (last - first) >> 3; the element's trivial assignment is a two-dword block copy through esi
//   (MSVC copies trivially-assignable classes bitwise, padding included); the source walks as result + (first - result).
struct _D3DVERTEXELEMENT9 {                                // d3d9types.h / Ego_r.pdb, 8 bytes
    unsigned short Stream;                                // +0x00
    unsigned short Offset;                                // +0x02
    unsigned char Type;                                   // +0x04
    unsigned char Method;                                 // +0x05
    unsigned char Usage;                                  // +0x06
    unsigned char UsageIndex;                             // +0x07
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
template _D3DVERTEXELEMENT9* __fastcall std::__copy_ptrs(const _D3DVERTEXELEMENT9*, const _D3DVERTEXELEMENT9*, _D3DVERTEXELEMENT9*, const std::__false_type&);