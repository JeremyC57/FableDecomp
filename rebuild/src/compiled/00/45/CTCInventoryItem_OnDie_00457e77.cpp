#pragma optimize("s",on)
// Leaf: OnDie
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
class CTCInventoryItem { public: void* vptr; char _pad4[0x44]; void* ptr48; };
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    void* p = self->ptr48;
    if (p) {
        free(p);
    }
    ((CParentDefClassBase*)(self))->CParentDefClassBase::~CParentDefClassBase();
}