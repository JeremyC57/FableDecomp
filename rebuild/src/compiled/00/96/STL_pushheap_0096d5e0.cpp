// std::__push_heap(CThing** first, int holeIndex, int topIndex, CThing* val, CAdaptThingCompareToSTLComparitor<CThingCompare_Nearest> comp) -- heap sift-up of the screamer pursue-target sort: move val up while its parent sorts before it
// Leaf: ??$__push_heap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0096D5E0, 119 bytes (+ int3 padding to 128), __fastcall (/Gr algorithm TU: first
//   in ecx, holeIndex in edx, topIndex, val and the comparator on the stack), `ret 0Ch`.
// Instantiation: the screamer target sort: std::partial_sort / sort<CThing**,
//   CAdaptThingCompareToSTLComparitor<CThingCompare_Nearest> > of CAIStateGroup_ScreamerPursueTarget::Update
//   (0x0096DF50) and its __introsort_loop (0x0096E410).
// Callees: CThingCompare_Nearest::operator()<CThing> @ 0x004A8D80 (through the adapter, inline).
// Shape: SGI/STLport `__push_heap` (parent = (hole - 1) / 2).
// Label: catalog "_Push_heap<CThing**,int,CThing*,CAdaptThingCompareToSTLComparitor<CThingCompare_Nearest>_>" -> the
//   STLport 4.x std::__push_heap instantiation for CAdaptThingCompareToSTLComparitor<CThingCompare_Nearest> (retail
//   links STLport; the Dinkumware names in Ego_r.pdb are a different build).
// Build: the lane defaults (/O2 /Oy /W3), no pragma.
class CThing;
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6 (import thunk)

// C3DVector with the engine's memberwise user copy constructor (Ego_r.pdb: X, Y, Z).
class C3DVector { public:
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
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val,
                            _Compare __comp)
{
    _Distance __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && __comp(*(__first + __parent), __val)) {
        *(__first + __holeIndex) = *(__first + __parent);
        __holeIndex = __parent;
        __parent = (__holeIndex - 1) / 2;
    }
    *(__first + __holeIndex) = __val;
}
template void __fastcall __push_heap<CThing**, int, CThing*, TComp>(CThing**, int, int, CThing*, TComp);
} // namespace std