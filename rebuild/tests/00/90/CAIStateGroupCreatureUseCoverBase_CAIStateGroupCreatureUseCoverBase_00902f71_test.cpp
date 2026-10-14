#include <cstdio>
#include <cstring>
static char g_log[512]; static int g_n;
static void Log(const char* s) { int l = (int)std::strlen(s); if (g_n + l + 1 < (int)sizeof(g_log)) { std::memcpy(g_log + g_n, s, l); g_n += l; g_log[g_n++] = ' '; g_log[g_n] = 0; } }
static int Check(const char* addr, const char* want) { if (std::strcmp(g_log, want) != 0) { std::printf("%s_TEST FAIL\n%s\n", addr, g_log); return 1; } std::printf("%s_TEST PASS\n", addr); return 0; }
struct CBaseIntelligentPointer { virtual void v00() {} void* m_pItem; ~CBaseIntelligentPointer() { Log("cover-ptr"); } };
struct CIntelligentPointer : CBaseIntelligentPointer { };
struct CThingPointerVector { void* m_first; void* m_last; void* m_end; ~CThingPointerVector() { Log("list"); } };
struct CAIStateGroup_EnemyCreatureBase { virtual void v00() {} ~CAIStateGroup_EnemyCreatureBase() { Log("enemy-base"); } unsigned char _pad[0x18]; };
#pragma pack(push, 1)
struct CAIStateGroup_CreatureUseCoverBase : CAIStateGroup_EnemyCreatureBase {
    bool HasDiveAnim; bool HasJumpOutAnims; bool TimingCritical; CIntelligentPointer PCoverThing; unsigned char _pad_0x27; CThingPointerVector CoverList;
    ~CAIStateGroup_CreatureUseCoverBase() { }
};
#pragma pack(pop)
int main()
{
    { CAIStateGroup_CreatureUseCoverBase g; (void)g; }
    return Check("00902f71", "list cover-ptr enemy-base ");
}