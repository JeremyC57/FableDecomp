// Behaviour test for 0x005B4753 CTCInventoryClothing::AddClothing(long): AddClothingToThing(item, ParentThing) on this,
// then SelectItemWithDefIndex(item). Portable (g++ 64-bit and VC7.1 /Od); prints the catalog PassPattern.
#include <cstdio>

class CThing { public: int Tag; };
static long g_add_item = -1;
static CThing* g_add_thing = 0;
static void* g_add_self = 0;
static void* g_select_self = 0;
static long g_select_item = -1;

namespace NInventory {
class CTCInventoryBase { public:
    void*   __vftable;
    CThing* ParentThing;
protected:
    void SelectItemWithDefIndex(long defIndex);
};
}
class CTCInventoryClothing : public NInventory::CTCInventoryBase { public:
    void AddClothing(long item);
    void AddClothingToThing(long item, CThing* thing);
};

void NInventory::CTCInventoryBase::SelectItemWithDefIndex(long defIndex) { g_select_self = this; g_select_item = defIndex; }
void CTCInventoryClothing::AddClothingToThing(long item, CThing* thing) { g_add_self = this; g_add_item = item; g_add_thing = thing; }

void CTCInventoryClothing::AddClothing(long item)
{
    AddClothingToThing(item, ParentThing);
    SelectItemWithDefIndex(item);
}

int main() {
    CThing owner; owner.Tag = 7;
    CTCInventoryClothing inv;
    inv.__vftable = 0;
    inv.ParentThing = &owner;
    inv.AddClothing(0x42);
    if (g_add_self == &inv && g_add_item == 0x42 && g_add_thing == &owner &&
        g_select_self == &inv && g_select_item == 0x42) {
        printf("PARITY_OK\n");
    } else {
        printf("PARITY_FAIL\n");
    }
    return 0;
}
