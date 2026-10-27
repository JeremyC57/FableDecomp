// behaviour test for 0x004701AC: n copies of x land in consecutive slots and the end pointer is returned
#include <cstdio>
#include <cstring>
struct E { unsigned int w[6]; };
static E* Fill(E* first, unsigned int n, const E& x) { for (; n > 0; --n, ++first) *first = x; return first; }
int main()
{
    E buf[4]; std::memset(buf, 0, sizeof(buf)); E x; for (int i = 0; i < 6; ++i) x.w[i] = 5 + i;
    E* end = Fill(buf, 3, x);
    if (end != buf + 3 || buf[2].w[6 - 1] != 5 + 6 - 1 || buf[3].w[0] != 0 || Fill(buf, 0, x) != buf) { std::printf("004701ac_TEST FAIL\n"); return 1; }
    std::printf("004701ac_TEST PASS\n");
    return 0;
}