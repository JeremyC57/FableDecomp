// std::pair<const long, CNavigatorAStar::CAStarState>::pair(const long&, const CAStarState&) -- builds an A* state-map element
// Retail: Fable.exe 0x00A20410, 84 bytes: first = key; second = CAStarState copy (ParentState, Node, CostToThisState,
// TotalCost, OnOpenList, OnClosedList member by member), then the copy constructor's instance counters:
// ++NoOfStates (0x013D27B8), ++NoOfStatesOnOpenList (0x013D27BC) if OnOpenList, ++NoOfStatesOnClosedList (0x013D27C0)
// if OnClosedList; returns `this`, `ret 8`.
// Identity: the three counters are bound as CNavigatorAStar::CAStarState::NoOfStates / NoOfStatesOnOpenList /
// NoOfStatesOnClosedList (reloc_bindings); the element is the Ego_r.pdb CAStarState (ParentState +0, Node +4,
// CostToThisState +8, TotalCost +0xC, OnOpenList +0x10, OnClosedList +0x11; 0x14 bytes) stored after a long key in
// map<long, CNavigatorAStar::CAStarState> (Ego_r.pdb _Pair_base<long const, CAStarState>: first +0, second +4). Its
// only caller 0x00A20F30 placement-constructs the pair from (key, state). The inlined copy constructor is Ego_r.pdb
// ??0CAStarState@CNavigatorAStar@@QAE@ABV01@@Z. STLport 4.6 pair ctor: `pair(const _T1& __a, const _T2& __b)
// : first(__a), second(__b) {}`.
// Leaf: ??0?$pair@$$CBJVCAStarState@CNavigatorAStar@@@std@@QAE@ABJABVCAStarState@CNavigatorAStar@@@Z
class CANavQuadTreeNode;

class CNavigatorAStar {
public:
    class CAStarState {
    public:
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
        static long NoOfStates;                    // 0x013D27B8
        static long NoOfStatesOnOpenList;          // 0x013D27BC
        static long NoOfStatesOnClosedList;        // 0x013D27C0
        CAStarState*             ParentState;      // +0x00
        const CANavQuadTreeNode* Node;             // +0x04
        float                    CostToThisState;  // +0x08
        float                    TotalCost;        // +0x0C
        bool                     OnOpenList;       // +0x10
        bool                     OnClosedList;     // +0x11
    };
};

namespace std {
    template <class _T1, class _T2>
    struct pair {
        _T1 first;
        _T2 second;
        pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    };

    template struct pair<const long, CNavigatorAStar::CAStarState>;
}