// CGameScriptInterface::IsHeroInProjectileWeaponMode() const -- the main player's IsFirstPersonTargeting, a tail jump
// Leaf: ?IsHeroInProjectileWeaponMode@CGameScriptInterface@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00890670. Identity (NF431, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: ?Get@CMainGameComponent@@SIPAV1@XZ, ?PeekPlayerManager@CMainGameComponent@@QBEABVCPlayerManager@@XZ (inline, +0x1c),
//   ?GetMainPlayer@CPlayerManager@@QBEABVCPlayer@@XZ (retail 0x00449960), ?IsFirstPersonTargeting@CPlayer@@QBE_NXZ (retail 0x004874D0, the jmp).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#include "engine/CGameScriptInterface.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CMainGameComponent.h"

bool CGameScriptInterface::IsHeroInProjectileWeaponMode() const
{
    return CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().IsFirstPersonTargeting();
}
