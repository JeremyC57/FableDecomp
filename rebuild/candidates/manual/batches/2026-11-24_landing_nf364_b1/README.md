# nf364 b1: Reset<NUISystem::CComponent> copies made retail-shaped in 19 y-off size TUs (new opt-in ..._RESET_SIZE_BODY_YOFF_TU)

Agent NF364 (Account A naming queue; hand-back `pending/2026-11-24_nf364_handback/`), from NF358's census and NF362's Step 1. These rows set
`optimize("y", off)` + `("s", on)` above engine/CCountedPointer.h, so their Reset copy compiled at y off (62 B, NEQ) while the owner 0x00429C6F
is y on + s on (59 B). NF352's block cannot fix them: placed after the row's pragmas it still compiles at y off, and its `("", on)` re-modes the
leaf.

engine/CCountedPointer.h gains an add-only sibling block (38 lines, no existing line changed), FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY_YOFF_TU:
`optimize("y", on)` + `("s", on)`, the same `template<> __declspec(noinline) inline` Reset<NUISystem::CComponent> body token for token, then
restore `("y", off)` + `("s", on)`. Marked `// Fidelity: scaffolding, not retail` with the pragscan precondition; `#error` if defined together
with ..._RESET_SIZE_BODY (control measured). Each row adds one `#define` above its first include.
Rows: 00427216 00582ace 0058315b 0058a0c7 0058e208 005907ab 0059cb90 0059d270 0059ed42 0059f51e 0059fee3 005a8c66 005d64dd 005d6748 005d7081
005a29f4 005a9cbe 005aaf28 005ad571.

Ruling (advisor decision, owner delegated, 2026-11-24; NF362's P1): approved on these conditions, all met.
- Own macro, add-only, `#error` guard.
- Pragscan per row: state before the block (y off, s on) equals the restored state after it; the specialization compiles at y on, s on;
  the row's own pragmas and the end-of-TU state are unchanged.
- One COMDAT changes per row (Reset 62 -> 59 B), equal to the owner object; leaf identical.
- Whole-catalog equiv_check plus a planted control.

Agent gates: objsec one COMDAT; vland 19/19 RELOCATION_MATCH PASS HEAD and new, identical WARN sets; pkcmp GATE PASS, reversed control fails;
reach flips 0; equiv_check pristine v new tree 0 differing, planted control seen in exactly the 19 rows. ReleaseData stays EQ (no y-on
ReleaseData body needed). 005d64dd's DeleteData 0x00428B10 copy stays NEQ (existing nf360 debt).
Coordinator from the repo:
- `$S/v364N.sh`: 19/19 RELOCATION_MATCH PASS; pk_all v the agent's HEAD files: only Reset NEQ -> EQ, 0 new NEQ; pickany_vs_retail 0 new
  NEQ; reachflip 0 (19 pairs).
- Owner: comdat_eq of 00427216's and 005a9cbe's Reset COMDAT v the coordinator's build of 0x00429C6F: bytes, relocations, selection,
  characteristics all equal (OBJECT_EQ).
- equiv_check, HEAD include tree v the landed tree (both scratch copies) over the landed row texts: 57,155 rows, exactly the 19 user rows
  differ (src), nothing else (logs/equiv_coordinator.tsv).
Part 2 (diagnosis, 0x00603000): the same size body without `__declspec(noinline)` fixes Reset (59 B EQ) but moves the leaf (DIFFER 9794v9571)
and flips ReleaseData's reach. Closed; see docs/journal/FINDINGS_LOG.md 2026-11-24.
