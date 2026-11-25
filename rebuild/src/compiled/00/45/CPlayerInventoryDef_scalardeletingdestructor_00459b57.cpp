#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x00459B73 now binds its PDB spelling ??1CPlayerInventoryDef@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CPlayerInventoryDef_Destruct@@YIXPAX@Z.
class CPlayerInventoryDef { public: virtual ~CPlayerInventoryDef(); };
void* __fastcall CPlayerInventoryDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    reinterpret_cast<CPlayerInventoryDef*>(def)->CPlayerInventoryDef::~CPlayerInventoryDef();
    if (flags & 1)
        operator delete(def);
    return def;
}