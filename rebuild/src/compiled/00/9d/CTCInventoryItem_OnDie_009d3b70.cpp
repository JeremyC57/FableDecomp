// CTCInventoryItem::OnDie  @ 009d3b70  __fastcall void(CTCInventoryItem*)
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

struct CTCInventoryItemBase
{
    void* vtbl;      // +0x00
    void* thing;     // +0x04
    void OnDieBase(); // -> 0x0099eae0
};

class CTCInventoryItem : public CTCInventoryItemBase
{ public:
    void OnDie();
};

void CTCInventoryItem::OnDie()
{
    void* p = this->thing;
    if (p != 0)
        free(p);
    this->OnDieBase();
}