#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x00902F49 now binds its PDB spelling ??1CAIStateGroup_SummonerCharge@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CAIStateGroup_SummonerCharge_Destruct@@YIXPAX@Z.
class CAIStateGroup_SummonerCharge { public: virtual ~CAIStateGroup_SummonerCharge(); };
void* __fastcall CAIStateGroup_SummonerCharge_ScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    reinterpret_cast<CAIStateGroup_SummonerCharge*>(stateGroup)->CAIStateGroup_SummonerCharge::~CAIStateGroup_SummonerCharge();
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}