#pragma optimize("s",on)
// Leaf: OnDie
// Reconstruction of CTCInventoryItem::OnDie @ 0x004e0b7d
class CResource {
public:
    virtual ~CResource();   // 0x009FC550
};
typedef unsigned long DWORD;

class CTCInventoryItem { public:
    DWORD vtbl;      // +0x00
    char pad[0x30];  // +0x04 .. +0x33
    void* p34;       // +0x34
};

extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

#define VTBL_CTCINVENTORYITEM ((DWORD)0x1230ba0)

void __fastcall OnDie(CTCInventoryItem* self)
{
    if (self->p34 != 0)
        free(self->p34);
    self->vtbl = VTBL_CTCINVENTORYITEM;
    ((CResource*)(self))->CResource::~CResource();
}