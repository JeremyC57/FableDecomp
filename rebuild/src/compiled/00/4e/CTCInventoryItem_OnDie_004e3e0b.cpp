#pragma optimize("s",on)
// Leaf: OnDie
// Byte-exact reconstruction of CTCInventoryItem::OnDie @ 0x004e3e0b
// __fastcall(void)  -> modeled as __fastcall free fn (this in ecx)

class CResource {
public:
    virtual ~CResource();   // 0x009FC550
};
class CTCInventoryItem { public:
    void* vtable;      // +0x00
    char  pad[0x24];   // +0x04 .. +0x27
    void* member28;    // +0x28
};

// cdecl callee cleaned up by caller pop (single arg): operator delete / free
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

// base OnDie / dtor, tail-jumped (this in ecx)

void __fastcall OnDie(CTCInventoryItem* self)
{
    if (self->member28)
        free(self->member28);
    self->vtable = (void*)0x1230ba0;
    ((CResource*)(self))->CResource::~CResource();
}