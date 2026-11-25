#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x005F3CFE now binds its PDB spelling ??1CTCInventoryAbilities@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCInventoryAbilities_Destruct@@YIXPAX@Z.
class CTCInventoryAbilities { public: virtual ~CTCInventoryAbilities(); };
void* __fastcall CTCInventoryAbilities_ScalarDeletingDestructor(void* abilities, int, unsigned int flags)
{
    reinterpret_cast<CTCInventoryAbilities*>(abilities)->CTCInventoryAbilities::~CTCInventoryAbilities();
    if (flags & 1)
        operator delete(abilities);
    return abilities;
}