#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x005C00A0 now binds its PDB spelling ??1CTCInventoryBase@NInventory@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCInventoryBase_Destruct@@YIXPAX@Z.
namespace NInventory { class CTCInventoryBase { public: virtual ~CTCInventoryBase(); }; }
void* __fastcall CTCInventoryBase_ScalarDeletingDestructor(void* inventory, int, unsigned int flags)
{
    reinterpret_cast<NInventory::CTCInventoryBase*>(inventory)->NInventory::CTCInventoryBase::~CTCInventoryBase();
    if (flags & 1)
        operator delete(inventory);
    return inventory;
}