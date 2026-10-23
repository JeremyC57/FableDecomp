// 005b43a2 behaviour test: CTCInventoryClothing::AddClothingToThing (final.cpp body, modelled with standard containers)
#include <stdio.h>
#include <list>
#include <vector>
#include <map>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)

struct SModifier { bool IsRemoveable; long SuitPart; };
// def index -> modifier sub-def (absent = none); suit defs: index -> (suit part -> item def)
static std::map<long, SModifier> g_modifiers;
static std::map<long, std::map<long, long> > g_suits;
static std::map<long, long> g_inventory;

struct CModifiers {
    std::list<long> Worn;
    std::vector<long> Log;   // +def = added, -def = removed, 1000 = remove all
    void GetAppearanceModifierList(std::list<long>& out, long) const { out = Worn; }
    void RemoveAppearanceModifier(long def) { Log.push_back(-def); }
    void RemoveAllAppearanceModifiers(long) { Log.push_back(1000); }
    void AddAppearanceModifierWithReplace(long def) { Log.push_back(def); }
};
struct CThing { CModifiers* Modifiers; };
struct SEntry { long SuitPart; int Slot93Calls; int ArrowParam; };

struct CTCInventoryClothing {
    CThing* ParentThing;
    std::vector<SEntry> ClothingEntries;
    std::vector<SEntry> Closed;
    void AddClothingToThing(long item, CThing* thing);
};

void CTCInventoryClothing::AddClothingToThing(long item, CThing* thing)
{
    CModifiers* modifiers = thing->Modifiers;
    if (!modifiers) return;
    std::list<long> worn;
    modifiers->GetAppearanceModifierList(worn, 2);
    if (g_suits.count(item)) {
        std::vector<long> keptParts;
        for (std::list<long>::iterator node = worn.begin(); node != worn.end(); ++node) {
            std::map<long, SModifier>::iterator m = g_modifiers.find(*node);
            if (m != g_modifiers.end()) {
                if (m->second.IsRemoveable) modifiers->RemoveAppearanceModifier(*node);
                else keptParts.push_back(m->second.SuitPart);
            }
        }
        std::map<long, long>& parts = g_suits[item];
        for (std::map<long, long>::iterator part = parts.begin(); part != parts.end(); ++part) {
            if (g_inventory[part->second] > 0) {
                bool kept = false;
                for (size_t i = 0; i < keptParts.size(); ++i) if (keptParts[i] == part->first) { kept = true; break; }
                if (!kept) modifiers->AddAppearanceModifierWithReplace(part->second);
            }
        }
    } else {
        long suitPart = 0;
        const SModifier* clothing = 0;
        if (g_modifiers.count(item)) { clothing = &g_modifiers[item]; suitPart = clothing->SuitPart; }
        std::list<long>::iterator it = worn.begin();
        while (it != worn.end()) {
            std::map<long, SModifier>::iterator m = g_modifiers.find(*it);
            if (m == g_modifiers.end()) { ++it; continue; }   // retail loops on an unknown def; the test avoids that case
            if (m->second.SuitPart == suitPart) {
                if (!m->second.IsRemoveable) return;
                it = worn.erase(it);
            } else ++it;
        }
        modifiers->RemoveAllAppearanceModifiers(2);
        modifiers->AddAppearanceModifierWithReplace(item);
        if (ParentThing == thing && clothing && !clothing->IsRemoveable) {
            size_t i = 0;
            while (i < ClothingEntries.size()) {
                if (ClothingEntries[i].SuitPart == clothing->SuitPart) {
                    ClothingEntries[i].ArrowParam = 1;
                    ++ClothingEntries[i].Slot93Calls;
                    Closed.push_back(ClothingEntries[i]);
                    ClothingEntries.erase(ClothingEntries.begin() + i);
                } else ++i;
            }
        }
        for (std::list<long>::iterator node = worn.begin(); node != worn.end(); ++node)
            modifiers->AddAppearanceModifierWithReplace(*node);
    }
}

int main()
{
    SModifier hatR = { true, 1 }, hatFixed = { false, 1 }, boots = { true, 2 }, shirtFixed = { false, 3 };
    g_modifiers[10] = hatR; g_modifiers[11] = hatFixed; g_modifiers[20] = boots; g_modifiers[30] = shirtFixed;
    // single item replacing a removable hat; boots stay and are re-added
    {
        CModifiers mods; mods.Worn.push_back(10); mods.Worn.push_back(20);
        CThing thing = { &mods };
        CTCInventoryClothing inv; inv.ParentThing = 0;
        inv.AddClothingToThing(11, &thing);
        CHECK(mods.Log.size() == 3);
        CHECK(mods.Log[0] == 1000 && mods.Log[1] == 11 && mods.Log[2] == 20);
    }
    // a non-removable worn piece in the same part blocks the change
    {
        CModifiers mods; mods.Worn.push_back(11);
        CThing thing = { &mods };
        CTCInventoryClothing inv; inv.ParentThing = 0;
        inv.AddClothingToThing(10, &thing);
        CHECK(mods.Log.empty());
    }
    // the owner's menu entries of a fixed piece's part are closed
    {
        CModifiers mods; mods.Worn.push_back(20);
        CThing thing = { &mods };
        CTCInventoryClothing inv; inv.ParentThing = &thing;
        SEntry e1 = { 3, 0, 0 }, e2 = { 2, 0, 0 }, e3 = { 3, 0, 0 };
        inv.ClothingEntries.push_back(e1); inv.ClothingEntries.push_back(e2); inv.ClothingEntries.push_back(e3);
        inv.AddClothingToThing(30, &thing);
        CHECK(inv.ClothingEntries.size() == 1 && inv.ClothingEntries[0].SuitPart == 2);
        CHECK(inv.Closed.size() == 2 && inv.Closed[0].ArrowParam == 1 && inv.Closed[1].Slot93Calls == 1);
        CHECK(mods.Log.size() == 3 && mods.Log[1] == 30 && mods.Log[2] == 20);
    }
    // a suit: removable pieces removed, fixed parts kept, owned parts added
    {
        g_suits[500][1] = 11; g_suits[500][2] = 20; g_suits[500][3] = 30;
        g_inventory[11] = 1; g_inventory[20] = 0; g_inventory[30] = 2;
        CModifiers mods; mods.Worn.push_back(10); mods.Worn.push_back(30);
        CThing thing = { &mods };
        CTCInventoryClothing inv; inv.ParentThing = 0;
        inv.AddClothingToThing(500, &thing);
        CHECK(mods.Log.size() == 2);
        CHECK(mods.Log[0] == -10 && mods.Log[1] == 11);
    }
    // no modifier interface: nothing happens
    {
        CThing thing = { 0 };
        CTCInventoryClothing inv; inv.ParentThing = 0;
        inv.AddClothingToThing(10, &thing);
    }
    if (!g_fail) printf("005b43a2_TEST PASS\n");
    return g_fail;
}