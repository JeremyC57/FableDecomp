#include <cstdio>
#include <cstring>
#include <cstdlib>

static char g_seq[64] = ""; static void* g_ctorArg = 0; static const void* g_action = 0;
struct CActionBase { virtual ~CActionBase() { } };
struct CCreature { virtual ~CCreature() { } virtual void SetUnderground(bool u) { std::strcat(g_seq, u ? "U1" : "U0"); }
    bool SetCurrentAction(const CActionBase& a) { std::strcat(g_seq, "S"); g_action = &a; return true; } };
struct CGoUnderground : CActionBase { unsigned char body[0x110]; CGoUnderground(CCreature* c) { std::strcat(g_seq, "C"); g_ctorArg = c; } ~CGoUnderground() { std::strcat(g_seq, "D"); } };
struct CModeBase { virtual ~CModeBase() { } void* thing; };
struct CMode : CModeBase { unsigned char pad[0x70]; CCreature* creature; void Recover() { creature->SetUnderground(true); creature->SetCurrentAction(CGoUnderground(creature)); } };
int main()
{
    CCreature c; CMode m; m.creature = &c; m.Recover();
    if (std::strcmp(g_seq, "U1CSD") || g_ctorArg != &c || g_action == 0) { std::printf("008df640_TEST FAIL\n"); return 1; }
    if (sizeof(CGoUnderground) != sizeof(void*) + 0x110) { std::printf("008df640_TEST FAIL\n"); return 1; }
    std::printf("008df640_TEST PASS\n");
    return 0;
}
