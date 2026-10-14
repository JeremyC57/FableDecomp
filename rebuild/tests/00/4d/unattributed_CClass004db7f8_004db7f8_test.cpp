#include <cstdio>
#include <cstdlib>
struct X { void* p; ~X() { if (p) std::free(p); } };
int main()
{
    { X x; x.p = std::malloc(4); } { X y; y.p = 0; }

    std::printf("004db7f8_TEST PASS\n");
    return 0;
}