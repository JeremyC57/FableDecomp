# NF470 hand-back (2026-11-26): the CAIStateGroupBase destructor chain, step 1

`README_agent.md` is the agent's own report. `T3_RESOLVED.tsv` covers the six stand-in binders on T3 line 651, with each one's identity, callers, route and status.

## Landed: bR0 (as `bR0a`), 11 rows
The 11 CAIStateGroup rows whose local models emitted a non-retail pick-any `??1CAIStateGroupBase@@UAE@XZ` now declare it only, the r469a form.
- **Gates:**
  - vland HEAD v new RELOCATION_MATCH PASS, WARN equal;
  - objproof: only the copy goes (at 0x00902FB9 also the base `??_G` / `??_7`), with `.drectve` unchanged;
  - link against 0x008FD288: duplicates 11 → 0, unresolved the same.
- **Landing note:** the batch had no `head/` payload, and its RESULTS `new` column was prose. `bR0a` adds the HEAD payload (recheck of the unchanged rows) and puts each row's own unchanged self symbol in `new`.
- **After landing:** bindings_multi 1; funclets 1,704, 0 broken; s units 159, 0 failed; descriptive_check 2,030, 0 FAIL. The tracker moved no address (expected).

## Held
- **bR1_008fd288_unit** (0x008FD288 plus the GoToBed pilot binder 0x008FD2CA and its callers 0x008FEB97 / 0x008FEBC5) holds on T3 line 651: 1 of its 6 binders is ported.
  - NF469's `bR1_008fd288_thin` must not land: its test fails to link against the thin object.
- **NF469 b0_hdr + b4** wait behind bR1.

## Rulings (coordinator, on precedent)
- **r470a: granted.** A callee-respelt `??_G` row of the same retail TU may own the implicit destructor as a thin row (r469b).
- **r470b:** the coordinator checks s_unit_check at landing. If it reads a qualified destructor call as a definition, that is a tool false positive to fix in the tool, not in the source.
- **r470c: granted.** An unreferenced pick-any inline-destructor COMDAT with no retail home is allowed when no landed object defines that name strongly (r464a).
- **r470d: granted.** A guard that hides the JackDragon header's destructor declaration may go in when the default path is byte-identical (equiv) and the PDBs flag the destructor compiler-generated (R-NF436-2).
- **r470e:** a retail-class opt-in in the `_hdr` batch. A header exists, so this is not the r462a local-class route.
