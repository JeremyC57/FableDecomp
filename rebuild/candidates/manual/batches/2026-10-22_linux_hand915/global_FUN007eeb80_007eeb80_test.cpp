#include <cstdio>
// Behaviour test for 007eeb80: the getter returns the constant 0x4E.
struct Owner { long Get() { return 0x4E; } };
int main()
{
    Owner o;
    if (o.Get() != 78) { std::printf("007eeb80_TEST FAIL\n"); return 1; }
    std::printf("007eeb80_TEST PASS\n");
    return 0;
}
