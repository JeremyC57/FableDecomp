#include <cstdio>
#include <cstdlib>
struct X { void* b; ~X() { if (b) std::free(b); } };
static void Kill(X* x) { delete x; }
int main()
{
    X* x = new X; x->b = std::malloc(4); Kill(x);

    std::printf("00567520_TEST PASS\n");
    return 0;
}
