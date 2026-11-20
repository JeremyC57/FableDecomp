# NF169 header overlay

Base copies of every overlaid header are in ../orig/ (copied from the repo working tree when first needed; never re-copied).

## engine/CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose.h (orig/engine/...)
- Adds `FABLE_RETAIL_CAISTATEGROUP_MELEECOMBATNEARBYATTACKWHENVERYCLOSE` (row 0x0093DC10): the polymorphic retail class on
  CAIStateGroup_MeleeCombatNearbyBase (RTTI rtti_bases.tsv line 212; no own members, sizeof 0x1C) with the protected virtual const
  IsStillRunnable(bool) declaration (Ego_r.pdb 0x00961B60 + FableWin.pdb 0x02B92290). Declaration only.
- Default path token-identical: the generated struct, its `#pragma pack` pair and its two FABLE_STATIC_ASSERT_AT lines sit unchanged in
  the `#else` branch. pin_asserts.py: pinned 0 (the asserts already use the `_AT(18/19)` forms).
- Landed includers (grep): 0x0093E2D0, 0x008FD8FD, 0x008FF9D2, 0x0121F86F. None defines the new macro, so none changes.
