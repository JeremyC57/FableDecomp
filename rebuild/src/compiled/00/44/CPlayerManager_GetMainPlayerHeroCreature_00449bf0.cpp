// CPlayerManager::GetMainPlayerHeroCreature(long) -- returns the hero creature of the main player: GetPlayer(MainPlayer)->GetHeroCreature(); the argument is unused
// Leaf: GetMainPlayerHeroCreature
// Retail: Fable.exe (TLC, VC7.1) @ 0x00449BF0, 19 bytes (byte twin of 0x00449C10). Callees:
// CPlayerManager::GetPlayer(long) 0x004498C0, CPlayer::GetHeroCreature() 0x00487DC0 (its CIntelligentPointer at +0x2C).
// Unreferenced in the image (recovered from a mis-cut catalog row); `this` is a CPlayerManager because
// GetPlayer is called on the unchanged ecx. One 4-byte stack argument is popped (`ret 4`) but never read.
// Shape: `mov eax,[ecx+1Ch] ; push eax ; call GetPlayer ; mov ecx,eax ; call GetHeroCreature ; ret 4`.
// Layout: retail STLport vectors are 12 bytes (the PDB header's Players is 0x10), so MainPlayer is at
// +0x1C, not the header's +0x20; GetPlayer 0x004498C0 itself reads Players at +0x0C/+0x10 and
// PlayerNeutral at +0x18. Local model instead of engine/CPlayerManager.h.
struct CThing;
struct CPlayer {
    CThing* GetHeroCreature();                 // 0x00487DC0
};

struct CPlayerManager {
    void* __vftable;                           // +0x00
    void* Component;                           // +0x04
    void* DefinitionManager;                   // +0x08
    CPlayer** PlayersFirst;                    // +0x0C vector<CPlayer*> (12-byte retail STLport)
    CPlayer** PlayersLast;                     // +0x10
    CPlayer** PlayersEnd;                      // +0x14
    long PlayerNeutral;                        // +0x18
    long MainPlayer;                           // +0x1C
    CPlayer* GetPlayer(long number);           // 0x004498C0
    CThing* GetMainPlayerHeroCreature(long unused);
};

CThing* CPlayerManager::GetMainPlayerHeroCreature(long unused)
{
    return GetPlayer(MainPlayer)->GetHeroCreature();
}