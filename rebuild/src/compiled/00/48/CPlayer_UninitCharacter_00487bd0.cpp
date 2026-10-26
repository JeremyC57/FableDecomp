// CPlayer::UninitCharacter() -- a non-main player drops all its modes, the main player re-adds mode 3; then the player character (if any) is killed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00487BD0, length 70 bytes.
// Identity: Ego_r.pdb `?UninitCharacter@CPlayer@@QAEXXZ`.
// Callees: CPlayerManager::GetMainPlayerNumber() const @ 0x00449700 (through the inlined
// CPlayer::IsMainPlayer, out of line @ 0x00487520), CPlayer::ResetModes @ 0x00634870,
// CPlayer::AddMode(EPlayerMode, bool) @ 0x006345C0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50
// (called twice: the test and the kill), CThing::Kill(bool) @ 0x004C9B80.
// Shape: the inlined IsMainPlayer reads Number into a LOCAL before the call (edi): written as
// `Number == PlayerManager->GetMainPlayerNumber()` the lane compares `[esi+28h]` after the call
// (the out-of-line copy 0x00487520 shows the same pre-call load into esi); the not-main arm
// falls through; `this` is advanced to the PlayerCharacter intelligent pointer (`add esi,34h`)
// once `this` is dead. Members (PlayerManager +0x10, Number +0x28, PlayerCharacter +0x34) from
// engine/CPlayer.h (its offsets agree with retail below +0x210).
#include "engine/CPlayer.h"

typedef long EPlayerMode;

class CPlayerManager { public:
    long GetMainPlayerNumber() const;              // 0x00449700
};
class CThing { public:
    void Kill(bool silently);                      // 0x004C9B80
};
struct CIntelligentPointer_CThing {                // CIntelligentPointer<CThingPlayerCreature>, 8 bytes
    void* m_pPointer;
    void* m_pLink;
    CThing* GetPItem() const;                      // 0x00A01B50 (CBaseIntelligentPointer::GetPItem)
};

struct CPlayer_Methods : CPlayer {
    bool IsMainPlayer() const                      // out of line @ 0x00487520
    {
        long number = this->Number;
        return number == this->PlayerManager->GetMainPlayerNumber();
    }
    void ResetModes();                             // 0x00634870
    void AddMode(EPlayerMode mode, bool flag);     // 0x006345C0
    void UninitCharacter();
};

void CPlayer_Methods::UninitCharacter()
{
    if (!IsMainPlayer())
        ResetModes();
    else
        AddMode(3, true);
    CIntelligentPointer_CThing* character = reinterpret_cast<CIntelligentPointer_CThing*>(this->PlayerCharacter);
    if (character->GetPItem())
        character->GetPItem()->Kill(false);
}