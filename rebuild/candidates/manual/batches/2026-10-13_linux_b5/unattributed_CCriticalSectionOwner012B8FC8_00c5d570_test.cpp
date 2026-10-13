#include <cstdio>
struct S; static void* g_p = 0; static void __stdcall Del(S* s) { g_p = s; }
struct O { virtual void v0() {} unsigned char cs[0x18]; ~O() { Del(reinterpret_cast<S*>(&cs[0])); } };
int main()
{
    O* o = new O; void* exp = &o->cs[0]; delete o; if (g_p != exp) { std::printf("00c5d570_TEST FAIL\n"); return 1; }

    std::printf("00c5d570_TEST PASS\n");
    return 0;
}
