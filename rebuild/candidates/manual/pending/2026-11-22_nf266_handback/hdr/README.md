# NF266 overlay headers

## engine/CCreatureAction_CastAssassinRush.h (q12743)
- Base: the repo working tree copy (orig/engine/CCreatureAction_CastAssassinRush.h, byte-equal to rebuild/include at copy time).
- Added (add-only, retail branch only, inside `#ifdef FABLE_RETAIL_CCREATUREACTION_CASTASSASSINRUSH`, right after the NF259 TARGETCTOR block,
  still under `public:`): `#ifdef FABLE_RETAIL_CCREATUREACTION_CASTASSASSINRUSH_INITIALISE` -> `virtual void Initialise();` with a two-line
  comment. Evidence: Ego_r.pdb 0x009047C0 + FableWin.pdb 0x029F9410 ?Initialise@CCreatureAction_CastAssassinRush@@UAEXXZ, retail vftable
  0x01284254 slot 12 = 0x008CD470 (81 slots). Declaration only: it overrides the base's slot 12 (no slot appended, no layout change).
- Default path: token-identical (the new macro is defined by no landed row; only rows/008cd470.cpp defines it).
- pin_asserts.py: pinned 0 (every assert in the file is already FABLE_STATIC_ASSERT_AT; the 5 added lines move none of their names).
- Landed includers (grep of rebuild/src, rebuild/tests, rebuild/include): 0x008C2550, 0x008CD470, 0x008CEF50, 0x009735D0, 0x0072D990,
  0x0072CAA0, 0x0096AE50 (+ engine/INDEX.tsv). None defines the new macro, so none changes. Whole-tree equiv_check (repo v repo + this file): 57,125 rows, 0 differ (evidence/equiv.log); the six includers rechecked RELOCATION_MATCH + PASS (evidence/vland_includers.log).
- The 3-argument ctor declaration (for 0x0072CAA0) is NOT added: that row was not ported (q12743 partial), and no opt-in that no finished
  row uses is handed back.
