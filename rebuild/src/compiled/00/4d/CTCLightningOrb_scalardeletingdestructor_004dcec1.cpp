#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004DCEDD now binds its PDB spelling ??1CTCLightningOrb@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCLightningOrb_Destruct@@YIXPAX@Z.
class CTCLightningOrb { public: virtual ~CTCLightningOrb(); };
void* __fastcall CTCLightningOrb_ScalarDeletingDestructor(void* orb, int, unsigned int flags)
{
    reinterpret_cast<CTCLightningOrb*>(orb)->CTCLightningOrb::~CTCLightningOrb();
    if (flags & 1)
        operator delete(orb);
    return orb;
}