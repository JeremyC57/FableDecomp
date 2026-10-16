#include <cstdio>
// Behaviour model of CTCInventoryAbilities::Copy (0x005F3DC6): the base state is copied unless the global suppress flag
// is set; the spent experience, abilities, button map and the initialised flag are always copied.
static bool g_suppress;
struct Inv {
    long base; long spent; long abilities; long buttons; bool Initialised; long order;
    void Copy(const Inv* o)
    {
        if (!g_suppress) base = o->base;
        spent = o->spent; abilities = o->abilities; buttons = o->buttons; Initialised = o->Initialised;
    }
};
int main()
{
    Inv a = { 1, 2, 3, 4, true, 5 };
    Inv b = { 0, 0, 0, 0, false, 0 };
    b.Copy(&a);
    if (b.base != 1 || b.spent != 2 || b.abilities != 3 || b.buttons != 4 || !b.Initialised || b.order != 0) { std::printf("005f3dc6_TEST FAIL\n"); return 1; }
    Inv c = { 0, 0, 0, 0, false, 0 };
    g_suppress = true; c.Copy(&a);
    if (c.base != 0 || c.spent != 2) { std::printf("005f3dc6_TEST FAIL\n"); return 1; }
    std::printf("005f3dc6_TEST PASS\n");
    return 0;
}