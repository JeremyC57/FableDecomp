#pragma optimize("s",on)
// Leaf: OnDie
class CInitBaseClass {
public:
    virtual ~CInitBaseClass();   // 0x0099A410
};
class CTCInventoryItem { public:
    unsigned char _pad_0x0[0x8];
    void* ptr8;
    unsigned char _pad_0xc[0x4];
    void* PInventory;
    unsigned char _pad_0x14[0xc];
    char DefaultItem;
};
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    void* p = self->ptr8;
    if (p)
        free(p);
    ((CInitBaseClass*)(self))->CInitBaseClass::~CInitBaseClass();
}