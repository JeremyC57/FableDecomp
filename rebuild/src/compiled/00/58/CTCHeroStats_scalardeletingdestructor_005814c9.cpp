#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x005814E5 now binds its PDB spelling ??1CTCHeroStats@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCHeroStats_Destruct@@YIXPAX@Z.
class CTCHeroStats { public: virtual ~CTCHeroStats(); };
void* __fastcall CTCHeroStats_ScalarDeletingDestructor(void* stats, int, unsigned int flags)
{
    reinterpret_cast<CTCHeroStats*>(stats)->CTCHeroStats::~CTCHeroStats();
    if (flags & 1)
        operator delete(stats);
    return stats;
}