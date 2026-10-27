// std::__copy_ptrs(const C2DBoxF* first, const C2DBoxF* last, C2DBoxF* result, const __false_type&) -- STLport forward copy of 16-byte records: n = last - first, then `*result = *first` n times front to back; returns the end of the destination
// Leaf: ??$__copy_ptrs@PBVC2DBoxF@@PAV1@@std@@YIPAVC2DBoxF@@PBV1@0PAV1@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A7EA30, 57 bytes, `__fastcall` (first ecx, last edx, result and the empty tag on the
//   stack), `ret 8`.
// Identity (probable): called (twice) only by 0x00A81790, which CNavQuadTree::Initialise(const CNavigationAreaInit&) 0x00A7A8D0 calls for the member at +0x5C: with STLport's 12-byte vectors CNavQuadTree's Ego_r.pdb members land NavigationLayers +0x1C, TransitionPoints +0x28, TopologyWeights +0x34, SwitchableNodes +0x40, ActionPoints +0x50, DetailedAreas +0x5C, RegionSeeds +0x68 -- ActionPoints and RegionSeeds (both vector<CNavigationPosition>) share the assignment 0x00A80860 there, so +0x5C is DetailedAreas, vector<C2DBoxF> (Ego_r.pdb C2DBoxF {float TLX, TLY, BRX, BRY}, 16 bytes); 0x00A81790 is its operator=.
// Shape (family M106, representative 0x00511FF0): the landed 0x006E1C10 model -- STLport 4.6 `__copy_ptrs(first, last,
//   result, const __false_type&)` = `__copy(first, last, result, random_access_iterator_tag(), (ptrdiff_t*)0)` with __copy
//   `__forceinline` (VC7.1 keeps it out of line under plain `inline`); the implicit assignment is four dword moves.
class C2DBoxF {                                          // Ego_r.pdb, 16 bytes
public:
    float TLX, TLY, BRX, BRY;
};
namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
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
template ::C2DBoxF* __fastcall __copy_ptrs<const ::C2DBoxF*, ::C2DBoxF*>(const ::C2DBoxF*, const ::C2DBoxF*, ::C2DBoxF*, const __false_type&);
}