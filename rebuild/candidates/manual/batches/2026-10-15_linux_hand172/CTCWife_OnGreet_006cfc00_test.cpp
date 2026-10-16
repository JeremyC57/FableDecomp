#include <cstdio>
static long g_frame = 100;
static int g_posted = 0; static long g_deed = 0;
struct CLog { void PostDeed(long d, const float* pos, void* t, void* c, long type) { (void)pos; (void)t; (void)c; (void)type; ++g_posted; g_deed = d; } };
struct CThing { bool dead; CLog* log; unsigned char flags6c; float pos[3]; };
struct CTCWife {
    CThing* parent; CThing* hero; long married;
    void OnGreet() {
        if (married > g_frame) return;
        CThing* h = hero; if (!h || h->dead) return;
        if (h->log) { if (parent->flags6c & 0x10) h->log->PostDeed(0x50, parent->pos, parent, parent, 1); }
    }
};
int main()
{
    CLog log; CThing p = { false, 0, 0x10, {1, 2, 3} }, h = { false, &log, 0, {0} }, hd = { true, &log, 0, {0} };
    CTCWife a = { &p, &h, 50 }, b = { &p, &h, 200 }, c = { &p, &hd, 50 }, d = { &p, 0, 50 };
    a.OnGreet(); b.OnGreet(); c.OnGreet(); d.OnGreet();
    if (g_posted != 1 || g_deed != 0x50) { std::printf("006cfc00_TEST FAIL\n"); return 1; }
    std::printf("006cfc00_TEST PASS\n");
    return 0;
}
