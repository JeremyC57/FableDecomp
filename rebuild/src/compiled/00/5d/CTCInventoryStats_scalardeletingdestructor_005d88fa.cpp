#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x005CD2FE now binds its PDB spelling ??1CTCInventoryStats@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCInventoryStats_Destruct@@YIXPAX@Z.
class CTCInventoryStats { public: virtual ~CTCInventoryStats(); };
void* __fastcall CTCInventoryStats_ScalarDeletingDestructor(void* stats, int, unsigned int flags)
{
    reinterpret_cast<CTCInventoryStats*>(stats)->CTCInventoryStats::~CTCInventoryStats();
    if (flags & 1)
        operator delete(stats);
    return stats;
}