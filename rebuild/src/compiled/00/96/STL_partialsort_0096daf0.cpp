// std::__partial_sort(CThing** first, CThing** middle, CThing** last, CThing** valueType, CAdaptThingCompareToSTLComparitor<CThingCompare_Nearest> comp) -- partial sort of the screamer pursue-target sort: heapify [first, middle), push every better element of [middle, last) through the heap, then sort the heap
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0096DAF0, 112 bytes, __fastcall (/Gr algorithm TU: first in ecx, middle in edx,
//   last, the value-type tag and the comparator on the stack), `ret 0Ch`.
// Instantiation: the screamer target sort: std::partial_sort / sort<CThing**,
//   CAdaptThingCompareToSTLComparitor<CThingCompare_Nearest> > of CAIStateGroup_ScreamerPursueTarget::Update
//   (0x0096DF50) and its __introsort_loop (0x0096E410).
// Callees: CThingCompare_Nearest::operator()<CThing> @ 0x004A8D80 (through the adapter, inline), __make_heap @
//   0x0096D900, __adjust_heap @ 0x0096D7E0, sort_heap @ 0x0096DAA0.
// Shape: STLport `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, T(*i), comp, (int*)0); sort_heap(first, middle, comp);` with make_heap the inline
//   front end of __make_heap.
// Label: catalog "std_sort_HeapSortFinalize_Adapted" -> the STLport 4.x std::__partial_sort instantiation for
//   CAdaptThingCompareToSTLComparitor<CThingCompare_Nearest> (retail links STLport; the Dinkumware names in Ego_r.pdb
//   are a different build).
// Build: the lane defaults (/O2 /Oy /W3), no pragma.
class CThing;
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6 (import thunk)

// C3DVector with the engine's memberwise user copy constructor (Ego_r.pdb: X, Y, Z).
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

// Ego_r.pdb CThingCompare_Nearest: 12 bytes, Pos (C3DVector) +0x00; operator() returns the nearer thing.
class CThingCompare_Nearest {
public:
    C3DVector Pos;                                        // +0x00
    template <class T> T* operator()(T* a, T* b) const;   // 0x004A8D80 (the nearer of the two; a null loses)
};

// CAdaptThingCompareToSTLComparitor<TCompare> (Ego_r.pdb: 4 bytes, Functor pointer): turns a "pick the better thing"
// functor into a strict-weak-ordering predicate -- a sorts first when the functor picks it. Inline, so every
// comparison is `push b ; push a ; mov ecx,[functor] ; call 0x004A8D80 ; cmp eax,a`.
template <class TCompare>
class CAdaptThingCompareToSTLComparitor {
public:
    TCompare* Functor;                                    // +0x00
    template <class T> bool operator()(T* a, T* b) const { return (*Functor)(a, b) == a; }
};
typedef CAdaptThingCompareToSTLComparitor<CThingCompare_Nearest> TComp;

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);   // 0x0096D7E0
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*,
                            _Distance*);   // 0x0096D900
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x0096DAA0

template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, (CThing**)0, (int*)0);
}
template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                       _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, (int*)0);
    sort_heap(__first, __middle, __comp);
}
template void __fastcall __partial_sort<CThing**, CThing*, TComp>(CThing**, CThing**, CThing**, CThing**, TComp);
} // namespace std