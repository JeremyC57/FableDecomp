// std::__introsort_loop(CThing** first, CThing** last, CThing**, int depth_limit, CThingCompare_InDirectionOfAndNearestToHeuristicBitch comp) -- STLport introsort recursion of the advance-in-cover candidate sort: median-of-three partition until 16 elements remain, partial (heap) sort when the depth budget runs out
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00979760, 272 bytes (+ int3 padding), __fastcall (/Gr algorithm TU: first in ecx,
//   last in edx; the _Tp* tag, the depth limit and the 16-byte comparator on the stack), `ret 18h`.
// Instantiation: std::sort<CThing**, CThingCompare_InDirectionOfAndNearestToHeuristicBitch> of
//   CAIStateGroup_CreatureAdvanceInCover::ProcessCoverThingList (0x00979260 -> sort 0x00979870 (landed) -> here);
//   called by that sort (0x009798BA) and recursively by itself (0x00979818). The catalog "std_sort_QuickSort" is
//   descriptive.
// Callees: __median(a, b, c, comp) @ 0x00978920 (out of line in this TU, returns a reference), __unguarded_partition
//   @ 0x00978A80 (landed), __partial_sort(first, middle, last, _Tp*, comp) @ 0x00979650 (landed; STLport's partial_sort
//   inlined around it), itself.
// Shape: STLport 4.x `__introsort_loop` verbatim. The comparator (two C2DVectors with user copy constructors, so it is
//   built dword by dword in the argument area) is copied for each callee; the `sub esp,10h` both arms begin with is
//   hoisted above the `depth_limit == 0` test; the partition's comparator is built before the median's (right-to-left
//   argument evaluation). The model (operator() as a same-TU noinline stand-in) is the landed sort 0x00979870's.
class CThing;
extern "C" double __cdecl pow(double x, double y);                                    // _CIpow 0x00BFEB40
extern "C" double __cdecl sqrt(double x);

class C3DVector { public:
    float X, Y, Z;
};

class CThing {
public:
    const C3DVector& GetPosition() const;                 // 0x004C73D0
};

// C2DVector (Ego_r.pdb: X +0, Y +4) with the engine's memberwise user copy constructor.
class C2DVector { public:
    float X, Y;
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) { }
};

namespace {
// Ego_r.pdb: 16 bytes in the anonymous namespace of CAIStateGroup_CreatureAdvanceInCover.cpp (?A0xffab493a).
class CThingCompare_InDirectionOfAndNearestToHeuristicBitch {
public:
    C2DVector Pos;                                        // +0x00 the searcher's XY position
    C2DVector Dir;                                        // +0x08 the unit XY direction it wants to move in
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

const int __stl_threshold = 16;

template <class _Tp, class _Compare>
const _Tp& __fastcall __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp);   // 0x00978920

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last,
                                                   _Tp __pivot, _Compare __comp);                 // 0x00978A80

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last,
                               _Tp*, _Compare __comp);                                            // 0x00979650

template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last,
                         _Compare __comp)
{
    __partial_sort(__first, __middle, __last, (CThing**)0, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit,
                                 _Compare __comp)
{
    while (__last - __first > __stl_threshold) {
        if (__depth_limit == 0) {
            partial_sort(__first, __last, __last, __comp);
            return;
        }
        --__depth_limit;
        _RandomAccessIter __cut =
            __unguarded_partition(__first, __last,
                                  _Tp(__median(*__first, *(__first + (__last - __first) / 2), *(__last - 1), __comp)),
                                  __comp);
        __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp);
        __last = __cut;
    }
}

template void __fastcall __introsort_loop<CThing**, CThing*, int, TComp>(CThing**, CThing**, CThing**, int, TComp);

} // namespace std