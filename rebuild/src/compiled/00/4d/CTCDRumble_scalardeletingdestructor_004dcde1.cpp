#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004DCDFD now binds its PDB spelling ??1CTCDRumble@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCDRumble_Destruct@@YIXPAX@Z.
class CTCDRumble { public: virtual ~CTCDRumble(); };
void* __fastcall CTCDRumble_ScalarDeletingDestructor(void* rumble, int, unsigned int flags)
{
    reinterpret_cast<CTCDRumble*>(rumble)->CTCDRumble::~CTCDRumble();
    if (flags & 1)
        operator delete(rumble);
    return rumble;
}