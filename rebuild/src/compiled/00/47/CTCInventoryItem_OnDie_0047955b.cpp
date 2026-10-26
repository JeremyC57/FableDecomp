#pragma optimize("s",on)
// Leaf: OnDie
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
class CTCInventoryItem
{ public:
    char pad[0x3c];
    void* field3c;
};

extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall OnDie(CTCInventoryItem* self)
{
    void* p = self->field3c;
    if (p)
        free(p);
    ((CParentDefClassBase*)(self))->CParentDefClassBase::~CParentDefClassBase();
}