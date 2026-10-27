// std::sort(CEngineSubPrimitiveRenderer::CSortElement* first, CEngineSubPrimitiveRenderer::CSortElement* last) -- STLport sort with std::less<CEngineSubPrimitiveRenderer::CSortElement>: introsort with depth limit 2 * floor(log2(n)), then the final insertion sort (guarded over the first 16 elements, unguarded over the rest)
// Leaf: ??$sort@PAVCSortElement@CEngineSubPrimitiveRenderer@@@std@@YIXPAVCSortElement@CEngineSubPrimitiveRenderer@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00b868c0, 120 bytes, /Gr __fastcall (first in ecx, last in edx), plain `ret`.
// Identity (proven): its __introsort_loop 0x00B867A0 and __insertion_sort 0x00B85B30 are bound to CEngineSubPrimitiveRenderer::CSortElement with less<CSortElement>; caller CEngineSubPrimitiveRenderer::SortList 0x00B84990 (families L053/L054 serve the same sort).
// Callees: __introsort_loop 0x00B867A0, __insertion_sort 0x00B85B30 (twice) and __unguarded_insertion_sort_aux 0x00B851D0
//   (all __fastcall, out of line).
// Shape (family L061): as family L050 with __final_insertion_sort inline (`cmp esi,10h ; jle`, first + 16 elements =
//   `lea esi,[edi+0x80]`), the element count kept in esi from the __lg loop; the two empty less<T> temporaries
//   share the frame slot of the `push ecx` (their undefined byte is what gets pushed; less<> needs a user-declared
//   constructor or the byte is zeroed). VC7.1 inlines __final_insertion_sort only for the long row on its own; the
//   8-byte rows need it __forceinline.
class CEngineSubPrimitiveBase;
class CEngineSubPrimitiveRenderer {
public:
    class CSortElement {                                   // Ego_r.pdb, 8 bytes
    public:
        unsigned long SortValue;                          // +0x00
        CEngineSubPrimitiveBase* SubPrimitive;            // +0x04
        bool operator<(const CSortElement& other) const { return SortValue < other.SortValue; }
    };
};
namespace std {
template <class _Tp> struct less {
    less() { }                                            // user-declared: the empty temporaries are not zero-filled
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
}
namespace std {
template <class _Size> inline _Size __lg(_Size __n)
{
    _Size __k;
    for (__k = 0; __n != 1; __n >>= 1)
        ++__k;
    return __k;
}
template <class _Tp> inline _Tp* __value_type(_Tp*) { return (_Tp*)0; }
template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit,
                                 _Compare __comp);                                          // 0x00B867A0
template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x00B85B30
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*,
                                               _Compare __comp);                            // 0x00B851D0
template <class _RandomAccessIter, class _Compare>
inline void __unguarded_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    __unguarded_insertion_sort_aux(__first, __last, __value_type(__first), __comp);
}
template <class _RandomAccessIter, class _Compare>
__forceinline void __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__last - __first > 16) {                          // __stl_threshold
        __insertion_sort(__first, __first + 16, __comp);
        __unguarded_insertion_sort(__first + 16, __last, __comp);
    }
    else
        __insertion_sort(__first, __last, __comp);
}
template <class _Tp> inline less<_Tp> __less(_Tp*) { return less<_Tp>(); }
template <class _RandomAccessIter>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last)
{
    if (__first != __last) {
        __introsort_loop(__first, __last, __value_type(__first), __lg(__last - __first) * 2, __less(__value_type(__first)));
        __final_insertion_sort(__first, __last, __less(__value_type(__first)));
    }
}
}
template void __fastcall std::sort(CEngineSubPrimitiveRenderer::CSortElement*, CEngineSubPrimitiveRenderer::CSortElement*);