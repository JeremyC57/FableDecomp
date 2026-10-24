// _global::_Push_heap(CAStarState**, long, long, CAStarState*, CAStarStateTotalGreaterThan) -- heap sift-up of the A* open list (min TotalCost at the top)
// Leaf: _Push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A206F0, 89 bytes, `ret 0xc`.  No callees.
// Label correction: the catalog "_Push_heap<float*,int,float,std::less<float>>" is propagated; the
// landed callers _Adjust_heap @ 0x00A20750 and CPriorityQueue<CAStarState*>::UpdateElement
// @ 0x00A21C30 name this instantiation (CNavigatorAStar's open list).
// CAStarState (PDB, 20 bytes): ParentState +0, Node +4, CostToThisState +8, TotalCost +0xc,
// OnOpenList +0x10, OnClosedList +0x11.  Comparator `a->TotalCost > b->TotalCost`
// (`fld [parent+0xc] ; fcomp [value+0xc] ; test ah,41h ; jne`).
// Link-time __fastcall (/Gr algorithm TU): first in ecx, hole in edx, then top, the state pointer
// and the empty comparator (4 + 4 + 4 = 0xc).
// Shape: SGI/STLport `__push_heap`; the drop is tail-duplicated into both exits.
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

void __fastcall _Push_heap(CAStarState** first, long hole, long top, CAStarState* value, CAStarStateTotalGreaterThan comp)
{
    long parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}