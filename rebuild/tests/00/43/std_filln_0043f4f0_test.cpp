// behaviour test for 0x0043f4f0 std::fill_n<T*, unsigned, T>(first, n, value): assigns value to n consecutive 4-byte words and
// returns the end; n of zero writes nothing
#include <cstdio>
struct W { long v; };
static W* FillN(W* first, unsigned n, const W& val) { for (; n > 0; --n, ++first) *first = val; return first; }
int main()
{
    int fails = 0;
    W buf[6]; for (int i = 0; i < 6; ++i) buf[i].v = -1;
    W val; val.v = 42;
    W* e = FillN(buf + 1, 3, val);
    if (e != buf + 4) fails |= 1;
    for (int i = 0; i < 6; ++i) if (buf[i].v != ((i >= 1 && i < 4) ? 42 : -1)) fails |= 2;
    if (FillN(buf, 0, val) != buf || buf[0].v != -1) fails |= 4;
    if (fails) { std::printf("0043f4f0_TEST FAIL %d\n", fails); return 1; }
    std::printf("0043f4f0_TEST PASS\n");
    return 0;
}