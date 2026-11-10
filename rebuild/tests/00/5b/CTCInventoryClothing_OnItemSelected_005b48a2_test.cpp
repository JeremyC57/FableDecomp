// Behaviour test for 0x005B48A2 CTCInventoryClothing::OnItemSelected: when the selected item's def has a hero-suit sub-def, the
// owner's worn clothing appearance modifiers (only valid defs whose modifier sub-def is not removable) are collected, everything is
// deselected, every suit part the inventory holds (count > 0) is selected in map order, then the collected worn items; in every case
// the selected clothes are set up and the mannequin (when used and present) or else the owner is dressed.
#include <cstdio>
#include <vector>
#include <list>
#include <map>

struct ModDef { bool hasModifier; bool removeable; };
struct Clothing {
    std::map<long, bool> isSuit;                    // item def -> has a CHeroSuitDef sub-def
    std::map<long, std::map<long, long> > suits;    // suit def -> part -> item def (map order by part)
    bool ownerHasModifiers;
    std::list<long> wornClothing;                   // GetAppearanceModifierList(APPEARANCE_CLOTHING)
    std::map<long, ModDef> modDefs;                 // valid defs (index > 0)
    std::map<long, long> inventoryCount;
    bool UseMannequin; bool hasMannequin;
    std::vector<long> log;                          // -1 deselect, -2 setup, -3 dress mannequin, -4 dress owner, else select
    void OnItemSelected(long itemDefIndex, bool& handled)
    {
        (void)handled;
        if (isSuit[itemDefIndex]) {
            std::vector<long> worn;
            if (ownerHasModifiers) {
                std::list<long> mods(wornClothing);
                for (std::list<long>::iterator it = mods.begin(); it != mods.end(); ++it) {
                    std::map<long, ModDef>::iterator d = modDefs.find(*it);
                    if (*it > 0 && d != modDefs.end()) {
                        if (d->second.hasModifier && !d->second.removeable)
                            worn.push_back(*it);
                    }
                }
            }
            log.push_back(-1);
            std::map<long, long>& parts = suits[itemDefIndex];
            for (std::map<long, long>::const_iterator p = parts.begin(); p != parts.end(); ++p)
                if (inventoryCount[p->second] > 0) log.push_back(p->second);
            for (std::vector<long>::iterator w = worn.begin(); w != worn.end(); ++w) log.push_back(*w);
        }
        log.push_back(-2);
        if (UseMannequin) { if (hasMannequin) log.push_back(-3); }
        else log.push_back(-4);
    }
};
static int fails = 0;
static void check(bool c, const char* what) { if (!c) { std::printf("FAIL %s\n", what); ++fails; } }
static bool same(const std::vector<long>& a, const long* b, unsigned n)
{
    if (a.size() != n) return false;
    for (unsigned i = 0; i < n; ++i) if (a[i] != b[i]) return false;
    return true;
}
int main()
{
    bool handled = false;
    {   // not a suit: only setup + dress the owner
        Clothing c; c.ownerHasModifiers = true; c.UseMannequin = false; c.hasMannequin = false;
        c.OnItemSelected(5, handled);
        long want[] = { -2, -4 };
        check(same(c.log, want, 2), "non-suit dresses owner");
    }
    {   // suit: parts in map order filtered by count, then worn non-removable valid defs
        Clothing c; c.isSuit[7] = true; c.UseMannequin = true; c.hasMannequin = true; c.ownerHasModifiers = true;
        c.suits[7][3] = 30; c.suits[7][1] = 10; c.suits[7][2] = 20;
        c.inventoryCount[10] = 1; c.inventoryCount[20] = 0; c.inventoryCount[30] = 2;
        c.wornClothing.push_back(40); c.wornClothing.push_back(41); c.wornClothing.push_back(42); c.wornClothing.push_back(0);
        ModDef keep = { true, false }, rem = { true, true }, none = { false, false };
        c.modDefs[40] = keep; c.modDefs[41] = rem; c.modDefs[42] = none; c.modDefs[0] = keep;
        c.OnItemSelected(7, handled);
        long want[] = { -1, 10, 30, 40, -2, -3 };
        check(same(c.log, want, 6), "suit selection order");
    }
    {   // suit, owner without modifiers component, mannequin used but absent: nothing dressed
        Clothing c; c.isSuit[8] = true; c.UseMannequin = true; c.hasMannequin = false; c.ownerHasModifiers = false;
        c.suits[8][0] = 11; c.inventoryCount[11] = 1;
        c.wornClothing.push_back(40); ModDef keep = { true, false }; c.modDefs[40] = keep;
        c.OnItemSelected(8, handled);
        long want[] = { -1, 11, -2 };
        check(same(c.log, want, 3), "no modifiers, no mannequin");
    }
    if (fails) return 1;
    std::printf("005b48a2_TEST PASS\n");
    return 0;
}