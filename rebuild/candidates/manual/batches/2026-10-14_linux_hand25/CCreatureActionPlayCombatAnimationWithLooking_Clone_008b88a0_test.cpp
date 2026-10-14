#include <cstdio>
static int g_copies = 0;
struct Base { virtual ~Base() {} unsigned char body[0x110]; Base() {} Base(const Base& o) { ++g_copies; } };
struct Mid : Base { unsigned char a, b, c; };
struct Act : Mid { Act* Clone() { return new Act(*this); } };
int main()
{
    Act a; a.a = 1; a.b = 2; a.c = 3; Act* p = a.Clone();
    bool ok = p && p != &a && g_copies == 1 && p->a == 1 && p->b == 2 && p->c == 3; delete p;
    if (!ok) { std::printf("008b88a0_TEST FAIL\n"); return 1; }
    std::printf("008b88a0_TEST PASS\n");
    return 0;
}
