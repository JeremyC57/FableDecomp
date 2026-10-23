#include <cstdio>
#include <new>
static int g_stop = 0, g_fin = 0, g_det = 0, g_rel = 0, g_sleep = 0, g_help = 0, g_o60 = 0, g_mesh = 0, g_part = 0, g_o5c = 0, g_node = 0, g_p3c = 0; static unsigned char g_flag = 1; static unsigned long g_ms = 0;
static void Sl(unsigned long ms) { ++g_sleep; g_ms = ms; }
struct Nd { unsigned char b[0x28]; ~Nd() { ++g_node; } }; struct Rt { virtual void v0() {} Nd* pn; ~Rt() {} }; struct Ct : Rt { Nd n; ~Ct() {} };
struct Pt { ~Pt() { ++g_part; } }; struct Ms { ~Ms() { ++g_mesh; } }; struct P3 { unsigned char b[0x20]; ~P3() { ++g_p3c; } }; struct Hp { ~Hp() { ++g_help; } }; struct I60 { virtual ~I60() { ++g_o60; } }; struct I5c { virtual void v0() {} virtual ~I5c() { ++g_o5c; } };
struct Bs { virtual ~Bs() {} };
struct X : Bs { Ct c; Pt* pp; Ms* pm; P3 p3; I5c* p5c; I60* p60; long m64; char* blk; Hp* ph; void Stop() { ++g_stop; } void Finish() { ++g_fin; } void Detach() { ++g_det; } void Release() { ++g_rel; } ~X() { Stop(); Finish(); if (ph) { delete ph; ph = 0; } Detach(); Sl(200); Release(); if (p60) { delete p60; p60 = 0; } delete blk; blk = 0; if (pm) delete pm; pm = 0; if (pp) delete pp; pp = 0; if (p5c) delete p5c; p5c = 0; g_flag = 0; } };
static bool Check() { X* x = new X; x->pp = new Pt; x->pm = new Ms; x->p5c = new I5c; x->p60 = new I60; x->blk = new char; x->ph = new Hp; delete x; return g_stop == 1 && g_fin == 1 && g_det == 1 && g_rel == 1 && g_sleep == 1 && g_ms == 200 && g_help == 1 && g_o60 == 1 && g_mesh == 1 && g_part == 1 && g_o5c == 1 && g_node == 1 && g_p3c == 1 && g_flag == 0; }

int main()
{
    if (!Check()) { std::printf("00c09380_TEST FAIL\n"); return 1; }
    std::printf("01210b91_TEST PASS\n");
    return 0;
}