#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x005C39F0 now binds its PDB spelling ??1CTCInventoryWeapons@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCInventoryWeapons_Destruct@@YIXPAX@Z.
class CTCInventoryWeapons { public: virtual ~CTCInventoryWeapons(); };
void* __fastcall CTCInventoryWeapons_ScalarDeletingDestructor(void* weapons, int, unsigned int flags)
{
    reinterpret_cast<CTCInventoryWeapons*>(weapons)->CTCInventoryWeapons::~CTCInventoryWeapons();
    if (flags & 1)
        operator delete(weapons);
    return weapons;
}