// std::__copy_ptrs(first, last, result, const __false_type&) -- (CNavigationTransitionPoint) STLport forward copy of 16-byte records: n = last - first, then `*result = *first` n times front to back (the implicit assignment is four dword moves); returns the end of the destination
// Leaf: ??$__copy_ptrs@PBVCNavigationTransitionPoint@@PAV1@@std@@YI
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A7E930, 57 bytes (+ int3 padding), __fastcall (first ecx, last edx, result and the empty tag on the stack), `ret 8`.
// Identity: element CNavigationTransitionPoint -- proven; evidence: its only caller 0x00A81690 is the vector operator= that CNavQuadTree::Initialise 0x00A7A8D0 runs on this + 0x28 (Ego_r.pdb TransitionPoints, vector<CNavigationTransitionPoint> after the 12-byte retail NavigationLayers vector at +0x1C); CNavigationTransitionPoint is 16 bytes.
// Label: the manifest's "Vector_CopyRange_16ByteElements_v2" is descriptive / propagated across byte-identical bodies.
// Body: STLport 4.6 _algobase.h: `__copy_ptrs(first, last, result, const __false_type&)` = `__copy(first, last, result,
//   random_access_iterator_tag(), (ptrdiff_t*)0)` (the loop above); `__fastcall` on the template definition (the
//   TU's /Gr convention). VC7.1 keeps __copy out of line under plain `inline`, so it is `__forceinline` here (as in G034).
class C2DVector { public: float X, Y; };
class CNavigationTransitionPoint { public: C2DVector Position; long Layer1; long Layer2; };
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

template CNavigationTransitionPoint* __fastcall __copy_ptrs<const CNavigationTransitionPoint*, CNavigationTransitionPoint*>(const CNavigationTransitionPoint*, const CNavigationTransitionPoint*, CNavigationTransitionPoint*, const __false_type&);
}