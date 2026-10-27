// behaviour test for 0x00526EF0: the deleting destructor destroys both members (second first) and frees only with flag 1
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_order[8]; static int g_n = 0; static int g_freed = 0;
struct M { int id; ~M() { g_order[g_n++] = id; } };
struct R { long w; M a; M b; };
static void* Deleting(R* r, unsigned flags) { r->~R(); if (flags & 1) { ++g_freed; std::free(r); } return r; }
int main()
{
    R* r = (R*)std::malloc(sizeof(R)); new (&r->a) M(); new (&r->b) M(); r->a.id = 1; r->b.id = 2;
    Deleting(r, 1);
    R s; s.a.id = 3; s.b.id = 4; Deleting(&s, 0); new (&s.a) M(); new (&s.b) M();
    if (g_order[0] != 2 || g_order[1] != 1 || g_order[2] != 4 || g_order[3] != 3 || g_freed != 1) { std::printf("00526ef0_TEST FAIL\n"); return 1; }
    std::printf("00526ef0_TEST PASS\n");
    return 0;
}