# Brief: port matched rows from another lane onto today's shared headers (staging only)

You are helping land byte-matched C++ rows of the Fable decompilation (VC7.1). Another lane authored and verified these rows against
its OWN copies of the shared headers. Those copies are stale: this branch changed the same headers meanwhile. Your job is to make each
row of your group compile, byte-match and pass its test against TODAY's headers plus the smallest additive header changes it needs.
You only STAGE the work in a private workspace; the coordinator lands it. Be fast but accurate.

## Hard limits
- Read-only for you: everything under /home/user/FableDecomp. Never write there, never run a git command that changes state, never land.
- Write only in your workspace `$WS` and in the `fix/` folder of YOUR rows (`$S/w22/acctb/<addr>/fix/`). Do not touch other groups' rows.
- Every Bash command starts with the environment prefix below and ends with `; true`. Never pipe a grader into `head`.
- No `__asm`, `naked`, `_emit`, `register`, no bare `__forceinline` in row sources or tests (the macro `FABLE_FORCEINLINE` is the
  established spelling in headers); `volatile` only where the row already carries a `// Retail-volatile:` line.
- Names: keep retail PDB spellings; never invent a name to make something compile. If the repo already has a spelling for a callee,
  member or global, use the repo's.

## Environment prefix
```
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S; cd /home/user/FableDecomp; unset FABLE_HDR_OVERLAY; export WS=<your workspace>
```

## What exists per row: `$S/w22/acctb/<addr>/`
- `payload.json` (the row: `result.authored[0].source_cpp`, `test_cpp`, `module`, `name`, optional `flags`), `src.cpp` (the same source),
  `oracle.tsv` (retail bytes), `meta.json` (leaf symbol, flags).
- `hdr/`: the header overlay the row was authored against: full copies of `rebuild/include` headers at an OLDER state plus the row's
  additions (`#ifdef FABLE_RETAIL_*` opt-ins, new headers). Other rows of the same original folder share the same overlay, so it also
  contains additions this row does not need.
- Background on the row (dossier, notes): the other lane's folder, see `rebuild/candidates/manual/pending/2026-11-13_acctB_FINAL/INDEX.tsv`
  (columns best_source, notes, open_items).

## Your workspace `$WS`
- `$WS/inc`: your PRIVATE copy of `rebuild/include`. The staging driver changes headers only there. It accumulates the header changes of
  the rows you have staged, so later rows build on earlier ones. Work through your rows in the given order.
- `$WS/stage/rows/<addr>.cpp|.test.cpp|.json`: written by the driver when a row is staged (this is your deliverable, with `$WS/inc`).

## The staging driver
`ACCTB_INC=$WS/inc ACCTB_STAGE=$WS/stage python3 $S/tools/acctb_stage.py <run-name> <addr> [<addr> ...]`
(use a new run-name each time, e.g. r1, r2...; add `ACCTB_REDO=1` to restage a row). Per row it:
1. finds the headers the row really includes, merges the overlay's additions into the current `$WS/inc` header (3-way against the repo
   version the overlay came from; a block both sides added is kept twice; an opt-in name both lanes introduced is dropped if its content
   is already there, grown if the incoming one is a superset, else renamed `<NAME>_B` in the header and in the row);
2. checks the merged headers: balanced `#if`s, the default path (no macro defined) preprocesses exactly as the REPO header, and for every
   existing opt-in whose meaning changed it collects the landed files that use it;
3. copies the merged headers into `$WS/inc`, compiles the row (must be MATCH or RELOCATION_MATCH), runs its test (must PASS), and
   re-verifies the collected landed files against `$WS/inc` (all must still match);
4. on success prints `STAGED`; otherwise it restores `$WS/inc` and prints the reason:
   `HDR_DEFAULT_CHANGES <header>`, `HDR_UNBALANCED`, `HDR_STRUCTURE`, `HDR_USED_MACRO_DEFINED_BY_HEADER`, `LANDED_USERS_BREAK ... (log path)`,
   `ROW_DOES_NOT_MATCH` (with compiler errors or the differing instructions), `TEST_FAILS`, `TOKENS`.
The automatic merge has already failed for every row you are given. You fix that by hand:
- **Hand-merged header:** write the complete merged header to `$S/w22/acctb/<addr>/fix/<relative header path>` (for example
  `fix/engine/CMatrix3x4.h`). Start from the CURRENT `$WS/inc/<header>` and add what the row needs from `hdr/<header>`. The driver then
  uses your file as the merge result for that header (all checks still apply).
- **Edited row:** `fix/src.cpp` and / or `fix/test.cpp` replace the row's source / test (start from `src.cpp` / the payload's test).
- Viewer: `python3 $S/tools/acctb_show.py <addr> [header-substring]` prints, per header, the 3-way merge of the overlay against the REPO
  header with conflict markers and the added lines (`N=200` env for more lines). Use plain `diff` against `$WS/inc` as well.
- Single compile without the driver: `W=<scratch dir> FABLE_HDR_OVERLAY=<include dir> python3 $S/tools/fg.py <addr> '<leaf>' <src.cpp> [--flags ...]`
  (an overlay directory wins over the repo headers).

## How to merge (the rules the coordinator applies)
1. Additive only: new declarations, bodies and members go under `#ifdef FABLE_RETAIL_<...>` opt-ins. The default path of a header and
   the meaning of an opt-in that landed files use must not change (the driver's recheck is the proof when you must touch one).
2. Both lanes used the same opt-in name: equivalent content -> keep the repo's block and drop the incoming one; different content ->
   rename the incoming one (suffix `_B`, `_B2`...) in the header AND in the row source / test.
3. Both lanes changed the same line (for example each extended an `#if defined(A)` condition, or each retyped the same member under its
   own opt-in): write ONE line that serves both (`#if defined(A) || defined(B)`), never two competing copies.
4. Both lanes wrote the same header independently: the repo's header stands. Add what the row needs to it as an opt-in, and respell the
   row to the repo's names where they differ (same retail meaning only).
5. An incoming block that wraps existing code in `#ifdef X ... #else <existing> #endif` is fine as long as the default path is unchanged.
6. Change the row source only as far as the port needs (opt-in renames, includes, the repo's spellings). It must still byte-match.
7. If a row only matches with something that looks wrong (a stand-in body for another retail function, a local re-model of a class that
   has a shared header, an invented name), do not stage it: report it.

## When you finish
1. `python3 $S/tools/acctb_stage_verify.py $WS` re-verifies every staged row against the final `$WS/inc`; fix any `REGRESSED` row.
2. Write `$WS/REPORT.md`: per row STAGED or not (why, what you tried); each hand-merged header with one line on what you did; every
   opt-in rename (old -> new); every source change beyond renames; anything the coordinator should review.
3. Your final message: the same summary, short. Give at most about eight real attempts to a row; if it still fails, record why and go on.
