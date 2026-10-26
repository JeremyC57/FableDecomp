#pragma optimize("s",on)
// Leaf: OnDie
// CTCInventoryItem::OnDie @ 004f9cfc
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
class CTCInventoryItem { public:
    char pad[0x3c];
    void* f3c;
};

extern "C" void __cdecl free_bfea14(void* p);

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    if (self->f3c)
        free_bfea14(self->f3c);
    ((CParentDefClassBase*)(self))->CParentDefClassBase::~CParentDefClassBase();
}