#include <cstdio>
// Behaviour test for 007eeb90: the setter stores its argument in the dword at +0x0C.
struct Owner { long a, b, c; long value; long after; void Set(long v) { value = v; } };
int main()
{
    Owner o; o.c = 3; o.value = 0; o.after = 5;
    o.Set(-42);
    int ok = (o.value == -42 && o.c == 3 && o.after == 5);
    if (!ok) { std::printf("007eeb90_TEST FAIL\n"); return 1; }
    std::printf("007eeb90_TEST PASS\n");
    return 0;
}