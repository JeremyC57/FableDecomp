// CPlayer::IsFirstPersonTargeting() const -- whether the current mode is first-person targeting or using a projectile weapon
// Leaf: ?IsFirstPersonTargeting@CPlayer@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004874D0, 20 bytes: call GetCurrentMode ; cmp eax,16h ; jne ; mov al,1 ; ret ;
//   cmp eax,19h ; sete al ; ret (`this` stays in ecx for the call).
// Name (agentNF22, Codex pass 21 queue): Ego_r.pdb + FableWin.pdb ?IsFirstPersonTargeting@CPlayer@@QBE_NXZ (engine/CPlayer.h declares
//   it so; called from CTCTargetingPlayer::GetPTargetedThing 0x00802610 on CThing::PeekPlayer's const CPlayer&). Callee
//   CPlayer::GetCurrentMode() const 0x00633BE0 (both PDBs). Was the extern "C" stand-in `Is_4874d0` / `Mode_4874d0`.
// Modes: 0x16 PLAYER_MODE_FIRST_PERSON_TARGETING, 0x19 PLAYER_MODE_USE_PROJECTILE_WEAPON (engine/EPlayerMode.h).
// Shape: the second compare as `? true : false` keeps the result byte-wide (`sete al`); a plain `return mode == ...;` widens it
//   (`xor ecx,ecx ; sete cl ; mov al,cl`), and `||` or a merged switch case jumps to a shared `mov al,1`.
#define FABLE_RETAIL_PLAYER
#include "engine/EPlayerMode.h"
#include "engine/CPlayer.h"

bool CPlayer::IsFirstPersonTargeting() const
{
    EPlayerMode mode = GetCurrentMode();
    if (mode == PLAYER_MODE_FIRST_PERSON_TARGETING)
        return true;
    return mode == PLAYER_MODE_USE_PROJECTILE_WEAPON ? true : false;
}
