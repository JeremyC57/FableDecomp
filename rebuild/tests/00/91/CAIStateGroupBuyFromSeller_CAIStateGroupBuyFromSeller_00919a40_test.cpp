#include <cstdio>
#include <cstring>
static char g_log[512]; static int g_n;
static void Log(const char* s) { int l = (int)std::strlen(s); if (g_n + l + 1 < (int)sizeof(g_log)) { std::memcpy(g_log + g_n, s, l); g_n += l; g_log[g_n++] = ' '; g_log[g_n] = 0; } }
static int Check(const char* addr, const char* want) { if (std::strcmp(g_log, want) != 0) { std::printf("%s_TEST FAIL\n%s\n", addr, g_log); return 1; } std::printf("%s_TEST PASS\n", addr); return 0; }
namespace N_0099a2d0 { struct CBase { virtual ~CBase() {} CBase() { Log("CBase"); } }; } using N_0099a2d0::CBase;
struct CCharString { char* m_pData; CCharString() : m_pData(0) { Log("Name"); } };
struct CAIStateGroupBase : CBase { void* PBrain; long ScriptAIPriority; CCharString Name; bool Terminated; unsigned char Priority; bool Enabled; CAIStateGroupBase() : Name() { } };
struct CAIStateGroup_BuyFromSeller : CAIStateGroupBase { long FrameLastBought; CAIStateGroup_BuyFromSeller() : FrameLastBought(0) { } };
int main()
{
    CAIStateGroup_BuyFromSeller* g = new CAIStateGroup_BuyFromSeller;
    if (g->FrameLastBought != 0 || g->Name.m_pData != 0) return Check("00919a40", "x");
    delete g;
    return Check("00919a40", "CBase Name ");
}