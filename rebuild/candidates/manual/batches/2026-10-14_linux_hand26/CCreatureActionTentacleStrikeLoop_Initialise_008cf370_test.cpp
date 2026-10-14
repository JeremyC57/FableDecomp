#include <cstdio>
#include <cstring>
static int g_base = 0; static char g_name[64]; static long g_frame = 42;
long GetWorldFrame() { return g_frame; }
struct Str { const char* p; Str(const char* t, long l) : p(t) {} ~Str() {} };
struct Creature { unsigned char pad[0xb4]; long m_targetId; };
struct Ptr { virtual ~Ptr() {} Creature* p; Creature* GetPItem() const { return p; } };
struct Base { virtual ~Base() {} virtual void Initialise() { ++g_base; } Ptr PCreature; void SetAnimationName(const Str& s) { std::strcpy(g_name, s.p); } };
struct Act : Base { long m_start, m_118, m_target;
    void Initialise() { { Str n("TENTACLE_SMASH_LOOP", -1); SetAnimationName(n); } if (m_start == -1) m_start = GetWorldFrame(); m_target = PCreature.GetPItem()->m_targetId; Base::Initialise(); } };
int main()
{
    Creature c; c.m_targetId = 77; Act a; a.PCreature.p = &c; a.m_start = -1; a.Initialise();
    if (g_base != 1 || a.m_start != 42 || a.m_target != 77 || std::strcmp(g_name, "TENTACLE_SMASH_LOOP") != 0) { std::printf("008cf370_TEST FAIL\n"); return 1; }
    g_frame = 50; a.Initialise();
    if (a.m_start != 42) { std::printf("008cf370_TEST FAIL\n"); return 1; }
    std::printf("008cf370_TEST PASS\n");
    return 0;
}
