#include <cstdio>
struct T { unsigned char p[0x40]; unsigned char v; }; struct X { virtual void v00() {} virtual unsigned char Get() { return t->v; } unsigned char p[0x34]; T* t; };
int main()
{
    T t; t.v = 9; X x; x.t = &t; if (x.Get() != 9) { std::printf("006bf890_TEST FAIL\n"); return 1; }

    std::printf("006bf890_TEST PASS\n");
    return 0;
}
