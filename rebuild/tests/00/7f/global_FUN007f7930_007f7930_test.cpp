#include <cstdio>
// Behaviour test for 007f7930: the member ignores its argument and returns *this.
struct It { int v; It& Op(long) { return *this; } };
int main()
{
    It a; a.v = 3;
    It& r = a.Op(99);
    if (&r != &a || r.v != 3) { std::printf("007f7930_TEST FAIL\n"); return 1; }
    std::printf("007f7930_TEST PASS\n");
    return 0;
}