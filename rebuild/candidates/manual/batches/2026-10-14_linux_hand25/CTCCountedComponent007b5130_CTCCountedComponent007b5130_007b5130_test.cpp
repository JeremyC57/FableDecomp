#include <cstdio>
static int g_order = 0, g_o14 = 0, g_oc = 0, g_base = 0, g_del = 0;
struct Obj { virtual void Destroy() {} virtual void Delete() { ++g_del; } long refs; };
struct Cp { Obj* p; ~Cp() { Obj* o = p; if (o) { if (--o->refs == 0) o->Delete(); p = 0; } g_oc = ++g_order; } };
struct Buf { unsigned char b[0x10]; ~Buf() { g_o14 = ++g_order; } };
struct Base { virtual ~Base() { g_base = ++g_order; } unsigned char pad[8]; };
struct C : Base { Cp action; long m10; Buf buffer; };
int main()
{
    Obj o; o.refs = 2; { C c; c.action.p = &o; }
    if (g_o14 != 1 || g_oc != 2 || g_base != 3 || g_del != 0 || o.refs != 1) { std::printf("007b5130_TEST FAIL\n"); return 1; }
    std::printf("007b5130_TEST PASS\n");
    return 0;
}
