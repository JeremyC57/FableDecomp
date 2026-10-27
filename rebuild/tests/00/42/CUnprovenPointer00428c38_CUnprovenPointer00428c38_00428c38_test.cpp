// behaviour test for 0x00428c38 one-pointer wrapper copy constructor: copies the single pointer member
#include <cstdio>
struct Ptr { void* Pointer; Ptr(void* p) : Pointer(p) { } Ptr(const Ptr& o) : Pointer(o.Pointer) { } };
int main()
{
    int fails = 0;
    int x = 0;
    Ptr a(&x);
    Ptr b(a);
    if (b.Pointer != &x) fails |= 1;
    Ptr c(0); Ptr d(c);
    if (d.Pointer != 0) fails |= 2;
    if (fails) { std::printf("00428c38_TEST FAIL %d\n", fails); return 1; }
    std::printf("00428c38_TEST PASS\n");
    return 0;
}