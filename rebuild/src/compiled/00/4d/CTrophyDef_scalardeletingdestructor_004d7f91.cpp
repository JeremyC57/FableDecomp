#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004D7FAD now binds its PDB spelling ??1CTrophyDef@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTrophyDef_Destruct@@YIXPAX@Z.
class CTrophyDef { public: virtual ~CTrophyDef(); };
void* __fastcall CTrophyDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    reinterpret_cast<CTrophyDef*>(def)->CTrophyDef::~CTrophyDef();
    if (flags & 1)
        operator delete(def);
    return def;
}