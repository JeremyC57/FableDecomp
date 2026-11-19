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
