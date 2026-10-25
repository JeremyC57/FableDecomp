// std::__partial_sort(CThing** first, CThing** middle, CThing** last, CThing** valueType, CThingCompare_InDirectionOfAndNearestToHeuristicBitch comp) -- partial sort of the advance-in-cover candidate sort: heapify [first, middle), push every better element of [middle, last) through the heap, then sort the heap
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00979650, 194 bytes, __fastcall (/Gr algorithm TU: first in ecx, middle in edx,
//   last, the value-type tag and the comparator on the stack), `ret 18h`.
// Instantiation: the advance-in-cover candidate sort: std::sort<CThing**,
//   CThingCompare_InDirectionOfAndNearestToHeuristicBitch> called from
//   CAIStateGroup_CreatureAdvanceInCover::ProcessCoverThingList (0x00979260 -> sort 0x00979870); the Ego_r.pdb
//   symbols carry the anonymous namespace ?A0xffab493a of CAIStateGroup_CreatureAdvanceInCover.cpp.
// Callees: CThingCompare_InDirectionOfAndNearestToHeuristicBitch::operator() @ 0x00978790, __make_heap @ 0x00978D00,
//   __adjust_heap @ 0x00978B90, sort_heap @ 0x00979370.
// Shape: STLport `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, T(*i), comp, (int*)0); sort_heap(first, middle, comp);` with make_heap the inline
//   front end of __make_heap. The model compiles operator() as a same-TU stand-in (auto_inline(off) + noinline); with
//   it VC7.1 reproduces retail's comparator-copy scheduling (copies loaded two words ahead) and slot sharing.
// Label: catalog "std_sort_HeapSortFinalize_HeuristicBitch" -> the STLport 4.x std::__partial_sort instantiation for
//   CThingCompare_InDirectionOfAndNearestToHeuristicBitch (retail links STLport; the Dinkumware names in Ego_r.pdb
//   are a different build).
// Build: the lane defaults (/O2 /Oy /W3), no pragma; the stand-ins use `#pragma auto_inline(off)` + `__declspec(noinline)`.
class CThing;
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6 (import thunk)
extern "C" double __cdecl pow(double x, double y);                                    // _CIpow 0x00BFEB40
extern "C" double __cdecl sqrt(double x);

struct C3DVector {
    float X, Y, Z;
};

class CThing {
public:
    const C3DVector& GetPosition() const;                 // 0x004C73D0
};

// C2DVector (Ego_r.pdb: X +0, Y +4) with the engine's memberwise user copy constructor: the by-value comparator
// is then built dword by dword in each callee's argument area (`sub esp,10h ; mov eax,esp ; mov [eax],..`).
struct C2DVector {
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
    // The heuristic of the retail operator(): the direction cosine to the thing raised to an odd power that grows
    // with the distance (pow / sqrt imports), so far things must lie much closer to Dir to rank first.
    float GetScore(CThing* thing) const
    {
        const C3DVector& p = thing->GetPosition();
        float dx = p.X - Pos.X;
        float dy = p.Y - Pos.Y;
        float dist = (float)sqrt(dx * dx + dy * dy);
        float inv = 1.0f / dist;
        dx *= inv;
        dy *= inv;
        float along = dy * Dir.Y + dx * Dir.X;
        return (float)pow(along, (float)((int)dist / 4 * 2 + 1));
    }
    __declspec(noinline) bool operator()(CThing* a, CThing* b) const;   // 0x00978790
};

// Same-TU stand-in for operator() (its own retail body is 0x00978790, outside these bytes): compiled in this TU
// under auto_inline(off) VC7.1 can see that the call neither writes nor leaks the comparator, which is what
// gives retail's pipelined comparator copies (declaration-only, each copy is reloaded one call later).
#pragma auto_inline(off)
bool CThingCompare_InDirectionOfAndNearestToHeuristicBitch::operator()(CThing* a, CThing* b) const
{
    float scoreA = GetScore(a);
    if (GetScore(b) < scoreA)
        return true;
    return false;
}
#pragma auto_inline(on)
}
typedef CThingCompare_InDirectionOfAndNearestToHeuristicBitch TComp;

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);   // 0x00978B90
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*,
                            _Distance*);   // 0x00978D00
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x00979370

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