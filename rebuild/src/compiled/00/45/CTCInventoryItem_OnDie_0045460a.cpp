#pragma optimize("s",on)
// Leaf: OnDie
class CTCInventoryItem { public:
    unsigned char _pad_0x0[0x10];
    void* PInventory;
    unsigned char _pad_0x14[0xc];
    char DefaultItem;
    unsigned char _pad_0x21[0xf];
    void* ptr30;
};
extern void __cdecl engine_free(void*);
extern void __fastcall CTCInventoryItem_Base_OnDie_009fc550(CTCInventoryItem* self);
extern const void* const CTCInventoryItem_vftable;
void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    if (self->ptr30)
        engine_free(self->ptr30);
    *(const void**)self = &CTCInventoryItem_vftable;
    CTCInventoryItem_Base_OnDie_009fc550(self);
}