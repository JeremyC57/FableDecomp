#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004DBF56 now binds its PDB spelling ??1CTCTargeted@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCTargeted_Destruct@@YIXPAX@Z.
class CTCTargeted { public: virtual ~CTCTargeted(); };
void* __fastcall CTCTargeted_ScalarDeletingDestructor(void* targeted, int, unsigned int flags)
{
    reinterpret_cast<CTCTargeted*>(targeted)->CTCTargeted::~CTCTargeted();
    if (flags & 1)
        operator delete(targeted);
    return targeted;
}