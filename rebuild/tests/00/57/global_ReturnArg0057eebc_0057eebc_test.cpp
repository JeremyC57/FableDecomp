#include <cstdio>
static void* __cdecl R(void* a0) { return a0; }
int main()
{
    int i; if (R(&i) != &i) { std::printf("0057eebc_TEST FAIL\n"); return 1; }

    std::printf("0057eebc_TEST PASS\n");
    return 0;
}