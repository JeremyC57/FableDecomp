// std::__insertion_sort(CThing** first, CThing** last, CThingCompare_InDirectionOfAndNearestToHeuristicBitch comp) -- guarded insertion sort of the advance-in-cover candidate sort (each element goes through __linear_insert)
// Leaf: ??$__insertion_sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00978DA0, 75 bytes, __fastcall (/Gr algorithm TU: first in ecx, last in edx, the
//   comparator on the stack), `ret 10h`.
// Instantiation: the advance-in-cover candidate sort: std::sort<CThing**,
//   CThingCompare_InDirectionOfAndNearestToHeuristicBitch> called from
//   CAIStateGroup_CreatureAdvanceInCover::ProcessCoverThingList (0x00979260 -> sort 0x00979870); the Ego_r.pdb
//   symbols carry the anonymous namespace ?A0xffab493a of CAIStateGroup_CreatureAdvanceInCover.cpp.
// Callees: __linear_insert @ 0x00978C50.
// Shape: STLport `if (first == last) return; for (i = first + 1; i != last; ++i) __linear_insert(first, i, *i,
//   comp);` with __linear_insert out of line. The model compiles operator() as a same-TU stand-in (auto_inline(off) +
//   noinline); with it VC7.1 reproduces retail's comparator-copy scheduling (copies loaded two words ahead) and slot
//   sharing.
// Label: catalog "std_for_each" -> the STLport 4.x std::__insertion_sort instantiation for
//   CThingCompare_InDirectionOfAndNearestToHeuristicBitch (retail links STLport; the Dinkumware names in Ego_r.pdb
//   are a different build).
// Build: the lane defaults (/O2 /Oy /W3), no pragma; the stand-ins use `#pragma auto_inline(off)` + `__declspec(noinline)`.
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
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00978C50

template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first == __last) return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        __linear_insert(__first, __i, *__i, __comp);
}
template void __fastcall __insertion_sort<CThing**, TComp>(CThing**, CThing**, TComp);
} // namespace std