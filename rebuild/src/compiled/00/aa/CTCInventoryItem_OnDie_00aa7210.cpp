// CTCInventoryItem::OnDie @ 0x00aa7210  __fastcall void(CTCInventoryItem*)
// Frees the pointer member at +0x10 (cdecl helper) if non-null, then
// tail-calls the base-class OnDie (__fastcall) on this.

extern "C" void __cdecl FreeHelper_bfea14(void* p);

struct CTCInventoryItemBase
{
    void BaseOnDie();   // __fastcall @ 0x00aa75c0
};

class CTCInventoryItem : public CTCInventoryItemBase
{ public:
    char pad0[0x10];
    void* PInventory;     // +0x10
    void OnDie();
};

void CTCInventoryItem::OnDie()
{
    if (this->PInventory)
        FreeHelper_bfea14(this->PInventory);
    this->BaseOnDie();
}