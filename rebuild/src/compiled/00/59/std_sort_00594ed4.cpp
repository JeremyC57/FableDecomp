// std::sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp) -- (pair<long, CDefPointer<const CContextSensitiveItemDef> >*, CContextSensitivePriorityCompare) STLport introsort: for a non-empty range run __introsort_loop to depth 2*lg(n), then __final_insertion_sort
// Flags: /Gr
// Leaf: ??$sort@PAU?$pair@JV?$CDefPointer@$$CBVCContextSensitiveItemDef@@@@@std@@VCContextSensitivePriorityCompare@@@std@@YIXPAU?$pair@JV?$CDefPointer@$$CBVCContextSensitiveItemDef@@@@@0@0VCContextSensitivePriorityCompare@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00594ED4, 66 bytes, __fastcall (/Gr TU: first ecx, last edx, the comparator by value on the stack), `ret 4`.
// Identity (proven): std::sort<std::pair<long, CDefPointer<const CContextSensitiveItemDef> >*, CContextSensitivePriorityCompare >; evidence: its only caller is CTCInventory::UpdateContextSensitiveItems 0x00591B7F (site 0x00591D35), sorting its local vector [ebp-0x24, ebp-0x20) of 8-byte pairs with an empty comparator (the uninitialised frame word [ebp-0x10] pushed by value); the introsort loop 0x00594E4D reaches __unguarded_partition 0x00594637, which calls the comparator 0x005928A8 by reference (thiscall, two pair references) -- the bindings' and Ego_r.pdb's CContextSensitivePriorityCompare::operator()(const pair<long, CDefPointer<const CContextSensitiveItemDef> >&, ...) const -- and destroys its pivot through the pair destructor 0x005930F3 (??1?$pair@JV?$CDefPointer@$$CBVCContextSensitiveItemDef@@@@@std@@)
// Label: the manifest's "Heap_CalculateDepth" is a behavioural / propagated label (family J067: five byte-identical sort bodies, each its own element and comparator).
// Callees: __introsort_loop(first, last, (value_type*)0, int depth_limit, comp) 0x00594E4D, __final_insertion_sort(first, last, comp) 0x00594C33
//   (both out of line; the bindings' ??$__introsort_loop spellings for this element where present).
// Shape: STLport 4.6 _algo.c `sort` verbatim in a size-optimised TU (the __lg loop enters at its test, the comparator is re-pushed
//   from its stack slot); __lg is inlined (`for (k = 0; n != 1; n >>= 1) ++k;`), (last - first) is the exact
//   pointer difference (`sar 3` for the 0x8-byte pair); the empty comparator travels as one pushed dword.
// Element: std::pair<long, CDefPointer<const CContextSensitiveItemDef> > (8 bytes, Ego_r.pdb), ordered by CContextSensitivePriorityCompare (empty, 1 byte)
#pragma optimize("s", on)
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> struct iterator_traits;
template <class _Tp> struct iterator_traits<_Tp*> {
    typedef _Tp value_type;
    typedef int difference_type;
};
template <class _Size> inline _Size __lg(_Size __n)
{
    _Size __k;
    for (__k = 0; __n != 1; __n >>= 1) ++__k;
    return __k;
}
template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp);   // 0x00594E4D
template <class _RandomAccessIter, class _Compare>
void __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x00594C33
template <class _RandomAccessIter, class _Compare>
void sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first != __last) {
        __introsort_loop(__first, __last, (typename iterator_traits<_RandomAccessIter>::value_type*)0,
                         __lg(__last - __first) * 2, __comp);
        __final_insertion_sort(__first, __last, __comp);
    }
}
}   // namespace std
class CDefPointeeBase;
class CContextSensitiveItemDef;
template <class T> class CDefPointer {                   // Ego_r.pdb 4 bytes
public:
    CDefPointeeBase* Object;
};
class CContextSensitivePriorityCompare {                 // Ego_r.pdb 1 byte
public:
    bool operator()(const std::pair<long, CDefPointer<const CContextSensitiveItemDef> >& a,
                    const std::pair<long, CDefPointer<const CContextSensitiveItemDef> >& b) const;   // 0x005928A8
};
namespace std {
template void sort<pair<long, CDefPointer<const CContextSensitiveItemDef> >*, CContextSensitivePriorityCompare >(pair<long, CDefPointer<const CContextSensitiveItemDef> >*, pair<long, CDefPointer<const CContextSensitiveItemDef> >*, CContextSensitivePriorityCompare);
}