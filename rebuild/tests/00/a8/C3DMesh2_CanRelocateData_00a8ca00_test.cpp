#include <cstdio>
struct M { unsigned char pad[0xe5]; bool locked; bool Can(long) { return !locked; } };
int main()
{
    M m; m.locked = false; if (!m.Can(0)) { std::printf("00a8ca00_TEST FAIL\n"); return 1; }
    m.locked = true; if (m.Can(0)) { std::printf("00a8ca00_TEST FAIL\n"); return 1; }

    std::printf("00a8ca00_TEST PASS\n");
    return 0;
}