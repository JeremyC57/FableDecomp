# Resume here (one page) — updated 2026-11-03 late night (landing rounds e and f: CWorld::UpdateFrame, 20 pragma-free inline-depth redos, chunk 735's 4 rows)

**Overarching plan:** `docs/PLAN.md` (every known function by pool, the parked residue classes with their cheapest
fixes, the naming debt, and the order of work). 2026-11-03 evening: `tools/sync_manifest_from_catalog.py` put the
manifest back on the catalog names (33,656 landed rows, 275 unlanded rows from the newest labels, 331 stale
PARKED_INDEX statuses); run it after every landing round. The open PRIORITY name-fix count is 188 (185 addresses),
not 154 (189 after wave 33's one row).
**Same evening, three Opus agents (records in FINDINGS_LOG 2026-11-03):** 999 Leaf prefixes completed; 391 EH funclets landed
(218 parked with evidence); naming wave 33 landed 840 scalar deleting destructors (DECISIONS_W33 D1/D3/D5/D6 need the owner).
Codex pass 12 return verified and being regraded (`batches/2026-11-03_codex_pass12/` when it lands). Next: land the pass 12
verified rows (pass 11 procedure), decide DECISIONS_W33, add the 6 EH parents + the 90 proposed starts to the manifest after
review, fix the 2 parent Leafs, re-cut the 183 parent-unlanded funclets once their parents land.

The previous page (the 2026-11-01 state: chunks to 717, naming waves 13-32, the never-attempted pool) is archived at the
end of `docs/journal/HANDOFF_ARCHIVE.md`. This session's details: `docs/journal/2026-11/2026-11-03_SESSION.md`. Solved
problems: the 2026-11-02 / 2026-11-03 lines at the end of `docs/pipeline/GOTCHAS.md`. Agent rules:
`tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md`.

## State
- Branch `FullDecomp` (`JeremyC57/FableDecomp`), everything committed and pushed except the running agents' scratch.
- Coverage (refresh with `tools/write_decomp_dashboard.py` then `tools/update_readme_progress.py`; never hand-edit):
  46,548 of the 49,564-function catalog (93.91%), 55.92% of retail function bytes, after landing round f
  (`batches/2026-11-03_landing_f/`, `_landing_e/`; earlier rounds `_landing_b/` to `_landing_d/`, whose HEADER_NEEDS.md list what parked rows still need).
- Codex pass 13 landed 47 of 51 (`batches/2026-11-03_codex_pass13_landing/`; data names to check with `cvdump` on the owner's
  PDBs are in its NAME_DECISIONS.tsv); 0x00501450 is still held on CVectorMap::LowerBound. Pass 14 was never sent; its 100 rows
  are released to in-house chunks (`pending/2026-11-02_codex_pass14/RELEASED.tsv`).
- **Owner 2026-11-03 (late): finish the running agents (pragma-row study, landing round g, chunk 737), land their finished
  rows in one final landing round, then launch NO further agents until the owner says so.**
- **Owner 2026-11-03: at most 3 concurrent agents.** Verify and land, then launch (10+ attempts per row). Running at the time of
  writing: landing round g (chunk 736), chunk 737 (CPSCSingleSprite / CPSCLight), and the pragma-row agent on the 70 remaining `// Revisit: inline-depth`
  rows (mechanism and levers: `docs/pipeline/INLINE_DEPTH_REVISIT.md`, `batches/2026-11-03_inline_revisit2/README.md`).
- Landing round e left chunk 733's two rows (0x004075C0, 0x0040BCA0) unlanded: CUIScreenUnitConverter static access needs a
  `cvdump` of the owner's PDB. Its README lists 9 PRIORITY re-model lines queued in NAME_FIXES_QUEUE.tsv (`land0311e`); round f queued 17 more (`land0311f`). Chunk 735 parks AddAbilityToList(PC)
  (call-order of a two-call compare) and OpenInventoryInternalPC (member-sum reassociation), `pending/2026-11-03_linux_hand735/differ/`.
- Parked with drafts, not landed: 723 r2/r3, 724 r3, 727 (sky ctor held on CDefPointer::SetDynamicCast, `pending/2026-11-03_linux_hand727/DECISION_00b625e0.md`),
  728 (DrawDebugInfo, CTCWife Global/FrameUpdate), 729, 730, 731 (SetDefaultRenderStates, bookmark operand-order gap).
- Chunks are cut from the category A groups with `$S/cut_groups.py N <group ids>` (reads `pending/2026-11-03_groups/ROWS.tsv`,
  skips landed and chunk_history rows, writes `$S/w22/chunkN.tsv` + chunk_history); oracle via `oracle_from_exe.py $S/w22/chunkN.tsv chunkN`.

## Lane (already set up in the scratch directory)
```
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh; export S
# (2026-11-03: the 5221bbec-... scratch of the previous page is gone; this one holds the same lane. Check `ls $S/wine/env.sh` first.)
cd /home/user/FableDecomp   # every tool runs from the repo root
```
`$S` holds Wine + cl 13.10.3077 RTM (`$S/wine/vc71rtm`), QFE 4035 (`$S/wine/vc71`), VS 2003 libs, LZO, and the game
(`$S/dl/game/Fable The Lost Chapters/Fable.exe`, never committed). A fresh container rebuilds it with
`docs/pipeline/FRESH_SESSION_HANDOFF.md`. Write scratch only under `$S`, never under `/tmp` or `/`.
- Leftover files at the filesystem root (`/gotchas_new.txt`, `/pw`, `/ov_final`) are agent scratch. Leave them for the
  owner; the safety check blocks agents from deleting them.

## Land / repair one row (T = tools/decomp_pipeline/linux_lane/lane_tools)
1. **Payload:** `{"result":{"authored":[{address,module,name,source_cpp,test_cpp,pass_pattern,leaf,flags?}]}}`.
2. **Oracle:** the catalog row (`address name length bytes`), or `python3 $T/chunk_pool/oracle_from_exe.py rows.tsv NAME` for a new row.
3. **Read-only grade:** `FABLE_HDR_OVERLAY=<hdr> LANDVERIFY_WORK=<dir> python3 -u $T/vland.py payload oracle`. Add `--recheck-landed` for a row that is already landed.
4. **Headers:** `python3 $T/coordinator/hdrcheck2.py <hdr>` must print `default=same` for every changed header (it stubs system headers; `hdrcheck.py` can compare nothing on fable_string.h). Recheck every landed includer and every definer of a touched FABLE_RETAIL_ macro: `python3 $T/recheck.py <out> <src paths>`, then vland `--recheck-landed`, 4 parallel splits.
   - A header with only `#pragma once` (fable_string.h) is included twice under a partial overlay (C2011). Recheck on a full mirror (`cp -r rebuild/include X; cp -r hdr/. X/`).
5. **Re-landing a landed row:** `python3 $T/coordinator/repair_remove.py <addr>` first.
6. **Land:** `python3 $T/chunk_pool/fixmove.py P; python3 $T/naming/keypayload.py P; LANDVERIFY_WORK=<dir> python3 -u $T/vland.py P O --land`.
7. **After landing:**
   - `python3 $T/coordinator/bindings_multi_check.py rebuild/oracles/reloc_bindings.tsv` must print 0.
   - Labels file `ghidra_out/labels_*.tsv`, then `python3 $T/coordinator/apply_labels.py <labels>` and `python3 tools/sync_catalog_names.py`.
   - Write records in `rebuild/candidates/manual/batches/<date>_<name>/` (README, payload, oracle, verify.log, includer_recheck_grades.tsv).
   - Second spellings go to `rebuild/candidates/manual/naming/NAME_FIXES_QUEUE.tsv` as PRIORITY. Append ` [<date>: ...; done]` to the note when closed.
   - Run vland with a 120 s Bash timeout or in the background: a fatal cl error under Wine can spin at 100% CPU for minutes.

## Owner rules in force (latest first)
- 2026-11-03 "whichever way retail does it": SDK structs take `<pshpack1.h>/<poppack.h>`. An engine class retail packed (odd PDB offsets) takes `#pragma pack(push, 1)` in its own FABLE_RETAIL_<CLASS> branch: C2DVector, CIntelligentPointer.
- 2026-11-02: retail faithfulness in every decision; names right the first time (two sources, exact PDB spelling, one address = one name; respell old callers in the same commit or queue PRIORITY).
  - One class, one retail model, behind FABLE_RETAIL_<CLASS> with the default path unchanged.
  - Never add a device retail did not have: no inline_depth, no forwarding overloads, no member-dropping or invented local class copies, no pointer casts between "descriptive" element types.
  - Agents get 10+ attempts per row; no stopping agents mid-landing.

## Next (in order; small first while usage is tight)
1. **Codex passes 12-14** (reserved NA077-NA106 until 2026-11-30, `rebuild/candidates/manual/pending/2026-11-02_codex_pass1{2,3,4}/`). When a return arrives, regrade it like pass 11 (`batches/2026-11-02_codex_pass11/README.md`):
   - grade each row in its own header context, then land the rows that need no new header;
   - consolidate per-feature macros into per-class branches;
   - send retries to agents.
2. **Priority name fixes:** 154 open in `NAME_FIXES_QUEUE.tsv` (no ` done]` marker).
   - Cheapest first: a single landed caller whose fix is a declaration spelling (const, return type, namespace, access).
   - Callers built on invented local classes need a rebase onto the shared header first: 0x0041649C (`CWorld_Methods`), 0x0065935E (`CTCScriptedControl`), 0x00521AE0 (`CCharStringSet` / `CThingVector`).
3. **Pass 11 leftovers:**
   - 39 parked rows: best sources in `pending/2026-11-02_codex_pass11/retry_differ/` and `retry_a_differ/`; most are inline-budget residues.
   - 0x0077F5C0 packs an owner class in a local model, which contradicts the CIntelligentPointer evidence. Review it.
   - The `retail-parity.tsv` MATCH records for 0x00449990 / 0x004496B0 / 0x004499E0 are stale. Leave them for the Windows gate.
4. Older open work: items 2-6 of the archived page (naming wave 32 queue, migration wave 10, Leaf lines), `docs/ROADMAP.md`.
