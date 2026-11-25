#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004DC916 now binds its PDB spelling ??1CTCInterestingToVillagers@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCInterestingToVillagers_Destruct@@YIXPAX@Z.
class CTCInterestingToVillagers { public: virtual ~CTCInterestingToVillagers(); };
void* __fastcall CTCInterestingToVillagers_ScalarDeletingDestructor(void* villagers, int, unsigned int flags)
{
    reinterpret_cast<CTCInterestingToVillagers*>(villagers)->CTCInterestingToVillagers::~CTCInterestingToVillagers();
    if (flags & 1)
        operator delete(villagers);
    return villagers;
}