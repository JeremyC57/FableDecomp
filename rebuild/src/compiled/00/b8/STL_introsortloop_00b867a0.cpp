// std::__introsort_loop(CSortElement* first, CSortElement* last, CSortElement*, int depth_limit, less<CSortElement> comp) -- STLport introsort recursion over the sub-primitive renderer's sort elements (ordered by SortValue): median-of-three partition until 16 elements remain, heap sort when the depth budget runs out
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B867A0, 180 bytes, __fastcall (first ecx, last edx), `ret 0xc` (the _Tp* tag, the
//   depth limit, the empty less<> comparator).
// Identity: called by sort<CSortElement*> 0x00B868C0 (from CEngineSubPrimitiveRenderer::SortList 0x00B849CF) with
//   `__lg(n) * 2` and recursively by itself; Ego_r.pdb sorts CEngineSubPrimitiveRenderer::CSortElement (SortValue,
//   SubPrimitive; 8 bytes) with its operator< (the Dinkumware name set has _Med3/_Median/_Insertion_sort1 over the same
//   element). STLport's comparator-less sort passes `__less(value_type*)` = less<CSortElement>. Catalog
//   "Std_Introsort_8BytePartition" is descriptive.
// Callees: __unguarded_partition(first, last, pivot, comp) 0x00B85430 (pivot by value: two dword pushes, then comp),
//   __partial_sort(first, middle, last, _Tp*, comp) 0x00B86450 (STLport partial_sort inlined around it), itself.
// Shape: the landed __introsort_loop 0x00AD6F10 with the comparator inlined: `while (last - first > __stl_threshold)`
//   (`and eax,-8 ; cmp eax,80h`), `--depth_limit` kept in its own argument slot, __median inlined to unsigned compares
//   of SortValue and copied into the by-value pivot, `last = cut` at the end.
class CEngineSubPrimitiveBase;

class CEngineSubPrimitiveRenderer {
public:
    class CSortElement {                           // Ego_r.pdb, 8 bytes
    public:
        unsigned long            SortValue;        // +0x0
        CEngineSubPrimitiveBase* SubPrimitive;     // +0x4
        bool operator<(const CSortElement& other) const { return SortValue < other.SortValue; }
    };
};
typedef CEngineSubPrimitiveRenderer::CSortElement CSortElement;

namespace std {

template <class _Tp> struct less {
    less() { }
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

const int __stl_threshold = 16;

template <class _Tp, class _Compare>
inline const _Tp& __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c))
            return __b;
        else if (__comp(__a, __c))
            return __c;
        else
            return __a;
    else if (__comp(__a, __c))
        return __a;
    else if (__comp(__b, __c))
        return __c;
    else
        return __b;
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp);   // 0x00B85430

template <class _RandomAccessIterator, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIterator __first, _RandomAccessIterator __middle, _RandomAccessIterator __last, _Tp*, _Compare __comp);   // 0x00B86450

template <class _RandomAccessIterator, class _Compare>
inline void partial_sort(_RandomAccessIterator __first, _RandomAccessIterator __middle, _RandomAccessIterator __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, (CSortElement*)0, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp)
{
    while (__last - __first > __stl_threshold) {
        if (__depth_limit == 0) {
            partial_sort(__first, __last, __last, __comp);
            return;
        }
        --__depth_limit;
        _RandomAccessIter __cut =
            __unguarded_partition(__first, __last,
                                  _Tp(__median(*__first, *(__first + (__last - __first) / 2), *(__last - 1), __comp)),
                                  __comp);
        __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp);
        __last = __cut;
    }
}

template void __fastcall __introsort_loop<CSortElement*, CSortElement, int, less<CSortElement> >(CSortElement*, CSortElement*, CSortElement*, int, less<CSortElement>);

} // namespace std