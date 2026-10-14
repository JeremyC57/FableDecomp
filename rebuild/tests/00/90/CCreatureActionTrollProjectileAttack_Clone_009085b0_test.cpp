#include <cstdio>
static int g_copies = 0;
struct Str { const char* p; Str(const char* t) : p(t) {} Str(const Str& o) : p(o.p) {} };
struct Base { virtual ~Base() {} unsigned char body[0x110]; Base() {} Base(const Base& o) { ++g_copies; } };
struct Act : Base { unsigned char m_use; Str m_name; Act() : m_use(1), m_name("X") {} Base* Clone() { return new Act(*this); } };
int main()
{
    Act a; Act* b = (Act*)a.Clone();
    if (!b || g_copies != 1 || b->m_use != 1 || b->m_name.p != a.m_name.p) { std::printf("009085b0_TEST FAIL\n"); return 1; }
    delete b;
    std::printf("009085b0_TEST PASS\n");
    return 0;
}