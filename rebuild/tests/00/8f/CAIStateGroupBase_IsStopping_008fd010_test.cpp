#include <cstdio>
struct CAIBrain { void* __vftable; unsigned char _pad[1]; bool m_bStopping; };
struct CAIStateGroupBase { void* __vftable; CAIBrain* PBrain; unsigned char ScriptAIPriority[4]; void* Name_PStringData; bool Terminated; unsigned char Priority; bool Enabled; long IsStopping() const; };
long CAIStateGroupBase::IsStopping() const { return PBrain->m_bStopping || Terminated; }
int main()
{
    CAIBrain b = { 0, { 0 }, false }; CAIStateGroupBase g = { 0, &b, { 0 }, 0, false, 0, true };
    if (g.IsStopping() != 0) { std::printf("008fd010_TEST FAIL\n"); return 1; }
    b.m_bStopping = true; if (g.IsStopping() != 1) { std::printf("008fd010_TEST FAIL\n"); return 1; }
    b.m_bStopping = false; g.Terminated = true; if (g.IsStopping() != 1) { std::printf("008fd010_TEST FAIL\n"); return 1; }
    std::printf("008fd010_TEST PASS\n");
    return 0;
}