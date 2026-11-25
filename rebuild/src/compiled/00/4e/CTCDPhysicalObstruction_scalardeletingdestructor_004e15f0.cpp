#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004E160C now binds its PDB spelling ??1CTCDPhysicalObstruction@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCDPhysicalObstruction_Destruct@@YIXPAX@Z.
class CTCDPhysicalObstruction { public: virtual ~CTCDPhysicalObstruction(); };
void* __fastcall CTCDPhysicalObstruction_ScalarDeletingDestructor(void* obstruction, int, unsigned int flags)
{
    reinterpret_cast<CTCDPhysicalObstruction*>(obstruction)->CTCDPhysicalObstruction::~CTCDPhysicalObstruction();
    if (flags & 1)
        operator delete(obstruction);
    return obstruction;
}