#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004DC659 now binds its PDB spelling ??1CTCVillageMember@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCVillageMember_Destruct@@YIXPAX@Z.
class CTCVillageMember { public: virtual ~CTCVillageMember(); };
void* __fastcall CTCVillageMember_ScalarDeletingDestructor(void* member, int, unsigned int flags)
{
    reinterpret_cast<CTCVillageMember*>(member)->CTCVillageMember::~CTCVillageMember();
    if (flags & 1)
        operator delete(member);
    return member;
}