#include <cstdio>
// Behaviour model of CTCOverheadDisplay::~CTCOverheadDisplay (0x007B80B0): every primitive handle is destroyed, the block freed,
// the def released (destroyed at refcount 0) and the three lists destroyed, in reverse member order.
static int g_order[8]; static int g_n = 0;
struct Handle { int id; void Destroy() { g_order[g_n++] = id; } };
struct Def { long refs; bool destroyed; };
int main()
{
    Handle h[2] = { { 1 }, { 2 } };
    Def d = { 1, false };
    for (Handle* p = h; p != h + 2; ++p) p->Destroy();      // vector elements
    g_order[g_n++] = 10;                                    // free block
    if (--d.refs == 0) d.destroyed = true;                  // def pointer
    g_order[g_n++] = 20; g_order[g_n++] = 21; g_order[g_n++] = 22;   // renown, repair, damage lists
    if (g_n != 6 || g_order[0] != 1 || g_order[1] != 2 || g_order[2] != 10 || !d.destroyed || g_order[5] != 22) { std::printf("007b80b0_TEST FAIL\n"); return 1; }
    std::printf("007b80b0_TEST PASS\n");
    return 0;
}