#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x009017F2 now binds its PDB spelling ??1CAIStateGroup_CarryCrateAndFollow@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CAIStateGroup_TavernKeeper_Destruct@@YIXPAX@Z.
class CAIStateGroup_CarryCrateAndFollow { public: virtual ~CAIStateGroup_CarryCrateAndFollow(); };
void* __fastcall CAIStateGroup_TavernKeeper_ScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    reinterpret_cast<CAIStateGroup_CarryCrateAndFollow*>(stateGroup)->CAIStateGroup_CarryCrateAndFollow::~CAIStateGroup_CarryCrateAndFollow();
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}