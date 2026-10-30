# Wave 10: remaining class-model debt

Counts come from `scripts/count.py` (rows that define `class|struct <X> {` at a line start under rebuild/src/compiled,
`*.pre.cpp` excluded). "Wave 9 start" is HEAD 3e83ccc7f; "after" is the wave 9 worktree.

| class | wave 9 start | after wave 9 | header | what is left |
|---|---:|---:|---|---|
| CCountedPointer (local) | 4,294 | **2,843** | CCountedPointer.h (new) | 1,317 deferred rows, already proven (below); 96 untested rows unlocked by the regex fix; the rest have other shapes (table below) |
| CPlayerManager | 104 | **79** | CPlayerManager.h | 7 const-only rows reach the manager another way (`g_pEngine->PlayerManager`, `g_pEngineGlobals`, `g_pWorld->PPlayerManager`, CGameScriptInterface +0x0C); 3 pointer stand-ins called non-const (0x0056FBED, 0x00570009, 0x005701DA; same fix as CPMP); vptr-word rows; 16 donor rows |
| CScriptGameResourceObjectMovieBase | 85 | **84** | MovieBase.h | 19 header-shaped rows DIFFER on the header (REVERTED.tsv MB); 39 have another base (CBaseObject + DestroyInline scaffolding); 8 declare the dtor |
| CScriptThing | 428 | 428 | CScriptThing.h | unchanged: 40 rows that pass the ST2 dry run all fail to compile (std arity, a non-template `struct CCountedPointer`, row-local vectors); the wave 8 table holds |
| CEntityScriptBase / CIsThingAlive / CPredicateBase / CIsThingConscious | 293 / 154 / 142 / 26 | same | | follow ST |
| CActiveEntityScript(Base) / CEntityScriptBinding | 35 / 42 / 94 | same | | 35 namespace rows; the 85 template rows keep a local pointer (below) |
| CTimer / CScriptBase / CGSI / CGSIBase | 16 / 88 / 220 / 36 | same | | not reached |

## Wave 9 result

- **Rows.** 1,578 rows edited (ROWS.tsv):
  - CP 1,548: the local pointer model replaced by engine/CCountedPointer.h, rows under 00/d* and 00/e*;
  - CPMP 12: const-only CPlayerManager rows through `CMainGameComponent::PeekPlayerManager() const`;
  - CPMX 13: pointer stand-ins respelled to the reference accessors;
  - CPS 5: SetStaticCast rows, the proof of the header's member template;
  - MB 1.
- **mcheck** (final source and final header, logs/mc_post_hdr.txt; identical to logs/mc_final_all.txt from before the
  SetStaticCast addition): 1,041 SAME and 537 RELOC_RENAMED:
  - 530 `Destroy_<addr> -> DeleteData<T>`;
  - 7 CPlayerManager pointer -> reference accessor spellings;
  - 1 `$L` renumbering.

  All renames keep their target: both spellings are bound already, so bind_add.tsv is empty.
- **Regrade.** logs/recheck1..5.log, one regrade per row: **1,578 / 1,578 PASS** (1,515 RELOCATION_MATCH, 63 MATCH),
  0 failures. payload_check.py: 0 missing or stale. bindings_multi_check.py: 0.
- **Reverts.** REVERTED.tsv lists them by family:
  - CP 27: the shapes in HEADERS.md, plus the retail_asm row 0x00E81B40;
  - CPA 85: the CreateActiveEntityScript experiment;
  - MB 19;
  - ST2 40: a retry of the wave 8 residue;
  - CPMX 4.

## Ready next (in order)

1. **CP deferred: 1,317 rows, ready.**
   - They were applied and mcheck'd in wave 9 (logs/mc_cp_final.txt: 1,249 SAME, 68 RELOC_RENAMED), then restored to
     keep the wave within budget. The list is work/cp_deferred.tsv (copied to `cp_deferred.tsv` in this folder).
   - To redo them, run `fam.py apply CP mig_cp.py -- <files>` and then `cp_post.py`. Before regrading, re-run mcheck,
     because the header gained SetStaticCast after that run.
   - One rename needs a binding: run bind_new on the new log.
2. **CP: 96 untested rows.** The out-of-class regex fix (LESSONS 5) unlocked them: `mig_cp.py --dry`, minus the deferred
   list, the 85 CAES rows and this wave's reverts.
3. **CP: other shapes.** These are mechanical respellings onto the header's names; prove each on 5 rows first:

   | shape | rows | what to do |
   |---|---:|---|
   | non-template `struct CCountedPointer` | 289 | give the row its T |
   | `m_pObject` / `m_pCount` (SCountBlock) | 178 | respell to Data / Info and the CCPPointerInfo fields |
   | `SetStaticCast` | 88 | FableWin member template, now in the header; the 5-row proof is SAME + PASS (family CPS), and the other 88 pass the dry run with the same body |
   | `m_pData` / `m_pInfo` | 69 | respell |
   | `void* Data` | 55 | |
   | `CCountedPointer<void>` | 26 | needs the real T |
   | SCountBlock / CCountedPointerInfo Info | 45 | |
   | Object / Count / Block word names | ~110 | |
   | namespace-local blocks | 77 | the CAES namespace rows among them |
   | `Release()` | 16 | |
   | `operator==` | 10 | FableWin has free `??$?8` __fastcall templates, not members |
4. **CreateActiveEntityScript (85 + 35).** Retail calls the pointer's (T*) ctor 0x00CE10B0, ShareData 0x00CE1040 and dtor
   0x00CE1000 out of line, and VC7.1 expands all three on the shared header (347 v 272 in every row). The local
   noinline / declared model stays until the per-function budget is understood. Rewriting the 35 namespace rows onto
   the template body (wave 8 NEXT 6) is still open; they would keep the same local pointer.
5. **MovieBase.** Retail's destruction form follows ctor expansion in the same function: 35 of 35 AB0-only rows expand the
   ctor, and 26 of 26 INL-only rows do not. Next experiment: make the ctor cost what retail's costs, then see whether
   VC7.1 calls the PImp dtor (work/mbv/ has the variants already tried).
6. **CPM.** The 3 non-const pointer rows and the 7 const rows reached through other owners. FableWin also has
   `CTCBase::PeekPlayerManager() const` and `CThing::PeekPlayerManager() const` (IBEABV). Each needs its body proven
   before use.
7. **ST residue / CTimer / CScriptBase / CGSI** as in wave 8 NEXT items 4-7.

## Note for the coordinator

- The new header turns on verify_and_land's shared-header policy for CCountedPointer. Every row that still defines a
  local `CCountedPointer {` (and does not include the header) now prints `WARN SHARED_HEADER_AVAILABLE CCountedPointer`
  on regrade. That is 2,843 rows by count.py; recheck1..5 show the warning on edited rows that keep another local
  pointer class.
- The warning is the debt signal and does not change a grade. With `LANDVERIFY_STRICT_HEADERS=1` such a row would be
  refused, so new chunk rows should include engine/CCountedPointer.h. The CreateActiveEntityScript shape is the known
  exception (item 4).
