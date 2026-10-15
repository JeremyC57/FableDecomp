// CTCFishing::GoFishing(bool arg) -- when the hero creature exists, is alive and a fish could be created, put the hero's player into fishing mode (0x1d) and move the minigame to stage 1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B1EB0, length 92 bytes.
// Callees: CTCBase::GetPlayerManager() @ 0x00686D50 (`[g_0x013B8A1C]+0x30`), CPlayerManager::GetMainPlayer() @ 0x00449960
// (`GetPlayer(MainPlayer)`, a byte-identical copy of the landed 0x00449970), CPlayer::GetPlayerCreature() @ 0x00487DC0
// (landed naming), CTCFishing::CreateFish(bool) @ 0x007B1A30, CThing::GetPlayer() @ 0x004C7A30, CPlayer::AddMode(EPlayerMode,
// bool) @ 0x006345C0.
// Layout: PDB CTCFishing (100 bytes) over CTCBase (engine/CTCBase.h): PFish +0xc (CIntelligentPointer<CThing>),
// Stage +0x14, StageInitialised +0x18; the hero's dead bit is bit 0 of +0x91 (landed 0x00836BE0).
// Shape: the getter chain into edi with `this` in esi; three early exits to the shared epilogue; `push 0 ; push 1Dh`
// precede `mov ecx,edi ; call GetPlayer` (VC evaluates AddMode's arguments before the object expression); the stage
// change is an inline SetStage (`cmp [esi+14h],1 ; je` then the two stores).
#include "engine/CTCBase.h"

struct CPlayer_007b1eb0;

struct CThing {
    unsigned char _pad[0x91];
    unsigned char m_flags;                         // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags & 1) != 0; }
    CPlayer_007b1eb0* GetPlayer();                 // 0x004C7A30
};

struct CPlayer_007b1eb0 {
    CThing* GetPlayerCreature();                   // 0x00487DC0
    void AddMode(long mode, bool flag);            // 0x006345C0
};

struct CPlayerManager_007b1eb0 {
    CPlayer_007b1eb0* GetMainPlayer();             // 0x00449960
};

struct CTCFishing : CTCBase {
    unsigned char PFish[8];                        // +0x0c CIntelligentPointer<CThing>
    long          Stage;                           // +0x14 EFishingStage
    bool          StageInitialised;                // +0x18
    CPlayerManager_007b1eb0* GetPlayerManager();   // 0x00686D50
    bool CreateFish(bool arg);                     // 0x007B1A30
    void SetStage(long stage)
    {
        if (Stage != stage) {
            Stage = stage;
            StageInitialised = false;
        }
    }
    void GoFishing(bool arg);
};

void CTCFishing::GoFishing(bool arg)
{
    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetPlayerCreature();
    if (hero && !hero->IsDead() && CreateFish(arg)) {
        hero->GetPlayer()->AddMode(0x1d, false);
        SetStage(1);
    }
}