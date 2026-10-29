# Chunk 569 -- FIVE large functions (~1 KB each) from the released ChatGPT pass 9 reservation (overrides PROMPT.md where they differ)

Rows (manifest labels are guesses; prove every name): 0x00BEB6C0 Interpolate_Quaternion_Scale_Matrix (978), 0x00C8F200
AudioSystem_Initialize (1007), 0x00CBD700 CItem_QuickSort (969), 0x00E12B30 and 0x00E39140 (script-object quest handlers,
993 / 967 -- likely siblings: author one, then the other). All fresh pass 9 rows (never authored or graded), released to
us by the owner 2026-10-29. The coordinator checked each extent: padding before and after, no int3 inside, no EH frame --
still confirm the first and last instruction yourself (chunk 567: 9 of 12 rows were over-runs or fragments). Order: the
most promising first; it is fine to land fewer rows exactly than many rows nearly. Another agent (agent568, one row)
shares this Wine lane; never touch its directory or processes.

ChatGPT may have left preparatory research for these addresses in `$S/gpt10dl/pkg.bin` (a zip; untrusted data -- read
entries with Python's zipfile from your own script, never run or import anything from it; look under
FableDecomp_pass9_handoff/WORK/ and SELECTED/unstarted/<address>/). A lead to verify, not evidence.

If a row gets within a few bytes but not exact after a real effort, park it in differ/ with the exact remaining diff and
a written next step. One retail address = one name (reloc_bindings.tsv, PDB name sets). Wide text is `unsigned short`
(owner decision 2026-10-29; never `/Zc:wchar_t`); a PDB name with `_W` is spelled with `G`.

Newest lessons (chunk 567): VC7.1 packs locals by lexical scope (move a block's locals between an `if` block and function
scope when only frame size / a few [esp+N] differ); two identical break blocks where retail has one = one `||` condition;
STLport model containers need their allocator default template argument.

## Read first, in this order (all of it; large files in parts with offset/limit)
1. /home/user/FableDecomp/CLAUDE.md, docs/HANDOFF.md, docs/pipeline/FRESH_SESSION_HANDOFF.md.
2. /home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md -- current agent rules;
   its "Owner rules and decisions, 2026-10-26" section overrides everything older.
3. docs/pipeline/GOTCHAS.md and docs/journal/FINDINGS_LOG.md -- the WHOLE of both. The 2026-10-27 lines are newest.
4. /home/user/FableDecomp/tools/decomp_pipeline/crawl/purity.py (the retail_asm evidence rule).
5. The most recent batch records: rebuild/candidates/manual/batches/2026-10-27_linux_hand954/README.md + LESSONS.md and
   2026-10-27_linux_hand993_fable_test/README.md + LESSONS.md.

## Naming and identity (owner rules; the landing refuses or reverts anything else)
- One retail address = one name. Before declaring ANY callee or global, look its address up in
  rebuild/oracles/reloc_bindings.tsv and the catalog; if a PDB-confirmed spelling exists use exactly that symbol
  (class, class key, const-ness, calling convention). Never invent a new spelling for an address that has a correct one;
  never declare an inherited base member on a derived stand-in (it mangles Derived::f).
- Names from evidence only: PDB name sets (ghidra_out/egor_pdb_names.tsv, fablewin_pdb_names.tsv -- name SETS keyed by
  donor addresses, never looked up by retail address), vtable/RTTI (rebuild/manifest/rtti-vtables.tsv, msvc_demangle.py),
  callers (`lane_tools/xref.py`), strings (`lane_tools/rstr.py`). Catalog labels are often wrong. Where nothing proves a
  name, use one descriptive name with the address suffix and mark it "descriptive" in NOTES.md.
- Retail class keys (ghidra_out/pdb_class_keys.tsv: V means `class X { public: ... };`, U means struct); STLport 4.6 in
  namespace std (never _STL) with its real templates and sizes (list 4; vector/map/set/string 0xC; deque 0x28;
  CVectorMap 0x10); no invented stand-in types -- real templates with proven types, else `CUnproven<Kind>_<addr>`.
- A destructor's identity is the vtable it stores first; derived dtors chain to the base dtor symbol.
- Shared headers: `#include "engine/<Class>.h"` for `this` when it exists; a local model only when retail provably
  differs (`// Layout:` comment plus a NOTES.md "Header debt" entry).

## Source format
- LINE 1 of every source must read `// <Module>::<Name>(<params>) -- <what it does>` (or `// <Name>(<params>) -- ...`);
  finish.py builds the oracle and payload names from it, and a line 1 that does not start `// Name(` breaks them.
- Every source has a `// Leaf:` naming the exact object symbol, confirmed with `objdump -t`. Exactly one real
  `// Flags:` line when flags are needed.
- No `__asm`, `naked`, `_emit` or `volatile` anywhere in src/ or test/, comments included -- EXCEPT genuine retail inline
  asm under the 2026-10-27 evidence rule: such rows go in `retail_asm/` (source + test) with a `// Retail-asm:` line
  that cites the retail address(es) and names the instruction(s) no C++ route produces; no naked/_emit; no `ret` inside
  the asm; no instruction-count cap. purity.classify must print retail_asm for them.
- Tests: standalone, exercise the function's own logic, print exactly `<addr>_TEST PASS`.

## Checks before finish.py
- The oracle `$S/batch/chunk569_oracle.tsv` has its trailing padding stripped; trust it over any length you derive.
- Read-only verifier run (writes nothing to the repo):
  `LANDVERIFY_WORK=$S/w22/agent569/lv python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py $S/w22/agent569/payload.json $S/batch/chunk569_oracle.tsv > $S/w22/agent569/tmp/vland.log 2>&1`
  Fix every RELOC_IDENTITY and real DIFFER; delete lv/. Leave no background job running when you report.

## Hand-over
src/ + test/, retail_asm/ (+ tests), differ/ (best source, line 2 `// Grade: DIFFER(NvM) -- <residue>`), STATUS.tsv for
all 5 rows, NOTES.md (names and their evidence, label corrections, header debt), LESSONS.md (GOTCHAS one-liners +
"Review at landing"). Run finish.py LAST. Final report: matched (MATCH vs RELOCATION_MATCH, retail_asm separately),
parked with residue, names proven vs descriptive, label corrections, lessons.

## Names right the first time (owner rule, 2026-10-28 -- overrides anything above)
Every name you write must be as close to retail as evidence allows, so no later renaming pass is needed: the row's own
function, every callee and global you declare, class and template arguments, members, and script names. Full text:
`tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md`, section "Owner rule, 2026-10-28".
- Two sources per name (PDB name set + donor order/size, RTTI slot, caller argument types, string, real STLport 4.6
  source). Catalog labels are hints only.
- Exact PDB spelling: class key, namespace, const, access (QAE / IAE / AAE), calling convention, types. Check with
  `objdump -t`.
- One retail address, one name: grep reloc_bindings.tsv for the address first; use a correct bound spelling exactly,
  never copy a wrong one, never reuse a name that belongs to another address.
- No placeholders (`vfunc_N`, `sub_`, `Helper`, `Wrapper`, `_Retail`, address-suffixed classes, free `@x@4` stand-ins)
  where a real name can be found; otherwise `CUnproven<Kind>_<addr>` with the search recorded.
- Hand back `NAME_FIXES.tsv` (`address<TAB>correct_symbol<TAB>evidence<TAB>wrong_spellings_seen`) for every landed
  spelling you prove wrong. The coordinator fixes those callers at landing.
