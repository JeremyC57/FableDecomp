#pragma optimize("s",on)
// Leaf: OnDie
// CTCInventoryItem::OnDie @ 004de60a
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
class CTCInventoryItem { public:
    char pad[0x10];
    void* PInventory;   // +0x10
};

extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    if (self->PInventory)
        free(self->PInventory);
    ((CTCBase*)(self))->CTCBase::~CTCBase();
}