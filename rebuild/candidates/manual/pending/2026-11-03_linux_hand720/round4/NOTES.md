# agent720c -- chunk 720 (w22, round 4)

## Rows
| row | grade | attempts | best |
|---|---|---|---|
| 0x005C6B26 ConstructWeaponDescription (1668) | DIFFER(1668v1668), 2 lines, parked | 10 | differ/005c6b26.cpp |
| 0x005C90FC ConstructWeaponDescriptionContainer (2150) | DIFFER(2150v2150), 2 lines, parked | 11 | differ/005c90fc.cpp |
| 0x005CACB9 RefreshWeaponDescriptions (1135) | DIFFER(1135v1135), 2 lines, parked | 16 | differ/005cacb9.cpp |
| 0x0065B854 CTCVillage::AddThingToVillage (1502) | not attempted (budget) | 0 | skeleton below |
| 0x00659800 CTCVillage::CalculateSecuritySweepTasks (1074) | not attempted (budget) | 0 | call map below |

All three differ files are self-contained TUs: grade with `FABLE_HDR_OVERLAY=<agent720c>/hdr`. The two description
functions were built as one TU model (work/tu_head2.h + cwd_models.h + tu_model2.h, assembled by work/mk.sh); the
Refresh TU is the round-3 model with the same classes.

### The shared residue (all three rows, and landed 0x005C3180)
Every `CDefPointer<const CThingShotDef> shotDef = GetDefinitionManager().GetPDefFromGlobalIndex(weaponDef->Shot);
x = shotDef->HitThingDamage;` compiles to `dec [esi+4]; mov eax,[esi+54h]; mov [ebp-n],eax; jne`; retail loads first
(`mov eax,[esi+54h]; dec [esi+4]; mov [ebp-n],eax; jne`). A 186-byte sandbox on 0x005C3180 GetActiveProjectileWeaponDamage
(probe/LOG.tsv, 23 runs) reproduces it exactly and was used to test the CDefPointer release forms (probe/hdr is a scratch
copy; nothing from it is in hdr/): `--Object->IVRefCount` without the local, `object->Release()`, separate dec / test,
`Release(); GetRefCount()`, nulling first, typed `Get()` pointer, `-= 1`, `!--`, `long&` reinterpret, a count local --
none moves it. Only a *reference to the count* (`unsigned long& count = object->IVRefCount; if (--count == 0)`) gives
retail's load-before-dec for the named pointer, but it also turns the by-value temporary's release into the flag form
(`dec [eax+4]; jne`; retail `add eax,4; dec [eax]; cmp [eax],0`), so it is not the retail model either (the landed
0x005C3180 gets both only with two invented release helpers). Caller-side variants (const-ref binding, scoped block,
explicit converted temporary, direct-init, `(*shotDef).`, a raw pointer local, early `return shotDef->HitThingDamage`)
moved nothing or made it worse. Next idea: whether CDefPointer stores a typed `T*` (FableWin has
GFDebugPDynamicCast<CDefPointeeBase*, const CThingShotDef*> inside the converting ctor) -- that is a header-wide change.

### Levers (in grading order)
- 0x005C6B26: the TEXT_GUI_MENU_NONE text written in both failing arms (empty augmentation vector / no positive entry):
  retail threads both `jle` to one block; a shared `numAugmentations` outside the block stays in memory (197 -> 4 lines).
  The resize count read through a raw `const CObjectAugmentationsDef*` local puts the zero temporary's store last (as
  the landed ConstructWeaponDescriptionWeight 0x005C770C) (4 -> 2).
- 0x005C90FC: one `if (IsLiveGUIMode()) Reset(Create("PC_..")) else Reset(Create("UI_.."))` per text component (a `?:`
  argument selects into eax; retail tail-merges the two pushes) 757 -> 20; `CDefPointer<const CHeroCombatDef> combatDef;
  combatDef = experience->GetPHeroCombatDef();` (retail inc, inc, dec: the getter's temporary is copied, not elided) 20 -> 14;
  a same-TU noinline `map<unsigned long, NUISystem::CUIState>::operator[]` body (retail 0x0042D5B1 COMDAT): the key
  temporaries then share the fild slot and the next key is stored before the colour byte 14 -> 4.
- 0x005CACB9: see round-3 levers (same-TU map bodies, GetDef<T> body, named end iterator) + `PeekTC` (const LowerBound
  0x004365B0, the retail call target) for the melee weapon; regraded after the header rebase: unchanged.
- The bare `fld; fistp dword` float-to-long is retail inline asm: `GFFastFloatToLong` (FableWin.pdb ?GFFastFloatToLong@@YIJM@Z,
  out of line only in the debug build) as a `__forceinline` with the two-instruction `__asm` body and a `// Retail-asm:`
  line citing 0x005C6CB4..0x005C6CBA (grade retail_asm; the landed GUI rows spell the same helper ROUND).
- Container details: 0x0122DEDC is 0.0f (the heavy-weapon stat-threshold arm zeroes the text's B and G channels:
  `GetStates()[0].Colour.B *= 0.0f`); 0x00410DE0 bit 0x16 is `ParentThing->HasTC(TCI_INVENTORY_EXPERIENCE)`.

## Headers (hdr/ is the complete merged overlay; hdrcheck2.py: every file default=same)
New opt-in branches this round (all default-identical):
- engine/CStockItemDef.h `FABLE_RETAIL_CSTOCKITEMDEF`: `: CSubDefClassBase` (RTTI), inline StatGetTypeName, retail
  members (DefaultPrice +0x28 .. CanBeDisplayedInShop +0x3C, sizeof 0x40 = GetSizeofClass 0x004D4110).
- engine/CTCShop.h `FABLE_RETAIL_CTCSHOP`: inline StatGetInterfaceType (TCI_SHOP), IsValidStockItem(long) const 0x007723D0.
- engine/NInventory_CTCInventoryBase.h `FABLE_RETAIL_NINVENTORY_CTCINVENTORYBASE`: GetPerItemDataForItem(long) const 0x005BE8D3.
- engine/CHeroCombatDef.h `FABLE_RETAIL_CHEROCOMBATDEF`: CHeroAbilityStatPackage {map<EHeroTrainableStatType,long>
  StatThresholds} and HeavyWeapons at +0x13C (Ego_r +0x164; sizeof unchanged 0x148).
- engine/CTCHeroExperience.h `FABLE_RETAIL_CTCHEROEXPERIENCE_STATS` (with FABLE_RETAIL_CTCHEROEXPERIENCE): inline
  GetPHeroCombatDef() const (by value), GetTrainableStatLevel(EHeroTrainableStatType) const 0x006D6520.
- engine/CTCWeapon.h `FABLE_RETAIL_CTCWEAPON` (round 3, fixed): the stray `_pad_0x08` is gone -- retail CTCBase is 0xC, so
  WeaponDef sits at +0x0C directly (static asserts added).
- stlport/fable_containers.h: rebased on the current repo file (owner D2 2026-11-03 made the tag ctors the default; the
  round-3 copy predated it and showed default=DIFF); chunk 719's opt-ins re-applied. All three rows regraded: unchanged.
Carried: CPlayerGui (LiveGuiMode +0x2BE, IsLiveGUIMode), fable_game_text_bank.h (CDataBank::GetTextBySymbol), fable_map.h
(FABLE_RETAIL_STLP_TREE_ERASE_KEY), CThing, CVectorMap, CTCInventoryItem, C2DVector, C3DBoundingBox, CTCVillage (719).
TU-local models (no engine header): CWeaponDef (: CSubDefClassBase, RTTI), CThingShotDef, CObjectAugmentationsDef,
NInventory::CPerItemData (ctor as landed 0x005C14A6), CTCVolumeContainmentTracker (PThingInside +0x0C),
CTCObjectAugmentations statics, NUISystem::CText / CViewport slots, CTCInventoryWeapons.

## 0x0065B854 AddThingToVillage -- decoded skeleton (not graded)
```
void CTCVillage::AddThingToVillage(CThing& thing)
{
    CTCVillageMember* villageMember;               // uninitialised when the thing lacks TC 0x23 (retail reads the dead [ebp+8])
    thing.GetTC(&villageMember);                   // HasTC(0x23) + non-const LowerBound 0x0040F020
    if (villageMember->GetPVillage() == this) return;          // 0x007EE540
    villageMember->SetPVillage(this);                          // 0x007EE270
    switch (thing.GetType()) {                     // CClass::Type byte +0x10
    case 1:  // creature: VillagePeople(+0x2C).push_back(CIntelligentPointer<CThingAICreature>(&thing)) 0x006604EB (ctor 0x0050F3F0);
             // if (creature.GetLifeState() == 3) ++NumGuards (0x00662010); if (LaggyLimbo +0xB3) { if (!GFHasScriptThing(&thing)
             // || !thing.HasTC(0x1F)) { thing.SetInLimbo(true); thing byte +0x92 |= 0x40; } }
    case 3:  // building: VillageBuildings(+0x28).push_back(CIntelligentPointer<CThingBuilding>) 0x006604D1 (ctor 0x0050F380);
             // building->[+0x60]->vcall slot 93 (+0x174); if (LaggyLimbo) { building byte +0xD4 |= 1; LockAllDoors(true, false) 0x0082DB00 }
             // if HasTC(0x21): walk that TC's list (+0x0C) of CIntelligentPointer<CThing>: alive, Type 5, HasTC(0x27 light),
             // object GetProperties() bit 8 or 9 -> light->SetActive(building.HasTC(0xAB) ? !IsDay() 0x006BB2C0
             // : PeekWorld().GameTimeManager(+0x1C)->IsTimeOfDayInRange(def->+0xCC, def->+0xD0) 0x006BB390) 0x006412B0
             // (two CDefPointer<const CVillageDef> copies of PDef +0x94 held around the range call, released after)
    case 5:  // object: GetProperties() (CThingObject slot 48): bit 9 or 8 -> VillageTorches(+0x30) push_back, then unless
             // byte +0x91 bit0: light TC 0x27 SetActive(!IsDay()); bit 1 -> VillageCrops(+0x3C) push_back + (if the village def's
             // vector +0x28 contains 1, find 0x0065E5A4) CropTasks(+0x58).push_back(CCountedPointer<CVillageTaskPickCrops>(new(0x18) ..(&thing)));
             // bit 24 -> VillageHorns(+0x34); bit 28 -> VillageGates(+0x38); bit 3 -> VillageCages(+0x40); bit 14 -> VillageCampfires(+0x44)
             // (list<CIntelligentPointer<CThing>>::push_back 0x004BA4E0, ctor 0x004A96F0, dtor 0x00A01C10)
    case 10: // track node: if (thing +0xEC flag && thing CCharString +0xE8 == village def CDefString +0x34 GetString 0x00415DD0)
             // (conditional CCharString temporary, flag [ebp-8]) and the def's vector +0x28 contains 1:
             // VillagePatrols(+0x48).push_back(CIntelligentPointer<CThingTrackNode>) 0x00660FC9; PatrolTasks(+0x54).push_back(
             // CCountedPointer<CVillageTaskPatrol>(new(0x1C) CVillageTaskPatrol(&thing))) 0x00660553
    case 11: // marker: VillageMarkers(+0x70).push_back(CIntelligentPointer<CThingMarker>) 0x006605BB (ctor 0x0065E21E)
    case 13: break;
    default: if (thing.HasTC(0xD6)) VillageDummies(+0x74).push_back(CIntelligentPointer<CThing>(&thing));
    }
}
```
Needs: CTCVillage list members typed (the 719 branch keeps them opaque), PDef as CDefPointer<const CVillageDef> (no header),
CThingObject/CThingBuilding/CThingMarker/CThingTrackNode/CTCLight/CGameTimeManager opt-ins, the task classes. The
ThingType values and the GetProperties bits have no proven names (write them as literals with comments).

## 0x00659800 CalculateSecuritySweepTasks -- call map (not graded)
count_if over VillagePeople (+0x2C; by-value empty functor = the uninitialised [ebp-1Ch] byte pushed) 0x0065F6A6, then a
find_if 0x0066000A; first match's GetPItem -> GetTC(7); GetPlayerManager().GetMainPlayer().GetPControlledCreature()->GetPos()
copied to PositionPlayerWasInWhenSecuritySweepCalculated (+0x14, movsd x3); CWorld::GetCurrentPlayer; two
CThingSearchTools::GetPBestThing (0x0065F6DA with filter {100, 221}, 0x0065F744 HasTC/Near) on GetThingManager()+0x20;
radius = InitialSecuritySweepRadius (+0xB8) - (GetFrame() - FrameStartedSecuritySweep) / g_framesPerSecond * k
(0x0125A318); GFFastOneOverSqrt expansion (0x013CE558 mantissa table); a do-while of C3DVector::RotateXY 0x00A14040 +
CTCWalk::FindNearestWalkablePos 0x006CB8E0; SecuritySweepTasks (+0x5C).push_back(CCountedPointer<CVillageTaskSecuritySweep>(new ..))
0x00660587 (ctor 0x0065DCE8).

## GOTCHAS candidates
- A UI text whose `?:` picks one of two template names selects into eax; retail tail-merges an if/else of two
  CreateComponent calls (push in each arm, one CCharString ctor) (0x005C90FC).
- `CDefPointer<T> p = obj->GetP..Def();` from an inline by-value getter is elided by VC7.1 (one inc); retail's inc, inc,
  dec is `CDefPointer<T> p; p = obj->GetP..Def();` (0x005C90FC).
- A same-TU noinline `map<K,V>::operator[]` body lets VC7.1 treat the key temporary as uncaptured: its slot is shared with
  the fild temporary and the next key is stored before the previous statement's byte store (0x005C90FC).
- `CDefPointer<const CThingShotDef>` load/dec order (retail loads the member first) is not reproduced by any release form
  of the shared header; a reference-to-count release fixes the named pointer but breaks the temporary's (0x005C3180 sandbox).
