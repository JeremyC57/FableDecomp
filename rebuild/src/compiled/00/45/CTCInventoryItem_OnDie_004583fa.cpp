#pragma optimize("s",on)
// Leaf: OnDie
// CTCInventoryItem::OnDie @ 004583fa
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
class CTCInventoryItem { public:
    char pad[0x44];
    void* f44;
};

extern "C" void my_free(void* p);                        // cdecl free @ 0xbfea14

void __fastcall OnDie(CTCInventoryItem* self)
{
    if (self->f44)
        my_free(self->f44);
    ((CParentDefClassBase*)(self))->CParentDefClassBase::~CParentDefClassBase();
}