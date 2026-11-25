# NF427 hand-back (2026-11-25): the creature-action class family

**Landed: 125 rows**, Part 1 members of the CCreatureAction_* / CCombatAction_* classes:

| batch | rows | form | grade |
|---|---:|---|---|
| `batches/2026-11-24_landing_nf427_g1a` | 50 | G1 constant-return virtuals (`return true` / `false` / a long) | MATCH |
| `..._g1b` | 47 | G1 | MATCH |
| `..._g1f` | 9 | G1 returning a float literal; the row spells it (q3) | RELOCATION_MATCH |
| `..._g2a` | 19 | G2 tail jump to the base member; 19 callee respells from `_ForwardTarget*` stand-ins | RELOCATION_MATCH |

- **Tests and gates:** every row's test passes and `name_gate.py --test` passes on row and test.
- **Headers:** 74 class headers gain one add-only `FABLE_RETAIL_<C>_VIRTUAL_MODEL` opt-in, the polymorphic class over its RTTI base.
  - Each emitted vftable was checked against retail slot by slot.
  - Asserts are re-pinned with `FABLE_STATIC_ASSERT_AT`.
  - Equivalence: 0 of 57,160 landed rows change their preprocessed input (`evidence/equiv_g_batches.log`), measured copy against copy, as described below.
- **T3 lines closed:** the nf324 / nf330 lines these rows port.

**Part 2 waits** (p2a-p2d, 108 rows: NF425's held Clone rows moved onto the new class models).
- Their vftables are right. Every unbound slot carries retail's exact RTTI spelling: 1,724 slots (`vtx427.py`, `evidence/vtx_p2*.tsv`). The 68 slot-0 `??_E` entries are VC7.1's weak externals linked to `??_G`.
- But all 108 tests add a local body of a class that has an engine header, where HEAD's tests had none. NF427 round 2 is porting them, together with the 221 NF425 tests that landed with the same debt (GOTCHAS nf425 / nf427).

## Rulings (advisor decision, owner delegated, 2026-11-25)
- **q1 (p2c), granted:** the local class carrying the full retail override set lands under r1. HEAD already modelled it locally and there is no header to violate.
- **q2 (p2d), granted:** HEAD did not include `engine/<C>.h` and no row drops an include. The header port stays in T3.
- **q3 (g1f), granted:** a spelled float literal replaces a made-up data symbol (`?g_FloatOne@@3MA`). Its `__real@<bits>` bits equal retail's pooled literal.
- **q4, granted:** three models omit `#pragma pack(1)` and the trailing pad member inside their opt-in only. Retail's inlined copy constructor skips those bytes, so they are tail padding. Size and every offset stay asserted.

## A gotcha found while landing: equiv_check measures copy against copy
`equiv_check.py rebuild/include <copied tree>` reported 4,717 false DIFFs. An UNCHANGED copy of `rebuild/include` "differs" from the repo tree in the same way: 0x0081EE60's preprocessed text lacks CGameMap.h's classes against the repo tree and has them against any copy. Compare an unchanged copy with the changed copy; that gave 0.
