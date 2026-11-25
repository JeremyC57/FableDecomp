#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004E4045 now binds its PDB spelling ??1CTCDExplosiveTrail@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCDExplosiveTrail_Destruct@@YIXPAX@Z.
class CTCDExplosiveTrail { public: virtual ~CTCDExplosiveTrail(); };
void* __fastcall CTCDExplosiveTrail_ScalarDeletingDestructor(void* trail, int, unsigned int flags)
{
    reinterpret_cast<CTCDExplosiveTrail*>(trail)->CTCDExplosiveTrail::~CTCDExplosiveTrail();
    if (flags & 1)
        operator delete(trail);
    return trail;
}