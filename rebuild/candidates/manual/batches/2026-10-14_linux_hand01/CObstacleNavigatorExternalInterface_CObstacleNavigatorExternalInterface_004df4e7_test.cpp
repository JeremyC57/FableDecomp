#include <cstdio>
struct CThing; struct CSteeringAgent;
struct CObstacleOwner { void* m_vptr; CThing* m_pThing; };
struct CRetailVectorAgent { const CSteeringAgent** m_first; const CSteeringAgent** m_last; const CSteeringAgent** m_end; CRetailVectorAgent() : m_first(0), m_last(0), m_end(0) { } };
struct CObstacleNavigatorExternalInterface { virtual void v00() { } CThing* Obstacle; CRetailVectorAgent DummyAgentList;
    CObstacleNavigatorExternalInterface(const CObstacleOwner& o) : Obstacle(o.m_pThing) { } };
int main()
{
    CObstacleOwner o; o.m_pThing = (CThing*)0x1234;
    CObstacleNavigatorExternalInterface i(o);
    if (sizeof(i) != 0x14 || i.Obstacle != (CThing*)0x1234 || i.DummyAgentList.m_first != 0 || i.DummyAgentList.m_last != 0 || i.DummyAgentList.m_end != 0) { std::printf("004df4e7_TEST FAIL\n"); return 1; }
    std::printf("004df4e7_TEST PASS\n"); return 0;
}
