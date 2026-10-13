#include <cstdio>
static int g_r = 0, g_a = 0, g_b = 0, g_d = 0; static void __fastcall Destroy(void*) { ++g_d; }
struct Blk { long r; void (__fastcall* f)(void*); void* o; }; struct O { virtual void v00() {} }; struct X : O { unsigned char b[8]; X(void*, void*) {} };
struct P { O* o; Blk* c; P() : o(0), c(0) {} P& operator=(O* p) { R(p); return *this; } void R(O* p) { o = p; c = new Blk; c->r = 1; c->f = Destroy; c->o = p; ++g_r; }
    ~P() { if (c) { --c->r; if (c->r == 0) { c->f(c->o); delete c; } } o = 0; c = 0; } };
struct Base { virtual void v00() {} unsigned char pad[0x18]; void* a; void* t; void Add(const P&) { ++g_a; } virtual void OnCreate() { ++g_b; } };
struct D : Base { virtual void OnCreate(); }; void D::OnCreate() { P p; p = new X(a, t); Add(p); Base::OnCreate(); }
int main()
{
    D d; d.a = 0; d.t = 0; d.OnCreate(); if (g_r != 1 || g_a != 1 || g_b != 1 || g_d != 1) { std::printf("00906150_TEST FAIL\n"); return 1; }

    std::printf("00906150_TEST PASS\n");
    return 0;
}
