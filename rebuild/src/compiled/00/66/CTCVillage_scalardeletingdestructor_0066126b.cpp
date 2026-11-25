#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x0065B598 now binds its PDB spelling ??1CTCVillage@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCVillage_Destruct@@YIXPAX@Z.
class CTCVillage { public: virtual ~CTCVillage(); };
void* __fastcall CTCVillage_ScalarDeletingDestructor(void* village, int, unsigned int flags)
{
    reinterpret_cast<CTCVillage*>(village)->CTCVillage::~CTCVillage();
    if (flags & 1)
        operator delete(village);
    return village;
}