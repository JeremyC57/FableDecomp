#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x008FDCCA now binds its PDB spelling ??1CAIStateGroup_GoToPub@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CAIStateGroup_GoToPub_Destruct@@YIXPAX@Z.
class CAIStateGroup_GoToPub { public: virtual ~CAIStateGroup_GoToPub(); };
void* __fastcall CAIStateGroup_GoToPub_ScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    reinterpret_cast<CAIStateGroup_GoToPub*>(stateGroup)->CAIStateGroup_GoToPub::~CAIStateGroup_GoToPub();
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}