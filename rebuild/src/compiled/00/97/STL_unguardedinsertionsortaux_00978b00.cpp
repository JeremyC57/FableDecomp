// std::__unguarded_insertion_sort_aux(CThing** first, CThing** last, CThing** valueType, CThingCompare_InDirectionOfAndNearestToHeuristicBitch comp) -- unguarded insertion sort of the tail of the advance-in-cover candidate sort (every element is moved left until the comparator stops it; the sorted 16-element head is the sentinel)
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00978B00, 130 bytes (+ int3 padding to 144), __fastcall (/Gr algorithm TU: first
//   in ecx, last in edx, the value-type tag and the comparator on the stack), `ret 14h`.
// Instantiation: the advance-in-cover candidate sort: std::sort<CThing**,
//   CThingCompare_InDirectionOfAndNearestToHeuristicBitch> called from
//   CAIStateGroup_CreatureAdvanceInCover::ProcessCoverThingList (0x00979260 -> sort 0x00979870); the Ego_r.pdb
//   symbols carry the anonymous namespace ?A0xffab493a of CAIStateGroup_CreatureAdvanceInCover.cpp.
// Callees: CThingCompare_InDirectionOfAndNearestToHeuristicBitch::operator() @ 0x00978790.
// Shape: STLport `for (i = first; i != last; ++i) __unguarded_linear_insert(i, T(*i), comp)` with the insert inlined.
//   Here operator() is only DECLARED: with the same-TU stand-in body of the sibling rows VC7.1 hoists the
//   loop-invariant comparator copy out of the loop (retail rebuilds it per call).
// Label: catalog "std_sort_InsertionSort_HeuristicBitch" -> the STLport 4.x std::__unguarded_insertion_sort_aux
//   instantiation for CThingCompare_InDirectionOfAndNearestToHeuristicBitch (retail links STLport; the Dinkumware
//   names in Ego_r.pdb are a different build).
// Build: the lane defaults (/O2 /Oy /W3), no pragma.
class CThing;
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6 (import thunk)
extern "C" double __cdecl pow(double x, double y);                                    // _CIpow 0x00BFEB40
extern "C" double __cdecl sqrt(double x);

class C3DVector { public:
    float X, Y, Z;
};

class CThing {
public:
    const C3DVector& GetPosition() const;                 // 0x004C73D0
};

// C2DVector (Ego_r.pdb: X +0, Y +4) with the engine's memberwise user copy constructor: the by-value comparator
// is then built dword by dword in each callee's argument area (`sub esp,10h ; mov eax,esp ; mov [eax],..`).
class C2DVector { public:
    float X, Y;
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) { }
};

namespace {
// Ego_r.pdb: 16 bytes in the anonymous namespace of CAIStateGroup_CreatureAdvanceInCover.cpp (?A0xffab493a):
// Pos (C2DVector) +0x00, Dir (C2DVector) +0x08.
class CThingCompare_InDirectionOfAndNearestToHeuristicBitch {
public:
    C2DVector Pos;                                        // +0x00 the searcher's XY position
    C2DVector Dir;                                        // +0x08 the unit XY direction it wants to move in
    bool operator()(CThing* a, CThing* b) const;          // 0x00978790 (declared only, see Shape)
};
}
typedef CThingCompare_InDirectionOfAndNearestToHeuristicBitch TComp;

namespace std {
// __unguarded_linear_insert is defined here (as in STLport's _algo.c) and inlined into the loop below; its own
// out-of-line copy is the separate COMDAT at 0x009789B0.
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