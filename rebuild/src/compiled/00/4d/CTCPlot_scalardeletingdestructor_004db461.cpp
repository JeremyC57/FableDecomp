#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004D607C now binds its PDB spelling ??1CTCDiceGame@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCPlot_Destruct@@YIXPAX@Z.
class CTCDiceGame { public: virtual ~CTCDiceGame(); };
void* __fastcall CTCPlot_ScalarDeletingDestructor(void* plot, int, unsigned int flags)
{
    reinterpret_cast<CTCDiceGame*>(plot)->CTCDiceGame::~CTCDiceGame();
    if (flags & 1)
        operator delete(plot);
    return plot;
}