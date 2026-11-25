#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x007891F8 now binds its PDB spelling ??1CTCTrapBase@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCMeleeTrap_Destruct@@YIXPAX@Z.
class CTCTrapBase { public: virtual ~CTCTrapBase(); };
void* __fastcall CTCMeleeTrap_ScalarDeletingDestructor(void* trap, int, unsigned int flags)
{
    reinterpret_cast<CTCTrapBase*>(trap)->CTCTrapBase::~CTCTrapBase();
    if (flags & 1)
        operator delete(trap);
    return trap;
}