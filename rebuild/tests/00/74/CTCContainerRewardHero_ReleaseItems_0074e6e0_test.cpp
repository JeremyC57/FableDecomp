#include <cstdio>
// behaviour test: ReleaseItems does nothing without items; otherwise each item is created 0.1 above the container, launched by
// the popout vector for (index, perRing = min(count, 8), ring) where index wraps to 0 and ring advances after perRing items;
// all created objects then ignore each other and the item list is cleared
struct C3DVector { float X, Y, Z; };
struct CCall { int def; C3DVector pos; int index, perRing, ring; };
static CCall g_calls[32]; static int g_n = 0; static int g_ignoreCalls = 0; static int g_ignoreCount = 0;
struct CContainer {
    C3DVector Position; int Items[32]; int ItemCount;
    void ReleaseItems(float speed)
    {
        if (ItemCount == 0)
            return;
        C3DVector pos; pos.X = Position.X; pos.Y = Position.Y; pos.Z = Position.Z + 0.1f;
        int perRing = 8;
        if (ItemCount < 8)
            perRing = ItemCount;
        int index = 0, ring = 0, created = 0;
        for (int i = 0; i < ItemCount; ++i) {
            CCall& c = g_calls[g_n++];
            c.index = index; c.perRing = perRing; c.ring = ring;
            if (++index == perRing) { index = 0; ++ring; }
            c.def = Items[i]; c.pos = pos;
            ++created;
        }
        ++g_ignoreCalls; g_ignoreCount = created;
        ItemCount = 0;
    }
};
int main()
{
    CContainer c = { { 1.0f, 2.0f, 3.0f }, { 0 }, 0 };
    c.ReleaseItems(5.0f);
    bool ok = g_n == 0 && g_ignoreCalls == 0;
    for (int i = 0; i < 10; ++i) c.Items[i] = 100 + i;
    c.ItemCount = 10;
    c.ReleaseItems(5.0f);
    ok = ok && g_n == 10 && c.ItemCount == 0 && g_ignoreCalls == 1 && g_ignoreCount == 10;
    ok = ok && g_calls[0].perRing == 8 && g_calls[7].index == 7 && g_calls[7].ring == 0 && g_calls[8].index == 0 &&
         g_calls[8].ring == 1 && g_calls[9].def == 109 && g_calls[3].pos.Z > 3.09f && g_calls[3].pos.Z < 3.11f &&
         g_calls[3].pos.X == 1.0f;
    g_n = 0; c.ItemCount = 3;
    c.ReleaseItems(5.0f);
    ok = ok && g_n == 3 && g_calls[0].perRing == 3 && g_calls[2].index == 2 && g_calls[2].ring == 0;
    if (!ok) { std::printf("0074e6e0_TEST FAIL\n"); return 1; }
    std::printf("0074e6e0_TEST PASS\n");
    return 0;
}