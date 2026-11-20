# Name-fix agent NF130 (2026-11-20, round 10): PRIORITY reconciliation, PRIORITY slice 7, fourth remainder

Owner (2026-11-19): "start the 3 naming agents on the naming queue". This account (A, branch FullDecomp) does naming while Account B
(branch FullDecomp-B) does authoring: read `/home/user/FableDecomp/docs/COORDINATION.md` first. Agents NF127 (PRIORITY slice 9) and NF129 (PRIORITY slice 10) run beside you on disjoint slices.

Read first (all under /home/user/FableDecomp, read-only):
- `rebuild/candidates/manual/naming/2026-11-17_agentNF92/` BRIEF.md, LESSONS.md, NOTES.md (the last round; your brief is that brief adapted);
- `rebuild/candidates/manual/naming/2026-11-16_agentNF44/BRIEF.md` sections "Kinds of entry", "Lessons of earlier passes", "Disk",
  "Round-3 / Round-2 / Round-1 lessons", "Per entry", "Hard rules", "Lane", "Hand-back". They apply unchanged EXCEPT where this brief
  overrides them (Lane tools and the GameCODA rule below);
- LESSONS.md of `2026-11-17_agentNF90`, `_agentNF89`, `_agentNF88`;
- `docs/pipeline/GOTCHAS.md`; `tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md`; `docs/pipeline/ASM_RULES.md`.
- The batch README behind an entry's tag: `land1119<x>` = `rebuild/candidates/manual/batches/2026-11-19_*<x>*/README.md` (+ its
  NAME_FIXES.tsv); `land1118eg` / `land1117fu` / `land1118h1` = the 11-17 / 11-18 landing READMEs that queued them (grep the tag in docs/ and batches/).

## Your slice ($S/w22/agentNF130/)
- `ENTRIES.tsv`: your open queue lines (q id = queue line number - 1; count them yourself). **`REFS.tsv` is the binder list:** for each
  entry address, every landed row whose retail body calls / jumps to / holds that address (a Fable.exe scan,
  `python3 $S/w22/nftools/refs.py`). `WORKLIST.tsv` (from reloc_bindings.tsv) is partial: that table keeps ONE witness row per symbol.
  Data reached at an offset (+4 ...) and virtual calls are not in REFS: grep sources for those. `rows.txt`: rows locked as NF130 in
  `$S/w22/nflocks/` (self rows + WORKLIST binders; claim REFS rows before you touch them). `HELD.txt`: entries that touch a row
  Account B holds: record them `deferred: held by acctB` and do not touch those rows.
- The land1119 entries (q11350+) were copied from today's batch NAME_FIXES.tsv files whose formats varied; a few carry `(raw ...)`
  evidence with no separate correct_symbol column. Read the source batch file and settle what the claim is before acting.
- Some entry addresses are not landed rows (catalog-only or parked). For those there is no self row to port: fix the catalog name
  if two sources prove it (put it in NAME_FIXES.tsv of your hand-back with address, correct_symbol, evidence, wrong spelling) and respell
  any landed binders. Never author a new byte match: that is Account B's work (record `blocked: authoring`).
- Rows that need a row outside rows.txt: claim it first
  (`mkdir $S/w22/nflocks/<addr> 2>/dev/null && echo NF130 > $S/w22/nflocks/<addr>/owner; true`). If it is held, record `deferred: held by <owner>`.
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
  `python3 /home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/lane_merge/pin_asserts.py /home/user/FableDecomp/rebuild/include $S/w22/agentNF130/hdr <header>`
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
- **Grade one row:** `S=$S/w22/agentNF130/pS timeout 240 python3 $S/tools/q.py <addr> <file.cpp> '<full mangled leaf>' [--flags ...]`
  (flags from the row's `// Flags:` line, else the CompilerFlags of its entry in rebuild/build_candidates.ps1 minus /O2 /Oy /W3; add
  `FABLE_HDR_OVERLAY=$S/w22/agentNF130/hdr` when you use your overlay). `python3 $S/w22/nftools/xref.py <addr> <obj> '<leaf>'` shows the
  symbol each relocation binds.
- **Grade + test (required for every row you hand back):**
  `python3 $S/w22/nftools/mkpay.py $S/w22/agentNF130/work/b1 $S/w22/agentNF130 <addr> <addr> ...` (takes rows/<addr>.cpp and
  rows/<addr>_test.cpp when present), then
  `LANDVERIFY_WORK=$S/w22/agentNF130/wd1 FABLE_HDR_OVERLAY=$S/w22/agentNF130/hdr timeout 1800 python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py $S/w22/agentNF130/work/b1_payload.json $S/w22/agentNF130/work/b1_oracle.tsv --recheck-landed > $S/w22/agentNF130/work/b1.log 2>&1; true`
  Must say MATCH or RELOCATION_MATCH and PASS. Run long ones in the background and poll the log; a fatal cl error under Wine can spin.
- **Binding pairs:** tools/decomp_pipeline/reloc_identity.py `check()`.
- **Disk:** delete each `wd*` directory once its log is saved; never copy large trees. On "no space left", stop and report.
- One compile at a time, always inside `timeout`. Every Bash call must exit 0 (`; true`). Quoted heredocs (`<<'EOF'`) for any text with
  backticks. Absolute paths only. Never run git (not even `git status`). Never write under /home/user/FableDecomp. Never land anything.

## Hand-back (in $S/w22/agentNF130/)
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

## NF130 specifics: PRIORITY slice 7, fourth remainder (8 entries)
History: NF120, NF124, NF126 (all landed) and NF128 worked this slice. NF128's 5 finished rows (HELD.txt) and its header opt-ins are being
landed now and appear in the repo working tree (`batches/2026-11-20_landing_nf128` when committed). Notes: `work/NF128_round1/` (APPLIED,
STATUS, NOTES with the q6453 census, RENAMES, NAME_FIXES, hdr/) and the earlier rounds' folders. Read NF128's NOTES and APPLIED first.
If you need one of NF128's opt-ins before they appear in the repo tree, copy that header from work/NF128_round1/hdr/ into your hdr/ and say so in
hdr/README.md (rebase on the repo header once nf128 lands).
Entries (ENTRIES.tsv, 8):
- **First: 0x008E9A30 + 0x0073DFF0 (q6394, q6395 and the last q6454 binder).** NF128's finished 0x008E9A30 (`work/NF128_round1/008e9a30_held.cpp`,
  RELOCATION_MATCH, needs NF128's opt-in FABLE_RETAIL_CANIMFILTEROFGROUP_SETCURRENTGROUPNAME_INLINE in engine/CAnimFilterOfGroup.h, which
  lands with nf128) emits a COMDAT of the inline `CAnimFilterOfGroup::SetCurrentGroupName`, which the landed owner 0x0073DFF0 (yours now)
  defines strongly: a link of the two fails LNK2005. Fix it the nf88 way (docs/pipeline/GOTCHAS.md nf80 / nf86 / nf88 lines): 0x0073DFF0
  takes the inline body's address in an anonymous-namespace helper (`// Fidelity: scaffolding, not retail`) so it emits a pick-any COMDAT like
  every includer; both rows must still grade; then PROVE it by linking the two objects with `LINK=` cleared (env.sh sets
  `LINK=bufferoverflowu.lib`): LNK2005 before, none after. Save the link logs in work/ and hand back 0x008E9A30 + 0x0073DFF0 together (both or
  neither). 0x008E9A30's symbol now carries the NCreatureMode namespace: its RENAMES section-1 module column must be the namespaced class
  (e.g. `NCreatureMode::CModeStrafe`) so the build entry and file name follow at landing.
- q6453 / q6455 0x0073A680 (one unit): 12 of 18 referencing rows bind the claim; left 0x00862EC0, 0x008D7630, 0x008DC900, 0x008DCB60,
  0x008E7B90, 0x008E7CF0.
- q6408 0x0073ACC0 (25 binders; tools/mb.py + the FABLE_RETAIL_CTCCREATUREMODEMANAGER_ADDMODEPOINTER opt-in), q4802 0x0075B8D0 (NF126 census:
  0x00B21DD0 / 0x00B21E90 blocked, try `/d2inlS100 /d2inlT200` flags on 0x00B21E90 first; 7 rows not reached), q4747 0x00733480 (0x0064BE45 is a
  196-line full port).
- Order: 0x008E9A30 + 0x0073DFF0, then q6453 / q6455, q6408, q4802, q4747.
- **Local class models (advisor decision, owner delegated, 2026-11-20):** a "base-list local model" is a row-local definition
  `class|struct X [: bases] {` of a class X that HAS a shared header (verify_and_land.py `_DEF_RE` + `shared_header_redefinitions`); never add
  one. Before defining any class a row did not define at HEAD, grep `class X` / `struct X` across ALL of rebuild/include (mode classes live in
  files named otherwise, e.g. NCreatureMode_CModeSitFloor.h holds CModeSitBase): any hit means use that header. A new local model is allowed
  only when all three hold: no header for the class exists anywhere under rebuild/include; a landed sibling row already uses the same shape;
  its base comes from a shared header. List each such row in NOTES.
- Leaf-only self renames of rows outside your entries are accepted when both PDBs agree and the exe scan finds no landed row referencing
  the address; list them in NOTES.
- A new PDB spelling replacing a stand-in on a callee is accepted when the address's spelling count does not grow; list each in NOTES.
- **Flags:** if a row needs flags its build entry lacks, put them in a `// Flags:` line AND list the row in NOTES "Flags lines".
- **cpp precheck** before every grade (a missing include spins cl); grep your overlay first.
- RENAMES section 2 caller column: 0x-prefixed addresses.
- **Context:** hand back early; after every 2-3 finished entries make every hand-back file current.
- **Self-check before you hand back (required):** `python3 $S/w22/nftools/selfcheck.py $S/w22/agentNF130 > $S/w22/agentNF130/selfcheck.log`.
- Copy your orig/ and hdr/ bases from the repo working tree when you start. Read `$S/w22/agentNF127/hdr/` and `$S/w22/agentNF129/hdr/`
  (read-only) before adding an opt-in to the same header.
- Claim a lock with plain `mkdir` (never `mkdir -p`, which does not refuse an existing lock).
- tools/ holds helper scripts earlier agents wrote; read one before you use it (st.py reads stdin when given `-`).
