# NF259 header overlay (add-only opt-ins; base copies in ../orig/, byte-equal to the repo working tree at the start of the pass)

## engine/CTCHealLife.h
- Base: rebuild/include/engine/CTCHealLife.h (orig/engine/CTCHealLife.h).
- Adds `FABLE_RETAIL_CTCHEALLIFE_SETAICAST`: the declaration `void SetAICast(long spell_level);` (public; Ego_r.pdb 0x007C2CC0 + FableWin.pdb
  0x02714670 `?SetAICast@CTCHealLife@@QAEXJ@Z`; retail 0x007C3EA0, reloc_bindings witness 0x008B97E0), right after the class head's `public:`.
  Declaration only; no layout change. Used by rows/008c2ad0.cpp.
- pin_asserts: pinned 0 (the size assert was already `FABLE_STATIC_ASSERT_AT(24, ...)`).

## engine/CCreatureAction_CastAssassinRush.h
- Base: rebuild/include/engine/CCreatureAction_CastAssassinRush.h (orig/engine/CCreatureAction_CastAssassinRush.h).
- Adds `FABLE_RETAIL_CCREATUREACTION_CASTASSASSINRUSH_TARGETCTOR` inside the retail (FABLE_RETAIL_CCREATUREACTION_CASTASSASSINRUSH) class: the
  declaration of the (creature, target, z-targeting, level, final action) constructor
  `??0CCreatureAction_CastAssassinRush@@QAE@AAVCThingCreatureBase@@AAVCThing@@_NJV?$CCountedPointer@VCCreatureActionBase@@@@@Z` (Ego_r.pdb 0x009054E0 +
  FableWin.pdb 0x029F8E90; retail 0x008CE040, reloc_bindings witness 0x0072CAA0). Declaration only; no layout change. Used by rows/0096ae50.cpp.
- pin_asserts: pinned 16 (every FABLE_STATIC_ASSERT below the insertion, in both branches, now `FABLE_STATIC_ASSERT_AT(<old line>, ...)`, so the
  typedef names are unchanged).

## Default path
With neither new macro defined both headers preprocess to the same token text as the repo copies (the only other edit is the assert pinning,
which keeps every typedef name). Whole-tree equiv_check (repo rebuild/include v a hard-link copy with these two files replaced):
evidence/equiv_final.log. Landed includers (grep of rebuild/src/compiled, rebuild/tests, rebuild/include): CTCHealLife.h 0x007C3FB0, 0x007C4430
(+ its test); CCreatureAction_CastAssassinRush.h 0x0072D990, 0x0072CAA0, 0x008C2550, 0x008CD470, 0x009735D0, 0x0096AE50 (this hand-back). None of
them defines either new macro except 0x008C2AD0 / 0x0096AE50 (this hand-back); all were rechecked with the overlay on (evidence/vland_e2.log).
No other headers were overlaid (entry 1 and the other entry 2/3 rows use opt-ins that are already in the repo: nf255 / nf257 / earlier).
