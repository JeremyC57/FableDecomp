// CGameScriptInterface::IsHeroControlledByPlayer() const -- false for the player modes 4..15, 17, 18, 21, 23, 26, 27, 29, 30, 32, 36, 37 and 41 (CPlayer::GetCurrentMode @ 0x00633BE0 on the player from the manager at +0x14, 0x00449970); true for every other mode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088E980, length 96 bytes.
// Callees: the player getter @ 0x00449970 (on PlayerManager; the label "Catch@0040d959" is propagated), CPlayer::GetCurrentMode @ 0x00633BE0. Shape: a `switch` on `mode - 4` over 0..0x25 through a byte-indexed two-entry jump table (`xor al,al` / `mov al,1`), the out-of-range default true.
struct CDisplayEngine_Retail;
struct CPlayerManager_Retail;
struct CWorld;
struct CPlayer_Retail {
    long GetCurrentMode();                         // 0x00633BE0
};
struct CPlayerManager_Retail {
    CPlayer_Retail* GetPlayer();                   // 0x00449970
};
struct CGameScriptInterface {
    virtual void v000();
    CWorld*                 World;                           // +0x04 (engine/CGameScriptInterface.h)
    long                    Component;
    CDisplayEngine_Retail*  DisplayEngine;                   // +0x0c
    long                    DefinitionManager;
    CPlayerManager_Retail*  PlayerManager;                   // +0x14
    unsigned char           _pad18[0x15];
    bool                    AllowScreenFadingIfAlreadyFaded; // +0x2d
    bool IsHeroControlledByPlayer() const;
};

bool CGameScriptInterface::IsHeroControlledByPlayer() const
{
    switch (PlayerManager->GetPlayer()->GetCurrentMode()) {
    case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15:
    case 17: case 18: case 21: case 23: case 26: case 27: case 29: case 30: case 32: case 36: case 37: case 41:
        return false;
    }
    return true;
}