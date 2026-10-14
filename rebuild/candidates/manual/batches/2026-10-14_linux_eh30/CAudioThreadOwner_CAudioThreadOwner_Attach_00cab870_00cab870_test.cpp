#include <cstdio>
#include <new>
static int g_enter = 0, g_leave = 0, g_dup = 0, g_fb = 0; static bool g_dupok = true; static bool g_ready = true; static unsigned long g_tid = 5; static void* g_target = 0;
static void En(void* s) { ++g_enter; } static void Le(void* s) { ++g_leave; } static unsigned long Tid() { return g_tid; } static void* Proc() { return (void*)1; } static int Dup(void* a, void* b, void* c, void** d, unsigned long acc, int inh, unsigned long opt) { ++g_dup; g_target = b; if (!g_dupok) return 0; *d = (void*)0x77; return 1; } static long LastErr() { return (long)0x80070005; }
struct Sl { void* s; Sl(void* x) : s(x) { En(s); } ~Sl() { Le(s); } };
struct O { virtual bool IsReady() { return g_ready; } unsigned char pad[0xa0]; unsigned char sec[0x1c]; void* h; long att; unsigned long tid; void Fallback() { ++g_fb; } long Attach(void* t) { Sl l(sec); if (att) { if (tid == Tid()) return (long)0x80040293; return (long)0x80040294; } if (!Dup(Proc(), t, Proc(), &h, 2, 0, 0)) return LastErr(); att = 1; tid = Tid(); if (!IsReady()) Fallback(); return 0; } };
static bool Check() { O* o = new O; o->att = 0; o->h = 0; o->tid = 0; int t; if (o->Attach(&t) != 0 || o->att != 1 || o->tid != 5 || o->h != (void*)0x77 || g_target != &t || g_fb != 0) return false; if (o->Attach(&t) != (long)0x80040293) return false; g_tid = 6; if (o->Attach(&t) != (long)0x80040294) return false; o->att = 0; g_dupok = false; if (o->Attach(&t) != (long)0x80070005 || o->att != 0) return false; g_dupok = true; g_ready = false; if (o->Attach(&t) != 0 || g_fb != 1 || o->tid != 6) return false; delete o; return g_enter == 5 && g_leave == 5 && g_dup == 3; }

int main()
{
    if (!Check()) { std::printf("00cab870_TEST FAIL\n"); return 1; }
    std::printf("00cab870_TEST PASS\n");
    return 0;
}
