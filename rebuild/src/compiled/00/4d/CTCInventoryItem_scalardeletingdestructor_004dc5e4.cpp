#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004DC600 now binds its PDB spelling ??1CTCInventoryItem@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCInventoryItem_Destruct@@YIXPAX@Z.
class CTCInventoryItem { public: virtual ~CTCInventoryItem(); };
void* __fastcall CTCInventoryItem_ScalarDeletingDestructor(void* item, int, unsigned int flags)
{
    reinterpret_cast<CTCInventoryItem*>(item)->CTCInventoryItem::~CTCInventoryItem();
    if (flags & 1)
        operator delete(item);
    return item;
}