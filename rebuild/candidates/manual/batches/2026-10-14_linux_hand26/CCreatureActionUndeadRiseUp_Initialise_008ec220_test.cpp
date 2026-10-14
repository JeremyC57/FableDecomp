#include <cstdio>
static int g_base = 0; static long g_flag = 0; static long g_susp = 0;
struct Creature { virtual ~Creature() {} virtual void SetUnderground(long f) { g_flag = f; } void SetSuspendAnimation(bool s, long r) { g_susp = s ? 1 + r : 100; } };
struct Ptr { virtual ~Ptr() {} Creature* p; Creature* GetPItem() const { return p; } };
struct Base { virtual ~Base() {} virtual void Initialise() { ++g_base; } Ptr PCreature; };
struct Act : Base { void Initialise() { PCreature.GetPItem()->SetUnderground(1); Base::Initialise(); PCreature.GetPItem()->SetSuspendAnimation(true, 0); } };
int main()
{
    Creature c; Act a; a.PCreature.p = &c; a.Initialise();
    if (g_base != 1 || g_flag != 1 || g_susp != 1) { std::printf("008ec220_TEST FAIL\n"); return 1; }
    std::printf("008ec220_TEST PASS\n");
    return 0;
}
