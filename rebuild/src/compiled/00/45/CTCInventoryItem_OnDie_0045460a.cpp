#pragma optimize("s",on)
// Leaf: OnDie
class CResource {
public:
    virtual ~CResource();   // 0x009FC550
};
class CTCInventoryItem { public:
    unsigned char _pad_0x0[0x10];
    void* PInventory;
    unsigned char _pad_0x14[0xc];
    char DefaultItem;
    unsigned char _pad_0x21[0xf];
    void* ptr30;
};
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)
extern const void* const CTCInventoryItem_vftable;
void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    if (self->ptr30)
        free(self->ptr30);
    *(const void**)self = &CTCInventoryItem_vftable;
    ((CResource*)(self))->CResource::~CResource();
}