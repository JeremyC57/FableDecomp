#include <cstdio>
struct T { bool Q() { return false; } }; struct X { unsigned char p[4]; T* t; bool G() { if (t) return t->Q(); return true; } };
int main()
{
    X x; x.t = 0; if (x.G() != true) { std::printf("00a23ff0_TEST FAIL\n"); return 1; }
    T t; x.t = &t; if (x.G()) { std::printf("00a23ff0_TEST FAIL\n"); return 1; }

    std::printf("00a23ff0_TEST PASS\n");
    return 0;
}