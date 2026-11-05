// CPlayer::UninitCharacter() -- a non-main player drops all its modes, the main player re-adds mode 3; then the player character (if any) is killed
// Leaf: ?UninitCharacter@CPlayer@@QAEXXZ
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
// Naming wave 16: the CPlayer_Methods stand-in respelled to CPlayer on the layout header (#define-Layout precedent 0x00830150);
//   AddMode takes the Ego_r EPlayerMode (name-only: the bytes are unchanged).
#define CPlayer CPlayer_Layout
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CPlayer.h"
#undef CPlayer

enum EPlayerMode { PLAYER_MODE_NULL = 0 };    // enumerator name is ours; mode 3 has no recorded enumerator name

// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
#include "engine/CThing.h"
class CBaseObject;
class CBaseObjectPointer;
#include "engine/CIntelligentPointer.h"


class CPlayer : public CPlayer_Layout { public:
    bool IsMainPlayer() const                      // out of line @ 0x00487520
    {
        long number = this->Number;
        return number == this->PlayerManager->GetMainPlayerNumber();
    }
    void ResetModes();                             // 0x00634870
    void AddMode(EPlayerMode mode, bool flag);     // 0x006345C0
    void UninitCharacter();
};

void CPlayer::UninitCharacter()
{
    if (!IsMainPlayer())
        ResetModes();
    else
        AddMode((EPlayerMode)3, true);
    CIntelligentPointer<CThing>* character = reinterpret_cast<CIntelligentPointer<CThing>*>(this->PlayerCharacter);
    if (character->Get())
        character->Get()->Kill(false);
}