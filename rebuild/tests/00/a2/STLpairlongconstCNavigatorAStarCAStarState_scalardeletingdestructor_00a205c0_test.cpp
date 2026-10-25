// Link test for 0x00A205C0: std::pair<const long, CAStarState>'s scalar deleting destructor (reached through its Kill_00a205c0 helper)
// must count the CAStarState out of NoOfStates and, per flag, out of the open/closed-list counters, then free it.
#include <cstdio>
#include <cstdlib>
class CANavQuadTreeNode;
class CNavigatorAStar {
public:
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
    };
};
long CNavigatorAStar::CAStarState::NoOfStates = 10;
long CNavigatorAStar::CAStarState::NoOfStatesOnOpenList = 10;
long CNavigatorAStar::CAStarState::NoOfStatesOnClosedList = 10;
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
typedef std::pair<const long, CNavigatorAStar::CAStarState> SCountedInstance_00a205c0;
void Kill_00a205c0(SCountedInstance_00a205c0* object);   // from the source object: delete object
static SCountedInstance_00a205c0* Make(bool open, bool closed)
{
    SCountedInstance_00a205c0* p = (SCountedInstance_00a205c0*)operator new(sizeof(SCountedInstance_00a205c0));
    *(long*)&p->first = 7; p->second.OnOpenList = open; p->second.OnClosedList = closed;
    return p;
}
int main()
{
    typedef CNavigatorAStar::CAStarState St;
    int ok = sizeof(SCountedInstance_00a205c0) == 0x18;
    Kill_00a205c0(Make(true, false));
    ok = ok && St::NoOfStates == 9 && St::NoOfStatesOnOpenList == 9 && St::NoOfStatesOnClosedList == 10;
    Kill_00a205c0(Make(false, true));
    ok = ok && St::NoOfStates == 8 && St::NoOfStatesOnOpenList == 9 && St::NoOfStatesOnClosedList == 9;
    Kill_00a205c0(Make(true, true));
    ok = ok && St::NoOfStates == 7 && St::NoOfStatesOnOpenList == 8 && St::NoOfStatesOnClosedList == 8;
    Kill_00a205c0(0);
    ok = ok && St::NoOfStates == 7;
    if (!ok) { std::printf("00a205c0_TEST FAIL\n"); return 1; }
    std::printf("00a205c0_TEST PASS\n");
    return 0;
}
