#include <cstdio>
#include <cstring>
static char g_log[512]; static int g_n;
static void Log(const char* s) { int l = (int)std::strlen(s); if (g_n + l + 1 < (int)sizeof(g_log)) { std::memcpy(g_log + g_n, s, l); g_n += l; g_log[g_n++] = ' '; g_log[g_n] = 0; } }
static int Check(const char* addr, const char* want) { if (std::strcmp(g_log, want) != 0) { std::printf("%s_TEST FAIL\n%s\n", addr, g_log); return 1; } std::printf("%s_TEST PASS\n", addr); return 0; }
struct CBaseIntelligentPointer { virtual void v00() {} void* m_pItem; ~CBaseIntelligentPointer() { char b[16]; std::sprintf(b, "ptr%d", (int)(long)m_pItem); Log(b); } };
struct CIntelligentPointer : CBaseIntelligentPointer { };
struct CAIStateGroupBase { virtual void v00() {} ~CAIStateGroupBase() { Log("base"); } unsigned char _pad[0x10]; };
struct CAIStateGroup_CarryCrateAndFollow : CAIStateGroupBase {
    CIntelligentPointer PCrate; CIntelligentPointer PPreviouslyCarriedItem; CIntelligentPointer PThingToFollow; CIntelligentPointer PCarrier;
    ~CAIStateGroup_CarryCrateAndFollow() { }
};
int main()
{
    { CAIStateGroup_CarryCrateAndFollow g; g.PCrate.m_pItem = (void*)1; g.PPreviouslyCarriedItem.m_pItem = (void*)2; g.PThingToFollow.m_pItem = (void*)3; g.PCarrier.m_pItem = (void*)4; }
    return Check("009017f2", "ptr4 ptr3 ptr2 ptr1 base ");
}
