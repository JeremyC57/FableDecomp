#include <cstdio>
// Behaviour model of CTCTavernGame::ClearBox (0x005EC5C0): each handle tells its primitive when it is the last handle, releases
// it (destroying it at refcount 0) and is cleared; then the vector is emptied.
struct Prim { long refs; int lastNotified; bool destroyed; };
struct Handle { Prim* p; };
static int Clear(Handle* h, int n)
{
    for (Handle* it = h; it != h + n; ++it) {
        long count = it->p ? it->p->refs : 0;
        if (count == 1) it->p->lastNotified++;
        if (Prim* p = it->p) { if (--p->refs == 0) p->destroyed = true; it->p = 0; }
        it->p = 0;
    }
    return 0;
}
int main()
{
    Prim shared = { 2, 0, false }, single = { 1, 0, false };
    Handle h[3] = { { &shared }, { &single }, { 0 } };
    int n = Clear(h, 3);
    if (n != 0 || shared.refs != 1 || shared.destroyed || shared.lastNotified || !single.destroyed || single.lastNotified != 1 || h[0].p) { std::printf("005ec5c0_TEST FAIL\n"); return 1; }
    std::printf("005ec5c0_TEST PASS\n");
    return 0;
}
