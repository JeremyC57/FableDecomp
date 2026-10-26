#pragma optimize("s",on)
// Leaf: OnDie
// Byte-exact reconstruction of CTCInventoryItem::OnDie @ 0x004e3e89
// this-call modeled as __fastcall free fn (this in ecx).

class CResource {
public:
    virtual ~CResource();   // 0x009FC550
};
class CTCInventoryItem { public:
    void* vtbl;      // +0x00
    char  pad[0x24]; // +0x04 .. +0x27
    void* m28;       // +0x28
};

// operator delete (cdecl) @ 0xbfea14
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

// base OnDie (this-call) @ 0x9fc550

extern const int g_CTCInventoryItem_vtbl;

void __fastcall OnDie(CTCInventoryItem* self)
{
    if (self->m28)
        free(self->m28);
    self->vtbl = (void*)0x1230ba0;
    ((CResource*)(self))->CResource::~CResource();
}