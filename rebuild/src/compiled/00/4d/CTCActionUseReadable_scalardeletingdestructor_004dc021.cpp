#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004DC03D now binds its PDB spelling ??1CTCActionUseReadable@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCActionUseReadable_Destruct@@YIXPAX@Z.
class CTCActionUseReadable { public: virtual ~CTCActionUseReadable(); };
void* __fastcall CTCActionUseReadable_ScalarDeletingDestructor(void* readable, int, unsigned int flags)
{
    reinterpret_cast<CTCActionUseReadable*>(readable)->CTCActionUseReadable::~CTCActionUseReadable();
    if (flags & 1)
        operator delete(readable);
    return readable;
}