// std::__final_insertion_sort(CThing** first, CThing** last, CThingCompare_InDirectionOfAndNearestToHeuristicBitch comp) -- final pass of the take-cover candidate sort: a guarded insertion sort of the first 16 elements, then the unguarded one for the rest
// Leaf: ??$__final_insertion_sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0097B240, 118 bytes (+ int3 padding to 128), __fastcall (/Gr algorithm TU: first
//   in ecx, last in edx, the comparator on the stack), `ret 10h`.
// Instantiation: the take-cover candidate sort: std::sort<CThing**,
//   CThingCompare_InDirectionOfAndNearestToHeuristicBitch> called from
//   CAIStateGroup_CreatureTakeCover::ProcessCoverThingList (0x0097AAE0 -> sort 0x0097B660); the second copy of the
//   0x00978xxx AdvanceInCover family (same bytes, own anonymous namespace ?A0xeaff0e08, own COMDATs).
// Callees: __insertion_sort @ 0x0097B180, __unguarded_insertion_sort_aux @ 0x0097AEE0.
// Shape: STLport `__final_insertion_sort` (threshold 16); `__unguarded_insertion_sort` is the inline front end
//   passing the value-type tag `(CThing**)0`. The model compiles operator() as a same-TU stand-in (auto_inline(off) +
//   noinline); with it VC7.1 reproduces retail's comparator-copy scheduling (copies loaded two words ahead) and slot
//   sharing.
// Label: catalog "make_heap<CThing**,CThingCompare_BestForTargeting2>" -> the STLport 4.x
//   std::__final_insertion_sort instantiation for CThingCompare_InDirectionOfAndNearestToHeuristicBitch (retail
//   links STLport; the Dinkumware names in Ego_r.pdb are a different build).
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
// Ego_r.pdb: 16 bytes in the anonymous namespace of CAIStateGroup_CreatureTakeCover.cpp (?A0xeaff0e08):
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
    __declspec(noinline) bool operator()(CThing* a, CThing* b) const;   // 0x0097ABF0
};

// Same-TU stand-in for operator() (its own retail body is 0x0097ABF0, outside these bytes): compiled in this TU
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
template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x0097B180
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*,
                                               _Compare __comp);   // 0x0097AEE0

template <class _RandomAccessIter, class _Compare>
inline void __unguarded_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    __unguarded_insertion_sort_aux(__first, __last, (CThing**)0, __comp);
}

template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__last - __first > 16) {                          // __stl_threshold
        __insertion_sort(__first, __first + 16, __comp);
        __unguarded_insertion_sort(__first + 16, __last, __comp);
    }
    else
        __insertion_sort(__first, __last, __comp);
}
template void __fastcall __final_insertion_sort<CThing**, TComp>(CThing**, CThing**, TComp);
} // namespace std