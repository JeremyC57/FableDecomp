// Behaviour test for 0x00766920 CTCObjectAugmentations::GetAugmentations: the output vector is cleared, then gets one entry
// per slot in order -- the global index of the slot def's instantiation name, or 0 for an empty slot.
#include <cstdio>
#include <cstring>
#include <vector>
#include <string>
struct Def { std::string InstantiationName; };
struct DefinitionManager {
    long GetDefGlobalIndexFromName(const std::string& name) const
    {
        static const char* names[] = { "", "OBJECT_AUGMENTATION_FIRE", "OBJECT_AUGMENTATION_SILVER", "OBJECT_AUGMENTATION_GOLD" };
        for (long i = 1; i < 4; ++i) if (name == names[i]) return i * 10;
        return -1;
    }
};
static void GetAugmentations(const std::vector<const Def*>& slots, const DefinitionManager& manager, std::vector<long>& out)
{
    out.clear();
    out.reserve(slots.size());
    for (unsigned long i = 0; i < slots.size(); ++i) {
        if (slots[i] != 0) {
            std::string name = slots[i]->InstantiationName;
            out.push_back(manager.GetDefGlobalIndexFromName(name));
        } else
            out.push_back(0);
    }
}
int main()
{
    Def fire = { "OBJECT_AUGMENTATION_FIRE" }, gold = { "OBJECT_AUGMENTATION_GOLD" };
    std::vector<const Def*> slots;
    slots.push_back(&gold); slots.push_back(0); slots.push_back(&fire);
    DefinitionManager manager;
    std::vector<long> out(5, 99);
    GetAugmentations(slots, manager, out);
    bool ok = out.size() == 3 && out[0] == 30 && out[1] == 0 && out[2] == 10;
    std::vector<const Def*> none;
    GetAugmentations(none, manager, out);
    ok = ok && out.empty();
    if (!ok) { std::printf("00766920_TEST FAIL\n"); return 1; }
    std::printf("00766920_TEST PASS\n");
    return 0;
}