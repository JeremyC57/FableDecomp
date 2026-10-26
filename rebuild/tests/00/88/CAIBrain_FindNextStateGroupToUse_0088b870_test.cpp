#include <cstdio>
struct CAIStateGroupBaseCalls { unsigned char _pad_0x00[0x11]; unsigned char Priority; bool Enabled; bool m_runnable; bool IsEnabledAndRunnable() { return Enabled && m_runnable; } };
struct CStateGroupEntry { long Key; CAIStateGroupBaseCalls* Group; };
struct CStateGroupVectorMap { CStateGroupEntry* Begin; CStateGroupEntry* End; };
class CAIBrain { public: unsigned char _pad[0x28]; CStateGroupVectorMap StateGroupList; CAIStateGroupBaseCalls* FindNextStateGroupToUse(long priority); };
CAIStateGroupBaseCalls* CAIBrain::FindNextStateGroupToUse(long priority)
{
    CStateGroupVectorMap* groups = &StateGroupList;
    for (CStateGroupEntry* it = groups->Begin; it != groups->End; ++it) {
        CAIStateGroupBaseCalls* group = it->Group;
        if ((long)group->Priority <= priority) return 0;
        if (group->IsEnabledAndRunnable()) return group;
    }
    return 0;
}
int main()
{
    CAIStateGroupBaseCalls g[3]; g[0].Priority = 9; g[0].Enabled = false; g[0].m_runnable = true; g[1].Priority = 7; g[1].Enabled = true; g[1].m_runnable = true; g[2].Priority = 3; g[2].Enabled = true; g[2].m_runnable = true;
    CStateGroupEntry e[3] = { { 0, &g[0] }, { 1, &g[1] }, { 2, &g[2] } };
    CAIBrain b; b.StateGroupList.Begin = e; b.StateGroupList.End = e + 3;
    if (b.FindNextStateGroupToUse(5) != &g[1]) { std::printf("0088b870_TEST FAIL\n"); return 1; }
    if (b.FindNextStateGroupToUse(8) != 0) { std::printf("0088b870_TEST FAIL\n"); return 1; }
    g[1].Enabled = false; if (b.FindNextStateGroupToUse(2) != &g[2]) { std::printf("0088b870_TEST FAIL\n"); return 1; }
    b.StateGroupList.End = e; if (b.FindNextStateGroupToUse(0) != 0) { std::printf("0088b870_TEST FAIL\n"); return 1; }
    std::printf("0088b870_TEST PASS\n");
    return 0;
}