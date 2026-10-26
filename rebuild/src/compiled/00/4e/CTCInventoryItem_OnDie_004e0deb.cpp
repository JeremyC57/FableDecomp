#pragma optimize("s",on)
// Leaf: OnDie
// Byte-exact reconstruction of CTCInventoryItem::OnDie @ 0x004e0deb
// thiscall(void) modeled as __fastcall(self) so this arrives in ecx.

class CResource {
public:
    virtual ~CResource();   // 0x009FC550
};
typedef unsigned long DWORD;

class CTCInventoryItem { public:
    DWORD* vtable;      // +0x00
    char pad[0x28 - 4]; // up to +0x28
    void* p28;          // +0x28
};

// cdecl callee at 0xbfea14 (single-arg free/delete-like), stack cleaned by caller (pop ecx)
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

// base destructor / OnDie at 0x9fc550, thiscall -> model as __fastcall(self)

void __fastcall OnDie(CTCInventoryItem* self)
{
    void* p = self->p28;
    if (p) {
        free(p);
    }
    self->vtable = (DWORD*)0x1230ba0;
    ((CResource*)(self))->CResource::~CResource();
}