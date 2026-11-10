// Behaviour test for 0x00583E26 CTCInventory::OnItemSelected(long, bool&): by the item's use-button action, action 1 creates the
// object (out of limbo), uses it through its carried action-use component when it has one and the hero is idle or the component is
// usable mid-action (then handled = true), else returns it to the inventory; action 2 does the same use/return (no presence check),
// re-highlights the current item when the use killed the object, and always clears handled; any other action does nothing.
#include <cstdio>
#include <vector>

struct Thing { bool alive; bool hasUse; bool usableMidAction; bool dieOnUse; bool dead; };
struct Log { std::vector<int> ev; };   // 1 limbo, 2 use, 3 return, 4 highlight
struct Inventory {
    int action; Thing* created; bool heroBusy; long highlighted; Log log;
    void OnItemSelected(long itemDefIndex, bool& handled)
    {
        (void)itemDefIndex;
        switch (action) {
        case 1: {
            Thing* object = created;
            if (object && object->alive) {
                log.ev.push_back(1);
                if (object->hasUse) {
                    if (!heroBusy || object->usableMidAction) { handled = true; Use(object); }
                    else log.ev.push_back(3);
                } else log.ev.push_back(3);
            }
            break;
        }
        case 2: {
            Thing* object = created;
            if (object && object->alive) {
                log.ev.push_back(1);
                if (!heroBusy || object->usableMidAction) {
                    Use(object);
                    if (object->dead) log.ev.push_back(4);
                } else log.ev.push_back(3);
            }
            handled = false;
            break;
        }
        }
    }
    void Use(Thing* t) { log.ev.push_back(2); if (t->dieOnUse) t->dead = true; }
};
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { std::printf("FAIL %s\n", w); ++fails; } }
static bool same(const std::vector<int>& a, const int* b, unsigned n)
{
    if (a.size() != n) return false;
    for (unsigned i = 0; i < n; ++i) if (a[i] != b[i]) return false;
    return true;
}
int main()
{
    {   Thing t = { true, true, false, false, false }; Inventory inv; inv.action = 1; inv.created = &t; inv.heroBusy = false;
        bool h = false; inv.OnItemSelected(7, h); int w[] = { 1, 2 }; check(h && same(inv.log.ev, w, 2), "action 1 idle use"); }
    {   Thing t = { true, true, false, false, false }; Inventory inv; inv.action = 1; inv.created = &t; inv.heroBusy = true;
        bool h = false; inv.OnItemSelected(7, h); int w[] = { 1, 3 }; check(!h && same(inv.log.ev, w, 2), "action 1 busy return"); }
    {   Thing t = { true, true, true, false, false }; Inventory inv; inv.action = 1; inv.created = &t; inv.heroBusy = true;
        bool h = false; inv.OnItemSelected(7, h); int w[] = { 1, 2 }; check(h && same(inv.log.ev, w, 2), "action 1 busy usable"); }
    {   Thing t = { true, false, true, false, false }; Inventory inv; inv.action = 1; inv.created = &t; inv.heroBusy = false;
        bool h = false; inv.OnItemSelected(7, h); int w[] = { 1, 3 }; check(!h && same(inv.log.ev, w, 2), "action 1 no use component"); }
    {   Thing t = { false, true, true, false, false }; Inventory inv; inv.action = 1; inv.created = &t; inv.heroBusy = false;
        bool h = false; inv.OnItemSelected(7, h); check(!h && inv.log.ev.empty(), "action 1 dead object"); }
    {   Thing t = { true, true, false, true, false }; Inventory inv; inv.action = 2; inv.created = &t; inv.heroBusy = false;
        bool h = true; inv.OnItemSelected(7, h); int w[] = { 1, 2, 4 }; check(!h && same(inv.log.ev, w, 3), "action 2 consumed"); }
    {   Thing t = { true, true, false, false, false }; Inventory inv; inv.action = 2; inv.created = &t; inv.heroBusy = true;
        bool h = true; inv.OnItemSelected(7, h); int w[] = { 1, 3 }; check(!h && same(inv.log.ev, w, 2), "action 2 busy return"); }
    {   Inventory inv; inv.action = 2; inv.created = 0; inv.heroBusy = false;
        bool h = true; inv.OnItemSelected(7, h); check(!h && inv.log.ev.empty(), "action 2 no object"); }
    {   Thing t = { true, true, true, false, false }; Inventory inv; inv.action = 0; inv.created = &t; inv.heroBusy = false;
        bool h = true; inv.OnItemSelected(7, h); check(h && inv.log.ev.empty(), "other action"); }
    if (fails) return 1;
    std::printf("00583e26_TEST PASS\n");
    return 0;
}