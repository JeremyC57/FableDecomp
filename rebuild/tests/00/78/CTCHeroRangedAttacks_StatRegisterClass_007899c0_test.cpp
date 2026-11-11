#include <cstdio>
// Behaviour model: the body stores one constant into one global.
static float g_value = 0.0f;
static void Body() { g_value = -1.0f; }
int main()
{
    Body();
    if (g_value != -1.0f) { std::printf("007899c0_TEST FAIL\n"); return 1; }
    std::printf("007899c0_TEST PASS\n");
    return 0;
}