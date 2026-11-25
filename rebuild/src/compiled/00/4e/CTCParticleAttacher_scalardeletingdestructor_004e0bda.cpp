#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004E0BF6 now binds its PDB spelling ??1CTCParticleAttacher@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCParticleAttacher_Destruct@@YIXPAX@Z.
class CTCParticleAttacher { public: virtual ~CTCParticleAttacher(); };
void* __fastcall CTCParticleAttacher_ScalarDeletingDestructor(void* attacher, int, unsigned int flags)
{
    reinterpret_cast<CTCParticleAttacher*>(attacher)->CTCParticleAttacher::~CTCParticleAttacher();
    if (flags & 1)
        operator delete(attacher);
    return attacher;
}