// CThing::SetPlayerNumber(long) -- store the owning player number (truncated to the signed char PlayerNumber)
// Leaf: ?SetPlayerNumber@CThing@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C7440, 13 bytes: `mov al,[esp+4] ; mov [ecx+90h],al ; ret 4`.
// Naming (NF108, 2026-11-19, q5046): was the __fastcall stand-in EnableNavigator(CTCPhysicsNavigator*, void*, char). Ego_r.pdb +
//   FableWin.pdb ?SetPlayerNumber@CThing@@QAEXJ@Z; engine/CThing.h declares it at 0x004C7440 (FABLE_RETAIL_CTHING_STATEACCESS) with
//   PlayerNumber (schar, retail +0x90) in the layout; callers CPlayer::InitCharacterFromSave 0x00487EF0 and
//   CPlayer::SwitchPlayerCharactersWith 0x004888C0 pass CPlayer::Number and bind this symbol. Shape as SetCurrentMapNumber 0x004C7580.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#include "fable_string.h"
#include "engine/CThing.h"

void CThing::SetPlayerNumber(long playerNumber)
{
    PlayerNumber = (signed char)playerNumber;
}
