// std::__adjust_heap(CThing** first, int holeIndex, int len, CThing* val, CThingCompare_InDirectionOfAndNearestToHeuristicBitch comp) -- sift a hole down to a leaf, then push_heap the value back up
// Leaf: ??$__adjust_heap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0097AF70, 142 bytes, __fastcall (/Gr algorithm TU: first in ecx, holeIndex in edx,
//   len, val and the 16-byte comparator on the stack), `ret 18h`.
// Instantiation: the take-cover candidate sort std::sort<CThing**, CThingCompare_InDirectionOfAndNearestToHeuristicBitch>
//   (CAIStateGroup_CreatureTakeCover::ProcessCoverThingList 0x0097AAE0 -> sort 0x0097B660 -> partial_sort/make_heap ->
//   this). Caller evidence: landed __make_heap 0x0097B0E0 and __sort_heap 0x0097B2C0 call 0x0097AF70.
// Callees: CThingCompare_InDirectionOfAndNearestToHeuristicBitch::operator() @ 0x0097ABF0, __push_heap @ 0x0097ADF0.
// Shape: STLport 4.6 `__adjust_heap` (_heap.c). The comparator is copied by value into __push_heap's argument area;
//   retail loads its words two ahead of the stores (`mov ecx,[c+0] ; mov edx,[c+4] ; sub esp,10h ; mov eax,esp ...`).
//   That pipelined copy needs (1) C2DVector with the engine's memberwise user copy constructor and (2) operator()
//   compiled in this TU as a stand-in under auto_inline(off) + noinline, so VC7.1 sees the call neither writes nor
//   leaks the comparator. Declaration-only operator() gave `fld/fstp` and late word loads (the parked residue).
//   The stand-in and the comparator model are copied from the landed sibling 0x0097B0E0 (__make_heap).
// Label: catalog "_Adjust_heap<...>" is the Dinkumware spelling; retail links STLport 4.6 (std::__adjust_heap).
// Build: lane defaults (/O2 /Oy /W3); the stand-in uses `#pragma auto_inline(off)` + `__declspec(noinline)`.
class CThing;
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6 (import thunk)
extern "C" double __cdecl pow(double x, double y);                                    // _CIpow 0x00BFEB40
extern "C" double __cdecl sqrt(double x);

class C3DVector { public:
    float X, Y, Z;
};

class CThing {
public:
    const C3DVector& GetPos() const;                 // 0x004C73D0
};

// C2DVector (Ego_r.pdb: X +0, Y +4) with the engine's memberwise user copy constructor: the by-value comparator
// is then built dword by dword in each callee's argument area (`sub esp,10h ; mov eax,esp ; mov [eax],..`).
class C2DVector { public:
    float X, Y;
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) { }
};

namespace {
// Ego_r.pdb: 16 bytes in the anonymous namespace of CAIStateGroup_CreatureTakeCover.cpp (?A0xeaff0e08):
// Pos (C2DVector) +0x00, Dir (C2DVector) +0x08.
class CThingCompare_InDirectionOfAndNearestToHeuristicBitch {
public:
    C2DVector Pos;                                        // +0x00 the searcher's XY position
    C2DVector Dir;                                        // +0x08 the unit XY direction it wants to move in
    // The heuristic of the retail operator(): the direction cosine to the thing raised to an odd power that grows
    // with the distance (pow / sqrt imports), so far things must lie much closer to Dir to rank first.
    float GetScore(const CThing* thing) const
    {
        const C3DVector& p = thing->GetPos();
        float dx = p.X - Pos.X;
        float dy = p.Y - Pos.Y;
        float dist = (float)sqrt(dx * dx + dy * dy);
        float inv = 1.0f / dist;
        dx *= inv;
        dy *= inv;
        float along = dy * Dir.Y + dx * Dir.X;
        return (float)pow(along, (float)((int)dist / 4 * 2 + 1));
    }
    __declspec(noinline) bool operator()(const CThing* a, const CThing* b) const;   // 0x0097ABF0
};

// Same-TU stand-in for operator() (its own retail body is 0x0097ABF0, outside these bytes): compiled in this TU
// under auto_inline(off) VC7.1 can see that the call neither writes nor leaks the comparator, which is what
// gives retail's pipelined comparator copies (declaration-only, each copy is reloaded one call later).
#pragma auto_inline(off)
bool CThingCompare_InDirectionOfAndNearestToHeuristicBitch::operator()(const CThing* a, const CThing* b) const
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
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val,
                            _Compare __comp);   // 0x0097ADF0

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp)
{
    _Distance __topIndex = __holeIndex;
    _Distance __secondChild = 2 * __holeIndex + 2;
    while (__secondChild < __len) {
        if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1))))
            __secondChild--;
        *(__first + __holeIndex) = *(__first + __secondChild);
        __holeIndex = __secondChild;
        __secondChild = 2 * (__secondChild + 1);
    }
    if (__secondChild == __len) {
        *(__first + __holeIndex) = *(__first + (__secondChild - 1));
        __holeIndex = __secondChild - 1;
    }
    __push_heap(__first, __holeIndex, __topIndex, __val, __comp);
}
template void __fastcall __adjust_heap<CThing**, int, CThing*, TComp>(CThing**, int, int, CThing*, TComp);
} // namespace std