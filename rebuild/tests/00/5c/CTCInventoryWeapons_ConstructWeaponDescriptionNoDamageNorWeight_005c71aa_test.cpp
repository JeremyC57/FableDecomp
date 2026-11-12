// Behaviour test for CTCInventoryWeapons::ConstructWeaponDescriptionNoDamageNorWeight (0x005C71AA): a copy of the row's logic on a
// small model -- the def's initial augmentations copied into NumberOfSlots slots (fromDef), or the owned item's per-item data, or
// an early return when the item is not owned; then "AUG: " + the filled slot names comma separated (or "NONE"), NEWLINE, details.
#include <cstdio>
#include <string>
#include <vector>

struct AugDef { long NumberOfSlots; std::vector<long> InitialAugmentations; };
struct PerItemData { std::vector<long> Augmentations; };
struct Inventory { long owned; PerItemData owned_data; };

static std::string AugName(long type) { char b[16]; std::sprintf(b, "A%ld", type); return b; }

static void Construct(std::string& text, const AugDef* augDef, const Inventory& inv, bool fromDef)
{
    PerItemData data;
    const PerItemData* itemData;
    if (fromDef) {
        if (augDef) {
            data.Augmentations.resize(augDef->NumberOfSlots, 0);
            for (unsigned long i = 0; i < augDef->InitialAugmentations.size(); ++i) {
                if (i >= data.Augmentations.size())
                    break;
                data.Augmentations[i] = augDef->InitialAugmentations[i];
            }
        }
        itemData = &data;
    } else {
        if (inv.owned == 0)
            return;
        itemData = &inv.owned_data;
    }
    text += std::string("AUG") + ": ";
    const std::vector<long>& augmentations = itemData->Augmentations;
    if (!augmentations.empty()) {
        long last = -1;
        long count = (long)augmentations.size();
        long used = 0;
        for (long i = 0; i < count; ++i) {
            if (augmentations[i] > 0) { ++used; last = i; }
        }
        if (used > 0) {
            for (long i = 0; i < count; ++i) {
                if (augmentations[i] > 0) {
                    text += AugName(augmentations[i]);
                    if (i < last)
                        text += ",";
                }
            }
        } else {
            text += "NONE";
        }
    } else {
        text += "NONE";
    }
    text += "\n";
    text += "DETAILS";
}

int main()
{
    int fails = 0;
    AugDef def; def.NumberOfSlots = 3; def.InitialAugmentations.push_back(5); def.InitialAugmentations.push_back(0);
    def.InitialAugmentations.push_back(7); def.InitialAugmentations.push_back(9);   // 4 initial, only 3 slots: the 9 is dropped
    Inventory inv; inv.owned = 0;
    std::string t;
    Construct(t, &def, inv, true);
    if (t != "AUG: A5,A7\nDETAILS") ++fails;
    t = "";
    Construct(t, &def, inv, false);                     // not owned: nothing appended
    if (t != "") ++fails;
    inv.owned = 1; inv.owned_data.Augmentations.push_back(0); inv.owned_data.Augmentations.push_back(-1);
    t = "x";
    Construct(t, &def, inv, false);                     // owned, no slot filled: "none"
    if (t != "xAUG: NONE\nDETAILS") ++fails;
    t = "";
    Construct(t, 0, inv, true);                         // no augmentations def: empty vector -> "none"
    if (t != "AUG: NONE\nDETAILS") ++fails;
    inv.owned_data.Augmentations[1] = 4;
    t = "";
    Construct(t, &def, inv, false);                     // single filled slot: no separator
    if (t != "AUG: A4\nDETAILS") ++fails;
    if (fails) { std::printf("005c71aa_TEST FAIL\n"); return 1; }
    std::printf("005c71aa_TEST PASS\n");
    return 0;
}