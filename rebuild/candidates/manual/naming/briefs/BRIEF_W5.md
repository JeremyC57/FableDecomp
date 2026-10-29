# Naming pass, wave 5 (one agent; a chunk-authoring agent shares the lane)

Landed (HEAD a65629d3f): operator=, waves 1, 2A, 2B, 3, LowerBound, 4A, 4B. Records:
`rebuild/candidates/manual/naming/2026-10-2*`. Workspaces: `naming/w4a/`, `naming/w4b/` (DEFERRED.tsv, scripts, tools).
Owner decision (2026-10-29): wide text is `unsigned short` (`G`); never `/Zc:wchar_t`.

## Scope, in this order (stop at ~500 edited rows)
1. `naming/w4b/DEFERRED.tsv` LowerBound (453): 337 stand-in models under other class names (CTCMapRemaining,
   InterfaceMap, CTCMapPair, ...): a per-model run of `fam_lb4.py` (its `LB4_MODELS` idea); the 70 that already model
   CVectorMap -- check they emit the final spelling; the out-of-line / extra-member shapes if cheap.
2. `naming/w4b/DEFERRED.tsv` uimgr (19), Get (11), CBase (14), gpd (8), 0x00993CA0 (6), 00699560.
3. `naming/w4a/DEFERRED.tsv` + HEADERS.md: `rebuild/include/fable_string.h` (1,269 includers) lacks
   `GetAsCharArray`, `Sprintf`, `FindCharPos`, and `AllocStringData` should be protected -- only if you recheck EVERY
   includer (a byte change or compile failure in any = drop it); the restructuring rows and own-identity re-matches
   (0x00404280 / 0x0040F0A0 STLport operator==, 0x00993F00 CDataOutputStream::WriteNullTerminatedWString).

## Rules and hand-back
As `naming/BRIEF_W4.md` (idempotent scripts + verified file lists, BINDINGS.tsv with RETIRE / KEEP / RETARGET and
"not yet bound" notes, LABELS.tsv with proven names only, DEFERRED.tsv, recheck logs; final message = report). Work only
in `naming/w5/`. Copy files from the CURRENT repo; before the final recheck re-copy and re-apply. Never run git; never
write under /home/user/FableDecomp; never kill a process; no recursive find/glob over `/` or the whole scratchpad.
</content>
</invoke>
<invoke name="Agent">
<parameter name="description">Naming wave 5 (Opus high)</parameter>
<parameter name="subagent_type">general-purpose</parameter>
<parameter name="model">opus</parameter>
<parameter name="effort">high</parameter>
<parameter name="run_in_background">true</parameter>
<parameter name="prompt">You are a FableDecomp naming-pass agent for wave 5. FableDecomp is a byte-exact VC7.1 reconstruction of retail Fable.exe; landed C++ rows live in /home/user/FableDecomp/rebuild/src/compiled and rebuild/tests. Owner rule (2026-10-28), "names right the first time": exact PDB spellings, one retail address = one mangled name. Owner decision 2026-10-29: wide text stays unsigned short (G form), never /Zc:wchar_t.

Read in full first:
1. /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/naming/BRIEF_W5.md -- your task, scope order, rules and hand-back. Work in naming/w5/.
2. naming/BRIEF_W4.md, naming/BRIEF_W2.md and naming/BRIEF.md (verification method, transform-script convention), naming/DECISIONS.tsv.
3. naming/w4b/ (DEFERRED.tsv, fam_lb4.py and the other scripts, tools/, lb_rest.log, mkbind.py) and naming/w4a/ (DEFERRED.tsv, HEADERS.md, w4lib.py) -- working examples to reuse (copy, do not modify in place).
4. /home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md section "Owner rule, 2026-10-28".

Hard rules: never run git (not even read-only); never write under /home/user/FableDecomp; never kill a process; work only in naming/w5/; no recursive find/glob over / or the whole scratchpad. A chunk-authoring agent (w22/agent561) shares the Wine lane.

Every edited row must recheck MATCH/RELOCATION_MATCH with test PASS (dry run --recheck-landed, never --land). Final message = report: rows per family, scripts and file lists, header changes (every includer rechecked), bindings, labels, reverts and why, what is left.</parameter>
</invoke>
