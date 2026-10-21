// CPlayerManager::CreatePlayers() -- main player 0, create players 0..3, then the neutral player 4
// Leaf: ?CreatePlayers@CPlayerManager@@QAEXXZ
// Retail: Fable.exe @ 0x0044A530, 49 bytes; Ego_r.pdb ?CreatePlayers@CPlayerManager@@QAEXXZ. Catalog label
// CTCHeroExperience::UpdateAbilitiesForAllStats is wrong: the only caller, CMainGameComponent::CreatePlayers (0x004166A8),
// calls it on PPlayerManager (CMainGameComponent +0x1c), and the callee 0x0044A1A0 (CPlayerManager::IsPlayer 0x00449880
// test, then a 0x22c-byte CPlayer pushed onto Players +0x0c) is the private CPlayerManager::CreatePlayer(long). Body:
// MainPlayer = 0; CreatePlayer(i) for i = 0..3; PlayerNeutral = 4; CreatePlayer(4).
// Layout: retail CPlayerManager -- Players (12-byte STLport vector) +0x0c, PlayerNeutral +0x18, MainPlayer +0x1c (Ego_r.pdb
// +0x1c/+0x20 with a 16-byte vector); rebuild/include/engine/CPlayerManager.h carries those PDB offsets, so a local class.
// Shape: /O2 (`lea esp,[esp]` loop-head pad); the neutral index is the constant 4 both stored and passed.
class CMainGameComponent;
class CGameDefinitionManager;
class CPlayer;
class CPlayerManager {
public:
    void*                   _vfptr_or_pad;               // +0x00
    CMainGameComponent*     Component;                   // +0x04
    CGameDefinitionManager* DefinitionManager;           // +0x08
    CPlayer**               Players[3];                  // +0x0c STLport vector<CPlayer*>
    long                    PlayerNeutral;               // +0x18
    long                    MainPlayer;                  // +0x1c
    void CreatePlayers();
private:
    void CreatePlayer(long player_number);               // 0x0044A1A0
    friend struct CPlayerManager_Test;                   // behaviour-test hook (CreatePlayer is private)
};
void CPlayerManager::CreatePlayers()
{
    MainPlayer = 0;
    for (long i = 0; i < 4; ++i)
        CreatePlayer(i);
    PlayerNeutral = 4;
    CreatePlayer(4);
}