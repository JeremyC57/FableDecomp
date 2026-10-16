#include <cstdio>
// Behaviour model of CTCInventory::IsQuickAccessItemUsable (0x005849EA): an empty slot (<= 0) is unusable; a held
// item is usable when the hero allows augmentations or the item is not an augmentation.
static bool IsAugmentation(long item) { return item >= 100; }
static bool Usable(const long* slots, long slot, bool allowAug)
{
    if (slots[slot] > 0)
        return allowAug || !IsAugmentation(slots[slot]);
    return false;
}
int main()
{
    long slots[3] = { 0, 5, 150 };
    if (Usable(slots, 0, true) || !Usable(slots, 1, false) || Usable(slots, 2, false) || !Usable(slots, 2, true)) { std::printf("005849ea_TEST FAIL\n"); return 1; }
    std::printf("005849ea_TEST PASS\n");
    return 0;
}