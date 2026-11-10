# chunk 1002b -- the 10 CTCInventoryWeapons rows chunk 1002 left (agent1002b, 2026-11-10)

Result: 6 of 10 RELOCATION_MATCH with passing tests (src/, test/): 0x005C4F42, 0x005C574E, 0x005C5F11, 0x005C3E91, 0x005C47D5,
0x005C6B26. 1 DIFFER parked (differ/005c71aa.cpp, 680v680, 5 diff lines). 3 not finished (0x005C90FC, 0x005CACB9, 0x005CA68E: analysed,
no source yet -- out of session budget).

**Header overlay:** grade every src/ and differ/ file with `FABLE_HDR_OVERLAY=$A/hdr` exported (gall.sh as shipped does not export it:
run `FABLE_HDR_OVERLAY=$A/hdr bash gall.sh`). hdr/README.md lists the opt-ins; NUISystem_CMenuEntry.h and CTCInventoryItem.h were re-based
on HEAD after chunk 1003 landed (c183de7c4), so the overlay is a pure addition.

## Identities (Ego_r.pdb + FableWin.pdb name sets; full symbols in LABELS.tsv)
- 0x005C4F42 ConstructAugmentationsLeftDescription, 0x005C574E ConstructAugEntry, 0x005C5F11 ConstructAugEntryPC,
  0x005C3E91 / 0x005C47D5 FinalConstructWeaponItemEntries(PC) (private, M22K signature), 0x005C6B26 ConstructWeaponDescription(CWideString&,long,float,bool) const.
- Callees proven: 0x004854DF CActionParamCCountedPointer<CComponent>(CCountedPointer<CComponent>&); CMenuEntry slots 150 AddThingActionParam,
  151 AddUIActionParam, 152 AddActionOnUnselected 0x0053E2A0, 153 AddActionOnVisible 0x0053E3C0; CList 158 SetDefaultChild(long) +0x278;
  CComponent slot 3 FindChild(CCharString); 0x0059350C GetSubDef<CStockItemDef>; 0x005D96A0 CTCInventoryItem::GetEngineGraphic(long).
- Name corrections in NAME_FIXES.tsv (landed 0x004854DF is spelled as a non-template CBankFileAsyncEntry ctor; several reloc bindings).
  Seen but not filed (shared bodies, possibly folded): 0x004DF8E9 map<EObjectAugmentationType,long>() and 0x004DD3AD its _Rb_tree dtor
  (bound ??0/??1CArrowTrailMap), 0x005CC04A _Rb_tree::_M_find<EObjectAugmentationType> member template.

## Levers (LESSONS.md has the one-liners)
- 0x005C4F42: `counts.insert(std::make_pair(...))` + named `CCharString guide(...)`.
- 0x005C574E / 0x005C5F11: FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA, reference static_cast for the augment action's entry param, CText
  pointer declared before SetName.
- 0x005C3E91 / 0x005C47D5: implicit L"INVALID NAME" / template conversions, `defIndex == ...` order, non-POD CEngineGraphic, CMenuEntry
  action-list slots, CActionParamCCountedPointer template; the PC twin rebuilds the wield entry per item (if/else of two
  ConstructWieldEntryPC calls) and looks up the parent's PC_UI_FRAME twice.
- 0x005C6B26: the landed 0x005C7452 TU + nested-else "none" text; CStockItemDef / CTCShop interface-key opt-ins; same-TU IS_THING_ALIVE<CThing*>.

## Parked / next steps
- 0x005C71AA (DIFFER 680v680, 5 lines): only the TEXT_GUI_MENU_NONE block's schedule. The 0x005C6B26 lever (nested else) fixes it but then
  count in the copy loop gets ecx instead of the [ebp+14h] split (c2prio: the 2nd NONE copy drops const 0 from 25 to 22, below i / this,
  so the split pass over count never fires). Next: find a copy-loop or names-loop spelling that keeps const 0 >= 23 with the nested else
  (e.g. raise const-0 references, or lower i#26 / this), or trace the split pass with c2prio --trace (wip/005c71aa_b.cpp is the start).
- 0x005C90FC ConstructWeaponDescriptionContainer (2150 b): same TU header as 0x005C6B26 (damage / augmentations / value parts identical, written
  into a local CWideString), then per line a "PC_TEXT_INSIDE_CONTAINER_TEMPLATE" / "UI_TEXT_INSIDE_CONTAINER_TEMPLATE" component chosen by
  CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().IsLiveGUIMode() (FableWin ?IsLiveGUIMode@CPlayerGui@@QBE_NXZ, +0x2BE; needs an opt-in
  body), CCountedPointer::Reset 0x00429C6F, container->AddChild (+0xEC), CText::SetValue (+0x240), text = empty wide 0x013BCA24 (name
  unproven); for heavy weapons (Class == 2) with PeekTC<CTCHeroExperience> (0x68) and HasTC(TCI_INVENTORY_EXPERIENCE 0x16): copy of the
  hero experience's CDefPointer<const CHeroCombatDef> (+0x58), loop over HeavyWeapons.StatThresholds (std::map<EHeroTrainableStatType,long>
  at combat def +0x13C; CHeroAbilityStatPackage, Ego_r), GetTrainableStatLevel 0x006D6520 < threshold => the weight component's
  GetStates()[0].Colour.B / .G *= float 0x0122DEDC (__ftol2). Headers needed: CHeroCombatDef HeavyWeapons opt-in, CTCHeroExperience
  PHeroCombatDef as CDefPointer, CPlayerGui IsLiveGUIMode.
- 0x005CACB9 RefreshWeaponDescriptions (1135 b): uses WeaponsList +0x17C FindChild(AUGMENTATIONS_MENU_ENTRY_NAME 0x013B8BC0) -> slot 8
  (+0x20) twice, title text TEXT ... + GetXString + GFIntToWideString(manager slot 31 +0x7C) into CText::SetValue; components list ctor
  0x0042ABAF and WeaponsList slot 5 (+0x14) collecting WEAPON_DESCRIPTION_STRING children; active melee damage (GetDamage 0x005DAAA0) and
  ranged shot damage; per child: WeaponMenuItemMap[child] (0x0042D1B7), CDeletion slot 62, map erase 0x005CC31B, new container (LiveGuiMode
  template), SetName(WEAPON_DESCRIPTION_STRING) +0x14C, parent slot 99 (+0x18C) -> AddFakeChild (+0x238), then
  ConstructWeaponDescriptionContainer(container, def, melee ? meleeDamage : rangedDamage), then slot 52 (+0xD0)...
- 0x005CA68E SetupActiveWeaponsOnCarrier: not attempted (needs CCreatureAction_UnsheatheItemFromInventory, CTCMultiStrike headers; see
  NOTES_chunk1002.md).
