// Test for 0x00A20410 pair<const long, CNavigatorAStar::CAStarState>::pair(key, state): copies the key and the state and the
// state copy bumps the live-state counters (open/closed only when the copied flags are set).
#include <stdio.h>
class CNavigatorAStar {
public:
    class CAStarState {
    public:
        CAStarState() { }
        CAStarState(const CAStarState& o) : ParentState(o.ParentState), Node(o.Node), CostToThisState(o.CostToThisState),
            TotalCost(o.TotalCost), OnOpenList(o.OnOpenList), OnClosedList(o.OnClosedList)
        { ++NoOfStates; if (OnOpenList) ++NoOfStatesOnOpenList; if (OnClosedList) ++NoOfStatesOnClosedList; }
        static int NoOfStates, NoOfStatesOnOpenList, NoOfStatesOnClosedList;
        CAStarState* ParentState; const void* Node; float CostToThisState; float TotalCost; bool OnOpenList; bool OnClosedList;
    };
};
int CNavigatorAStar::CAStarState::NoOfStates = 0;
int CNavigatorAStar::CAStarState::NoOfStatesOnOpenList = 0;
int CNavigatorAStar::CAStarState::NoOfStatesOnClosedList = 0;
template <class T1, class T2> struct Pair { T1 first; T2 second; Pair(const T1& a, const T2& b) : first(a), second(b) { } };
int main()
{
    typedef CNavigatorAStar::CAStarState S;
    S s; s.ParentState = 0; s.Node = &s; s.CostToThisState = 1.5f; s.TotalCost = 4.0f; s.OnOpenList = true; s.OnClosedList = false;
    Pair<const int, S> p(42, s);
    bool ok = p.first == 42 && p.second.Node == &s && p.second.TotalCost == 4.0f && p.second.OnOpenList &&
              S::NoOfStates == 1 && S::NoOfStatesOnOpenList == 1 && S::NoOfStatesOnClosedList == 0;
    s.OnOpenList = false; s.OnClosedList = true;
    Pair<const int, S> q(7, s);
    if (S::NoOfStates != 2 || S::NoOfStatesOnOpenList != 1 || S::NoOfStatesOnClosedList != 1) ok = false;
    puts(ok ? "00a20410_TEST PASS" : "00a20410_TEST FAIL");
    return ok ? 0 : 1;
}