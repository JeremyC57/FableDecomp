#include <cstdio>
struct T { unsigned char p[4]; float v; }; struct X { unsigned char q[4]; T* t; float s; float G() { return s * t->v; } };
int main()
{
    T t; t.v = 2.0f; X x; x.t = &t; x.s = 1.5f; if (x.G() != 3.0f) { std::printf("007bee10_TEST FAIL\n"); return 1; }

    std::printf("007bee10_TEST PASS\n");
    return 0;
}