#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004E3CB3 now binds its PDB spelling ??1CContainerRewardHeroDef@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CContainerRewardHeroDef_Destruct@@YIXPAX@Z.
class CContainerRewardHeroDef { public: virtual ~CContainerRewardHeroDef(); };
void* __fastcall CContainerRewardHeroDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    reinterpret_cast<CContainerRewardHeroDef*>(def)->CContainerRewardHeroDef::~CContainerRewardHeroDef();
    if (flags & 1)
        operator delete(def);
    return def;
}