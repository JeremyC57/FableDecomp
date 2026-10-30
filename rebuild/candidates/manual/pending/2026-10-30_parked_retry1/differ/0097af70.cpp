// _global::_Adjust_heap(CThing**, long, long, CThing*, CThingCompare_InDirectionOfAndNearestToHeuristicBitch) -- heap sift-down of things by the direction/distance heuristic comparator, then _Push_heap @ 0x0097ADF0
// Grade: IMPROVED(142v142) -- retry1: a user copy constructor on the comparator gives retail's eax argument-area pointer (the trivial copy picks ecx); the one residue is scheduling: retail loads words 0 and 1 (ecx, edx) before `sub esp,10h`, VC7.1 here loads word 1 after the first store. Same result with C2DVector members. Lever for 00978b90 (same instantiation shape).
// Flags: /Gr
// Leaf: _Adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0097AF70, 142 bytes.
// Callees: 0x0097ABF0 CThingCompare_InDirectionOfAndNearestToHeuristicBitch::operator() (out of
// line, __thiscall on the by-value comparator, args (first[idx], first[idx-1])), 0x0097ADF0
// _Push_heap (same instantiation).  Link-time __fastcall: first in ecx, hole in edx, then bottom,
// the CThing* value and the 16-byte comparator by value; `ret 0x18`.
// The comparator (anonymous namespace in its TU) holds a 2D origin (+0,+4) and a 2D direction
// (+8,+0xc): operator() reads the thing's position, subtracts the origin, normalises and dots it
// with the direction (0x0097ABF0).  It is copied into the _Push_heap argument area word by word
// (`sub esp,0x10 ; mov eax,esp`); `top` lives in the frame slot reserved by `push ecx`.
// Shape: VC7.1 <algorithm> `_Adjust_heap`; the bottom case reuses idx (`first[idx - 1]`).
class CThing;

namespace {
struct CThingCompare_InDirectionOfAndNearestToHeuristicBitch {
    float m_OriginX;                   // +0x0
    float m_OriginY;                   // +0x4
    float m_DirectionX;                // +0x8
    float m_DirectionY;                // +0xc
    CThingCompare_InDirectionOfAndNearestToHeuristicBitch(const CThingCompare_InDirectionOfAndNearestToHeuristicBitch& o)
        : m_OriginX(o.m_OriginX), m_OriginY(o.m_OriginY), m_DirectionX(o.m_DirectionX), m_DirectionY(o.m_DirectionY) {}
    bool operator()(CThing* a, CThing* b);   // 0x0097ABF0
};
}

void __fastcall _Push_heap(CThing** first, long hole, long top, CThing* value, CThingCompare_InDirectionOfAndNearestToHeuristicBitch pred);   // 0x0097ADF0

void __fastcall _Adjust_heap(CThing** first, long hole, long bottom, CThing* value, CThingCompare_InDirectionOfAndNearestToHeuristicBitch pred)
{
    long top = hole;
    long idx = 2 * hole + 2;
    for (; idx < bottom; idx = 2 * idx + 2) {
        if (pred(first[idx], first[idx - 1]))
            --idx;
        first[hole] = first[idx], hole = idx;
    }
    if (idx == bottom) {
        first[hole] = first[idx - 1];
        hole = idx - 1;
    }
    _Push_heap(first, hole, top, value, pred);
}
