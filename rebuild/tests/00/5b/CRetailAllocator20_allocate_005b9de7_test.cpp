#include <cstdio>
#include <cstdlib>
struct A { struct e { unsigned char b[12]; }; e* alloc(unsigned long n, const void*) { return n ? (e*)std::malloc(n * sizeof(e)) : 0; } };
int main()
{
    A a; e_check: if (a.alloc(0, 0)) { std::printf("005b9de7_TEST FAIL\n"); return 1; }
    A::e* p = a.alloc(2, 0); if (!p) { std::printf("005b9de7_TEST FAIL\n"); return 1; }
    std::free(p);

    std::printf("005b9de7_TEST PASS\n");
    return 0;
}