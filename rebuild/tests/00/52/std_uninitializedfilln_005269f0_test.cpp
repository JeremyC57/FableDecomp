// Behaviour test for 005269f0 (plain C++98 model of the reconstructed logic).
#include <cstdio>
#include <new>
#include <new>
struct E16 { unsigned int W0, W1, W2, W3; };
static E16* fill_n(E16* first, unsigned int n, const E16& x) { E16* cur = first; for (; n > 0; --n, ++cur) new (cur) E16(x); return cur; }
int main() {
    E16 buf[5]; E16 v = { 1, 2, 3, 4 }; for (int i = 0; i < 5; ++i) buf[i].W0 = 0;
    E16* end = fill_n(buf, 3, v);
    if (end != buf + 3 || buf[2].W3 != 4 || buf[0].W1 != 2 || buf[3].W0 != 0) { std::printf("005269f0_TEST FAIL\n"); return 1; }
    if (fill_n(buf, 0, v) != buf) { std::printf("005269f0_TEST FAIL\n"); return 1; }
    { std::printf("005269f0_TEST PASS\n"); return 0; }
}