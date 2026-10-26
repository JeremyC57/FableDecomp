#pragma optimize("s",on)
// Leaf: OnDie
// Byte-exact reconstruction of CTCInventoryItem::OnDie @ 0x004e2b2c
class CResource {
public:
    virtual ~CResource();   // 0x009FC550
};
class CTCInventoryItem { public:
    void* vtbl;      // +0x00
    char pad[0x24];  // +0x04 .. +0x27
    void* p28;       // +0x28
};

// cdecl free (push arg; call; pop ecx cleanup)
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

// base class OnDie (this-call) forwarded to via tail jmp

void __fastcall OnDie(CTCInventoryItem* self)
{
    if (self->p28 != 0)
        free(self->p28);
    self->vtbl = (void*)0x1230ba0;
    ((CResource*)(self))->CResource::~CResource();
}