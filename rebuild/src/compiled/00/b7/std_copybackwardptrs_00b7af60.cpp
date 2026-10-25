// std::__copy_backward_ptrs(first, last, result, const __false_type&) -- (CPreTransposedBoneMatrix) STLport backward copy of 48-byte records: n = last - first by the reciprocal divide, then `*--result = *--last` n times (the implicit assignment is one 12-dword `rep movsd`); returns result - n
// Leaf: ??$__copy_backward_ptrs@PAUCPreTransposedBoneMatrix@@PAU1@@std@@YI
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B7AF60, 62 bytes (+ int3 padding), __fastcall (first ecx, last edx, result and the empty tag on the stack), `ret 8`.
// Identity: element CPreTransposedBoneMatrix -- proven; evidence: its only caller 0x00B7CAB0 is the _M_fill_insert of the landed vector<CPreTransposedBoneMatrix>::resize 0x00B7EDF0 (CShaderRenderManager::BoneMatrices); Ego_r.pdb CPreTransposedBoneMatrix is 48 bytes (a struct: U in FableWin.pdb).
// Label: the manifest's "_Copy_backward<std::pair<unsigned_long,C3DDummyObject>*,...>" is propagated across byte-identical
//   bodies (an Ego_r Dinkumware spelling; no row of the family copies that pair).
// Body: STLport 4.6 _algobase.h: `__copy_backward_ptrs(first, last, result, const __false_type&)` = `__copy_backward(first,
//   last, result, random_access_iterator_tag(), (ptrdiff_t*)0)` (the loop above); `__fastcall` on the template
//   definition (the TU's /Gr convention). VC7.1 keeps __copy_backward out of line under plain `inline` (a 25-byte
//   forwarder, graded), so it is `__forceinline` here.
struct CPreTransposedBoneMatrix {                       // 48 bytes (Ego_r.pdb)
    float Data[3][4];
};
namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>   // inline in every retail body
__forceinline _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                          _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _RandomAccessIter, class _BidirectionalIter>
inline _BidirectionalIter __fastcall __copy_backward_ptrs(_RandomAccessIter __first, _RandomAccessIter __last,
                                                          _BidirectionalIter __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template CPreTransposedBoneMatrix* __fastcall __copy_backward_ptrs<CPreTransposedBoneMatrix*, CPreTransposedBoneMatrix*>(CPreTransposedBoneMatrix*, CPreTransposedBoneMatrix*, CPreTransposedBoneMatrix*, const __false_type&);
}