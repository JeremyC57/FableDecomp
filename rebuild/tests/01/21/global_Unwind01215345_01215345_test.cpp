#include <cstdio>
static float g_d = 0; struct Sc { float G() { return 2.0f; } }; struct O { unsigned char p[0x4c]; Sc s; }; struct V { virtual void v() {} float d; V(float x) : d(x) {} ~V() {} }; struct Tg { virtual void a() {} virtual void b() {} virtual void c() {} virtual void d() {} virtual void e() {} virtual void f() {} virtual void g() {} virtual void h() {} virtual void i() {} virtual void j() {} virtual void Vt(V* v) { g_d = v->d; } }; struct X { O* o; unsigned char p[0x4c]; Tg t; float D() { return 3.0f; } void F() { float s = o->s.G(); V v(D() * s); t.Vt(&v); } };
static bool Check() { O o; X x; x.o = &o; x.F(); return g_d == 6.0f; }

int main()
{
    if (!Check()) { std::printf("00c44dc0_TEST FAIL\n"); return 1; }
    std::printf("01215345_TEST PASS\n");
    return 0;
}