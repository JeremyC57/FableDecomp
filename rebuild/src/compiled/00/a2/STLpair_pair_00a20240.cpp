// _STL::pair::pair(const pair<long, CNavigatorAStar::CAStarState>& p) -- converting constructor of the A* state map's node value pair<const long, CAStarState> from the make_pair temporary: the key, then the state (copied and counted)
// Leaf: ??$?0JVCAStarState@CNavigatorAStar@@@?$pair@$$CBJVCAStarState@CNavigatorAStar@@@_STL@@QAE@ABU?$pair@JVCAStarState@CNavigatorAStar@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A20240, 81 bytes, __fastcall `ret 4`, returns `this` (held in eax throughout).
// Element: the node value of CNavigatorAStar's state map (STLport map<long, CAStarState>): key +0, then Ego_r.pdb
//   CNavigatorAStar::CAStarState at +4 (ParentState, Node, CostToThisState, TotalCost, OnOpenList +0x14, OnClosedList
//   +0x15). CAStarState's copy constructor counts the new state into three statics, 0x013D27B8 always, 0x013D27BC when
//   it is on the open list and 0x013D27C0 when on the closed list -- the mirror of the inline ~CAStarState in the landed
//   _Rb_tree::_M_erase 0x00A21640 and 0x00A205C0 (their descriptive NoOfStates* names are reused; not in the PDB name sets).
// Identity: the one caller 0x00A1D016 builds a pair<long, CAStarState> temporary (make_pair, inlined, with the counter
//   increments of its state copy), converts it into a second temporary through this function and hands that to the map's
//   insert_unique 0x00A221B0: STLport `map::insert(make_pair(key, state))`. The byte twin 0x00A20670 is the copy constructor.
// Shape: STLport's member-template converting constructor `pair(const pair<_U1, _U2>& __p) : first(__p.first),
//   second(__p.second)`, explicitly instantiated; CAStarState's user copy constructor inline.
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

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};
template pair<const long, CNavigatorAStar::CAStarState>::pair(const pair<long, CNavigatorAStar::CAStarState>&);
}