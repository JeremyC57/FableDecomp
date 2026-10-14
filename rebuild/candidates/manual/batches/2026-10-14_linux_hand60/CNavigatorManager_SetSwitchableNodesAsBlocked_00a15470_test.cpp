#include <cstdio>
#include <cstdlib>
#include <cstring>
static void* g_blocked = 0; static long g_a, g_b, g_c, g_int = 0; struct CTree { void BlockRange(long a, long b, long c) { g_blocked = this; g_a = a; g_b = b; g_c = c; } }; struct SM { SM* next; SM* prev; CTree* tree; long c, id; };
struct INav { virtual void v000() { } virtual void v001() { } virtual void v002() { } virtual void v003() { } virtual void v004() { } virtual void v005() { } virtual void v006() { } virtual void v007() { } virtual void v008() { } virtual void v009() { } virtual void v010() { } virtual void v011() { } virtual void v012() { } virtual void v013() { } virtual void v014() { } virtual void v015() { } virtual void v016() { } virtual void v017() { } virtual void Interrupt() { ++g_int; } }; struct SN { SN* next; SN* prev; INav* nav; };
struct CMgr { unsigned char _pad[0x18]; SN* navs; unsigned char _p[8]; SM* maps; void Block(long id, long a, long b, long c) { for (SM* n = maps->next; n != maps; n = n->next) if (n->id == id) { n->tree->BlockRange(a, b, c); break; } for (SN* n = navs->next; n != navs; n = n->next) n->nav->Interrupt(); } };

int main() {
    SM mh, m1, m2; CTree t1, t2; mh.next = &m1; m1.next = &m2; m2.next = &mh; m1.tree = &t1; m2.tree = &t2; m1.id = 1; m2.id = 2; SN nh, n1; INav v; nh.next = &n1; n1.next = &nh; n1.nav = &v; CMgr* m = new CMgr; m->navs = &nh; m->maps = &mh;
    m->Block(2, 5, 6, 7); if (g_blocked != &t2 || g_a != 5 || g_b != 6 || g_c != 7 || g_int != 1) { std::printf("00a15470_TEST FAIL\n"); return 1; } m->Block(9, 1, 1, 1); if (g_blocked != &t2 || g_int != 2) { std::printf("00a15470_TEST FAIL\n"); return 1; } delete m;
    std::printf("00a15470_TEST PASS\n"); return 0;
}
