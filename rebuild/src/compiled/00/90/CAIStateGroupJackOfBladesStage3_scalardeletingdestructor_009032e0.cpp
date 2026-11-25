#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x00903046 now binds its PDB spelling ??1CAIStateGroup_JackOfBladesBase@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CAIStateGroup_JackOfBladesStage3_Destruct@@YIXPAX@Z.
class CAIStateGroup_JackOfBladesBase { public: virtual ~CAIStateGroup_JackOfBladesBase(); };
void* __fastcall CAIStateGroup_JackOfBladesStage3_ScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    reinterpret_cast<CAIStateGroup_JackOfBladesBase*>(stateGroup)->CAIStateGroup_JackOfBladesBase::~CAIStateGroup_JackOfBladesBase();
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}