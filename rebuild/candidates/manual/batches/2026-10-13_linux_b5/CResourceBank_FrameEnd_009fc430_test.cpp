#include <cstdio>
struct L { unsigned char pad[0x2c]; unsigned long Frame; unsigned char pad2[8]; unsigned long Unloaded; };
struct B { void* vt; L a; L b; void FrameEnd() { a.Frame++; a.Unloaded = 0; b.Frame++; b.Unloaded = 0; } };
int main()
{
    B x; x.a.Frame = 5; x.a.Unloaded = 9; x.b.Frame = 1; x.b.Unloaded = 3; x.FrameEnd();
    if (x.a.Frame != 6 || x.a.Unloaded || x.b.Frame != 2 || x.b.Unloaded) { std::printf("009fc430_TEST FAIL\n"); return 1; }

    std::printf("009fc430_TEST PASS\n");
    return 0;
}
