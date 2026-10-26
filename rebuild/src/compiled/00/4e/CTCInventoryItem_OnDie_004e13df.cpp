// Byte-exact reconstruction of CTCInventoryItem::OnDie @ 0x004e13df
// Leaf: OnDie
#pragma optimize("s",on)

class CResource {
public:
    virtual ~CResource();   // 0x009FC550
};
class CTCInventoryItem;

// cdecl free callee at 0xbfea14 (push eax; call; pop ecx cleanup)
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

// base class OnDie (this-call member) at 0x9fc550

class CTCInventoryItem { public:
    void* vtbl;      // +0x00
    char pad[0x68];  // +0x04 .. +0x6b
    void* field6c;   // +0x6c
};

void __fastcall OnDie(CTCInventoryItem* self)
{
    if (self->field6c) {
        free(self->field6c);
    }
    self->vtbl = (void*)0x1230ba0;
    ((CResource*)(self))->CResource::~CResource();
}