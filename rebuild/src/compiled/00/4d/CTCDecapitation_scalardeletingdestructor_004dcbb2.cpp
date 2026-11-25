#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004DCBCE now binds its PDB spelling ??1CTCDecapitation@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCDecapitation_Destruct@@YIXPAX@Z.
class CTCDecapitation { public: virtual ~CTCDecapitation(); };
void* __fastcall CTCDecapitation_ScalarDeletingDestructor(void* decapitation, int, unsigned int flags)
{
    reinterpret_cast<CTCDecapitation*>(decapitation)->CTCDecapitation::~CTCDecapitation();
    if (flags & 1)
        operator delete(decapitation);
    return decapitation;
}