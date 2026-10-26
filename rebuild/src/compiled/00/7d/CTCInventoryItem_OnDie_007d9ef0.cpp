// Byte-exact reconstruction of CTCInventoryItem::OnDie @ 0x007d9ef0
// Leaf: OnDie
class CTCInventoryItem;

// cdecl helper @ 0xbfea14  (add esp,4 after call => __cdecl, one ptr arg)
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

// base OnDie member @ 0x5eda50 (this-call, void)
void __fastcall CTCInventoryItem_BaseOnDie(CTCInventoryItem* self);

class CTCInventoryItem
{ public:
    char pad[0x110];
    void* m_ptr; // at +0x110
};

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    void* p = self->m_ptr;
    if (p != 0)
        free(p);
    CTCInventoryItem_BaseOnDie(self);
}