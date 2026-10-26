#pragma optimize("s",on)
// Leaf: OnDie
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
class CTCInventoryItem { public:
    unsigned char _pad_0x0[0x10];
    void* PInventory;
    unsigned char _pad_0x14[0xc];
    char DefaultItem;
    unsigned char _pad_0x21[0x5f];
    void* ptr80; // +0x80
};

extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    if (self->ptr80)
        free(self->ptr80);
    ((CParentDefClassBase*)(self))->CParentDefClassBase::~CParentDefClassBase();
}