#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x00901750 now binds its PDB spelling ??1CAIStateGroup_TavernServer@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CAIStateGroup_TavernServer_Destruct@@YIXPAX@Z.
class CAIStateGroup_TavernServer { public: virtual ~CAIStateGroup_TavernServer(); };
void* __fastcall CAIStateGroup_TavernServer_ScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    reinterpret_cast<CAIStateGroup_TavernServer*>(stateGroup)->CAIStateGroup_TavernServer::~CAIStateGroup_TavernServer();
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}