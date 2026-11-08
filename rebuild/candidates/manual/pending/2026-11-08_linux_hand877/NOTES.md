# Hand chunk 877 (CThingGameObject .. CTCTargetingPlayer): 0 of 2 byte-exact

## What matched (0)

## Parked for Fable 5.1 (2)
- 00839940 CThingGameObject::OnHit DIFFER(5189v5195): c2 colours this=ebx / hitter=ebp where retail has this=ebp / hitter=ebx (every later block differs by register and spill slot; 267 register-masked diff lines, frame sub esp,0A4h v 0A0h), four of the six late `call std::bitset<274>::test` (0x0083A83B/A897/A916/A988) expanded inline, the electrocute return cross-jumped into the normal end instead of the battle-charge tail (0x0083AD60), the second AddEvent not tail-duplicated, two material-name CCharString ctors merged | tried: named engine references + direction copy in the debug draw (best, 295->267), weapon/hitter fetch order, RemoveTC placement, position by copy/const, /d2inlT52-59 (all-or-nothing per callee: 210 v 175 calls), /d2inlS12-100, `__forceinline` HasTC + S15-17 (4 of 6 test calls, but GetTCDriver<CTCDExplosion> then expands), GetTCDriver declared + S, CCharString::operator== without the forceinline opt-in (+/d2inlT80/120: stays a call) | next: c2prio --trace the early this/hitter pieces for a spelling that colours hitter first; then the inline budget (what retail spends early that ours does not)
- 00804020 CTCTargetingPlayer::UpdateTargetList DIFFER(-v5587): no compiling body | tried: decoded the phase map to 0x00805083 and the tail call order (differ/00804020.cpp) | next: author phase by phase with CTargetPoint (user copy ctor), the STLport vector push_back, and the seven algorithm instances (remove_if / copy_if / find / introsort / erase / range-assign) declared out of line under their PDB template names

## Skipped
- none

## Label corrections
- 0x0083AF60 catalog/landed `CThing::GetPBaseDef` -> `CHitParameters::GetPHitMaterialDef` (FableWin.pdb; body reads +0x4C).
- 0x00815270 landed `CTCShotDefinitionBase::CanBeHitByMeleeStrike` -> OnHit calls it on a CHitLocation (`NHitLocations::CHitLocation::IsToPlayGetHitResponse`, FableWin.pdb); candidate, check folding.
- 0x0083AE30 CHitThing ctor argument `const CCreatureStatsDef&` (Ego_r + FableWin), landed as `CThing*`.
- 0x0057A56C CTCHeroStats::InformOfEvent takes `const NHeroStatModifierEvent::CBase&` (Ego_r + FableWin), landed callers use a pointer.
- Also proven from OnHit call sites (FableWin.pdb names): 0x00868860 CHitParameters::IsMeleeStrike, 0x00814BE0 CTCHitLocations::GetPDefaultHitLocation,
  0x007331E0 CTCCombat::IsBlocking, 0x00780410 CTCFlammable::SetOnFire, 0x006A92B0 CJoystick::CInitRumble ctor, 0x0083B000 / 0x0066C340
  CDefinitionManager::GetDef<CMaterialDef> (by name / by index), 0x007BEFF0 NThingShot::CTCShotDefinitionBase::GetPWeapon,
  0x004C79F0 / 0x004C7A00 CThing::PeekPlayerManager / GetPlayerManager, 0x00661EF0 CThingCreatureBase::PerformBlockedReaction,
  0x007512C0 CThing::GetTCDriver<CTCDExplosion>; CThingPhysical slot 58 PeekPMaterial (+0xE8) and slot 59 ModifyHealth (+0xEC);
  the TC 0x67 virtual at +0x78 is CTCDegradableBase::OnHit (slot 30).
