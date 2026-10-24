#pragma optimize("s",on)
// Leaf: OnDie
struct CTCInventoryItem {
    unsigned char _pad_0x0[0x10];
    void* PInventory;
    unsigned char _pad_0x14[0xc];
    char DefaultItem;
    unsigned char _pad_0x21[0x5f];
    void* ptr80; // +0x80
};

extern "C" void __cdecl FreeThing(void* p);
extern void __fastcall Base_OnDie(CTCInventoryItem* self);

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    if (self->ptr80)
        FreeThing(self->ptr80);
    Base_OnDie(self);
}