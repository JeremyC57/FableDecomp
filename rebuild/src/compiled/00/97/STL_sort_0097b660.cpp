// _STL::sort(CThing** first, CThing** last, CThingCompare_InDirectionOfAndNearestToHeuristicBitch comp) -- entry of the take-cover candidate sort: introsort to depth 2*lg(n), then the final insertion sort
// Leaf: ??$sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0097B660, 125 bytes, __fastcall (/Gr algorithm TU: first in ecx, last in edx,
//   the comparator on the stack), `ret 10h`.
// Instantiation: the take-cover candidate sort: _STL::sort<CThing**,
//   CThingCompare_InDirectionOfAndNearestToHeuristicBitch> called from
//   CAIStateGroup_CreatureTakeCover::ProcessCoverThingList (0x0097AAE0 -> sort 0x0097B660); the second copy of the
//   0x00978xxx AdvanceInCover family (same bytes, own anonymous namespace ?A0xeaff0e08, own COMDATs).
// Callees: __introsort_loop @ 0x0097B4E0, __final_insertion_sort @ 0x0097B240.
// Shape: STLport `if (first != last) { __introsort_loop(first, last, (T*)0, __lg(last - first) * 2, comp);
//   __final_insertion_sort(first, last, comp); }`; `__lg` is the shift-count loop. The model compiles operator() as a
//   same-TU stand-in (auto_inline(off) + noinline); with it VC7.1 reproduces retail's comparator-copy scheduling
//   (copies loaded two words ahead) and slot sharing.
// Label: catalog "std_sort_Introsort_HeuristicBitch_Variant" -> the STLport 4.x _STL::sort instantiation for
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

namespace _STL {
template <class _Size>
inline _Size __lg(_Size __n)
{
    _Size __k;
    for (__k = 0; __n != 1; __n >>= 1) ++__k;
    return __k;
}
template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit,
                                 _Compare __comp);   // 0x0097B4E0
template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x0097B240

template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first != __last) {
        __introsort_loop(__first, __last, (CThing**)0, __lg(__last - __first) * 2, __comp);
        __final_insertion_sort(__first, __last, __comp);
    }
}
template void __fastcall sort<CThing**, TComp>(CThing**, CThing**, TComp);
} // namespace _STL