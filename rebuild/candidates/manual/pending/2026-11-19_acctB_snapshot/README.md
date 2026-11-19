# Account B snapshot (2026-11-19): matched rows not yet landed, plus open escalations

This is a durability copy of scratchpad work. The live workspaces may be lost if the container restarts.
- Copied with `pending/2026-11-19_authoring_handoff/tools/stage_ws.py`, which leaves out retail bytes, oracles, disassembly listings and binaries.
- No GameCODA SDK file is included.
- Nothing here is landed. Landings go through the HANDOFF.md procedure and are recorded in batches/.

## Matched rows, waiting to land
| folder | row | state |
|---|---|---|
| hand4d_b10 | 0x00C77800 CFloatReverbParameterTranslator::UpdateDecayTimes | LANDED (batches/2026-11-19_landing_hand4d_b10) |
| hand4d_b11 | 0x00C88E60 CReverbFnsSSE::ApplyFilterOrder2_quad | the author's version: the retail_asm route, plus an alt_library draft |
| hand4d_b11_audit_fixed | 0x00C88E60, library route | LANDED as toolchain_library (batches/2026-11-19_landing_hand4d_b11) |
| hand3c_b25 | 0x00986210 CallingChildrenIsRunnable | LANDED (batches/2026-11-19_landing_hand3c_b25_b26) |
| hand3c_b26 | 0x00AC7EB0 COptimisedPrimitive::ComputeSelfContraction | LANDED with the audit's GFSqr form, not this snapshot's in-place squares (alt: hand3c_b26_alt_bracket) |

**Route for 0x00C88E60:** library grade, not the retail_asm source in hand4d_b11/src. This is an advisor decision (owner delegated). The asm route would need ASM_RULES rule 4's attempt table to be waived, and that table is not waived. Five checks still block the landing:
- record the plain-C++ DIFFER;
- portable_twins_check must give a named SKIP when the SDK is absent;
- the host-ABI gap must be resolved;
- the coordinator must re-run lib_verify, purity and the dual modes on the fixed files;
- the landing must mirror the latest toolchain_library landing.

## Open escalations (no parking; work continues)
These are in escalations/. Each holds the residue, the levels tried and the advisor candidates.
- **0x00989230:** the period-4 live-code measurement is in p989_ESCALATE_00989230.md and p989_PROBES.tsv.
- **0x008185B0:** one inlined call site still differs.
- **0x00A4D770:** one lea differs. The CopyPtrs cross-check shows retail's base operand is a later compiler temp.
- **0x00774D20:** the register allocator splits a different value than retail; the next step is measuring c2's allocation decision.
