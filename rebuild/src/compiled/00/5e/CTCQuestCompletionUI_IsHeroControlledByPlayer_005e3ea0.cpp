// CTCQuestCompletionUI::IsHeroControlledByPlayer() const -- false while the main player is in one of the non-control modes (4..15, 17, 18, 21, 23, 26, 27, 29, 30, 32, 36, 37), true otherwise
// Leaf: IsHeroControlledByPlayer
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E3EA0, 90 bytes incl. the two-entry jump table 0x005E3ED0 and the 34-entry byte index
// table 0x005E3ED8 (plain `ret`; `this` unused).
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970 (on g_pMainGameComponent 0x013B86A0 +0x1C, the player manager; as the landed
// 0x0083BE90), CPlayer::GetCurrentMode() const @ 0x00633BE0 (EPlayerMode).
// Shape: a switch over the mode with every false case listed and `default: return true` -- VC7.1 builds the `sub 4 ; cmp 21h ; ja`
// range test, a byte index table and a two-entry jump table (index 0 = the false block, index 1 = the default true block).
typedef long EPlayerMode;

class CPlayer {
public:
    EPlayerMode GetCurrentMode() const;                 // 0x00633BE0
};

class CPlayerManager {
public:
    CPlayer* GetMainPlayer();                           // 0x00449970
};

class CMainGameComponent { public:
    unsigned char   _pad00[0x1c];
    CPlayerManager* PlayerManager;                      // +0x1c
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0

class CTCQuestCompletionUI {
public:
    bool IsHeroControlledByPlayer() const;
};

bool CTCQuestCompletionUI::IsHeroControlledByPlayer() const
{
    switch (g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetCurrentMode()) {
    case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15:
    case 17: case 18: case 21: case 23: case 26: case 27: case 29: case 30: case 32: case 36: case 37:
        return false;
    default:
        return true;
    }
}