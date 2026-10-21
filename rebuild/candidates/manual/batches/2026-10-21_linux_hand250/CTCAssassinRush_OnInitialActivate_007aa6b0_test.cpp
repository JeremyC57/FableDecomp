// Behaviour test for CTCAssassinRush::OnInitialActivate (0x007AA6B0): mode, alpha fade of hero and live items, final branch.
#include <cstdio>
struct Thing { bool dead; unsigned char type; bool hasGraphic; int alpha; bool hasModes, hasInventory; int mode; bool suspended; };
struct Rush {
    Thing* ParentThing; unsigned char defAlpha; Thing* items[4]; int nItems; Thing* remembered[4]; int nRemembered; int overrideCalls;
    void OnInitialActivate()
    {
        Thing* thing = ParentThing;
        if (thing->hasModes) thing->mode = 0x25;
        if (thing->hasGraphic) thing->alpha = defAlpha;
        if (thing->hasInventory) {
            for (int i = 0; i < nItems; ++i) {
                Thing* item = items[i];
                if (item && !item->dead) {
                    remembered[nRemembered++] = item;
                    if (item->hasGraphic) item->alpha = defAlpha;
                }
            }
        }
        if (!thing->dead && thing->type == 1) thing->suspended = true;
        else ++overrideCalls;
    }
};
int main()
{
    Thing hero = { false, 1, true, 255, true, true, 0, false };
    Thing a = { false, 0, true, 255, false, false, 0, false };
    Thing b = { true, 0, true, 255, false, false, 0, false };
    Thing c = { false, 0, false, 255, false, false, 0, false };
    Rush r = { &hero, 40, { &a, &b, 0, &c }, 4, { 0, 0, 0, 0 }, 0, 0 };
    r.OnInitialActivate();
    if (hero.mode != 0x25 || hero.alpha != 40 || a.alpha != 40 || b.alpha != 255 || r.nRemembered != 2 || r.remembered[1] != &c
        || !hero.suspended || r.overrideCalls != 0) { std::printf("007aa6b0_TEST FAIL\n"); return 1; }
    Thing ai = { false, 2, false, 255, false, false, 0, false };
    Rush s = { &ai, 10, { 0, 0, 0, 0 }, 0, { 0, 0, 0, 0 }, 0, 0 };
    s.OnInitialActivate();
    if (s.overrideCalls != 1 || ai.suspended || ai.mode != 0) { std::printf("007aa6b0_TEST FAIL\n"); return 1; }
    std::printf("007aa6b0_TEST PASS\n");
    return 0;
}
