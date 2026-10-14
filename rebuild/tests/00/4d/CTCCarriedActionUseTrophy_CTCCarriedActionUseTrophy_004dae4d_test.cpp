#include <cstdio>
#include <stddef.h>
struct CThing { int id; };
static CThing* g_baseThing = 0; static CThing* g_mountThing = 0; static int g_order = 0, g_baseOrder = 0, g_mountOrder = 0;
struct CTCCarriedActionUseBase { virtual void v00() { } unsigned char _body[0x10]; CTCCarriedActionUseBase(CThing* t) { g_baseThing = t; g_baseOrder = ++g_order; } };
struct CTCCarriedActionUseWallMount { virtual void v00() { } unsigned char _body[0x10]; CTCCarriedActionUseWallMount(CThing* t) { g_mountThing = t; g_mountOrder = ++g_order; } };
struct CTCCarriedActionUseTrophy : CTCCarriedActionUseBase {
    CTCCarriedActionUseWallMount PTCCarriedActionUseWallMount;
    virtual void v00() { }
    CTCCarriedActionUseTrophy(CThing* thing) : CTCCarriedActionUseBase(thing), PTCCarriedActionUseWallMount(thing) { }
};
int main()
{
    CThing thing = { 3 };
    CTCCarriedActionUseTrophy trophy(&thing);
    if (g_baseThing != &thing || g_mountThing != &thing || g_baseOrder != 1 || g_mountOrder != 2
        || offsetof(CTCCarriedActionUseTrophy, PTCCarriedActionUseWallMount) != 0x14 || sizeof(trophy) != 0x28) { std::printf("004dae4d_TEST FAIL\n"); return 1; }
    std::printf("004dae4d_TEST PASS\n"); return 0;
}