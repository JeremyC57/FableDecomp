// CTCInventoryItem::OnDie  0x004e0fce  __fastcall(void)
// Leaf: OnDie
class CResource {
public:
    virtual ~CResource();   // 0x009FC550
};
typedef unsigned int uintptr_t32;

class CTCInventoryItem { public:
    void* vtbl;      // +0x00
    char pad[0x40-4];
    void* m40;       // +0x40
};

// free-ish cleanup callee at 0xbfea14 (cdecl: caller pops -> pop ecx)
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

// tail-call member target at 0x9fc550 (__fastcall)

#pragma optimize("s",on)
void __fastcall OnDie(CTCInventoryItem* self)
{
    if (self->m40)
        free(self->m40);
    self->vtbl = (void*)0x1230ba0;
    ((CResource*)(self))->CResource::~CResource();
}