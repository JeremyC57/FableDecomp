#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004D9861 now binds its PDB spelling ??1CFireheartMinigameDef@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CFireheartMinigameDef_Destruct@@YIXPAX@Z.
class CFireheartMinigameDef { public: virtual ~CFireheartMinigameDef(); };
void* __fastcall CFireheartMinigameDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    reinterpret_cast<CFireheartMinigameDef*>(def)->CFireheartMinigameDef::~CFireheartMinigameDef();
    if (flags & 1)
        operator delete(def);
    return def;
}