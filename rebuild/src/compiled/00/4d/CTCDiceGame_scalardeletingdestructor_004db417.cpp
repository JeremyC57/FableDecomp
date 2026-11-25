#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004D607C now binds its PDB spelling ??1CTCDiceGame@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCDiceGame_Destruct@@YIXPAX@Z.
class CTCDiceGame { public: virtual ~CTCDiceGame(); };
void* __fastcall CTCDiceGame_ScalarDeletingDestructor(void* game, int, unsigned int flags)
{
    reinterpret_cast<CTCDiceGame*>(game)->CTCDiceGame::~CTCDiceGame();
    if (flags & 1)
        operator delete(game);
    return game;
}