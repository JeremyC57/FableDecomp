#include <cstdio>
#include <cstring>
#include <cmath>

static bool g_enabled; static void* g_first; static void* g_second;
void SetCurrentPair(void* a, void* b) { if (g_enabled) { g_first = a; g_second = b; } }
int main() {
    int x, y; g_enabled = false; SetCurrentPair(&x, &y); if (g_first || g_second) { std::printf("00cb2ee0_TEST FAIL\n"); return 1; }
    g_enabled = true; SetCurrentPair(&x, &y); if (g_first != &x || g_second != &y) { std::printf("00cb2ee0_TEST FAIL\n"); return 1; }
    std::printf("00cb2ee0_TEST PASS\n"); return 0;
}