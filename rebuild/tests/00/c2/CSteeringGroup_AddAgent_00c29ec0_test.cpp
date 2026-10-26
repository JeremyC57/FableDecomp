#include <cstdio>
struct CSteeringGroup;
class CSteeringAgent { public: unsigned char _pad_0x00[0x30]; CSteeringGroup* m_pGroup; CSteeringAgent* m_pNextInGroup; };
struct CSteeringGroup {
    void* __vftable; long m_index; CSteeringAgent* m_pFirst;
    void AddAgent(CSteeringAgent* agent) { agent->m_pGroup = this; agent->m_pNextInGroup = m_pFirst; m_pFirst = agent; }
};
int main()
{
    CSteeringGroup g; g.m_pFirst = 0; CSteeringAgent a, b;
    g.AddAgent(&a); g.AddAgent(&b);
    bool ok = g.m_pFirst == &b && b.m_pNextInGroup == &a && a.m_pNextInGroup == 0 && a.m_pGroup == &g && b.m_pGroup == &g
        && (char*)&a.m_pGroup - (char*)&a == 0x30;
    if (!ok) { std::printf("00c29ec0_TEST FAIL\n"); return 1; }
    std::printf("00c29ec0_TEST PASS\n");
    return 0;
}
