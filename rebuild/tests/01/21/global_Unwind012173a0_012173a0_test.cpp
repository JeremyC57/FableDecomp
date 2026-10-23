#include <cstdio>
#include <new>
static int g_wait = 0, g_close = 0, g_del = 0, g_clear = 0, g_ld = 0, g_nd = 0; static void* g_waited = 0; static unsigned long Wait(void* h, unsigned long ms) { ++g_wait; g_waited = h; return ms == 0xFFFFFFFF ? 0 : 1; } static long Close(void* h) { ++g_close; return 1; } static void Del(void* s) { ++g_del; }
struct Nd { Nd* next; ~Nd() { ++g_nd; } }; struct Li { Nd* head; unsigned char b[0x14]; Nd* Remove(Nd** c) { Nd* n = *c; *c = n->next; head = *c; return n; } void Clear() { ++g_clear; } ~Li() { ++g_ld; } }; struct Cs { unsigned char s[0x18]; ~Cs() { Del(s); } };
struct X { long m0, m4; void* th; Li li; Cs cs; void* ev; ~X() { if (th) { Wait(th, 0xFFFFFFFF); Close(th); } Nd* c = li.head; while (c) delete li.Remove(&c); li.Clear(); if (ev) Close(ev); } };
static bool Check() { int t, e; X* x = new X; x->th = &t; x->ev = &e; Nd* a = new Nd; Nd* b = new Nd; a->next = b; b->next = 0; x->li.head = a; delete x; if (!(g_wait == 1 && g_waited == &t && g_close == 2 && g_nd == 2 && g_clear == 1 && g_del == 1 && g_ld == 1)) return false; X* y = new X; y->th = 0; y->ev = 0; y->li.head = 0; delete y; return g_wait == 1 && g_close == 2 && g_clear == 2 && g_del == 2; }

int main()
{
    if (!Check()) { std::printf("00cb0f10_TEST FAIL\n"); return 1; }
    std::printf("012173a0_TEST PASS\n");
    return 0;
}