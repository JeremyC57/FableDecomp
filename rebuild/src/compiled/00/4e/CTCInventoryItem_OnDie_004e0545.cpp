#pragma optimize("s",on)
// Leaf: OnDie
// Byte-exact reconstruction of CTCInventoryItem::OnDie @ 0x004e0545
// __fastcall(void) -> modeled as __fastcall(self)

class CResource {
public:
    virtual ~CResource();   // 0x009FC550
};
class CTCInventoryItem { public:
    void* vtbl;        // +0x00
    char  pad[0x24];   // +0x04 .. +0x27
    void* obj28;       // +0x28
};

// operator delete / deallocator @ 0x00bfea14 (cdecl, 1 ptr arg, caller cleanup)
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

// base-class OnDie @ 0x009fc550 (this-call, void)

void __fastcall OnDie(CTCInventoryItem* self)
{
    if (self->obj28)
        free(self->obj28);
    self->vtbl = (void*)0x01230ba0;
    ((CResource*)(self))->CResource::~CResource();
}