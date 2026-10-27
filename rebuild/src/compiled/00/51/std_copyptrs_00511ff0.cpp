// std::__copy_ptrs(const C2DLineF* first, const C2DLineF* last, C2DLineF* result, const __false_type&) -- STLport forward copy of 16-byte records: n = last - first, then `*result = *first` n times front to back; returns the end of the destination
// Leaf: ??$__copy_ptrs@PBVC2DLineF@@PAV1@@std@@YIPAVC2DLineF@@PBV1@0PAV1@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00511FF0, 57 bytes, `__fastcall` (first ecx, last edx, result and the empty tag on the
//   stack), `ret 8`.
// Identity (proven): called (twice) only by the landed std::vector<C2DLineF>::operator= 0x00515320 (the const source range); Ego_r.pdb C2DLineF is {C2DVector Point1, Point2}, 16 bytes (the `sar 4`).
// Shape (family M106, representative 0x00511FF0): the landed 0x006E1C10 model -- STLport 4.6 `__copy_ptrs(first, last,
//   result, const __false_type&)` = `__copy(first, last, result, random_access_iterator_tag(), (ptrdiff_t*)0)` with __copy
//   `__forceinline` (VC7.1 keeps it out of line under plain `inline`); the implicit assignment is four dword moves.
class C2DVector { public: float X, Y; };
class C2DLineF {                                         // Ego_r.pdb, 16 bytes
public:
    C2DVector Point1;                                     // +0x0
    C2DVector Point2;                                     // +0x8
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
template ::C2DLineF* __fastcall __copy_ptrs<const ::C2DLineF*, ::C2DLineF*>(const ::C2DLineF*, const ::C2DLineF*, ::C2DLineF*, const __false_type&);
}