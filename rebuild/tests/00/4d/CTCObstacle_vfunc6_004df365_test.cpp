// Behaviour test for CTCObstacle::vfunc_6 @ 0x004DF365: the interface wraps the parent thing, the agent is built on it.
#include <cstdio>
struct CThing { long id; };
struct CObstacleNavigatorExternalInterface {
    CThing* m_pThing; void* m_first; void* m_last; void* m_end;
    CObstacleNavigatorExternalInterface(CThing* thing) : m_pThing(thing), m_first(0), m_last(0), m_end(0) {}
};
struct CSteeringAgent {
    CObstacleNavigatorExternalInterface* m_pInterface; bool m_flag;
    CSteeringAgent(CObstacleNavigatorExternalInterface* i, bool flag) : m_pInterface(i), m_flag(flag) {}
};
struct CTCObstacle {
    CThing* ParentThing;
    CSteeringAgent* SteeringAgent;
    CObstacleNavigatorExternalInterface* PInterface;
    void vfunc_6()
    {
        PInterface = new CObstacleNavigatorExternalInterface(ParentThing);
        SteeringAgent = new CSteeringAgent(PInterface, true);
    }
};
int main()
{
    CThing t; t.id = 6;
    CTCObstacle o; o.ParentThing = &t; o.SteeringAgent = 0; o.PInterface = 0;
    o.vfunc_6();
    bool ok = o.PInterface && o.PInterface->m_pThing == &t && o.PInterface->m_first == 0 && o.PInterface->m_end == 0 &&
              o.SteeringAgent && o.SteeringAgent->m_pInterface == o.PInterface && o.SteeringAgent->m_flag;
    delete o.SteeringAgent; delete o.PInterface;
    if (!ok) { std::printf("004df365_TEST FAIL\n"); return 1; }
    std::printf("004df365_TEST PASS\n");
    return 0;
}