#include <cstdio>
struct X { unsigned char p[4]; void* v; bool G(void** o) { *o = v; return true; } };
int main()
{
    X x; int i; x.v = &i; void* o = 0; if (!x.G(&o) || o != &i) { std::printf("00b1ff70_TEST FAIL\n"); return 1; }

    std::printf("00b1ff70_TEST PASS\n");
    return 0;
}