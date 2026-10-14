#include <cstdio>
#include <cstring>
static int g_base = 0; static char g_name[64];
struct Str { const char* p; Str(const char* t, long l) : p(t) {} ~Str() {} };
struct Base { virtual ~Base() {} virtual void Initialise() { ++g_base; } unsigned char pad[0x0c]; long m_duration; long m_hit; void SetAnimationName(const Str& s) { std::strcpy(g_name, s.p); } };
struct Act : Base { void Initialise() { { Str n("SNAP_ATTACK_FAST", -1); SetAnimationName(n); } Base::Initialise(); m_hit = (long)(m_duration * 0.25f); } };
int main()
{
    Act a; a.m_duration = 30; a.m_hit = 0; a.Initialise();
    if (g_base != 1 || a.m_hit != 7 || std::strcmp(g_name, "SNAP_ATTACK_FAST") != 0) { std::printf("008f2bc0_TEST FAIL\n"); return 1; }
    std::printf("008f2bc0_TEST PASS\n");
    return 0;
}
