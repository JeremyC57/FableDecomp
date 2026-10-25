// std::__copy_backward_ptrs(first, last, result, const __false_type&) -- (QuadricOptimiserInternals::COptimisedFace) STLport backward copy of 72-byte records: n = last - first by the reciprocal divide, then `*--result = *--last` n times (the implicit assignment is one 18-dword `rep movsd`); returns result - n
// Leaf: ??$__copy_backward_ptrs@PAVCOptimisedFace@QuadricOptimiserInternals@@PAV12@@std@@YI
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD19C0, 62 bytes (+ int3 padding), __fastcall (first ecx, last edx, result and the empty tag on the stack), `ret 8`.
// Identity: element QuadricOptimiserInternals::COptimisedFace -- proven; evidence: its only caller is the landed _M_fill_insert 0x00AD54F0 of vector<COptimisedFace>::resize 0x00AD6860 (the landed one-argument resize 0x00AD7350 names COptimisedFace, Ego_r.pdb / FableWin.pdb); Ego_r.pdb COptimisedFace is 72 bytes.
// Label: the manifest's "_Copy_backward<std::pair<unsigned_long,C3DDummyObject>*,...>" is propagated across byte-identical
//   bodies (an Ego_r Dinkumware spelling; no row of the family copies that pair).
// Body: STLport 4.6 _algobase.h: `__copy_backward_ptrs(first, last, result, const __false_type&)` = `__copy_backward(first,
//   last, result, random_access_iterator_tag(), (ptrdiff_t*)0)` (the loop above); `__fastcall` on the template
//   definition (the TU's /Gr convention). VC7.1 keeps __copy_backward out of line under plain `inline` (a 25-byte
//   forwarder, graded), so it is `__forceinline` here.
class C3DVector { public: float X, Y, Z; };
namespace QuadricOptimiserInternals {
class COptimisedFace {                                  // 72 bytes (Ego_r.pdb)
public:
    unsigned long VertexIndices[3];
    unsigned long UniqueVertexIndices[3];
    long Edges[3];
    C3DVector UTangent;
    C3DVector VTangent;
    bool Exists;
    unsigned long OutputIndex;
    long Segment;
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

template QuadricOptimiserInternals::COptimisedFace* __fastcall __copy_backward_ptrs<QuadricOptimiserInternals::COptimisedFace*, QuadricOptimiserInternals::COptimisedFace*>(QuadricOptimiserInternals::COptimisedFace*, QuadricOptimiserInternals::COptimisedFace*, QuadricOptimiserInternals::COptimisedFace*, const __false_type&);
}