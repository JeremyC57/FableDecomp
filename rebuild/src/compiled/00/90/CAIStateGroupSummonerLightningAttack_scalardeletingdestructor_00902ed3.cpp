#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x00902EEF now binds its PDB spelling ??1CAIStateGroup_SummonerLightningAttack@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CAIStateGroup_SummonerLightningAttack_Destruct@@YIXPAX@Z.
class CAIStateGroup_SummonerLightningAttack { public: virtual ~CAIStateGroup_SummonerLightningAttack(); };
void* __fastcall CAIStateGroup_SummonerLightningAttack_ScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    reinterpret_cast<CAIStateGroup_SummonerLightningAttack*>(stateGroup)->CAIStateGroup_SummonerLightningAttack::~CAIStateGroup_SummonerLightningAttack();
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}