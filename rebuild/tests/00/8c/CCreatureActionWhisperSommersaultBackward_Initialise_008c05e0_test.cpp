#include <cstdio>
#include <cstring>
static int g_base = 0; static char g_name[64];
struct Str { const char* p; Str(const char* t, long l) : p(t) {} ~Str() {} };
struct Base { virtual ~Base() {} virtual void Initialise() { ++g_base; } unsigned char body[0x110]; void SetAnimationName(const Str& s) { std::strcpy(g_name, s.p); } };
struct Act : Base { unsigned char m_flag; void Initialise() { { Str n("MOVEMENT_LONG_BACK", -1); SetAnimationName(n); } m_flag = 0; Base::Initialise(); } };
int main()
{
    Act a; a.m_flag = 9; a.Initialise();
    if (g_base != 1 || a.m_flag != 0 || std::strcmp(g_name, "MOVEMENT_LONG_BACK") != 0) { std::printf("008c05e0_TEST FAIL\n"); return 1; }
    std::printf("008c05e0_TEST PASS\n");
    return 0;
}