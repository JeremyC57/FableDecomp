// Behaviour test for 0x005BC9B0 NInventory::CCategory::GetItemInSlotWithIndex: a negative index gives 0, otherwise the item type
// (+0x00) of the 0x14-byte slot record at that index. Neutral recorder records (no model of the inventory classes); prints OK_0x005bc9b0.
#include <cstdio>
struct SlotRecord { long type; long count; long data[2]; long price; };   // 0x14 bytes, as the retail stride (lea eax,[eax+eax*4] ; *4)
static long ItemInSlot(const SlotRecord* slots, long index)
{
    if (index >= 0)
        return slots[index].type;
    return 0;
}
int main()
{
    SlotRecord slots[4];
    for (int i = 0; i < 4; ++i) { slots[i].type = 0x1000 + i; slots[i].count = 1; slots[i].data[0] = slots[i].data[1] = 0; slots[i].price = 7; }
    bool ok = sizeof(SlotRecord) == 0x14 && ItemInSlot(slots, -1) == 0 && ItemInSlot(slots, 0) == 0x1000 && ItemInSlot(slots, 2) == 0x1002
           && ItemInSlot(slots, 3) == 0x1003;
    std::printf(ok ? "OK_0x005bc9b0\n" : "FAIL_0x005bc9b0\n");
    return ok ? 0 : 1;
}
