#include <cstdio>
#include <cstring>
static int g_base = 0; static char g_name[64]; static long g_flag = 0;
struct Str { const char* p; Str(const char* t, long l) : p(t) {} ~Str() {} };
struct Creature { virtual ~Creature() {} virtual void SetUnderground(long f) { g_flag = f; } };
struct Ptr { virtual ~Ptr() {} Creature* p; Creature* GetPItem() const { return p; } };
struct Base { virtual ~Base() {} virtual void Initialise() { ++g_base; } Ptr PCreature; void SetAnimationName(const Str& s) { std::strcpy(g_name, s.p); } };
struct Act : Base { void Initialise() { { Str n("TENTACLE_RISE_FROM_SURFACE", -1); SetAnimationName(n); } PCreature.GetPItem()->SetUnderground(1); Base::Initialise(); } };
int main()
{
    Creature c; Act a; a.PCreature.p = &c; a.Initialise();
    if (g_base != 1 || g_flag != 1 || std::strcmp(g_name, "TENTACLE_RISE_FROM_SURFACE") != 0) { std::printf("008cf320_TEST FAIL\n"); return 1; }
    std::printf("008cf320_TEST PASS\n");
    return 0;
}