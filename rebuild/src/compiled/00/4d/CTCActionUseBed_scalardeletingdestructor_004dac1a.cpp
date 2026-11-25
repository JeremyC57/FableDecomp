#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x007EBDD0 now binds its PDB spelling ??1CTCActionUseBed@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCActionUseBed_Destruct@@YIXPAX@Z.
class CTCActionUseBed { public: virtual ~CTCActionUseBed(); };
void* __fastcall CTCActionUseBed_ScalarDeletingDestructor(void* bed, int, unsigned int flags)
{
    reinterpret_cast<CTCActionUseBed*>(bed)->CTCActionUseBed::~CTCActionUseBed();
    if (flags & 1)
        operator delete(bed);
    return bed;
}