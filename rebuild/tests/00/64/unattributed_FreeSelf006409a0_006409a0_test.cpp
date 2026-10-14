#include <cstdio>
struct X { void F() { ::operator delete(this); } };
int main()
{
    X* x = new X; x->F();

    std::printf("006409a0_TEST PASS\n");
    return 0;
}