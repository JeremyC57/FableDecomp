#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004DE44D now binds its PDB spelling ??1CTCStockItem@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCStockItem_Destruct@@YIXPAX@Z.
class CTCStockItem { public: virtual ~CTCStockItem(); };
void* __fastcall CTCStockItem_ScalarDeletingDestructor(void* item, int, unsigned int flags)
{
    reinterpret_cast<CTCStockItem*>(item)->CTCStockItem::~CTCStockItem();
    if (flags & 1)
        operator delete(item);
    return item;
}