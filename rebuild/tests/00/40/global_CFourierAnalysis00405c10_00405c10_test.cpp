#include <cstdio>
static void* __cdecl R(void* a0, void* a1) { return a1; }
int main()
{
    int i; if (R(&i, &i) != &i) { std::printf("00405c10_TEST FAIL\n"); return 1; }

    std::printf("00405c10_TEST PASS\n");
    return 0;
}