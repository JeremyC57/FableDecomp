// _STL::__unguarded_partition(CThing** first, CThing** last, CThing* pivot, CAdaptThingCompareToSTLComparitor<CThingCompare_Nearest> comp) -- quicksort partition step of the screamer pursue-target sort: walk both ends towards the pivot and swap each out-of-place pair, returning the cut
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0096D6F0, 141 bytes (+ int3 padding to 144), __fastcall (/Gr algorithm TU: first
//   in ecx, last in edx, pivot and the comparator on the stack), `ret 8`.
// Instantiation: the screamer target sort: _STL::partial_sort / sort<CThing**,
//   CAdaptThingCompareToSTLComparitor<CThingCompare_Nearest> > of CAIStateGroup_ScreamerPursueTarget::Update
//   (0x0096DF50) and its __introsort_loop (0x0096E410).
// Callees: CThingCompare_Nearest::operator()<CThing> @ 0x004A8D80 (through the adapter, inline).
// Shape: STLport 4.x `__unguarded_partition` with an inline `iter_swap`; the comparator is copied into the argument
//   area for each call.
// Label: catalog "std_sort_Partition_WithPivot" -> the STLport 4.x _STL::__unguarded_partition instantiation for
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

namespace _STL {
template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    CThing* __tmp = *__i1;
    *__i1 = *__i2;
    *__i2 = __tmp;
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last,
                                                   _Tp __pivot, _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}
template CThing** __fastcall __unguarded_partition<CThing**, CThing*, TComp>(CThing**, CThing**, CThing*, TComp);
} // namespace _STL