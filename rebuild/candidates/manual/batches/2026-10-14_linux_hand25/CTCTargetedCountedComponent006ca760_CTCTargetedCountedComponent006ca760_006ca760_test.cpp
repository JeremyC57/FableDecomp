#include <cstdio>
static int g_order = 0, g_o30 = 0, g_o1d = 0, g_o18 = 0, g_base = 0, g_del = 0;
struct Obj { virtual void Destroy() {} virtual void Delete() { ++g_del; } long refs; };
struct Cp { Obj* p; ~Cp() { Obj* o = p; if (o) { if (--o->refs == 0) o->Delete(); p = 0; } g_o18 = ++g_order; } };
struct Ip { virtual ~Ip() {} void* item; };
struct Ip30 : Ip { ~Ip30() { g_o30 = ++g_order; } }; struct Ip1d : Ip { ~Ip1d() { g_o1d = ++g_order; } };
struct Base { virtual ~Base() { g_base = ++g_order; } unsigned char pad[8]; };
#pragma pack(push, 1)
struct C : Base { unsigned char pad0[0xc]; Cp action; unsigned char pad1; Ip1d target; unsigned char pad2[0xb]; Ip30 source; };
#pragma pack(pop)
int main()
{
    Obj o; o.refs = 1; { C c; c.action.p = &o; }
    if (g_o30 != 1 || g_o1d != 2 || g_o18 != 3 || g_base != 4 || g_del != 1 || o.refs != 0) { std::printf("006ca760_TEST FAIL\n"); return 1; }
    std::printf("006ca760_TEST PASS\n");
    return 0;
}
