// CPlayerManager::SwapToHero(long player) -- make the given player the main player by swapping characters with the current main player (falling back to the old main player when the swap fails), then kill the old main player's hero thing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00449CB0, length 99 bytes.
// Callees: CPlayerManager::IsPlayer @ 0x00449880, GetPlayer @ 0x004498C0 (twice), CPlayer::SwitchPlayerCharactersWith
// @ 0x004888C0, CPlayer::GetHeroThing @ 0x00487DC0 (twice; manifest label is STL noise, see GOTCHAS chunk 140),
// CThing::Kill(bool) @ 0x004C9B80.
// Layout: engine/CPlayerManager.h with the retail 12-byte Players vector: MainPlayer +0x1c (PDB +0x20); CPlayer
// Number +0x28 (engine/CPlayer.h).
// Shape: the new main player number is stored before the swap and restored from the old player on failure; the hero
// thing is fetched twice (null test, then the call).
struct CThingCalls {
    void Kill(bool flag);                                   // 0x004C9B80
};

struct CPlayerRetail {
    unsigned char _pad00[0x28];
    long          Number;                                   // +0x28
    bool SwitchPlayerCharactersWith(CPlayerRetail* other);  // 0x004888C0
    CThingCalls* GetHeroThing();                            // 0x00487DC0
};

struct CPlayerManager {
    void*         __vftable;                                // +0x00
    void*         Component;                                // +0x04
    void*         DefinitionManager;                        // +0x08
    void*         Players[3];                               // +0x0c vector<CPlayer*> (12 bytes in retail)
    long          PlayerNeutral;                            // +0x18
    long          MainPlayer;                               // +0x1c
    bool IsPlayer(long player);                             // 0x00449880
    CPlayerRetail* GetPlayer(long player);                  // 0x004498C0
    void SwapToHero(long player);
};

void CPlayerManager::SwapToHero(long player)
{
    if (IsPlayer(player)) {
        CPlayerRetail* newPlayer = GetPlayer(player);
        CPlayerRetail* oldPlayer = GetPlayer(MainPlayer);
        MainPlayer = newPlayer->Number;
        if (!newPlayer->SwitchPlayerCharactersWith(oldPlayer))
            MainPlayer = oldPlayer->Number;
        if (oldPlayer->GetHeroThing())
            oldPlayer->GetHeroThing()->Kill(true);
    }
}