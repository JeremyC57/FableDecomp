// std::copy(first, last, result) -- (CPlane) STLport forward copy of 16-byte records: n = last - first, then `*result = *first` n times front to back (the implicit assignment is four dword moves); returns the end of the destination
// Leaf: ??$copy@PBVCPlane@@PAV1@@std@@YI
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A759E0, 57 bytes (+ int3 padding), __fastcall (first ecx, last edx, result on the stack), `ret 4`.
// Identity: element CPlane -- proven; evidence: the landed CContainmentVolume::Initialise 0x00A755C0 copies its const CPlane* argument into ContainmentPlanes with it (its header: std::copy<const CPlane*, CPlane*>, `ret 4`); the landed binding names it ?CopyPlanes@@YI... (invented).
// Label: the manifest's "Vector_CopyRange_16ByteElements" is descriptive / propagated across byte-identical bodies.
// Body: STLport 4.6 _algobase.h: `__copy_ptrs(first, last, result, const __false_type&)` = `__copy(first, last, result,
//   random_access_iterator_tag(), (ptrdiff_t*)0)` (the loop above), reached through STLport's copy -> __copy_aux -> __copy_ptrs dispatch, all inlined into this out-of-line copy; `__fastcall` on the template definition (the
//   TU's /Gr convention). VC7.1 keeps __copy out of line under plain `inline`, so it is `__forceinline` here (as in G034).
class C3DVector { public: float X, Y, Z; };
class CPlane { public: C3DVector Normal; float Distance; };
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

template <class _InputIter, class _OutputIter>                    // the record is not trivially assignable to STLport
inline _OutputIter __fastcall copy(_InputIter __first, _InputIter __last, _OutputIter __result)
{
    return __copy_ptrs(__first, __last, __result, __false_type());
}

template CPlane* __fastcall copy<const CPlane*, CPlane*>(const CPlane*, const CPlane*, CPlane*);
}