// Behaviour test for 0x00A20670 pair<const long, CNavigatorAStar::CAStarState> copy constructor.
#include <cstdio>
#include <cstring>

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
class CANavQuadTreeNode { public: int Id; };
class CAStarState {
public:
    CAStarState*       ParentState;
    CANavQuadTreeNode* Node;
    float              CostToThisState;
    float              TotalCost;
    bool               OnOpenList;
    bool               OnClosedList;
    static long NoOfStates;
    static long NoOfStatesOnOpenList;
    static long NoOfStatesOnClosedList;
    CAStarState(CANavQuadTreeNode* n, bool open, bool closed)
        : ParentState(0), Node(n), CostToThisState(1.5f), TotalCost(7.25f), OnOpenList(open), OnClosedList(closed) {}
    CAStarState(const CAStarState& other)
        : ParentState(other.ParentState), Node(other.Node), CostToThisState(other.CostToThisState),
          TotalCost(other.TotalCost), OnOpenList(other.OnOpenList), OnClosedList(other.OnClosedList)
    {
        ++NoOfStates;
        if (OnOpenList) ++NoOfStatesOnOpenList;
        if (OnClosedList) ++NoOfStatesOnClosedList;
    }
};
long CAStarState::NoOfStates = 0;
long CAStarState::NoOfStatesOnOpenList = 0;
long CAStarState::NoOfStatesOnClosedList = 0;
static void ResetCounts() { CAStarState::NoOfStates = 0; CAStarState::NoOfStatesOnOpenList = 0; CAStarState::NoOfStatesOnClosedList = 0; }
namespace stlp {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& a, const _T2& b) : first(a), second(b) {}
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& p) : first(p.first), second(p.second) {}
    pair(const pair& p) : first(p.first), second(p.second) {}
};
}

int main()
{
    CANavQuadTreeNode node; node.Id = 5;
    stlp::pair<const long, CAStarState> a(77, CAStarState(&node, true, false));
    ResetCounts();
    stlp::pair<const long, CAStarState> b(a);
    if (!(b.first == 77 && b.second.Node == &node && Near(b.second.CostToThisState, 1.5f) && Near(b.second.TotalCost, 7.25f) && b.second.OnOpenList && !b.second.OnClosedList && CAStarState::NoOfStates == 1 && CAStarState::NoOfStatesOnOpenList == 1 && CAStarState::NoOfStatesOnClosedList == 0)) {
        std::printf("00a20670_TEST FAIL\n");
        return 1;
    }
    std::printf("00a20670_TEST PASS\n");
    return 0;
}