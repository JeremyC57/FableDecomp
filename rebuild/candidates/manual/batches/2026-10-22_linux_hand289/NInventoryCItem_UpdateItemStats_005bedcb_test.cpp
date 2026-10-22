#include <cstdio>
// Behaviour model of NInventory::CItem::UpdateItemStats (0x005BEDCB).
struct Data { int kills, wield, witnesses; bool persistent; int refCount; float fish; int augs; };
struct Thing { bool weapon, aug, trophy, fish, loader, analogue; int kills, wield, witnesses, augCount; };
static float g_fish;
static bool g_between;
static Data* Ensure(Data*& d) { if (!d) { d = new Data(); d->refCount = -1; d->augs = 0; d->fish = 0; } return d; }
static void Update(Data*& d, const Thing& t)
{
    if (t.weapon) {
        Ensure(d); d->kills = t.kills; d->wield = t.wield;
        d->augs = t.aug ? t.augCount : 0;
    } else if (t.trophy) {
        if (t.witnesses > 0) { Ensure(d); d->witnesses = t.witnesses; }
    } else if (t.fish) {
        Ensure(d);
        if (g_fish > d->fish) { d->fish = g_fish; g_fish = 0.0f; }
    }
    if (!d || d->refCount == -1) {
        if (t.loader && t.analogue) {
            Ensure(d);
            d->persistent = g_between ? false : true;
            d->refCount = 0;
        }
    }
}
int main()
{
    int ok = 1;
    Data* d = 0;
    Thing w = { true, true, false, false, false, false, 7, 30, 0, 3 };
    Update(d, w);
    ok = ok && d && d->kills == 7 && d->wield == 30 && d->augs == 3 && d->refCount == -1;
    w.aug = false; Update(d, w); ok = ok && d->augs == 0;
    Data* f = 0; g_fish = 5.5f;
    Thing fish = { false, false, false, true, false, false, 0, 0, 0, 0 };
    Update(f, fish); ok = ok && f && f->fish == 5.5f && g_fish == 0.0f;
    g_fish = 1.0f; Update(f, fish); ok = ok && f->fish == 5.5f && g_fish == 1.0f;
    Data* t = 0;
    Thing trophy = { false, false, true, false, false, false, 0, 0, 0, 0 };
    Update(t, trophy); ok = ok && t == 0;
    Thing load = { false, false, false, false, true, true, 0, 0, 0, 0 };
    g_between = false; Update(t, load); ok = ok && t && t->persistent && t->refCount == 0;
    t->persistent = false; Update(t, load); ok = ok && !t->persistent;
    if (!ok) { std::printf("005bedcb_TEST FAIL\n"); return 1; }
    std::printf("005bedcb_TEST PASS\n");
    return 0;
}
