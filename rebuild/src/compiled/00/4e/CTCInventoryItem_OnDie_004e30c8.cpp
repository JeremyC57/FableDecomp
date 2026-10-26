#pragma optimize("s",on)
// Leaf: OnDie
// Byte-exact reconstruction of CTCInventoryItem::OnDie @ 0x004e30c8
// __fastcall (this in ecx) -> modeled as __fastcall free fn (self in ecx)

class CResource {
public:
    virtual ~CResource();   // 0x009FC550
};
typedef unsigned int uint;

class CTCInventoryItem
{ public:
    void* vtbl;        // +0x00
    char  pad[0x54];   // +0x04 .. +0x57
    void* sub;         // +0x58
};

// cdecl free-style callee @ 0xbfea14 (push arg; call; pop ecx cleanup)
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

// base OnDie (tail-called with this in ecx) @ 0x9fc550

void __fastcall OnDie(CTCInventoryItem* self)
{
    if (self->sub)
        free(self->sub);
    self->vtbl = (void*)0x1230ba0;
    ((CResource*)(self))->CResource::~CResource();
}