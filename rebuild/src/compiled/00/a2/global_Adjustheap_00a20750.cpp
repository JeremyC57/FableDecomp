// _global::_Adjust_heap(CAStarState**, long, long, CAStarState*, CAStarStateTotalGreaterThan) -- sift a hole down the A* open-list heap (min TotalCost at the top), then _Push_heap @ 0x00A206F0
// Flags: /Gr
// Leaf: _Adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A20750, 90 bytes.
// Callee: 0x00A206F0 _Push_heap (same instantiation).  Link-time __fastcall: first in ecx, hole in
// edx, then bottom, the state pointer and the empty comparator (4-byte slot); `ret 0xc`.
// CNavigatorAStar keeps its open list in CPriorityQueue<CAStarState*> (a vector heap, PDB);
// CAStarState (PDB, 20 bytes): ParentState +0, Node +4, CostToThisState +8, TotalCost +0xc,
// OnOpenList +0x10, OnClosedList +0x11.  The comparator is `a->TotalCost > b->TotalCost`
// (`fld [a+0xc] ; fcomp [b+0xc] ; test ah,0x41 ; jne`), giving a min-heap on total cost.
// Shape: VC7.1 <algorithm> `_Adjust_heap`; the bottom case reuses idx (`first[idx - 1]`).
struct CANavQuadTreeNode;

struct CAStarState {
    CAStarState* ParentState;          // +0x0
    CANavQuadTreeNode* Node;           // +0x4
    float CostToThisState;             // +0x8
    float TotalCost;                   // +0xc
    bool OnOpenList;                   // +0x10
    bool OnClosedList;                 // +0x11
};

struct CAStarStateTotalGreaterThan {
    char m_tag;
    bool operator()(const CAStarState* a, const CAStarState* b) const { return a->TotalCost > b->TotalCost; }
};

void __fastcall _Push_heap(CAStarState** first, long hole, long top, CAStarState* value, CAStarStateTotalGreaterThan pred);   // 0x00A206F0

void __fastcall _Adjust_heap(CAStarState** first, long hole, long bottom, CAStarState* value, CAStarStateTotalGreaterThan pred)
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