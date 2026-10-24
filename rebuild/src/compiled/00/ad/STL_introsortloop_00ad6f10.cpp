// _STL::__introsort_loop(COptimisedVertex::CGroup* first, COptimisedVertex::CGroup* last, CGroup*, int depth_limit, bool (__fastcall* comp)(const CGroup&, const CGroup&)) -- STLport introsort recursion over a vertex's bone groups: median-of-three partition until 16 elements remain, heap sort when the depth budget runs out
// Flags: /Gr
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD6F10, 0xD7 bytes (+ int3 padding), __fastcall (first ecx, last edx),
// `ret 0Ch` (the _Tp* tag, the depth limit, the comparator).
// Identity: called by sort<SElement8*> 0x00AD7550 (landed) with `__lg(n) * 2` and recursively by itself; the
// comparator is a /Gr function pointer (`call ebp` with the two element addresses in ecx/edx), the same TU as
// sort_heap 0x00AD4960: Ego_r.pdb sorts COptimisedVertex::CGroup (BoneIndex, BoneWeight; 8 bytes, moved as two
// dwords) with the static __fastcall CGroup::CompareWeight. Catalog "Heap_Partition" is descriptive.
// Callees: __unguarded_partition(first, last, pivot, comp) @ 0x00AD2AF0 (pivot by value: two dword pushes),
// __partial_sort(first, middle, last, _Tp*, comp) @ 0x00AD5BC0 (STLport partial_sort inlined around it),
// itself @ 0x00AD6F10.
// Shape: STLport 4.x `while (last - first > __stl_threshold)` (`and eax,-8 ; cmp eax,80h`); `if (depth_limit == 0)
// { partial_sort(first, last, last, comp); return; }`; `--depth_limit` kept in its own argument slot; the middle
// pointer `first + (last - first) / 2` is homed in the dead comparator slot (comp lives in ebp); __median inlined
// (six comparator calls, returns a reference) and copied into the by-value pivot; `last = cut` at the end.
namespace QuadricOptimiserInternals {
class COptimisedVertex {
public:
    class CGroup {                           // Ego_r.pdb COptimisedVertex::CGroup, 8 bytes
    public:
        unsigned long BoneIndex;             // +0x0
        float         BoneWeight;            // +0x4
        static bool __fastcall CompareWeight(const CGroup& a, const CGroup& b);
    };
};
}
typedef QuadricOptimiserInternals::COptimisedVertex::CGroup CGroup;
typedef bool (__fastcall* PFNGroupCompare)(const CGroup&, const CGroup&);

namespace _STL {

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
_RandomAccessIter __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp);   // 0x00AD2AF0

template <class _RandomAccessIterator, class _Tp, class _Compare>
void __partial_sort(_RandomAccessIterator __first, _RandomAccessIterator __middle, _RandomAccessIterator __last, _Tp*, _Compare __comp);   // 0x00AD5BC0

template <class _RandomAccessIterator, class _Compare>
inline void partial_sort(_RandomAccessIterator __first, _RandomAccessIterator __middle, _RandomAccessIterator __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, (CGroup*)0, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp)
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

template void __introsort_loop<CGroup*, CGroup, int, PFNGroupCompare>(CGroup*, CGroup*, CGroup*, int, PFNGroupCompare);

} // namespace _STL