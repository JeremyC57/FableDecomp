#include <cstdio>
#include <cstring>
static char g_name[64]; static int g_init = 0; static long g_flag = -1; static int g_live = 0;
struct S { const char* p; S(const char* t, long) : p(t) { ++g_live; } ~S() { --g_live; } };
struct Body { virtual void SetSummoning(long f) { g_flag = f; } };
struct Cre { unsigned char pad[0x60]; Body* body; };
struct Ptr { virtual ~Ptr() {} Cre* item; Cre* GetPItem() const { return item; } };
struct Base { virtual ~Base() {} virtual void Initialise() { ++g_init; } Ptr PCreature; void SetAnimationName(const S& n) { std::strcpy(g_name, n.p); } };
struct Act : Base { void Initialise() { { S n("SUMMON_CREATURES_INTO", -1); SetAnimationName(n); } PCreature.GetPItem()->body->SetSummoning(1); Base::Initialise(); } };
int main()
{
    Body b; Cre c; c.body = &b; Act a; a.PCreature.item = &c; a.Initialise();
    if (g_init != 1 || g_flag != 1 || g_live != 0 || std::strcmp(g_name, "SUMMON_CREATURES_INTO")) { std::printf("008c9df0_TEST FAIL\n"); return 1; }
    std::printf("008c9df0_TEST PASS\n");
    return 0;
}