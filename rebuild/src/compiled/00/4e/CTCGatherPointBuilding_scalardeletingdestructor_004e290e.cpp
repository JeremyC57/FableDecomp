#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004E0AE8 now binds its PDB spelling ??1CTCGatherPointBase@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCGatherPointBuilding_Destruct@@YIXPAX@Z.
class CTCGatherPointBase { public: virtual ~CTCGatherPointBase(); };
void* __fastcall CTCGatherPointBuilding_ScalarDeletingDestructor(void* building, int, unsigned int flags)
{
    reinterpret_cast<CTCGatherPointBase*>(building)->CTCGatherPointBase::~CTCGatherPointBase();
    if (flags & 1)
        operator delete(building);
    return building;
}