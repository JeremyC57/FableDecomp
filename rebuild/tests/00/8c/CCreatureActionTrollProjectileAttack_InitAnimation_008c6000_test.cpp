#include <cstdio>
#include <cstring>
static char g_name[64];
struct Str { const char* p; Str(const char* t, long l) : p(t) {} ~Str() {} };
struct Base { virtual ~Base() {} unsigned char body[0x110]; void SetAnimationName(const Str& s) { std::strcpy(g_name, s.p); } };
struct Act : Base { unsigned char m_use; Str m_custom; Act() : m_custom("CUSTOM", -1) {}
    void InitAnimation() { if (m_use) { SetAnimationName(m_custom); } else { Str n("PROJECTILE_ATTACK", -1); SetAnimationName(n); } } };
int main()
{
    Act a; a.m_use = 0; a.InitAnimation();
    if (std::strcmp(g_name, "PROJECTILE_ATTACK") != 0) { std::printf("008c6000_TEST FAIL\n"); return 1; }
    a.m_use = 1; a.InitAnimation();
    if (std::strcmp(g_name, "CUSTOM") != 0) { std::printf("008c6000_TEST FAIL\n"); return 1; }
    std::printf("008c6000_TEST PASS\n");
    return 0;
}