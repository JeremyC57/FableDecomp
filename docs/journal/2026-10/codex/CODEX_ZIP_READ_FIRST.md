# CURRENT CLAUDE UPDATE — SINGLE ZIP, NO GIT HISTORY

This is the current handoff requested by the user. Download and retain this entire ZIP. Git history and unchanged baseline source files are intentionally not repeated; this is an update for Claude's existing repository, not a standalone checkout.

## Scope

Base: `fbfaf9f904bb1ab4a47ad163c96d5aed3068a030` (Claude's prior FullDecomp checkpoint).
Resulting source/documentation state: `66e0128d8e9ae695cea1ed37878b9cde5e008f2b`; final accepted source checkpoint: `759d5ead9`.

This begins BEFORE Codex first started working for the user, at Claude's 34,098-function handoff. It includes the entire session: initial Wine recovery, early chunks 189 onward, pre-rootbatch30 work, later resumed chunks, retries, reviews and final closeout. It is NOT based on the later 34,246-function resume point. All 550 newly accepted functions and their source/fixture evidence are covered. `WHOLE_SESSION_COVERAGE.json` records this check. One unchanged baseline oracle referenced by the session is also supplied under `baseline-reference-files/` for convenient review.

Included: every new or modified tracked file since the baseline, all selected untracked attempt/recovery evidence, all documentation from the original sealed source package, and the complete compiled-artifact supplement. This covers 44,604 source/evidence files and 8,744 compiled-artifact/scratch files. All 7,163 new/modified tracked paths are accounted for across these payloads. The six deleted/renamed baseline paths are recorded separately and handled by the guarded helper below.

All source/fixture payload bytes included here were verified against the sealed original archive manifest. The compiled supplement decompresses to the exact same TAR bytes as the original supplement. No decompilation attempts or new compiler/test grades were performed while repackaging. Existing attempts, uncertainty reservations and worker evidence are preserved. Older Git-history and unchanged-baseline data remain in the original local archive; they are not required in this requested update download.

## Read first

1. `FINAL_PROGRESS.md` and `FINAL_PROGRESS.json`: exact accepted counts and unresolved work.
2. `CLAUDE_HANDOFF.md`: compiler/setup instructions, continuation priorities, attempt budgets and known verification exceptions.
3. `NAMING_RECOVERY.md`: proven original function/member names, aliases and unresolved variable names.
4. The five TSV review lists in this ZIP: accepted functions, whole-row details, READY candidates, near/parked/skipped work and blocked/unstarted work.

Accepted: **34,648 / 49,568 functions (69.8999%)**, +550 since Claude. Accepted native oracle spans: **1,715,457 bytes**, including **127,128 bytes** newly accepted. Audited relocation operands are treated separately by RELOCATION_MATCH. The inherited baseline was not collectively rebuilt/retested. The **40 worker READY rows are not accepted**. Fixtures are per-function checks, not a whole-game integration test.

## Apply to a clean baseline checkout

Prefer a fresh checkout or a backup of Claude's existing repository, so local edits are preserved. If a fresh checkout is needed, the existing public repository supplies the unchanged baseline:

```sh
git clone --branch FullDecomp https://github.com/JeremyC57/FableDecomp.git FableDecomp
git -C FableDecomp checkout fbfaf9f904bb1ab4a47ad163c96d5aed3068a030
```

From the extracted ZIP directory, with `FableDecomp` pointing to that clean baseline:

```sh
tar -xf fulldecomp-update.tar.xz -C FableDecomp
tar -xf compiled-artifacts.tar.xz -C FableDecomp
python apply_retired_paths.py FableDecomp
```

Use `python3` or `py -3` if that is your Python command. The helper checks all six retired paths against their exact baseline Git blob hashes before removing any; it refuses modified contents. It does not alter any other files. These removals matter because renamed canonical files and fixtures must not coexist under obsolete names.

No `.git` directory or commit history is imported. Git HEAD stays at the baseline until the receiving maintainer commits the updated files. The source checkpoint identifier above documents provenance, not a claim that a new Git commit was transferred.

Detailed evidence restores under `work/claude_handoff/`, `work/codex-lane/` and other original repository-relative paths. The source archive includes `_handoff_update_metadata/INCLUDED_SOURCE_FILES.tsv`, `UNCHANGED_BASELINE_FILES.tsv`, changed-path and deleted-path lists, and the update scope. The compiled archive includes its full artifact manifest. External installed Wine/VC7.1 distributions, runtime prefixes and external game/debug inputs still require the authorized local setup described in the handoff.

## Authority and verification

This `00_READ_FIRST.md` controls the packaging and restore procedure. The detailed reports originated in the earlier full-checkout package, so references there to `FullDecomp-history.bundle` or `fulldecomp-text.tar.gz` describe that superseded packaging. Use the two `.tar.xz` files here and the baseline checkout instead. Historical worker handoffs remain evidence; they do not supersede the final handoff or reset attempt counts.

`SHA256SUMS` covers this ZIP's files except the checksum list itself. `UPDATE_SOURCE_VERIFICATION.json` records per-file source verification and changed-path coverage; `compiled-equivalence.json` proves exact compiled TAR payload equivalence. `RESTORE_VERIFICATION.json` records a fresh baseline checkout plus archive restore: the resulting complete tracked Git tree exactly matches the final source checkpoint, including file contents, names, modes and retirements. The retired-path helper was also tested to refuse modified local files. Original local-variable names were not recovered where supporting evidence is absent; consult the name report rather than interpreting generated aliases as original names.
