// behaviour test for 0x00BC9EB0: __uninitialized_fill_n copies x into n slots and returns first + n
#include <cstdio>
struct E { long a, b, c, d; };
static E* fill_n(E* first, unsigned int n, const E& x) { E* cur = first; for (; n > 0; --n, ++cur) *cur = x; return cur; }
int main()
{
    E buf[4]; E x; x.a = 1; x.b = 2; x.c = 3; x.d = 4;
    for (int i = 0; i < 4; ++i) buf[i].a = buf[i].b = buf[i].c = buf[i].d = 0;
    E* end = fill_n(buf, 3, x);
    int ok = end == buf + 3 && buf[2].d == 4 && buf[0].a == 1 && buf[3].a == 0 && fill_n(buf, 0, x) == buf;
    if (!ok) { std::printf("00bc9eb0_TEST FAIL\n"); return 1; }
    std::printf("00bc9eb0_TEST PASS\n");
    return 0;
}