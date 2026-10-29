# Naming pass, wave 1 (owner go-ahead 2026-10-28)

Goal: landed code under `rebuild/src/compiled/` and `rebuild/tests/` spells every function, callee, global, class and
vtable as retail did (exact PDB spelling, two sources, one retail address = one mangled name). Retry chunks 520-541 handed
back 742 proven/probable fixes (`QUEUE_snapshot.tsv`: chunk, address, correct_symbol, evidence, wrong_spellings_seen).
You turn them into verified source edits. The coordinator copies your results into the repo, respells the bindings, rechecks
and commits.

## Hard rules
- Never run git. Never write under /home/user/FableDecomp (read it freely). Never kill a process. Work only in
  `$S/naming/` (`S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad`).
- Lane: `source $S/wine/env.sh` then `cd /home/user/FableDecomp`. Another agent (retry chunk 541) shares the Wine lane.
- Never run a recursive find/glob over `/` or the whole scratchpad. `grep -rl` inside `rebuild/src/compiled` and
  `rebuild/tests` is fine (~51k files each; prefer `grep -rlF` with a fixed string).
- Byte purity is untouched: every edited row must still grade MATCH or RELOCATION_MATCH and its test must still PASS.
  A rename that changes code bytes is wrong (or the old body only matched by accident) -- revert it and report it.
- Evidence: a fix lands only when the correct spelling is in `ghidra_out/egor_pdb_names.tsv`, `fablewin_pdb_names.tsv`
  or `egod_pdb_names.tsv` (exact mangled or demangled match) or is otherwise proven by two sources (RTTI/vtable + donor
  order, STLport header spelling for template helpers). `work/SETTLED_NAMES_2026-10-28.tsv` overrides older spellings.
  Probable-only fixes are listed, not applied.

## Step 1 -- consolidate (write `DECISIONS.tsv`)
One row per retail address: `address  final_symbol  confidence(proven|probable)  sources  chunks  wrong_spellings(all, |-joined)`.
- Many addresses appear more than once; merge them. When two chunks proposed different correct symbols, decide from the PDB
  lists and say why in `CONFLICTS.md`; if undecidable, mark `probable` and do not apply.
- Check every final symbol against the PDB name lists yourself (`grep -F`). Note demangling with `c++filt` is not
  MSVC-aware; compare mangled strings, or use `undname` from the VC7.1 lane (`$S/wine/vc71rtm` has `undname.exe`? if
  not, compare by hand).
- Also settle the open naming items left by landings (see "Known open items").

## Step 2 -- find where each wrong spelling comes from
`rebuild/oracles/reloc_bindings.tsv` (`symbol  target  first_caller_address`) lists every bound undefined symbol. For each
decision, the wrong spellings bound to that target are the ones landed code emits. Find the landed rows that emit them:
compile is expensive, so first narrow by source text (the class/function identifier in the declaration), then confirm with
the row's object symbols (`cl /c` + `objdump -t` in a temp dir, or the verifier's dry run). A row's paths come from
`rebuild/build_candidates.ps1` (`Source` / `TestSource` per Address) -- see `mk_reference.py`.

## Step 3 -- edit copies, in waves
Work in families (same class / same wrong declaration pattern), highest value first:
1. Wrong function identity (two names on one address, wrong class, swapped pairs) and wrong vtable symbols.
2. Signature spelling (const, access, calling convention, ref vs ptr, class vs struct key, return const-ness),
   NOT the `CCharString::operator=` / `CWideString::operator=` const-return family: a second agent handles that one
   (`$S/naming/opeq/`); do not edit those declarations.
3. Globals (`g_...`): only when a PDB data symbol exists; descriptive `g_name_<addr>` spellings that bind one address under
   several names should converge on ONE spelling (pick the most-used one if no PDB name exists) and list them.

For each edited row: copy the source and test to `$S/naming/overlay/<same repo-relative path>` and edit the copy. Keep
edits minimal (the declaration and its uses); keep comments truthful; when a function's own identity changes (the row's
own name), also list the file rename the coordinator must do (`RENAMES.tsv`: old path, new path, address).

## Step 4 -- verify
Build a recheck payload from the overlay (adapt `mk_reference.py`: same fields, but read source/test from the overlay)
and run, from the repo root with the lane env sourced:
`LANDVERIFY_WORK=$S/naming/lv python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py <payload> <oracle> --recheck-landed > $S/naming/recheck_wN.log 2>&1`
(dry run: it writes only under LANDVERIFY_WORK; never pass --land). Every row must be MATCH or RELOCATION_MATCH.
RELOC_NAME / RELOC_DUPLICATE warnings that come only from the old spelling still being in reloc_bindings.tsv are expected
-- list each one in `BINDINGS.tsv` (`old_symbol  target  new_symbol`) and the coordinator respells the bindings. Any other
RELOC_CONFLICT, a DIFFER, or a failed test means revert that row's edit.
Batch the recheck (hundreds of rows per run are fine; it takes ~2-4 s per row).

## Hand-back (per wave; stop after wave 1 and report -- aim for at most ~300 edited rows)
`$S/naming/wave1/`: `overlay/` (only rows that pass), `BINDINGS.tsv`, `RENAMES.tsv`, `LABELS.tsv`
(`address  name` for ghidra_out labels), `APPLIED.tsv` (decision rows applied, with the rows edited), `DEFERRED.tsv`
(decisions not applied and why), `recheck_w1.log`, `REPORT.md` (counts per family, conflicts settled, anything reverted).
Also keep `DECISIONS.tsv` / `CONFLICTS.md` at `$S/naming/` for the next waves.

## Known open items
- `??0CBase@@QAE@XZ` binds both 0x0099A2F0 (proven `CBaseClassNonCopyable` ctor, RTTI) and 0x0099A2D0; 0x0099A2E0 is
  `??1CBaseClass@@UAE@XZ`.
- libpk's CHandleBasedMemoryPool copy (0x00C28380 / 0x00C28640 / 0x00C28690, held in
  `rebuild/candidates/manual/pending/2026-10-27_linux_hand520/held/`) reuses the engine pool's bound names: one class
  name per address set; decide from evidence or list it.
- 0x00436AC0 vs 0x004AC610: `vector<CThing*>` vs `vector<const CThingBuilding*>` `_M_insert_overflow` (held row
  `pending/2026-10-27_linux_hand526/held/00946f90`).
- STLport `_Rb_tree` template-argument count in landed `_M_erase` / `_M_insert` leaves.
- `g_pQuestManager` 0x013B89FC: `CCountedPointer<CQuestManager>` (newest, proven by member Reset 0x004A9A10) vs
  `CQuestManager*` in older rows.
- 0x0129BA3C bound under 49 descriptive spellings; 0x013BCA24 under 5; 0x00686D40 (an ICF getter) under 6.
- `GFGetCurrentFrame` and friends -> `?GetFrame@CWorld@@SIJXZ` (0x0049D870).
</content>
</invoke>
<invoke name="Bash">
<parameter name="command">ls /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/wine/vc71rtm 2>/dev/null | head; ls /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/wine/vc71rtm/bin 2>/dev/null | grep -i undname