#include <cstdio>
static int g_n = 0; struct G { ~G() { ++g_n; } }; static G g;
int main()
{
    if (g_n != 0) { std::printf("0121a2dc_TEST FAIL\n"); return 1; }

    std::printf("0121a2dc_TEST PASS\n");
    return 0;
}