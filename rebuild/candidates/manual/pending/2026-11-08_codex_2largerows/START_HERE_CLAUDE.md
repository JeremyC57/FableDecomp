# Start here — two parked large rows, 2026-11-08

This is the current handoff. Resume from `rows/`, not from the older sources under PRIOR_HANDOFFS or the experimental VARIANTS directory.

**Neither complete function matches retail. Neither has a behavior test. Do not land either as finished.** The final ordinary VC7.1 RTM recompiles are `HISTORY/<address>/current_checkpoint_02/RESULT.json`. Both frame sizes now match retail exactly.

| Address | Function | Selected bytes / retail | Frame | Delivery |
|---|---|---:|---|---|
| 0041e6d3 | NUISystem::CManager::Action | 35,702 / 35,651 | 0x800 / 0x800 | PARKED, partial improvements |
| 00cbfb7d | RunCutsceneMacro | 74,906 / 74,778 | 0x18dc / 0x18dc | PARKED, partial improvements |

## Read and resume

1. Read ROW_DETAILS.md, LESSONS.md, STATUS.tsv and each row's ROW.json / PROBES.md / TEST_STATUS.md.
2. Use each row's separate `hdr/` first in the include path. BASE_HEADERS/rebuild/include contains the exact pinned source headers used here; use it second, then your VC7.1 RTM include directory. Do not overwrite newer shared repository headers with these snapshots. Merge only justified opt-in changes after review; keep default branches unchanged.
3. Compile with cl 13.10.3077 RTM, base `/c /O2 /Oy /W3`, plus Action `/GS` or cutscene `/d2inlS8`. Each source has `#pragma optimize("s", on)`. Select only its exact `// Leaf:` symbol. Do not land other emitted helpers from the TU.
4. Re-cut both complete extents from your own executable (SHA-1 39fd416ba23a91a89713ec80c789f57ddb22e2c8); compare complete selected sections, masking only relocated operand bytes. Action's 35,651 bytes include 1,032 jump-table bytes; its instruction diagnostic excludes those tables, but its byte gate includes them.
5. Complete the PDB / RTTI / caller / shared-header audit and check every relocation identity. Preserved naming evidence is not a claim that this continuation audited every declaration. NAME_FIXES.tsv preserves four prior Action findings; no new naming correction is claimed here.
6. Write and run complete behavior tests, then regrade every delivered final file before landing. There is currently no passing-test claim for either row.

## What changed

Action incorporates the newer Claude v23 handoff plus the real counted-pointer constructor visibility fix, proper trade receiver caching after successful FindChild, typed bulk-count access, the unsigned augmentation guard, and a named caption string. The 13 named-bool setter fixes were already in v23; they are not additional independent gains from this continuation.

Cutscene exposes the real read-only IS_THING_ALIVE body while retaining its call, then assigns flagWasSet in both actual SetFlag branches. These recover the two extra frame words by different mechanisms: sharing the pThing slot, then removing boolean-group padding. The last probe `slide_factor_compound` compiled to 74,882 bytes and is preserved as a promising alternate in HISTORY and VARIANTS. It still differs, and its local x87 changes had not received final manual review when the owner requested immediate wrap-up. The selected 74,906-byte source is the more reviewed checkpoint. Smaller output alone does not establish correctness.

## Contents and provenance

- rows/: selected sources, separate header overlays, exact result metadata, attempt index and explicit test status.
- HISTORY/: every local compiler attempt, including failures and regrades, with source/header snapshots, compiler logs, emitted symbol and relocation lists. Native objects and byte dumps are intentionally excluded.
- VARIANTS/: all proposal files, including unselected/uncompiled drafts. Match their source hashes to ATTEMPTS.tsv to determine whether a file was graded. Do not assume every proposal is compiled.
- PRIOR_HANDOFFS/: original two-row handoff and updated Action session-5 handoff, including their older attempt histories. Their status summaries are historical.
- BASE_HEADERS/: pinned header dependencies and fetch receipt.
- RESEARCH/: allocator evidence, frame maps and local diagnostic tooling. Tool paths are workspace-specific and need adapting; these are not a portable turnkey verifier. Allocator instrumentation is diagnostic only; ordinary RTM built all reported grades.
- SHA256SUMS / VALIDATION.json: integrity and selected-file verification, not behavioral correctness.

No compiler, Wine runtime, retail executable, game assets, retail byte dumps, native object files, or repository history is included. No commit or push was performed. Scope remains only these two rows; supporting helper inspections do not authorize landing extra functions.
