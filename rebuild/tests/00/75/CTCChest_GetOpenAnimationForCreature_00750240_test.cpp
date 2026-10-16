#include <cstdio>
#include <cstring>
// Behaviour model of CTCChest::GetOpenAnimationForCreature (0x00750240): def animation if set, else "OPEN_CHEST";
// the looked-up def reference is always released.
struct Def { const char* anim; long refs; };
static const char* Anim(Def* d, bool found)
{
    const char* r = (found && std::strcmp(d->anim, "") != 0) ? d->anim : "OPEN_CHEST";
    if (d) --d->refs;
    return r;
}
int main()
{
    Def a = { "OPEN_CHEST_KICK", 2 }, b = { "", 2 };
    if (std::strcmp(Anim(&a, true), "OPEN_CHEST_KICK") || std::strcmp(Anim(&b, true), "OPEN_CHEST") || std::strcmp(Anim(&a, false), "OPEN_CHEST") || a.refs != 0 || b.refs != 1) { std::printf("00750240_TEST FAIL\n"); return 1; }
    std::printf("00750240_TEST PASS\n");
    return 0;
}