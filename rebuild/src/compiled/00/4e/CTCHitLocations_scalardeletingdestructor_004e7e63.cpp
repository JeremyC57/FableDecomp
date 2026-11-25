#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004E7E7F now binds its PDB spelling ??1CTCHitLocations@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCHitLocations_Destruct@@YIXPAX@Z.
class CTCHitLocations { public: virtual ~CTCHitLocations(); };
void* __fastcall CTCHitLocations_ScalarDeletingDestructor(void* locations, int, unsigned int flags)
{
    reinterpret_cast<CTCHitLocations*>(locations)->CTCHitLocations::~CTCHitLocations();
    if (flags & 1)
        operator delete(locations);
    return locations;
}