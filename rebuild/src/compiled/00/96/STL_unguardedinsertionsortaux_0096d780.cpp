// std::__unguarded_insertion_sort_aux(CThing** first, CThing** last, CThing** valueType, CAdaptThingCompareToSTLComparitor<CThingCompare_Nearest> comp) -- unguarded insertion sort of the tail of the screamer pursue-target sort (every element is moved left until the comparator stops it; the sorted 16-element head is the sentinel)
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0096D780, 96 bytes, __fastcall (/Gr algorithm TU: first in ecx, last in edx, the
//   value-type tag and the comparator on the stack), `ret 8`.
// Instantiation: the screamer target sort: std::partial_sort / sort<CThing**,
//   CAdaptThingCompareToSTLComparitor<CThingCompare_Nearest> > of CAIStateGroup_ScreamerPursueTarget::Update
//   (0x0096DF50) and its __introsort_loop (0x0096E410).
// Callees: CThingCompare_Nearest::operator()<CThing> @ 0x004A8D80 (through the adapter, inline).
// Shape: STLport `for (i = first; i != last; ++i) __unguarded_linear_insert(i, T(*i), comp)` with the insert inlined.
// Label: catalog "std_sort_InsertionSort_Alternate" -> the STLport 4.x std::__unguarded_insertion_sort_aux
//   instantiation for CAdaptThingCompareToSTLComparitor<CThingCompare_Nearest> (retail links STLport; the Dinkumware
//   names in Ego_r.pdb are a different build).
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
// __unguarded_linear_insert is defined here (as in STLport's _algo.c) and inlined into the loop below; its own
// out-of-line copy is the separate COMDAT at 0x0096D590.
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last,
                                               _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}
template void __fastcall __unguarded_insertion_sort_aux<CThing**, CThing*, TComp>(CThing**, CThing**, CThing**, TComp);
} // namespace std