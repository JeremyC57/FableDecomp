#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004DA967 now binds its PDB spelling ??1CTCActionUseBase@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCActionUseBase_Destruct@@YIXPAX@Z.
class CTCActionUseBase { public: virtual ~CTCActionUseBase(); };
void* __fastcall CTCActionUseBase_ScalarDeletingDestructor(void* use, int, unsigned int flags)
{
    reinterpret_cast<CTCActionUseBase*>(use)->CTCActionUseBase::~CTCActionUseBase();
    if (flags & 1)
        operator delete(use);
    return use;
}