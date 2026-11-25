#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x00459E46 now binds its PDB spelling ??1CPlayerGuiDef@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CPlayerGuiDef_Destruct@@YIXPAX@Z.
class CPlayerGuiDef { public: virtual ~CPlayerGuiDef(); };
void* __fastcall CPlayerGuiDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    reinterpret_cast<CPlayerGuiDef*>(def)->CPlayerGuiDef::~CPlayerGuiDef();
    if (flags & 1)
        operator delete(def);
    return def;
}