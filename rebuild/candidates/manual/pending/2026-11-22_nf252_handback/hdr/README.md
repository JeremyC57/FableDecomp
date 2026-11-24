# NF252 header overlay (2026-11-22)

Base copies in ../orig/ (from the repo working tree at start). Only `#ifdef` opt-ins; the default path is token-identical.

## engine/CCreatureAction_ScreamerIntoAggressive.h
- New add-only opt-in `FABLE_RETAIL_CCREATUREACTION_SCREAMERINTOAGGRESSIVE_CTOR_OUTOFLINE` (row 0x0096DB60): wraps the existing in-class
  constructor `CCreatureAction_ScreamerIntoAggressive(CThingCreatureBase&, CThing&) : CCombatActionBase(creature,target,150,CSoundPair()) {}`
  as `#ifdef <macro> <declaration only> #else <the existing two lines, unchanged> #endif`. Evidence: FableWin.pdb S_PUB32
  ??0CCreatureAction_ScreamerIntoAggressive@?A0x4c815337@@QAE@AAVCThingCreatureBase@@AAVCThing@@@Z; retail keeps the out-of-line copy
  0x0096DB60 (58 B) while its callers expand it.
- pin_asserts.py: 1 pinned (`FABLE_STATIC_ASSERT(sizeof(...)==0x114)` moved from line 17 to 23 -> `FABLE_STATIC_ASSERT_AT(17, ...)`).
- equiv_check.py (repo rebuild/include v a hard-link copy with this file replaced): 57,125 rows checked, 0 differ (work/equiv.log).
- Landed includers: 0x0096DF50 only (`grep -rl CCreatureAction_ScreamerIntoAggressive.h rebuild/src rebuild/tests rebuild/include`); it does
  not define the macro, so nothing changes for it. The rows of this hand-back that use the overlay: 0x0096DB60 (defines the macro),
  0x0096DBA0 / 0x0096DBB0 / 0x0096E3F0 / 0x0096E510 / 0x0096DDB0 (default path). No other agent overlay has this header (NF251's hdr/ is empty).
