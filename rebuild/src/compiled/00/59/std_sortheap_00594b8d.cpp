// std::sort_heap(std::pair<long, CDefPointer<const CContextSensitiveItemDef> >* first, std::pair<long, CDefPointer<const CContextSensitiveItemDef> >* last, CContextSensitivePriorityCompare comp) -- STLport sort_heap: pop the root to the back until one element remains
// Leaf: ??$sort_heap@PAU?$pair@JV?$CDefPointer@$$CBVCContextSensitiveItemDef@@@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00594B8D, 60 bytes, /Gr __fastcall (first ecx, last edx, the empty comparator on the stack), `ret 4`.
// Identity (probable): the stand-in binding ?sort_heap@std@@YIXPAU?$pair@JV?$CDefPointer@$$CBVCContextSensitiveItemDef@@@@@1@0VCContextSensitivePriorityCompare@@@Z names the element and the comparator (8-byte stride); caller the partial_sort 0x00594C77 (landed, CContextSensitivePriorityCompare).
// Callee: __pop_heap_aux(first, last, value_type*, comp) 0x0059474E (__fastcall, out of line).
// Shape (family K122, representative 0x004E41DD): STLport 4.6 `while (__last - __first > 1) pop_heap(__first, __last--, __comp);`
//   with pop_heap inline and __pop_heap_aux out of line (the K088 body with the other inline split; the distance test folds to
//   `and ~(size-1) ; cmp size`); size TU (the comparator pushed from its argument slot each pass).
#pragma optimize("s", on)
class CContextSensitiveItemDef;
template <class T> class CDefPointer { public: void* Object; };
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
class CContextSensitivePriorityCompare {
public:
    bool operator()(const std::pair<long, CDefPointer<const CContextSensitiveItemDef> >& a,
                    const std::pair<long, CDefPointer<const CContextSensitiveItemDef> >& b) const;
};
namespace std {
template <class _RandomAccessIterator, class _Tp, class _Compare>
void __fastcall __pop_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Tp*, _Compare __comp);   // 0x0059474E
template <class _Tp> __forceinline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _RandomAccessIterator, class _Compare>
__forceinline void pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __pop_heap_aux(__first, __last, __value_type(__first), __comp);
}
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    while (__last - __first > 1)
        pop_heap(__first, __last--, __comp);
}
template void __fastcall sort_heap<std::pair<long, CDefPointer<const CContextSensitiveItemDef> >*, CContextSensitivePriorityCompare >(std::pair<long, CDefPointer<const CContextSensitiveItemDef> >*, std::pair<long, CDefPointer<const CContextSensitiveItemDef> >*, CContextSensitivePriorityCompare);
} // namespace std