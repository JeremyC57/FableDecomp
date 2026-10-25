// std::pair::pair(const pair& other) -- copy constructor of the A* state map's node value pair<const long, CNavigatorAStar::CAStarState>: the key, then the state (copied and counted)
// Leaf: ??0?$pair@$$CBJVCAStarState@CNavigatorAStar@@@std@@QAE@ABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A20670, 81 bytes, __fastcall `ret 4`, returns `this` (held in eax throughout).
// Element: the node value of CNavigatorAStar's state map (STLport map<long, CAStarState>): key +0, then Ego_r.pdb
//   CNavigatorAStar::CAStarState at +4 (ParentState, Node, CostToThisState, TotalCost, OnOpenList +0x14, OnClosedList
//   +0x15). CAStarState's copy constructor counts the new state into three statics, 0x013D27B8 always, 0x013D27BC when
//   it is on the open list and 0x013D27C0 when on the closed list -- the mirror of the inline ~CAStarState in the landed
//   _Rb_tree::_M_erase 0x00A21640 and 0x00A205C0 (their descriptive NoOfStates* names are reused; not in the PDB name sets).
// Identity: the landed _Rb_tree::_M_insert 0x00A21D90 names this out-of-line copy of the node value (its _M_create_node
//   _Construct); callers 0x00A21155, 0x00A217B9, 0x00A21DCE, 0x00A21DF6.
// Shape: the memberwise pair copy constructor with CAStarState's user copy constructor inline.
class CANavQuadTreeNode;

class CNavigatorAStar {
public:
    class CAStarState {                                 // 0x14 bytes
    public:
        CAStarState*       ParentState;                 // +0x00
        CANavQuadTreeNode* Node;                        // +0x04
        float              CostToThisState;             // +0x08
        float              TotalCost;                   // +0x0C
        bool               OnOpenList;                  // +0x10
        bool               OnClosedList;                // +0x11
        static long NoOfStates;                         // 0x013D27B8 (descriptive)
        static long NoOfStatesOnOpenList;               // 0x013D27BC (descriptive)
        static long NoOfStatesOnClosedList;             // 0x013D27C0 (descriptive)
        CAStarState(const CAStarState& other)
            : ParentState(other.ParentState), Node(other.Node), CostToThisState(other.CostToThisState),
              TotalCost(other.TotalCost), OnOpenList(other.OnOpenList), OnClosedList(other.OnClosedList)
        {
            ++NoOfStates;
            if (OnOpenList)
                ++NoOfStatesOnOpenList;
            if (OnClosedList)
                ++NoOfStatesOnClosedList;
        }
    };
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};
template pair<const long, CNavigatorAStar::CAStarState>::pair(const pair<const long, CNavigatorAStar::CAStarState>&);
}