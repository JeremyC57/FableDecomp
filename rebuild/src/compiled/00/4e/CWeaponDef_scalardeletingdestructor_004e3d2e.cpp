#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004E3D4A now binds its PDB spelling ??1CWeaponDef@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CWeaponDef_Destruct@@YIXPAX@Z.
class CWeaponDef { public: virtual ~CWeaponDef(); };
void* __fastcall CWeaponDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    reinterpret_cast<CWeaponDef*>(def)->CWeaponDef::~CWeaponDef();
    if (flags & 1)
        operator delete(def);
    return def;
}