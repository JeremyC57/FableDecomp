// std::__copy_ptrs(first, last, result, const __false_type&) -- (CVertexShaderConstant) STLport forward copy of 16-byte records: n = last - first, then `*result = *first` n times front to back (the implicit assignment is four dword moves); returns the end of the destination
// Leaf: ??$__copy_ptrs@PBVCVertexShaderConstant@@PAV1@@std@@YI
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDB670, 57 bytes (+ int3 padding), __fastcall (first ecx, last edx, result and the empty tag on the stack), `ret 8`.
// Identity: element CVertexShaderConstant -- proven; evidence: its only caller 0x00BDB9F0 is the vector operator= that CVertexShaderConstantLayout initialisation 0x00BDBB70 (and the wrappers 0x00BDBB40 / 0x00BDBB60) runs on this + 0x140 (Ego_r.pdb ReservedPresets, CArray<CVertexShaderConstant>).
// Label: the manifest's "std_vector_copy_stride" is descriptive / propagated across byte-identical bodies.
// Body: STLport 4.6 _algobase.h: `__copy_ptrs(first, last, result, const __false_type&)` = `__copy(first, last, result,
//   random_access_iterator_tag(), (ptrdiff_t*)0)` (the loop above); `__fastcall` on the template definition (the
//   TU's /Gr convention). VC7.1 keeps __copy out of line under plain `inline`, so it is `__forceinline` here (as in G034).
class CVertexShaderConstant { public: float X, Y, Z, W; };
namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _OutputIter, class _Distance>   // inline in every retail body
__forceinline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
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
inline _OutputIter __fastcall __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template CVertexShaderConstant* __fastcall __copy_ptrs<const CVertexShaderConstant*, CVertexShaderConstant*>(const CVertexShaderConstant*, const CVertexShaderConstant*, CVertexShaderConstant*, const __false_type&);
}