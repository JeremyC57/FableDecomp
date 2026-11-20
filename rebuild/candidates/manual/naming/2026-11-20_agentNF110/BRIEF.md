# Name-fix agent NF110 (2026-11-19, round 10): search tools: the GetPFirst / GetPBest rows of GetAllThingMapsInArea

Owner (2026-11-19): "start the 3 naming agents on the naming queue". This account (A, branch FullDecomp) does naming while Account B
(branch FullDecomp-B) does authoring: read `/home/user/FableDecomp/docs/COORDINATION.md` first. Agents NF108, NF109 run beside you on disjoint slices.

Read first (all under /home/user/FableDecomp, read-only):
- `rebuild/candidates/manual/naming/2026-11-17_agentNF92/` BRIEF.md, LESSONS.md, NOTES.md (the last round; your brief is that brief adapted);
- `rebuild/candidates/manual/naming/2026-11-16_agentNF44/BRIEF.md` sections "Kinds of entry", "Lessons of earlier passes", "Disk",
  "Round-3 / Round-2 / Round-1 lessons", "Per entry", "Hard rules", "Lane", "Hand-back". They apply unchanged EXCEPT where this brief
  overrides them (Lane tools and the GameCODA rule below);
- LESSONS.md of `2026-11-17_agentNF90`, `_agentNF89`, `_agentNF88`;
- `docs/pipeline/GOTCHAS.md`; `tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md`; `docs/pipeline/ASM_RULES.md`.
- The batch README behind an entry's tag: `land1119<x>` = `rebuild/candidates/manual/batches/2026-11-19_*<x>*/README.md` (+ its
  NAME_FIXES.tsv); `land1118eg` / `land1117fu` / `land1118h1` = the 11-17 / 11-18 landing READMEs that queued them (grep the tag in docs/ and batches/).

## Your slice ($S/w22/agentNF110/)
- `ENTRIES.tsv`: your open queue lines (q id = queue line number - 1; count them yourself). **`REFS.tsv` is the binder list:** for each
  entry address, every landed row whose retail body calls / jumps to / holds that address (a Fable.exe scan,
  `python3 $S/w22/nftools/refs.py`). `WORKLIST.tsv` (from reloc_bindings.tsv) is partial: that table keeps ONE witness row per symbol.
  Data reached at an offset (+4 ...) and virtual calls are not in REFS: grep sources for those. `rows.txt`: rows locked as NF110 in
  `$S/w22/nflocks/` (self rows + WORKLIST binders; claim REFS rows before you touch them). `HELD.txt`: entries that touch a row
  Account B holds: record them `deferred: held by acctB` and do not touch those rows.
- The land1119 entries (q11350+) were copied from today's batch NAME_FIXES.tsv files whose formats varied; a few carry `(raw ...)`
  evidence with no separate correct_symbol column. Read the source batch file and settle what the claim is before acting.
- Some entry addresses are not landed rows (catalog-only or parked). For those there is no self row to port: fix the catalog name
  if two sources prove it (put it in NAME_FIXES.tsv of your hand-back with address, correct_symbol, evidence, wrong spelling) and respell
  any landed binders. Never author a new byte match: that is Account B's work (record `blocked: authoring`).
- Rows that need a row outside rows.txt: claim it first
  (`mkdir $S/w22/nflocks/<addr> 2>/dev/null && echo NF110 > $S/w22/nflocks/<addr>/owner; true`). If it is held, record `deferred: held by <owner>`.
  Any row listed as `handed off` or `reserved (acctB` in `rebuild/candidates/manual/UNMATCHED_ROWS.tsv` is Account B's: never touch it.

Do the entries in the order your specifics give (else queue order), one entry to a terminal state before the next.

## Rules carried from NF92 (unchanged)
- **Every row ends `ported`, `not-a-binder` or `blocked`** (no `carry`). An entry ends done, done-already, rejected, blocked (reason, rows
  left) or deferred.
- **Two sources** for every claim; a claim that does not hold goes to REJECTED.tsv with the reason. Do not re-probe the whole slice.
- **Budget (round 10, advisor):** no family grade cap. Count failed forms per row: after 3 failed forms on one row mark it blocked with
  the exact compiler error or diff. A row that models a dozen classes locally is blocked/port after 3 failed forms; name the classes that must move first.
- **Transformer first, then batch:** prove a shared rewrite by hand on 2-3 rows, script it, grade the rest in one payload of 20-30 rows.
- **Static asserts:** never move a `FABLE_STATIC_ASSERT` to another line. After editing an overlay header run
  `python3 /home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/lane_merge/pin_asserts.py /home/user/FableDecomp/rebuild/include $S/w22/agentNF110/hdr <header>`
  and say in hdr/README.md what it pinned. Base copies of overlaid headers go in `orig/` (outside hdr/). Edit the existing hdr/ copy;
  never re-create it with a fresh cp.
- **Overlay headers carry `#ifdef` opt-ins only**, member-specific macro names (`FABLE_RETAIL_<CLASS>_<MEMBER>`); the default path must be
  token-identical; say so in hdr/README.md. The other two agents may add opt-ins to the same headers: read their overlays
  (`$S/w22/agentNF9x/hdr/`, read-only) before adding a member so it is never declared twice. A RENAME of an existing shared-header member
  is a coordinated `header` item: do not apply it; record it in NOTES.md as "needs header claim" with the rows it unblocks.
- No new local model of a class that has a shared header, in a row or a test (renaming a local stand-in to the real class name counts:
  the nf95b landing held 29 rows for it); no cast addresses; no `__asm` / `naked` / `_emit` / `register` / bare
  `__forceinline`, `volatile` only where retail used it; no `#error` probes; no `Unproven` / `VirtualNN_` / `FUN_` placeholder names.
- If you change a row that EH funclets include, regrade those funclets. A self rename moves the parent's file: list such funclets in NOTES.
- **Write STATUS.tsv and APPLIED.tsv after EVERY row.** Containers restart without warning; the first grade after one can time out while
  wineserver starts (retry once).
- Count before you report: every number comes from your files.

## Changed since NF92 (2026-11-19)
- **GameCODA:** the GameCODA 2.10 SDK was found legitimately on archive.org (`docs/BUILDING.md` "GameCODA 2.10 audio SDK";
  `docs/pipeline/LIBRARY_SIGNATURES.md`). Its CodeView libs (`$S/sdk/gamecoda/tree/CODA/.../CODAApiDbg.lib`, read-only) are now a valid
  name source. This supersedes NF44's Round-2 "GameCODA claims are rejected" lesson. A CODA name still needs a second source (the body,
  the callers, vtable slots). Never copy SDK files into your hand-back.
- **Unreachable end:** `__assume(0)` only as an `_MSC_VER`-guarded unreachable-end marker with an `#elif defined(__GNUC__)
  __builtin_unreachable();` twin (EXTRA_RULES).
- **Header users:** after any overlay change, list the landed includers of that header
  (`grep -rlE '<header name>' /home/user/FableDecomp/rebuild/src/compiled`) and say in hdr/README.md that your opt-ins change none of them.

## Lane (overrides NF44's Lane section: the old $S/w22/nftools set is gone; use these)
Start every Bash call with:

    export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh; export S; cd /home/user/FableDecomp

- **Callers:** `python3 $S/w22/nftools/callers.py <addr>` (catalog name, self source, landed binders and the symbol each binds).
- **Grade one row:** `S=$S/w22/agentNF110/pS timeout 240 python3 $S/tools/q.py <addr> <file.cpp> '<full mangled leaf>' [--flags ...]`
  (flags from the row's `// Flags:` line, else the CompilerFlags of its entry in rebuild/build_candidates.ps1 minus /O2 /Oy /W3; add
  `FABLE_HDR_OVERLAY=$S/w22/agentNF110/hdr` when you use your overlay). `python3 $S/w22/nftools/xref.py <addr> <obj> '<leaf>'` shows the
  symbol each relocation binds.
- **Grade + test (required for every row you hand back):**
  `python3 $S/w22/nftools/mkpay.py $S/w22/agentNF110/work/b1 $S/w22/agentNF110 <addr> <addr> ...` (takes rows/<addr>.cpp and
  rows/<addr>_test.cpp when present), then
  `LANDVERIFY_WORK=$S/w22/agentNF110/wd1 FABLE_HDR_OVERLAY=$S/w22/agentNF110/hdr timeout 1800 python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py $S/w22/agentNF110/work/b1_payload.json $S/w22/agentNF110/work/b1_oracle.tsv --recheck-landed > $S/w22/agentNF110/work/b1.log 2>&1; true`
  Must say MATCH or RELOCATION_MATCH and PASS. Run long ones in the background and poll the log; a fatal cl error under Wine can spin.
- **Binding pairs:** tools/decomp_pipeline/reloc_identity.py `check()`.
- **Disk:** delete each `wd*` directory once its log is saved; never copy large trees. On "no space left", stop and report.
- One compile at a time, always inside `timeout`. Every Bash call must exit 0 (`; true`). Quoted heredocs (`<<'EOF'`) for any text with
  backticks. Absolute paths only. Never run git (not even `git status`). Never write under /home/user/FableDecomp. Never land anything.

## Hand-back (in $S/w22/agentNF110/)
- rows/ (finished rows only; blocked drafts stay in work/); hdr/ and hdr/README.md;
- RENAMES.tsv: section 1 header exactly `address\tmodule\tname\tmangled_leaf\told_catalog_name\told_leaf\tevidence` (0x-prefixed
  addresses; `name` without `Class::`), then a `# Section 2` line with caller respellings (caller row, old symbol, new symbol, callee address);
- STATUS.tsv (address, change, grade, test, grades_used, entries, note: one line per row you touched or listed);
- APPLIED.tsv (one line per entry in ENTRIES.tsv: entry, address, status, binders respelled, note);
- NAME_FIXES.tsv (address, correct_symbol, evidence, wrong_spellings_seen) for proven fixes outside your rows (catalog-only names, rows
  held by others, new findings);
- REJECTED.tsv, NOTES.md (mark descriptive names), LESSONS.md;
- recheck_final.log: one vland --recheck-landed run over every changed row.

**Long slice.** If your context gets heavy before the slice is done, finish the current row, make sure every file above reflects what
is finished (APPLIED lines `open` for entries not started), run recheck_final.log over the rows changed so far, and report: the
coordinator lands what is done and resumes you. Report briefly: entries done / done-already / rejected / blocked / deferred / open; rows
ported, not-a-binder, blocked; header opt-ins; rule slips.

## Rules since round 9 (advisor decisions, owner delegated; apply them)
- **No new name splits.** A hand-back may leave an existing split (one address bound under two spellings by landed rows) but must never
  create one. If you rename an address (self leaf, callee or data), EVERY landed row in REFS.tsv for it must end on the new spelling in the
  same hand-back, or the rename waits. The coordinator gates each landing with an exe scan of all referencing rows (`$S/w22/nftools/gate.py`).
  A family (rows that must move together to avoid a split) hands back whole or stays blocked.
- **Same-TU strong callee copies** are legitimate when retail's caller keeps a register live across the call; the copy uses the PDB spelling,
  the owner's body verbatim, and `// Fidelity: same-TU strong definition of 0x... (retail TU-mate)` (GOTCHAS nf96a line).
- **In-place respelling** inside a local model the landed row already had is accepted; a NEW local model (or a stand-in renamed to the real
  class name) is not. A partial shared-header mode (e.g. `FABLE_RETAIL_NTHINGMAP_LAYERINFO_ONLY`, which declares only `NThingMap::CLayerInfo` and
  `LayerInfo[]`) is accepted when it lets a row drop a local declaration.
- `__assume(0)` only in the guarded EXTRA_RULES form with an `// Inferred:` line.

## NF110 specifics: the GetPFirst / GetPBest rows of GetAllThingMapsInArea (q11280 / q11283, the last group)
NF103 / NF106 / NF107 ported the other binders (`batches/2026-11-19_landing_nf103`, `_nf106`, `_nf107`); their NOTES are in `work/`
(NF106 NOTES "Rows left" says what this group needs; NF106's blocked drafts are in `work/try_nf106/`). You own engine/CThingSearchTools.h,
engine/NThingMap_CThingMap.h, engine/CThingDeadCreature.h, engine/CThingMarker.h this round (opt-ins only; Account B's landed 0x0063F4B0 and every
landed includer must compile unchanged).
Your 16 rows (rows.txt):
- **GetPFirstThingInRoughArea (8):** 0x007313B0, 0x007512E0, 0x0079A280, 0x007FCEF0, 0x007FD110, 0x007FE730, 0x0096F260, 0x00987430. The header's
  FIRSTROUGH_BODY differs (two frame slots, one SIB order). Add a FITTED first-rough body opt-in to CThingSearchTools.h (the CELL_BYTEOFFSET-style
  cell expression + the guarded y loop of the local models), or move the 0x009871C0 EGOD body into the header as an opt-in; then port the 8.
  0x0096F260 / 0x00987430 have no PDB instance for their local spelling: name the instantiation from its template arguments.
- **GetPBestThingInRoughArea (6):** 0x0079A050, 0x007F5D20, 0x009147E0, 0x00937690, 0x0093CDB0, 0x00942390. BESTROUGH_BODY is not their shape
  (0x009147E0 DIFFER 558v567); several local filters take `const CThing*` where the shared CFilter_Bind2 passes a reference (0x00942390
  SRC_FAIL): a fitted body opt-in and a pointer-taking filter-binder opt-in. 0x0093CDB0's local T (CThingMask10007) has no PDB name.
- **0x006CE300, 0x0080ECC0:** see NF106 / NF107 NOTES.
One body opt-in per shape; prove it on 2 rows, then script the rest. The self renames wait until every REFS row binds the new spelling.
- **Self-check before you hand back (required):** `python3 $S/w22/nftools/selfcheck.py $S/w22/agentNF110 > $S/w22/agentNF110/selfcheck.log`;
  it must end `selfcheck: 0 problem(s)` (or NOTES explains each line).

## Update 2026-11-20 (coordinator)
nf107 is landed (commit 14ae8c3b0f): engine/CThingSearchTools.h in the repo now carries NF107's BESTROUGH_THISEND opt-in (+ byte-offset cell
branch), and CThingMarker.h / CThingDeadCreature.h / CTCDExperienceOrb.h / CTCCarrying.h carry its STATTYPEFLAGS-style opt-ins. Copy your
orig/ and hdr/ bases from the repo NOW (not from older agent overlays). NF107's NOTES / STATUS are in work/NF107_NOTES.md, work/NF107_STATUS.tsv;
its GetPBestThingInRoughArea forms (0x007F7340, 0x007F70D0, 0x00937420: BESTROUGH_BODY + BESTROUGH_THISEND + the T type-flags opt-in) are the
starting point for your 6 GetPBest rows.
