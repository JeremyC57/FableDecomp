// std::__copy_backward_ptrs(first, last, result, const __false_type&) -- (N3DAnimationBuilder::CSequenceInfo) STLport backward copy of 52-byte records: n = last - first by the reciprocal divide, then `*--result = *--last` n times (the implicit assignment is one 13-dword `rep movsd`); returns result - n
// Leaf: ??$__copy_backward_ptrs@PAVCSequenceInfo@N3DAnimationBuilder@@PAV12@@std@@YI
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB13D0, 62 bytes (+ int3 padding), __fastcall (first ecx, last edx, result and the empty tag on the stack), `ret 8`.
// Identity: element N3DAnimationBuilder::CSequenceInfo -- proven; evidence: its only caller 0x00AB2E40 is the _M_fill_insert of the resize 0x00AB3290 that C3DAnimationBuilder::BuildMeshAnimation 0x00AB2520 runs on its COutputAnimation& argument's Sequences (+0, Ego_r.pdb CArray<N3DAnimationBuilder::CSequenceInfo>, 52-byte elements; GeneratedBoneCount follows at the retail +0xC).
// Label: the manifest's "_Copy_backward<std::pair<unsigned_long,C3DDummyObject>*,...>" is propagated across byte-identical
//   bodies (an Ego_r Dinkumware spelling; no row of the family copies that pair).
// Body: STLport 4.6 _algobase.h: `__copy_backward_ptrs(first, last, result, const __false_type&)` = `__copy_backward(first,
//   last, result, random_access_iterator_tag(), (ptrdiff_t*)0)` (the loop above); `__fastcall` on the template
//   definition (the TU's /Gr convention). VC7.1 keeps __copy_backward out of line under plain `inline` (a 25-byte
//   forwarder, graded), so it is `__forceinline` here.
class CMatrix3x4 { public: float M[12]; };
namespace N3DAnimationBuilder {
class CSequenceInfo {                                   // 52 bytes (Ego_r.pdb)
public:
    long OriginalIndex;
    CMatrix3x4 GeneratedTransform;
};
}
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

template N3DAnimationBuilder::CSequenceInfo* __fastcall __copy_backward_ptrs<N3DAnimationBuilder::CSequenceInfo*, N3DAnimationBuilder::CSequenceInfo*>(N3DAnimationBuilder::CSequenceInfo*, N3DAnimationBuilder::CSequenceInfo*, N3DAnimationBuilder::CSequenceInfo*, const __false_type&);
}