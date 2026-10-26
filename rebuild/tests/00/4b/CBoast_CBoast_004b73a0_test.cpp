#include <stdio.h>
static int g_cs, g_ws;
class CCharString { public: void* m_pStringData; CCharString() : m_pStringData(0) {} CCharString(const CCharString& o) : m_pStringData(o.m_pStringData) { ++g_cs; } };
class CWideString { public: void* m_pStringData; CWideString() : m_pStringData(0) {} CWideString(const CWideString& o) : m_pStringData(o.m_pStringData) { ++g_ws; } };
struct CBoast {
    long ID; long Cost; long Reward; CCharString ScriptName; CWideString Text; CWideString Title; bool Evil; bool Failed; bool Completed; long GroupIndex;
    CBoast() {}
    CBoast(const CBoast& other);
};
CBoast::CBoast(const CBoast& other)
    : ID(other.ID), Cost(other.Cost), Reward(other.Reward), ScriptName(other.ScriptName), Text(other.Text), Title(other.Title)
    , Evil(other.Evil), Failed(other.Failed), Completed(other.Completed), GroupIndex(other.GroupIndex) { }
int main()
{
    CBoast a; a.ID = 1; a.Cost = 2; a.Reward = 3; a.ScriptName.m_pStringData = (void*)4; a.Text.m_pStringData = (void*)5; a.Title.m_pStringData = (void*)6; a.Evil = true; a.Failed = false; a.Completed = true; a.GroupIndex = 8;
    CBoast b(a);
    bool ok = g_cs == 1 && g_ws == 2 && b.ID == 1 && b.Cost == 2 && b.Reward == 3 && b.ScriptName.m_pStringData == (void*)4 && b.Text.m_pStringData == (void*)5 && b.Title.m_pStringData == (void*)6 && b.Evil && !b.Failed && b.Completed && b.GroupIndex == 8 && sizeof(CBoast) == 32;
    if (!ok) { printf("004b73a0_TEST FAIL\n"); return 1; }
    printf("004b73a0_TEST PASS\n"); return 0;
}