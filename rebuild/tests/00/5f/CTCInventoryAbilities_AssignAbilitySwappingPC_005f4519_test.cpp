#include <cstdio>
// Behaviour model of CTCInventoryAbilities::AssignAbilitySwappingPC (0x005F4519): slots pair up (even, odd); a drag from
// the paired slot onto an empty slot clears the source, a duplicate in the pair clears this slot; the old ability moves
// to the source slot and the new one is assigned.
struct Inv {
    long slots[4]; unsigned int n;
    void UnAssign(unsigned int s) { slots[s] = 0; }
    void Assign(long a, unsigned int s) { slots[s] = a; }
    void Swap(long ability, unsigned int slot, unsigned int from)
    {
        long current = slots[slot];
        if (!(slot & 1) && slot < n - 1) {
            if (current == 0 && from == slot + 1) UnAssign(from);
            else if (slots[slot + 1] == ability) UnAssign(slot);
        } else if (slot > 0) {
            if (current == 0 && from == slot - 1) UnAssign(from);
            else if (slots[slot - 1] == ability) UnAssign(slot);
        }
        if (current) Assign(current, from);
        Assign(ability, slot);
    }
};
int main()
{
    Inv v = { { 5, 0, 7, 8 }, 4 };
    v.Swap(9, 0, 2);
    if (v.slots[0] != 9 || v.slots[2] != 5) { std::printf("005f4519_TEST FAIL\n"); return 1; }
    Inv w = { { 0, 6, 0, 0 }, 4 };
    w.Swap(6, 0, 1);
    if (w.slots[0] != 6 || w.slots[1] != 0) { std::printf("005f4519_TEST FAIL\n"); return 1; }
    std::printf("005f4519_TEST PASS\n");
    return 0;
}