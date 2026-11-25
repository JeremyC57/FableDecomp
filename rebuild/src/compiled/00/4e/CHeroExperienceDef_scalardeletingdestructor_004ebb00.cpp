#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004EBB1C now binds its PDB spelling ??1CHeroExperienceDef@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CHeroExperienceDef_Destruct@@YIXPAX@Z.
class CHeroExperienceDef { public: virtual ~CHeroExperienceDef(); };
void* __fastcall CHeroExperienceDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    reinterpret_cast<CHeroExperienceDef*>(def)->CHeroExperienceDef::~CHeroExperienceDef();
    if (flags & 1)
        operator delete(def);
    return def;
}