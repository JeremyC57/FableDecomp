// Leaf: @CGameScriptInterface_IsHeroInProjectileWeaponMode@0
#include "rebuild_abi.h"
// CGameScriptInterface::IsHeroInProjectile @ 0x00890670
// mov eax,ds:g; mov ecx,[eax+0x1c]; call h1; mov ecx,eax; jmp h2
// Resolve a subsystem from a global singleton, then tail-call a query on it.
// NF47 (round 7): 0x013B86A0 is ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static, read through the inline
//   CMainGameComponent::Get(), engine/CMainGameComponent.h), its PPlayerManager +0x1C through the inline PeekPlayerManager() (FableWin.pdb
//   ?PeekPlayerManager@CMainGameComponent@@QBEABVCPlayerManager@@XZ); the two callees are Ego_r/FableWin
//   ?GetMainPlayer@CPlayerManager@@QBEABVCPlayer@@XZ 0x00449960 (engine/CPlayerManager.h) and ?IsFirstPersonTargeting@CPlayer@@QBE_NXZ
//   0x004874D0 (engine/CPlayer.h); was the stand-in void* g_singleton_890670 (a cast +0x1C word read) and the extern "C" stand-ins FableProjH1_890670 / FableProjH2_890670.
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CMainGameComponent.h"
extern "C" bool FABLE_FASTCALL CGameScriptInterface_IsHeroInProjectileWeaponMode()
{
    return CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().IsFirstPersonTargeting();
}
// Stand-in (owner rule 2026-11-05): CGameScriptInterface_IsHeroInProjectileWeaponMode for CGameScriptInterface::IsHeroInProjectileWeaponMode; RTTI vtable slot ?IsHeroInProjectileWeaponMode@CGameScriptInterface@@UBE_NXZ + PDB name set (egor/fablewin).
