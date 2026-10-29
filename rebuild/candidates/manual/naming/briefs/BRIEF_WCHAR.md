# Naming pass, wchar_t wave (owner decision 2026-10-28)

Owner decision: wide text uses the PDB spelling -- native `wchar_t` (mangled `_W`, `PB_W`, `PA_W`) compiled with
`/Zc:wchar_t`. Genuine 16-bit data (index buffers, `CArray<unsigned short>`, WORD fields) stays `unsigned short`.
Evidence and caveats: docs/journal/FINDINGS_LOG.md, entry "wchar_t check (2026-10-28 ...)" and the owner-decision entry after it.

## Scope
1. The 16 wchar_t-family decisions in `naming/DECISIONS.tsv` / `naming/wave1/DEFERRED.tsv` (reason wchar_t), e.g.
   0x00403C60 `std::__char_traits_base<wchar_t,wint_t>::compare` (`_W` / `G` for wint_t), 0x0099B800
   `??4CWideString@@QAEABV0@PB_W@Z`, 0x0099AF00 `??BCWideString@@QBEPB_WXZ`, 0x0099AF10 GetAsCharArray `PB_W`.
2. The WCHAR_FLAG rows in `naming/opeq/BLOCKED.tsv` (258 declarations, 33 source rows): CWideString operator= taking
   `const wchar_t*` / `wchar_t` that mangle `PBG` / `G` without the flag.
3. Every other landed row whose emitted symbols contain a wide-text `G` where the PDB has `_W` (CWideString members,
   `L""`-taking functions, STLport `basic_string<wchar_t>` helpers). Find them from `rebuild/oracles/reloc_bindings.tsv`
   (bound symbols containing `PBG`/`PAG`/`G@` whose target's PDB spelling has `_W`) and source tokens
   (`naming/tools/tokindex.py`). Measure first; report the count before editing beyond ~500 rows.

## How
- Per row: add `/Zc:wchar_t` to the row's flags (`CompilerFlags` in `rebuild/build_candidates.ps1`; the verifier passes a
  payload `flags` list -- see `naming/mk_reference.py`), and change wide-TEXT types from `unsigned short` /
  `typedef unsigned short wchar16` to `wchar_t` so `L""` literals still convert. Leave 16-bit data as `unsigned short`.
  Tests: compile them with the same flag if they need it (check how vland passes flags to tests).
- Deliver: an idempotent transform script `fam_wchar.py` + verified file list, a `FLAGS.tsv` (address, old flags, new
  flags) for the coordinator to apply to build_candidates.ps1, `BINDINGS.tsv` (old_symbol target new_symbol, with
  RETIRE/KEEP notes), and an `overlay/` only for rows the script cannot express.
- Every edited row must recheck MATCH / RELOCATION_MATCH with test PASS (dry `--recheck-landed`, never `--land`), with the
  new flag actually passed. Check with `objdump -t` that the emitted names are exactly the PDB `_W` spellings.
- Shared headers: `rebuild/include/fable_string.h` and engine headers may declare wide types; a header change needs every
  including row rechecked -- prefer listing it in `HEADERS.md` unless you can recheck them all.

## Rules
Never run git; never write under /home/user/FableDecomp; never kill a process; work only in `naming/wchar/`; no recursive
find/glob over `/` or the whole scratchpad. Lane: `source $S/wine/env.sh; cd /home/user/FableDecomp`
(`S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad`). Copy files from the CURRENT
repo working tree. Naming agent A (`naming/w2a/`) is still working on static getters / CBase ctors and shares the Wine
lane; your script must compose with its edits. Final message = the report (counts, files, flags, bindings, reverts, left).
</content>
</invoke>
<invoke name="Agent">
<parameter name="description">Naming wchar_t wave (Opus high)</parameter>
<parameter name="subagent_type">general-purpose</parameter>
<parameter name="model">opus</parameter>
<parameter name="effort">high</parameter>
<parameter name="run_in_background">true</parameter>
<parameter name="prompt">You are a FableDecomp naming-pass agent for the wchar_t wave. FableDecomp is a byte-exact VC7.1 reconstruction of retail Fable.exe; landed C++ rows live in /home/user/FableDecomp/rebuild/src/compiled and rebuild/tests. Owner rule (2026-10-28), "names right the first time": exact PDB spellings, one retail address = one mangled name. The owner decided today that wide text uses native wchar_t with /Zc:wchar_t (PDB `_W` spellings).

Read in full first:
1. /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/naming/BRIEF_WCHAR.md -- your task, rules and hand-back. Work in naming/wchar/.
2. naming/BRIEF.md and naming/BRIEF_W2.md (verification method, transform-script convention), naming/DECISIONS.tsv, naming/opeq/BLOCKED.tsv and naming/opeq/transform.py, naming/w2b/REPORT.md and naming/w2b/tools/ (run.sh, prep.py, cmp.py, compose.py) as working examples.
3. /home/user/FableDecomp/docs/journal/FINDINGS_LOG.md (the two 2026-10-28 wchar_t entries at the end) and tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md section "Owner rule, 2026-10-28".

Hard rules: never run git; never write under /home/user/FableDecomp; never kill a process; work only in naming/wchar/; no recursive find/glob over / or the whole scratchpad. Naming agent A works in naming/w2a/ at the same time and shares the Wine lane.

Every edited row must recheck MATCH/RELOCATION_MATCH with test PASS (dry run --recheck-landed with the new flag passed, never --land). Measure scope 3 before editing beyond ~500 rows. Final message = report: rows and declarations edited per scope, script and file list, FLAGS.tsv, bindings (respell/retire/keep), header items, reverts and why, what is left.</parameter>
</invoke>
