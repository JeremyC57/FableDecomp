#pragma optimize("s",on)
// Leaf: OnDie
struct CTCInventoryItem { void* vptr; char _pad4[0x44]; void* ptr48; };
extern "C" void __cdecl freeThing(void* p);
extern void __fastcall CTCInventoryItem_base_OnDie_00430300(CTCInventoryItem* self);

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    void* p = self->ptr48;
    if (p) {
        freeThing(p);
    }
    CTCInventoryItem_base_OnDie_00430300(self);
}