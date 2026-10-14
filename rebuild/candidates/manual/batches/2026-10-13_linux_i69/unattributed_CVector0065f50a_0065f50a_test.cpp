#include <cstdio>
struct It { void* p; It(const It& o) : p(o.p) {} It(void* x) : p(x) {} }; struct Tg { Tg() {} };
static int g_n = 0; struct X { void C(It f, It l, const Tg&) { g_n = (int)((char*)l.p - (char*)f.p); } X(It f, It l) { C(f, l, Tg()); } };
int main()
{
    char buf[8]; X x(It(buf), It(buf + 5)); if (g_n != 5) { std::printf("0065f50a_TEST FAIL\n"); return 1; }

    std::printf("0065f50a_TEST PASS\n");
    return 0;
}
