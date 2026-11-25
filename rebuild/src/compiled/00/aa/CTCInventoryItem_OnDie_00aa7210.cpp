// CTCInventoryItem::OnDie @ 0x00aa7210  __fastcall void(CTCInventoryItem*)
// Frees the pointer member at +0x10 (cdecl helper) if non-null, then
// tail-calls the base-class OnDie (__fastcall) on this.

extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

struct CTCInventoryItemBase
{
};

class CTCInventoryItem : public CTCInventoryItemBase
{ public:
    char pad0[0x10];
    void* PInventory;     // +0x10
    void OnDie();
};

// Naming (NF449 gen, 2026-11-25): callee-only respell of this row's destructor stand-in calls to the PDB destructor (T1 tracker;
//   target self rows' Leaf), as a qualified destructor call on a bodiless declaration (no layout; rulings r2/r3):
//   0x00AA75C0 ??1CChunkedFileChunk@@UAE@XZ (was ?BaseOnDie@CTCInventoryItemBase@@QAEXXZ)
class CChunkedFileChunk { public: virtual ~CChunkedFileChunk(); };
void CTCInventoryItem::OnDie()
{
    if (this->PInventory)
        free(this->PInventory);
    reinterpret_cast<CChunkedFileChunk*>(this)->CChunkedFileChunk::~CChunkedFileChunk();
}