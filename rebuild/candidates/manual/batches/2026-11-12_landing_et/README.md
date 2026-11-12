# 2026-11-12 landing et -- name-fix pass NF30: 54 queue entries, 90 landed rows changed, 18 renamed

Source: `naming/2026-11-12_agentNF30/` (STATUS, APPLIED, REJECTED, RENAMES, NOTES, LESSONS, QUEUE, hdr README). Run as the workflow
`lane_tools/workflows/name_fix_queue_pass.js`, all Opus: apply, adversarial verify, one repair, verify again (ok: notes only).

**Queue (tags land1011eo .. es):** 34 done, 19 partial, 1 rejected (0x00BD9AB0: the body is a folded `ret 8` and both PDBs have two
members of that shape, so the name cannot be proven); one claim half wrong and corrected (0x00533CC0 is CComponent slot 60
AddChildDynamically, not AddChild).
**Rows:** 90 landed rows changed, 18 renames of the row's own name (RENAMES_applied.tsv section 1) and about 100 caller respellings
(section 2; the largest groups: SetBrainOutput -> CAIBrain::SetOutput in 21 rows, virtual slot 2 Yield -> SuspendSuspendableProcess
in 22 rows, the LoadDef callers). All 90 rechecked in this landing: MATCH or RELOCATION_MATCH, every test PASS (`logs/recheck.log`).
**Rework done:** 0x00658EC3 no longer carries STAND-IN bodies for the IS_THING_ALIVE instances; it uses the real bodies of
engine/ThingAlive.h and matched on the first grade (it still keeps two older local models, noted for a later port).
**Verifier findings repaired before landing:** the ClearEffects respelling had skipped two rows that declare the same callees
(0x0043E420, 0x00438460), which left four retail addresses with two symbols each; RENAMES section 2 had left out four respellings.
**Header:** CThing.h, one access-only opt-in (PEEKWORLD_FRIEND_CREATUREACTIONBASE), default unchanged.
**Bindings:** rebuilt by rebind.py for the 90 rows; bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
**Carried over (queue tag land1011et):** the 19 partial entries. Most are no longer naming work: five need a new byte match of
the row itself (marked AUTHORING), and the rest are callers built on stand-in classes that must first move onto shared headers
(marked PORT). They should not trigger another naming pass by themselves; see `naming/2026-11-12_agentNF30/NOTES.md`.
