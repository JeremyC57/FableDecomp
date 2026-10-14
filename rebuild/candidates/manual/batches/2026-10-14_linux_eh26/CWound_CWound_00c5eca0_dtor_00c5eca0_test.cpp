#include <cstdio>
#include <new>
static int g_close = 0, g_del = 0, g_kill = 0; static void Close(void* h) { ++g_close; } static void Del(void* s) { ++g_del; }
struct Hd { virtual void v0() {} void* h; void Close_() { if (h != (void*)-1) { Close(h); h = (void*)-1; } } ~Hd() { Close_(); } }; struct Hx : Hd { virtual void v4() {} ~Hx() {} }; struct Ev { virtual void v0() {} void* h; ~Ev() { if (h) { Close(h); h = 0; } } }; struct Mx { virtual void v0() {} void* h; ~Mx() { if (h) { Close(h); h = 0; } } }; struct Sc { virtual void v0() {} unsigned char s[0x18]; ~Sc() { Del(s); } }; struct Bs { virtual ~Bs() {} };
struct W : Bs { Hd m4; Hx mc; unsigned char p[0x18]; Ev m2c; Mx m34; unsigned char q[4]; Sc m40; virtual void v4() {} virtual void v8() {} virtual void vc() {} virtual void v10() {} virtual void Kill() { ++g_kill; } void NK() { Kill(); } virtual ~W() { NK(); m4.Close_(); } };
static bool Check() { int h; W* w = new W; w->m4.h = &h; w->mc.h = (void*)-1; w->m2c.h = &h; w->m34.h = 0; delete w; return g_kill == 1 && g_close == 2 && g_del == 1; }

int main()
{
    if (!Check()) { std::printf("00c5eca0_TEST FAIL\n"); return 1; }
    std::printf("00c5eca0_TEST PASS\n");
    return 0;
}
