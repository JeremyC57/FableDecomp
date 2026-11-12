// Behaviour test for 0x005D8E40 CTCInventoryItem::ConnectToInventory(NInventory::CTCInventoryBase&): the inventory's address is
// stored into PInventory (+0x10) and nothing else changes (name-fix 2026-11-12: was the stand-in AddExclusiveObserver). The item is
// a raw 0x28-byte block (the call is non-virtual; no constructor is linked).
#include <cstdio>
#include <cstring>
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CONNECTTOINVENTORY
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInventoryItem.h"

int main()
{
    static unsigned char item[0x28];
    static unsigned char inventory[0x40];
    std::memset(item, 0xAB, sizeof(item));
    reinterpret_cast<CTCInventoryItem*>(item)->ConnectToInventory(*reinterpret_cast<NInventory::CTCInventoryBase*>(inventory));
    void* stored = 0;
    std::memcpy(&stored, item + 0x10, 4);
    if (stored != (void*)inventory) { std::printf("FAIL_0x005d8e40 store\n"); return 1; }
    for (int i = 0; i < 0x28; ++i)
        if ((i < 0x10 || i >= 0x14) && item[i] != 0xAB) { std::printf("FAIL_0x005d8e40 byte %d\n", i); return 1; }
    std::printf("OK_0x005d8e40\n");
    return 0;
}
