#include <cstdio>
static int g_root = 0, g_slot = 0, g_ptr = 0; struct Rt { virtual void v0() {} unsigned char b[0x1ec]; Rt() { ++g_root; } }; struct Sl { unsigned char b[0x30]; Sl() { ++g_slot; } ~Sl() {} }; struct Pt { unsigned char b[8]; Pt() { ++g_ptr; } };
struct X : Rt { Sl s; void* t; Pt a; unsigned char pad[4]; Pt c; X() : s(), a(), c() { t = 0; } };
static bool Check() { X x; return g_root == 1 && g_slot == 1 && g_ptr == 2 && x.t == 0; }

int main()
{
    if (!Check()) { std::printf("00c555e0_TEST FAIL\n"); return 1; }
    std::printf("012161f8_TEST PASS\n");
    return 0;
}