#pragma optimize("s",on)
// Leaf: OnDie
class CResource {
public:
    virtual ~CResource();   // 0x009FC550
};
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

class CTCInventoryItem { public:
    void* vptr;                    // +0x00
    unsigned char _pad_0x4[0x24];  // to 0x28
    void* ptr28;                   // +0x28
};

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    if (self->ptr28)
        free(self->ptr28);
    self->vptr = (void*)0x1230ba0;
    ((CResource*)(self))->CResource::~CResource();
}