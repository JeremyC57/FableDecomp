#include <cstdio>
#include <cstring>
static char g_log[512]; static int g_n;
static void Log(const char* s) { int l = (int)std::strlen(s); if (g_n + l + 1 < (int)sizeof(g_log)) { std::memcpy(g_log + g_n, s, l); g_n += l; g_log[g_n++] = ' '; g_log[g_n] = 0; } }
static int Check(const char* addr, const char* want) { if (std::strcmp(g_log, want) != 0) { std::printf("%s_TEST FAIL\n%s\n", addr, g_log); return 1; } std::printf("%s_TEST PASS\n", addr); return 0; }
struct CAIStateGroupBase { virtual void v00() {} CAIStateGroupBase() { Log("base"); } unsigned char _pad[0x10]; };
struct CBaseIntelligentPointer { virtual void v00() {} void* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { Log("ptr"); } };
struct CIntelligentPointerOfThing : CBaseIntelligentPointer { CIntelligentPointerOfThing() { } };
struct CIntelligentPointerOfCrowd : CBaseIntelligentPointer { CIntelligentPointerOfCrowd() { } };
struct CIntelligentPointerOfCrowdMember : CBaseIntelligentPointer { CIntelligentPointerOfCrowdMember() { } };
struct CAIStateGroup_MinionBehaviourCrowdBase : CAIStateGroupBase {
    CIntelligentPointerOfThing m_pTargetThing; CIntelligentPointerOfCrowd m_pCrowd; CIntelligentPointerOfCrowdMember m_pCrowdMember;
    CAIStateGroup_MinionBehaviourCrowdBase() : m_pTargetThing(), m_pCrowd(), m_pCrowdMember() { }
};
int main()
{
    CAIStateGroup_MinionBehaviourCrowdBase g;
    if (g.m_pTargetThing.m_pItem != 0 || g.m_pCrowd.m_pItem != 0 || g.m_pCrowdMember.m_pItem != 0) return Check("009009ea", "x");
    return Check("009009ea", "base ptr ptr ptr ");
}
