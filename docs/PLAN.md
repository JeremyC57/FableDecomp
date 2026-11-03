# FableTLC reconstruction plan (overarching) — 2026-11-03

One page of **where every known function stands** and **what to do next, cheapest first**. Numbers are measured
from the repo on 2026-11-03 (`rebuild/COVERAGE.md` for the landed counts; the pool and name counts below come from
`tools/sync_manifest_from_catalog.py`, `rebuild/candidates/manual/PARKED_INDEX.tsv` and the labels files). Task
checklists stay in `docs/ROADMAP.md`; the resume point stays in `docs/HANDOFF.md`. Refresh the counts with
`tools/write_decomp_dashboard.py` then `tools/update_readme_progress.py`; never hand-edit them.

## 1. Where the 57,609 known functions stand

| Pool | Rows | Retail bytes | Note |
|---|---:|---:|---|
| Known functions (`rebuild/manifest/functions.tsv`) | 57,609 | 10,234,993 function bytes (14,858,408 `.text`) | 49,564 original catalog + 7,608 gap-scan + 437 boundary rows. `rebuild/manifest/status.json` (57,097) is the 2026-09-07 Windows snapshot; the live count is the manifest. |
| **Landed byte-exact** (`rebuild/oracles/auto-re-candidates.tsv`) | **53,498** (92.86%) | 5,474,516 (53.49%) | MATCH or RELOCATION_MATCH + behaviour PASS. 53,484 genuine C++ (incl. 181 `retail_asm`, 441 `toolchain_library`, 8 `library_source`); 13 `asm_bake` oracles excluded from the genuine figure. Original catalog: 46,014 / 49,564 = 92.84%. |
| Parked, unlanded (`PARKED_INDEX.tsv` status `parked`) | 1,418 | 971,721 (est.) | Every row has at least one compiled attempt and a residue class (table in §3). 412 are same-length ties. |
| Unworked (no landed source, no parked record) | 2,693 | 3,788,814 (est.) | Composition in §2. Codex passes 12 and 13 have returned (NA077–NA096); pass 14 (NA097–NA106) was released to in-house cuts on 2026-11-03. |

Byte estimates size an unlanded row as the gap to the next known start (capped at 64 KB), so they are upper bounds.
The function-count coverage is 92.9%; the byte coverage is 53.5% because **221 unlanded rows over 4 KB hold
1.82 MB, 38% of all unlanded bytes**. Row count and byte count are now two different projects (§4).

## 2. The unworked 2,693 rows, by what they are

| Kind | Rows | Bytes (est.) | How they land |
|---|---:|---:|---|
| EH funclets (`Unwind@…` / `Catch@…`) | 609 | 23,211 | 608 have a **landed** parent immediately before them. They are slices of the parent's object: a source that `#include`s the parent (the 145-row precedent in `rebuild/candidates/manual/headers/2026-11-02_funclet_includes/`), graded with `recheck.py`. No authoring. |
| ≤ 256 B | 500 | 35,993 | Chunk-pool rows (`chunk_pool/analyze_never.py` already classifies them: tiny stubs, vendor rows, retail_asm candidates). |
| 256 B – 1 KB | 458 | 274,977 | Normal chunks; the never-attempted chunks of this size went 8–10 of 10. |
| 1 – 4 KB | 475 | 997,341 | Normal chunks with the increased-attempts brief; the quest-script `Main` bodies have Aeon's Lua ports as an oracle. |
| > 4 KB | 117 | 1,000,868 | One row per agent session with a drafted skeleton first (`RunCutsceneMacro_Func` 0x00CBFB7D, `Init_RegisterGameComponents` 0x004EE23F 44 KB, `NUISystem::CManager::vfunc_14` 0x0041E6D3 35 KB, `CScriptManager_RegisterAllScripts` 0x00CD52D0 24 KB, `CHobbeTorrent::Main` 0x00D92380 20 KB). |
| Reserved for Codex 12–14 | 300 | 632,435 | Regrade each return like pass 11 (`batches/2026-11-02_codex_pass11/README.md`). |
| Gap-scan `sub_` starts | 234 | 823,989 | Xref-found starts in unmapped territory; the byte figure is the gap, not a length. Run `lane_tools/bounds/extent.py` first; several are 20–32 KB bodies (0x00CE7670, 0x00D4A270, 0x00F2D6E0). |

## 3. What we have the most trouble with (the 1,418 parked rows) and the cheaper way through each

Residue classes of the parked, unlanded rows (`PARKED_INDEX.tsv`, column `residue_class`):

| Residue | Rows | What it is | Cheapest known fix |
|---|---:|---|---|
| register-tie | 537 | Same length, one register colouring differs. | Not more probes. Chunks 604–650 landed 98 of 410 of these at 10+ probes each (1 in 4) against 92 of 110 for fresh rows. The levers that did work were **PDB signatures (const, reference parameters, return types), the shared retail header instead of a local stand-in, and PDB inline accessors** — i.e. the row was modelled wrong, not the compiler. Re-attempt a tie only after `naming/spellcheck.py` and `hdrcheck2.py` show the row is on the retail model; otherwise leave it. |
| x87-order | 236 | Operand order of float code. | Mechanical levers recorded in FINDINGS_LOG (named C3DVector / C2DVector copies through the user copy ctor, `result = expr` through a named local, const-reference alias of the loaded member, `(float)((float)a*b)` to force the spill, `<math.h>` float overloads). `lane_tools/x87sim.py` (new 2026-11-03) is the tool to make this a scripted probe instead of an agent guess. |
| stack-slot/frame | 227 | Frame packing / slot order. | Declaration order and scope; for `/Od` TUs the locals are laid out by **name hash**, so the PDB local names are the lever (chunk 627). Address-taken temporaries get their own slot. |
| block/arm-layout | 146 | Arm order, tail merging, early-out targets. | Whole statement per arm (merged tails), switch case order = retail block order, check retail's early-out jump targets before calling it a tie (chunk 632). Semi-mechanical. |
| ltcg/crt/asm-impossible | 131 | CRT hand-written asm, LTCG shapes, import thunks. | Retire them from the pool instead of re-probing: msvcrt.lib members grade `toolchain_library` via `library_signatures.tsv`; retail inline-asm blocks grade `retail_asm` (owner rule 2026-10-24); the rest get an `IMPOSSIBLE` evidence line and leave the chunk pool. |
| incomplete-model | 114 | Missing header, callee, layout. | The header waves (Codex pass 11: 29 existing + 77 new per-class headers) are the fix; most of these should be re-cut once the class has a `FABLE_RETAIL_<CLASS>` branch. |
| other (flags/pragma 9, bounds 5, eh/unwind 5, schedule 4, inline-vs-call 3, layout 1) | 27 | — | Case by case; the bounds ones through `BOUNDS_FIX.tsv`. |

The pattern across every retry sweep: **a parked row lands when its model is corrected (names, header, signature,
inlined callee defined in the TU), almost never when the same model is re-probed.** So the plan spends the compiler on
rows whose model is known to be wrong (incomplete-model, block/arm, frame) and spends none on ties until a model
fault is found.

## 4. Names: the catalog and manifest are the record — keep them so

Done 2026-11-03 (`tools/sync_manifest_from_catalog.py`): every landed row's manifest `module` / `name` now equals its
catalog name (33,656 rows resynced, 7,647 of them had the class repeated in both columns); 275 unlanded rows took their
newest label; 331 `PARKED_INDEX.tsv` rows that had landed were still `parked`. `ghidra_out/labels_manifest_sync_2026-11-03.tsv`
carries the same 33,931 names for `LabelApply.java`. Run the tool after every landing round, after `apply_labels.py`
and `sync_catalog_names.py`.

What is still not right, cheapest first:

1. **Truncated `// Leaf:` lines** — 18,144 landed sources have a mangled Leaf, but 4,544 distinct ones are prefixes
   (`??1CAIStateGroup_HoverWanderAroundThing@`), so `sync_catalog_names.py` cannot verify those 6,396 rows (it reports
   them as "demangle failed"). 1,839 complete to exactly one symbol in `ghidra_out/{fablewin,egor}_pdb_names.tsv`; 305 are
   ambiguous; 1,912 are STL / template instantiations absent from the PDB lists; 488 are C-decorated (`@ov_pcm_tell@4`).
   A pure-Python pass completes the 1,839 and lets the sync verify them. 12,213 more sources carry a bare leaf and
   23,074 none; those rows are named by their catalog entry only.
2. **12,569 landed rows still carry a descriptive or placeholder name.** Mechanical waves, each with two sources:
   - 1,269 `CVtable_<addr>::`scalar deleting destructor'` + 239 `ScalarDeletingDtor_<addr>` → class from the vtable's
     RTTI (`rebuild/manifest/rtti-vtables.tsv`), named `Class::`scalar deleting destructor'`.
   - 756 `CCountedPointer_Release_<addr>` + 580 `CIVCountedPointer_Release_<addr>` → the template instantiation from
     the pointee's destructor / vtable the body calls.
   - 253 `AtexitDtor_CCharString_at_*`, 225 `_static_init::g_CCharString_*` → `$E<n>` dynamic initialisers named through
     the `.CRT$XCU` order (chunk 716/717 precedent) and the PDB static they initialise.
   - 200 `SetMemberConst_*`, 190 `RbTree_Clear_*`, 159 `StdList_Dtor_*`, 152 `StdList_Clear_*`, 161 `CVtable_*::vfunc_4` →
     STLport instantiation names from the element type (one instantiation calls its own helpers: chunk consolidation 2).
   - 1,822 `sub_<addr>` are gap-scan fragments of ≤ 64 bytes (1,682 of ≤ 16); most are thunks and have no retail symbol.
     Leave them, but tag them so they stop counting as naming debt.
3. **518 open PRIORITY rows in `naming/NAME_FIXES_QUEUE.tsv`** (507 addresses, 2026-11-03 night: rows whose note starts with
   PRIORITY and has no ` done]`; 187 by the same rule that morning, the growth is the day's chunk and landing re-model lines). Cheapest
   first: a single landed caller whose fix is a declaration spelling. Callers built on invented local classes
   (0x0041649C, 0x0065935E, 0x00521AE0) need the shared-header rebase first.
4. **94 labels in `.text` name addresses the manifest does not list** (e.g. 0x00CB63F0 `std::iter_swap<…>`, 0x0042BEEA
   `NUISystem::CDeletion::~CDeletion`). Check each against the landed rows' extents: a start inside no landed row is a
   function the manifest is missing; one inside a landed row is a label on an inlined body.
5. `rebuild/manifest/status.json` and the analysis table in `COVERAGE.md` are the 2026-09-07 Windows snapshot
   (calling convention 77.7%, prototypes 69.2%). They need the Windows refresh (`tools/bootstrap_rebuild_tree.py`); the
   Linux lane cannot do it. Until then only the live table counts.
6. `docs_reorg.py --check-findings` already reports 9 FINDINGS_LOG headings with no reference-doc home (pre-existing;
   CI runs it on `main`).

## 5. Order of work (small and compiler-free first, while weekly usage is tight)

1. **No compiler, no agents (pure Python):** complete the 1,839 Leaf prefixes; the mechanical naming waves of §4.2 as
   labels files + `apply_labels.py` + `sync_catalog_names.py` + `sync_manifest_from_catalog.py`; the 94-label check;
   retire the 131 impossible parked rows with evidence lines; prepare the funclet payloads (parent source per funclet).
2. **Compiler, no authoring:** land the 608 funclets whose parent is landed (`recheck.py` grades the parent TU's funclet
   symbol; `vland.py --land`). Expected: ~600 rows, no bytes to speak of, but the pool stops carrying them.
3. **Agents, when usage allows:** Codex pass 12–14 returns (300 rows); then never-attempted chunks ≤ 1 KB (958 rows,
   the 8–10 of 10 band); then parked `incomplete-model` / `block/arm-layout` / `stack-slot/frame` rows re-cut **only after**
   their class has a retail header; ties last.
4. **Bytes:** the 221 rows over 4 KB (1.82 MB) one at a time, skeleton first, with the Lua ports and the FSE manifest as
   oracles for the script and script-API bodies. Budget one agent session per row; park with a drafted skeleton if it
   does not land, so the next attempt starts from the draft.

Each round ends with: `bindings_multi_check.py` = 0, labels applied, catalog + manifest synced, dashboard regenerated,
records under `rebuild/candidates/manual/batches/<date>_<name>/`, one FINDINGS_LOG line, HANDOFF updated.
