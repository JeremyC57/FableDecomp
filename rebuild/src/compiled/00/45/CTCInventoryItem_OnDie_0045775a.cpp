#pragma optimize("s",on)
// Leaf: OnDie
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
class CTCInventoryItem { public:
    void* vptr;
    unsigned char _pad4[0x94];
    void* ptr98; // +0x98
};

extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    void* p = self->ptr98;
    if (p != 0)
        free(p);
    ((CParentDefClassBase*)(self))->CParentDefClassBase::~CParentDefClassBase();
}