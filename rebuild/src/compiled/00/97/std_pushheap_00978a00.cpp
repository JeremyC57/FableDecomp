// std::__push_heap(CThing** first, int holeIndex, int topIndex, CThing* val, (anonymous namespace)::CThingCompare_InDirectionOfAndNearestToHeuristicBitch comp) -- STLport heap sift-up: while the hole is below the top and comp(parent, val) holds, move the parent down; then store val in the hole
// Leaf: ??$__push_heap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00978A00, 97 bytes, /Gr __fastcall (first ecx, holeIndex edx; topIndex, val and the
//   comparator on the stack), `ret 18h`.
// Identity (proven): only caller std::__adjust_heap 0x00978B90 of the advance-in-cover sort std::sort<CThing**, CThingCompare_InDirectionOfAndNearestToHeuristicBitch> (landed siblings 0x00978A80 __unguarded_partition / 0x00978C50 __linear_insert / 0x00978D00 __make_heap); comparator 0x00978790, the 16-byte anonymous-namespace functor of CAIStateGroup_CreatureAdvanceInCover.cpp (Ego_r.pdb) -> `ret 18h`.
// Shape (family K019, representative 0x00748880): STLport 4.6 _heap.c `parent = (hole - 1) / 2; while (hole > top &&
//   comp(*(first + parent), val)) { *(first + hole) = *(first + parent); hole = parent; parent = (hole - 1) / 2; }
//   *(first + hole) = val;` (SGI `hole > top`, signed divides by 2 through cdq/sub/sar).
class CThing;
extern "C" double __cdecl pow(double x, double y);                                    // _CIpow 0x00BFEB40
extern "C" double __cdecl sqrt(double x);
class C3DVector { public:
    float X, Y, Z;
};
class CThing {
public:
    const C3DVector& GetPos() const;                 // 0x004C73D0
};
// C2DVector (Ego_r.pdb: X +0, Y +4) with the engine's memberwise user copy constructor.
class C2DVector { public:
    float X, Y;
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) { }
};
namespace {
// Ego_r.pdb: 16 bytes in the anonymous namespace of CAIStateGroup_CreatureAdvanceInCover.cpp:
// Pos (C2DVector) +0x00, Dir (C2DVector) +0x08 (the landed 0x00978A80 model).
class CThingCompare_InDirectionOfAndNearestToHeuristicBitch {
public:
    C2DVector Pos;                                        // +0x00
    C2DVector Dir;                                        // +0x08
    float GetScore(CThing* thing) const
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
    __declspec(noinline) bool operator()(CThing* a, CThing* b) const;   // 0x00978790
};
// Same-TU stand-in for operator() (its retail body is 0x00978790), as the landed sibling rows of this sort compile it.
#pragma auto_inline(off)
bool CThingCompare_InDirectionOfAndNearestToHeuristicBitch::operator()(CThing* a, CThing* b) const
{
    float scoreA = GetScore(a);
    if (GetScore(b) < scoreA)
        return true;
    return false;
}
#pragma auto_inline(on)
}   // namespace
typedef CThingCompare_InDirectionOfAndNearestToHeuristicBitch TComp;

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