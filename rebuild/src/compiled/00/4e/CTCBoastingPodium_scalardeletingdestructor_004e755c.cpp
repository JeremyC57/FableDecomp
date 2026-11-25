#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004E7578 now binds its PDB spelling ??1CTCBoastingPodium@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCBoastingPodium_Destruct@@YIXPAX@Z.
class CTCBoastingPodium { public: virtual ~CTCBoastingPodium(); };
void* __fastcall CTCBoastingPodium_ScalarDeletingDestructor(void* podium, int, unsigned int flags)
{
    reinterpret_cast<CTCBoastingPodium*>(podium)->CTCBoastingPodium::~CTCBoastingPodium();
    if (flags & 1)
        operator delete(podium);
    return podium;
}