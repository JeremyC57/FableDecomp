#include <cstdio>
static int g_copies = 0;
struct Base { virtual ~Base() {} unsigned char body[0x110]; Base() {} Base(const Base& o) { ++g_copies; } };
struct Mid : Base { unsigned char m_a; };
struct Act : Mid { unsigned char m_b; Act() { m_a = 3; m_b = 5; } Base* Clone() { return new Act(*this); } };
int main()
{
    Act a; Act* b = (Act*)a.Clone();
    if (!b || g_copies != 1 || b->m_a != 3 || b->m_b != 5) { std::printf("0074df80_TEST FAIL\n"); return 1; }
    delete b;
    std::printf("0074df80_TEST PASS\n");
    return 0;
}