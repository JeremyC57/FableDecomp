# NF93 header overlay (round 8)

Every overlay header is a copy of the CURRENT repo header (base copy in ../orig/) with `#ifdef` opt-ins only. Without the opt-in macros
each header is token-identical to the repo copy. pin_asserts.py was run after every edit (results below).

## engine/CCategory.h (repo rebuild/include/engine/CCategory.h)
- `FABLE_RETAIL_CCATEGORY_REMOVEITEM`: NF94's block, copied VERBATIM from `$S/w22/agentNF94/hdr/engine/CCategory.h` at the same place
  (after the CATEGORYIDENTIFIER block), so a merge of both overlays is a no-op for it. Declares `void RemoveItem(long, long)` 0x005BF221
  (Ego_r.pdb + FableWin.pdb ?RemoveItem@CCategory@NInventory@@QAEXJJ@Z). Used by NF93 row 0x005BF3C6.
- `FABLE_RETAIL_CCATEGORY_SELECTITEMOFTYPE` (NF93, new): declares `void SelectItemOfType(long)` 0x005BDEC1 (Ego_r.pdb 0x00620BE3 +
  FableWin.pdb ?SelectItemOfType@CCategory@NInventory@@QAEXJ@Z, public). Declaration only. Used by row 0x005BDFB1.
- pin_asserts.py: pinned 0. The asserts at lines 30..39 are above the opt-ins (110..119); the one below (repo line 170, overlay 180) is
  already FABLE_STATIC_ASSERT_AT(67, ...), so its moved line does not change its identity.
- Header users: 48 landed rows include engine/CCategory.h (`grep -rlE 'engine/CCategory\.h' rebuild/src/compiled`); none defines either
  macro, so the opt-ins change none of them.

## engine/CRandomAppearanceMorph.h (repo rebuild/include/engine/CRandomAppearanceMorph.h)
- `FABLE_RETAIL_CRANDOMAPPEARANCEMORPH_GETRANDOMMORPHSET` (NF93, new, public, after the TRANSFERBINARYIN block): declares
  `void GetRandomMorphSet(unsigned long&, CMorphSet&, bool) const` 0x0086BF70 (Ego_r.pdb 0x00857780 + FableWin.pdb
  ?GetRandomMorphSet@CRandomAppearanceMorph@@QBEXAAKAAVCMorphSet@1@_N@Z). Declaration only. Used by row 0x0086BF70.
- pin_asserts.py: pinned 0 (the header has no FABLE_STATIC_ASSERT).
- Header users: 6 landed rows include it; none defines the new macro, so the opt-in changes none of them.

## engine/CCreatureAction_CastWillForceSpell.h (repo rebuild/include/engine/CCreatureAction_CastWillForceSpell.h)
- `FABLE_RETAIL_CCREATUREACTION_CASTWILLFORCESPELL_CLASS` (NF93, new): the polymorphic retail class on CCombatActionBase (rtti_bases.tsv)
  with ImmediateCast +0x114 / OneAnimCast +0x115 / LevelToCast +0x118 (sizeof 0x11c) and the Ego_r.pdb ctor
  ??0CCreatureAction_CastWillForceSpell@@QAE@AAVCThingCreatureBase@@J_NABVCCharString@@@Z (0x008BB200). The generated flat class is kept
  verbatim under `#else` (the CCreatureAction_CastStrengthMultiStrike _CLASS pattern), so the default path is token-identical apart from the
  pinned assert lines. Used by row 0x008C2980.
- pin_asserts.py: pinned 5 (the five FABLE_STATIC_ASSERT lines of the generated block now carry FABLE_STATIC_ASSERT_AT with their repo line
  numbers 83..87, because the opt-in block above them moved them).
- Header users: 2 landed rows include it; neither defines the macro, so the opt-in changes none of them.

## engine/CCombatAction_WillForcePushInto.h (repo rebuild/include/engine/CCombatAction_WillForcePushInto.h)
- `FABLE_RETAIL_CCOMBATACTION_WILLFORCEPUSHINTO_FINISHACTION` (NF93, new): declares `virtual void FinishAction();` (Ego_r.pdb + FableWin.pdb
  ?FinishAction@CCombatAction_WillForcePushInto@@UAEXXZ, slot 4, 0x008C2980) after the inline ctor. Declaration only. Used by row 0x008C2980.
- pin_asserts.py: pinned 0 (no asserts in the header).
- Header users: 1 landed row includes it (0x00974110); it does not define the macro, so the opt-in changes nothing there.

## Check at hand-back
Every FABLE_RETAIL_* macro used in rows/ resolves in rebuild/include or hdr/; every orig/ copy is byte-equal to the repo header.

## Part 2 (NF93 continued)

## engine/CCombatAction_WillEnflameInto.h (repo rebuild/include/engine/CCombatAction_WillEnflameInto.h)
- `FABLE_RETAIL_CCOMBATACTION_WILLENFLAMEINTO_FINISHACTION` (NF93, new, inside the existing `_MAZE` class block, public, after `Level`):
  declares `virtual void FinishAction();` (Ego_r.pdb 0x008FAEB0 + FableWin.pdb ?FinishAction@CCombatAction_WillEnflameInto@@UAEXXZ, slot 4,
  0x008C2A80). Declaration only. Used by row 0x008C2A80.
- pin_asserts.py: pinned 2 (the two _MAZE-block asserts below the class, now FABLE_STATIC_ASSERT_AT(22/23, ...)).
- Header users: see the count line at the end of this part.

## engine/CCreatureAction_CastWillEnflameSpell.h (repo rebuild/include/engine/CCreatureAction_CastWillEnflameSpell.h)
- `FABLE_RETAIL_CCREATUREACTION_CASTWILLENFLAMESPELL_CLASS` (NF93, new): the polymorphic retail class on CCombatActionBase (rtti_bases.tsv)
  with Level +0x114 / IsZTargetting +0x118 (sizeof 0x11c) and the Ego_r.pdb / FableWin.pdb ctor
  ??0CCreatureAction_CastWillEnflameSpell@@QAE@AAVCThingCreatureBase@@J@Z (0x008BB450). The generated flat class stays verbatim under `#else`
  (the part-1 CastWillForceSpell pattern), so the default path is token-identical apart from the pinned assert lines. Used by row 0x008C2A80.
- pin_asserts.py: pinned 4 (the four generated asserts, now FABLE_STATIC_ASSERT_AT(82..85, ...)).

## engine/CCombatAction_WillShieldInto.h (repo rebuild/include/engine/CCombatAction_WillShieldInto.h)
- `FABLE_RETAIL_CCOMBATACTION_WILLSHIELDINTO_FINISHACTION` (NF93, new, inside the existing FABLE_RETAIL_CCOMBATACTION_WILLSHIELDINTO block,
  public, after the ctor): declares `virtual void FinishAction();` (Ego_r.pdb 0x008FAE10 + FableWin.pdb
  ?FinishAction@CCombatAction_WillShieldInto@@UAEXXZ, slot 4, 0x008C29F0). Declaration only. Used by row 0x008C29F0.
- pin_asserts.py: pinned 4 (the asserts below the opt-in moved).
- Header users (part-2 headers so far): engine/CCombatAction_WillEnflameInto.h has no landed includer; engine/CCreatureAction_CastWillEnflameSpell.h
  is included by 0x008BA220 and 0x008BA370 (the 0x008C2A80 hit is only its comment); engine/CCombatAction_WillShieldInto.h by 0x00977E00. None
  defines the new macros, so the opt-ins change none of them.

## engine/CCreatureAction_BriarRoseShotFarInto.h (repo rebuild/include/engine/CCreatureAction_BriarRoseShotFarInto.h)
- `FABLE_RETAIL_CCREATUREACTION_BRIARROSESHOTFARINTO_CLASS` (NF93, new): retail class on CCombatActionBase (rtti_bases.tsv; no own members,
  sizeof 0x114) declaring the slot-4 override `virtual void FinishAction();` (Ego_r.pdb 0x008FB0F0 + FableWin.pdb
  ?FinishAction@CCreatureAction_BriarRoseShotFarInto@@UAEXXZ, 0x008C2C50). Generated flat class kept under `#else`. Used by row 0x008C2C50.
- pin_asserts.py: pinned 2. Header users: none.

## engine/CCreatureAction_BriarRoseShotFarOutof.h (repo rebuild/include/engine/CCreatureAction_BriarRoseShotFarOutof.h)
- `FABLE_RETAIL_CCREATUREACTION_BRIARROSESHOTFAROUTOF_CLASS` (NF93, new): retail class on CCombatActionBase (rtti_bases.tsv) with the Ego_r.pdb
  member PShot (CIntelligentPointer<CThing>, +0x114; sizeof 0x11c) and the Ego_r / FableWin ctor
  ??0CCreatureAction_BriarRoseShotFarOutof@@QAE@AAVCThingCreatureBase@@0@Z (0x008C39A0). Generated flat class kept under `#else`. Used by 0x008C2C50.
- pin_asserts.py: pinned 3. Header users: 0x008C39A0, 0x008C0900; neither defines the macro.

## engine/CCreatureAction_GoUnderground.h (repo rebuild/include/engine/CCreatureAction_GoUnderground.h)
- `FABLE_RETAIL_CCREATUREACTION_GOUNDERGROUND_CLASS` (NF93, new): retail class on CCombatActionBase (rtti_bases.tsv; sizeof 0x114) with the
  Ego_r.pdb 0x008F0E90 / FableWin.pdb ctor ??0CCreatureAction_GoUnderground@@QAE@AAVCThingCreatureBase@@@Z (0x008B8110; that body stores vtable
  0x0127A894, RTTI CCreatureAction_GoUnderground). Generated flat class kept under `#else`. For row 0x009208E0.
- pin_asserts.py: pinned 2. Header users: 0x008B6A20, 0x008B8110; neither defines the macro.

## engine/CCreatureAction_ScorpionRecover.h (repo rebuild/include/engine/CCreatureAction_ScorpionRecover.h)
- `FABLE_RETAIL_CCREATUREACTION_SCORPIONRECOVER_CLASS` (NF93, new): retail class on CCombatActionBase (rtti_bases.tsv; sizeof 0x114) with the
  Ego_r.pdb 0x00826980 / FableWin.pdb 0x02856D30 ctor ??0CCreatureAction_ScorpionRecover@@QAE@AAVCThingCreatureBase@@@Z (0x008E4770; that body
  stores vtable 0x01286724, RTTI CCreatureAction_ScorpionRecover). Generated flat class kept under `#else`. Used by row 0x008DF640.
- pin_asserts.py: pinned 3. Header users: 0x008C4B00, 0x008C3EE0; neither defines the macro.

## engine/CCreatureAction_CastStrengthMultiStrike.h (repo rebuild/include/engine/CCreatureAction_CastStrengthMultiStrike.h)
- `FABLE_RETAIL_CCREATUREACTION_CASTSTRENGTHMULTISTRIKE_CTOR` (NF93, new, nested in the NF75 `_CLASS` block, public, after FrameUpdate):
  declares the Ego_r.pdb 0x0090E7B0 / FableWin.pdb 0x02AE3A30 ctor
  ??0CCreatureAction_CastStrengthMultiStrike@@QAE@AAVCThingCreatureBase@@AAVCThing@@J@Z (0x008E6A70). Declaration only. Used by row 0x008E6AE0.
- pin_asserts.py: pinned 0 (the asserts below were already FABLE_STATIC_ASSERT_AT). Header users: 0x008E6390, 0x008E6A70; neither defines the macro.

## engine/CCreatureAction_ScreamerDrainAttack.h (repo rebuild/include/engine/CCreatureAction_ScreamerDrainAttack.h)
- `FABLE_RETAIL_CCREATUREACTION_SCREAMERDRAINATTACK_FINISHACTION` (NF93, new, nested in the wfE11 `_PROTOTYPE` class block after its default
  ctor): declares `virtual void FinishAction();` (Ego_r.pdb 0x008F6CA0 + FableWin.pdb ?FinishAction@CCreatureAction_ScreamerDrainAttack@@UAEXXZ,
  0x008B9010). Declaration only. Used by row 0x008B9010 (which defines CCreatureAction_CastWillDrainLifeSpell locally first, as 0x006F5C10 does).
- pin_asserts.py: pinned 1 (the _PROTOTYPE size assert). Header users: 3 landed rows; none defines the macro.

## engine/CCreatureAction_ScreamerDrainOutOf.h (repo rebuild/include/engine/CCreatureAction_ScreamerDrainOutOf.h)
- `FABLE_RETAIL_CCREATUREACTION_SCREAMERDRAINOUTOF_CTORS` (NF93, new, nested in the wfE11 `_PROTOTYPE` class block): with it, the block declares the
  two FableWin.pdb inline ctors ??0CCreatureAction_ScreamerDrainOutOf@@QAE@AAVCThingCreatureBase@@@Z / ...@AAVCThing@@@Z (bodies as retail expands
  them at 0x008B9010: CCombatActionBase(creature[, target], 150, CSoundPair())) INSTEAD of the prototype's default ctor (`#ifndef ... #else`): that
  default ctor calls CCombatActionBase(long), which only the non-MELEE branch of engine/CCombatActionBase.h declares. Without the macro the
  _PROTOTYPE block is token-identical to the repo. Used by row 0x008B9010.
- pin_asserts.py: pinned 1 (the _PROTOTYPE size assert). Header users: 2 landed rows; none defines the macro.

## engine/CThingCreatureBase.h (repo rebuild/include/engine/CThingCreatureBase.h)
- `FABLE_RETAIL_CTHINGCREATUREBASE_GETDAZEDHEALTHTHRESHOLD` (NF93, new, after the HEIGHTBELOW block, `public:`; the surrounding blocks leave
  the access public): declares `float GetDazedHealthThreshold() const;` (Ego_r.pdb 0x0053CEF0 + FableWin.pdb
  ?GetDazedHealthThreshold@CThingCreatureBase@@QBEMXZ, out of line at 0x00662090). Declaration only. Used by row 0x008663D0.
- pin_asserts.py: pinned 0. Header users: 493 landed rows; none defines the macro.

## engine/CCreatureAction_BeDazed.h (repo rebuild/include/engine/CCreatureAction_BeDazed.h)
- `FABLE_RETAIL_CCREATUREACTION_BEDAZED_CLASS` (NF93, new): retail class on CCreatureActionBase (rtti_bases.tsv; sizeof 0xa8) with the Ego_r.pdb
  0x0083DF40 / FableWin.pdb 0x0281A930 ctor ??0CCreatureAction_BeDazed@@QAE@AAVCThingCreatureBase@@@Z (0x00865A00; that body stores vtable
  0x01276F54, RTTI CCreatureAction_BeDazed) and the two pure-virtual overrides PerformAction (FableWin) / Clone (Ego_r + FableWin). Generated flat
  class kept under `#else`. Used by row 0x008663D0.
- pin_asserts.py: pinned 2. Header users: 1 landed row (0x008ED850); it does not define the macro.

## Coordinator note (2026-11-19, after landing part 2 as nf93b)
Parts 1 and 2 are landed. Every header in hdr/ and orig/ was replaced with the CURRENT repo header (which contains your opt-ins and NF95's),
so hdr/ == orig/ == repo now; pre-reset copies are in ../part2_overlay_snapshot/. Add part-3 opt-ins to these copies only.

## Part 3 (NF93 continued)

## engine/CAFile.h (repo rebuild/include/engine/CAFile.h)
- `FABLE_RETAIL_CAFILE_CREATEDIRECTORY` (NF93, new, nested in the FABLE_RETAIL_CAFILE block after the EXPANDPATH block): declares the static
  __fastcall CreateDirectoryW(const CWideString&) 0x009994A0, CreateDirectoryForFile(const CWideString&) 0x00999620 and MakePath(4 x const
  CWideString&) 0x00997780 (Ego_r.pdb + FableWin.pdb ?CreateDirectoryW@CAFile@@SIXABVCWideString@@@Z / ?CreateDirectoryForFile@CAFile@@SIXABVCWideString@@@Z
  / ?MakePath@CAFile@@SI?AVCWideString@@ABV2@000@Z). Declarations only. Used by rows 0x009994A0 and 0x00999620.
- pin_asserts.py: pinned 0 (the header has no FABLE_STATIC_ASSERT).
- Header users: 74 landed rows include engine/CAFile.h; 0 define the new macro, so the opt-in changes none of them.
