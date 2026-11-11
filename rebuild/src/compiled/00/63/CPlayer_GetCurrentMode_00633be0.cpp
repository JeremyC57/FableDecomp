// CPlayer::GetCurrentMode() const -- the player's current mode: the last entry of the PlayerModes list
// Leaf: ?GetCurrentMode@CPlayer@@QBE?AW4EPlayerMode@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00633BE0: mov eax,[ecx+210h] ; mov ecx,[eax+4] ; mov eax,[ecx+8] ; ret.
// Name (agentNF22, Codex pass 21 queue): Ego_r.pdb + FableWin.pdb ?GetCurrentMode@CPlayer@@QBE?AW4EPlayerMode@@XZ (engine/CPlayer.h
//   declares it so); body PlayerModes.back() -- the STLport list head node's _M_prev, then its data. Was the stand-in
//   `int __fastcall GetCurrentMode(A1*)` over three placeholder structs.
// Layout: the shared engine/CPlayer.h (FABLE_RETAIL_PLAYER) with PlayerModes +0x210 typed as std::list<EPlayerMode> under
//   FABLE_RETAIL_PLAYER_MODES_LIST; STLport list::back() from stlport/fable_containers.h (FABLE_RETAIL_STLP_LIST).
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_MODES_LIST
#define FABLE_RETAIL_STLP_LIST
#include "engine/CPlayer.h"

EPlayerMode CPlayer::GetCurrentMode() const
{
    return PlayerModes.back();
}
