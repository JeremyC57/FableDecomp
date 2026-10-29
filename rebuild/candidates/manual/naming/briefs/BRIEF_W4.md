# Naming pass, wave 4 (two agents)

Landed (committed, HEAD 4ce809816): operator= (2,107 files), wave 1 (315 rows), 2B (1,064), 2A (495), LowerBound (1,086),
wave 3 (461 + two header members). Records in the repo: `rebuild/candidates/manual/naming/2026-10-2*`. Workspaces with
scripts, DEFERRED.tsv and notes: `naming/wave1/`, `naming/w2a/`, `naming/w2b/`, `naming/w3/`, `naming/wchar/`.

OWNER DECISION (2026-10-29, supersedes 2026-10-28): wide text stays `unsigned short` (mangled `G`), no `/Zc:wchar_t` --
retail shares one STLport `uninitialized_fill_n` (0x005123A0) between `vector<unsigned short>` and the wide string. A PDB
name with `_W` lands as the same name with `_W` -> `G`. Never add `/Zc:wchar_t`.

## Agent A (`naming/w4a/`): wide-text names in the G form, and the wchar agent's non-wchar leftovers
- The 12 proven `_W` decisions in `naming/DECISIONS.tsv` (and the ones `naming/wchar/BINDINGS.tsv` respelled), applied in
  the `G` form. `naming/wchar/` has the row scan (which rows emit which wide symbol) -- reuse it, but its transform
  (`fam_wchar.py`) converts types to wchar_t: do NOT use that part.
- `naming/wchar/DEFERRED.tsv`: 35 rows that should call `GetAsCharArray()` (0x0099AF10) instead of
  `operator const unsigned short*` (0x0099AF00); `operator+=` const returns (5); `operator const ...*` const member (4);
  Sprintf (14 rows) and StartFileScan / ScanNextFile (13) identities; 0x00404280 (STLport `operator==`).

## Agent B (`naming/w4b/`): the remaining tails
- `naming/w3/DEFERRED.tsv` (148): 0x0041E5F2 models with non-CManager members (88), CBase roots (39), 0x009AD9E0
  two-argument getters (14 -- those are 0x009ADA10 / 0x009ADE10, name them right), 0x00993CA0 (6), 00699560.
- `naming/w2a/DEFERRED.tsv` leftovers: `?Get@CGameDefinitionManager@@SIAAV1@XZ` (25), `?Get@CUserProfileManager@@SIAAV1@XZ` (14).
- The ~380 non-canonical LowerBound shapes (`naming/w2b/fam_lowerbound.py` handles only the canonical model; the repo
  record is `rebuild/candidates/manual/naming/2026-10-29_lowerbound/`).
- Catalog-name corrections are coordinator work: list them in LABELS.tsv.

## Rules and hand-back (both)
As `naming/BRIEF_W2.md`: idempotent `fam_*.py` + verified `.files.txt` / `.rows.txt`, hand edits only as an overlay,
BINDINGS.tsv (RETIRE / KEEP / RETARGET, and say when a new spelling is not yet bound), LABELS.tsv, DEFERRED.tsv, recheck logs;
final message = report (you cannot write REPORT.md). Header changes only with every includer rechecked. Copy files from
the CURRENT repo; before the final recheck re-copy and re-apply. Stop at ~500 edited rows. Never run git (not even
`git log`); never write under /home/user/FableDecomp; never kill a process; no recursive find/glob over `/` or the whole
scratchpad. The two agents share the Wine lane and may touch the same files: your scripts must compose.
</content>
</invoke>
<invoke name="Agent">
<parameter name="description">Naming wave 4A: wide names in G form (Opus high)</parameter>
<parameter name="subagent_type">general-purpose</parameter>
<parameter name="model">opus</parameter>
<parameter name="effort">high</parameter>
<parameter name="run_in_background">true</parameter>
<parameter name="prompt">You are FableDecomp naming-pass agent A for wave 4. FableDecomp is a byte-exact VC7.1 reconstruction of retail Fable.exe; landed C++ rows live in /home/user/FableDecomp/rebuild/src/compiled and rebuild/tests. Owner rule (2026-10-28), "names right the first time": exact PDB spellings, one retail address = one mangled name.

Read in full first:
1. /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/naming/BRIEF_W4.md -- your task is "Agent A"; note the owner decision: wide text stays unsigned short (G form), never /Zc:wchar_t. Work in naming/w4a/.
2. naming/BRIEF_W2.md and naming/BRIEF.md (verification method, transform-script convention), naming/DECISIONS.tsv.
3. naming/wchar/EVIDENCE.md, BINDINGS.tsv, DEFERRED.tsv, tools/ (symscan etc.) -- reuse the row scans, not the wchar_t type conversion; naming/w3/ and naming/w2b/tools/ as working examples.
4. /home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md section "Owner rule, 2026-10-28".

Hard rules: never run git (not even read-only); never write under /home/user/FableDecomp; never kill a process; work only in naming/w4a/; no recursive find/glob over / or the whole scratchpad. Agent B works in naming/w4b/ at the same time and shares the Wine lane.

Every edited row must recheck MATCH/RELOCATION_MATCH with test PASS (dry run --recheck-landed, never --land). Final message = report: rows per family, scripts and file lists, bindings, labels, reverts and why, what is left.</parameter>
</invoke>
<invoke name="Agent">
<parameter name="description">Naming wave 4B: remaining tails (Opus high)</parameter>
<parameter name="subagent_type">general-purpose</parameter>
<parameter name="model">opus</parameter>
<parameter name="effort">high</parameter>
<parameter name="run_in_background">true</parameter>
<parameter name="prompt">You are FableDecomp naming-pass agent B for wave 4. FableDecomp is a byte-exact VC7.1 reconstruction of retail Fable.exe; landed C++ rows live in /home/user/FableDecomp/rebuild/src/compiled and rebuild/tests. Owner rule (2026-10-28), "names right the first time": exact PDB spellings, one retail address = one mangled name.

Read in full first:
1. /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/naming/BRIEF_W4.md -- your task is "Agent B"; note the owner decision: wide text stays unsigned short (G form), never /Zc:wchar_t. Work in naming/w4b/.
2. naming/BRIEF_W2.md and naming/BRIEF.md (verification method, transform-script convention), naming/DECISIONS.tsv.
3. naming/w3/ (DEFERRED.tsv, HEADERS.md, famlib.py, fam_*.py, cbase_role.py), naming/w2a/DEFERRED.tsv, naming/w2b/fam_lowerbound.py and tools/ -- working examples to reuse (copy, do not modify in place).
4. /home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md section "Owner rule, 2026-10-28".

Hard rules: never run git (not even read-only); never write under /home/user/FableDecomp; never kill a process; work only in naming/w4b/; no recursive find/glob over / or the whole scratchpad. Agent A works in naming/w4a/ at the same time and shares the Wine lane.

Every edited row must recheck MATCH/RELOCATION_MATCH with test PASS (dry run --recheck-landed, never --land). Final message = report: rows per family, scripts and file lists, bindings, labels, reverts and why, what is left.</parameter>
</invoke>
