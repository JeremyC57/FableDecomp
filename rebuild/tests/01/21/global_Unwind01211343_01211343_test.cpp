#include <cstdio>
static int g_a = 0, g_b = 0; struct VA { virtual void v() {} VA() {} ~VA() {} }; struct VB { virtual void v() {} VB() {} ~VB() {} }; struct Tg { virtual void a() {} virtual void b() {} virtual void c() {} virtual void d() {} virtual void e() {} virtual void f() {} virtual void g() {} virtual void h() {} virtual void i() {} virtual void j() {} virtual void V(VB*) { ++g_b; } }; struct X { virtual void a() {} virtual void b() {} virtual void c() {} virtual void d() {} virtual void e() {} virtual void f() {} virtual void g() {} virtual void h() {} virtual void i() {} virtual void j() {} virtual void V(VA*) { ++g_a; } unsigned char p[0xc]; Tg t; void F() { VA va; V(&va); VB vb; t.V(&vb); } };
static bool Check() { X x; x.F(); return g_a == 1 && g_b == 1; }

int main()
{
    if (!Check()) { std::printf("00c102b0_TEST FAIL\n"); return 1; }
    std::printf("01211343_TEST PASS\n");
    return 0;
}