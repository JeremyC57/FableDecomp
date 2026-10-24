#pragma optimize("s",on)
// Leaf: OnDie
// CTCInventoryItem::OnDie @ 004de60a
struct CTCInventoryItem {
    char pad[0x10];
    void* PInventory;   // +0x10
};

extern "C" void __cdecl free_bfea14(void* p);         // 0xbfea14
extern void __fastcall base_686830(CTCInventoryItem* self); // 0x686830

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    if (self->PInventory)
        free_bfea14(self->PInventory);
    base_686830(self);
}