#include <cstdio>
// Behaviour model: false for an unknown name, else whether a matching message exists.
struct Msg { int type; long frame; long data; };
static bool find(const Msg* m, int n, int type, long f0, long f1, long data)
{
    for (int i = 0; i < n; ++i) if (m[i].type == type && m[i].frame >= f0 && m[i].frame <= f1 && m[i].data == data) return true;
    return false;
}
static bool is(long number, const Msg* m, int n) { if (number == 0) return false; return find(m, n, 0x25, 10, 20, number); }
int main()
{
    Msg m[2] = { { 0x25, 15, 3 }, { 0x25, 30, 4 } };
    if (is(0, m, 2) || !is(3, m, 2) || is(4, m, 2)) { std::printf("00893240_TEST FAIL\n"); return 1; }
    std::printf("00893240_TEST PASS\n");
    return 0;
}