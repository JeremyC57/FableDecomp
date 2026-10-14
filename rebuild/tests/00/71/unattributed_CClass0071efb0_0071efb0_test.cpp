#include <cstdio>
#include <cstdlib>
struct X { void* p; ~X() { if (p) std::free(p); } };
int main()
{
    { X x; x.p = std::malloc(4); } { X y; y.p = 0; }

    std::printf("0071efb0_TEST PASS\n");
    return 0;
}