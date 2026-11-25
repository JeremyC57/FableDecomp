#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004585BD now binds its PDB spelling ??1CArmourDef@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CArmourDef_Destruct@@YIXPAX@Z.
class CArmourDef { public: virtual ~CArmourDef(); };
void* __fastcall CArmourDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    reinterpret_cast<CArmourDef*>(def)->CArmourDef::~CArmourDef();
    if (flags & 1)
        operator delete(def);
    return def;
}