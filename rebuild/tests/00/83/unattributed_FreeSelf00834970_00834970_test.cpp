#include <cstdio>
struct X { void F() { ::operator delete(this); } };
int main()
{
    X* x = new X; x->F();

    std::printf("00834970_TEST PASS\n");
    return 0;
}