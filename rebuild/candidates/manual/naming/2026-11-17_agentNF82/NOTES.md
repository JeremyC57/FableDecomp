# NF82 notes (round 7, land1117fu: q10989..q11008)

Slice: rows.txt (77 rows) plus 0x004BB270, claimed during q10993/q10994 (extra.txt): 78 rows in STATUS.tsv.
- 63 `ported`: every one is RELOCATION_MATCH or MATCH with its test PASS (recheck_final.log).
- 9 `not-a-binder`: the self row or binder was already right; STATUS says why for each.
- 6 `blocked`: 0x0049124E, 0x004DBF08, 0x004DD860, 0x00663F50, 0x009D2BA0, 0x00A2AC30.
- None deferred: every row I needed was free in nflocks, or already mine.

Entries (APPLIED.tsv):
- 14 done, 2 done-already (q10996 and q11006, via overrides.tsv), 4 blocked:
  - q11001: 0x004DBF08 / 0x004DD860.
  - q11005: 0x00663F50.
  - q11007: 43 of 45 ported; 0x009D2BA0 and 0x00A2AC30 are left.
  - q11008: 0x006C37D0 ported; 0x0049124E is left.

`grades_used` in STATUS counts the new version's compiles (g.sh) plus its vland run. Probes of the landed rows (oldprobe.sh) are not
counted. The exception is a row that ends blocked or not-a-binder: its 1 is that probe. grades.log has every call. Two lines at
17:18:32 are a g.sh call made without a file: q.py read '.', nothing was compiled, and they are marked in the log.

## Names
- No descriptive (made-up) names were introduced. Every new spelling falls in one of four groups:
  - PDB names: Ego_r.pdb, FableWin.pdb or Ego_d.pdb.
  - RTTI names: CSoundBankLH (`.?AVCSoundBankLH@@`).
  - STLport 4.6 template instances. These are descriptive-by-library: the instance is in no PDB of this build, and the function
    name is STLport's.
  - Instances whose element type is proven below.
- NParticleEngine::CRegisteredPSCInfo has no shared header. Rows 0x00AF2F10 and 0x00AF3290 declare it in the row with its Ego_r.pdb
  members (AllocFunc, EPSCType ComponentType, CCharString GUIName); EPSCType and CPSCBase come from engine/CPSCBase.h. This is a
  model of a class with no header, which the rule allows. A future engine header for it should take these members.

## Element-type judgments (q11007, one address = one instance: retail has no ICF)
- **0x009B47F0 / 0x009B4940 / 0x009B5800 / 0x009B5990 / 0x009B5C70: `pair<CCharString, unsigned long>`.**
  - The chain std::sort 0x009B8EF0 is called by CDefinitionManager::CheckDependencies 0x009B05F0. That function's file->checksum
    map holds GetChecksum's `K` result.
  - Ego_d.pdb lib_definition_manager.obj has `CVectorMap<CCharString,unsigned long,..>::Sort` and `std::sort<..pair<CCharString,unsigned long>..>`.
  - The PDBs have no CCharString-keyed map with a `long` value, so the old 0x009B5800 comment `pair<CCharString, long>` was wrong.
- **0x00A403F0 / 0x00A408E0: `pair<CCharString, CSoundBankLH*>`, not unsigned long.**
  - Their only callers are the sort chain of CSoundManagerLH::NewAtmos 0x00A3F140 (NameMap).
  - The landed 0x00A41480 `_M_insert_overflow` and 0x00A40590 `__median` already spell `PAVCSoundBankLH@@`.
  - The unsigned long instance is the 0x009B... family.
  - Landed 0x00A41260 `__pop_heap_aux` and 0x00A41960 `_Sort_heap` still model unsigned long (NAME_FIXES).
- **0x004B97D0 const LowerBound: count taken before `first`.** This is the overlay opt-in CONST_LOWERBOUND_COUNT_FIRST. A probe of
  the landed 0x004365B0 also MATCHes under it, so count-first may be the one original text for every instance (coordinator
  question in hdr/README.md).
- **0x00835AF0 is CThingDeadCreature::SetAnimationFilter, out of line.** Evidence: Ego_d thing_dead_creature.obj and the body. The
  CThingDeadCreature.h opt-in declares it; NF80's inline body stays the `#else` arm for Create 0x00835C80.
- **The 0x004B9xxx tree family is `set<CCharString>`.** Evidence: Ego_r members of CQuestManager, CSymbolMap (NamesUsed, via the
  CSymbolMap.h opt-in) and CFaction.
- **0x004B4E30 / 0x004B4F30 setters: written as Insert + Sort, not Insert + Update.** Both member functions exist for this instance
  in FableWin.pdb.
  - At 0x004B4F30 (map at +0x10), Insert + Update either reloads and tests Dirty or sinks the Dirty store below the sort's loads.
    Tried four ways: plain, through a map reference, with Update forced inline (an opt-in I then removed), and with the SORT
    Update.
  - Insert + Sort matches both rows. 0x004B4E30 matched under either form and now uses the same form.
  - The old 0x004B4F30 row got there with a may-alias `self` reference instead.

## Files and labels for the coordinator
- work/filemoves.txt: five rows whose file names no longer fit their symbol.
- Also the old `global_*` file names of rows that now emit std templates:
  - `_Pushheap` / `_Adjustheap` / `_popheap`: 0x004B9540, 0x004DD875, 0x004E1D20, 0x0077E730, 0x009B4940, 0x009B5C70,
    0x00A403F0, 0x00A408E0, 0x00AF2F10, 0x00CB64D0.
  - `_TreeLbound`: 0x009AAD60.
  - vland prints a non-blocking `WARN STANDIN_LEAF ... (catalog _global::_Push_heap / _Adjust_heap)` for several of these; RENAMES
    section 1 has the new names.
- NAME_FIXES.tsv (32 lines): landed rows outside the slice that bind slice addresses under stand-ins or under a wrong element
  type, including the same-TU heap and sort helpers of each ported family.

## Header overlay
- Five headers; see hdr/README.md. Every addition is an opt-in, and every landed includer sampled is token-identical. All 55 landed
  rows that define a parent macro were checked as well (tokid.log).
- fable_string.h was not overlaid.
- One rename during the work: my copy_backward opt-in first reused NF68's FABLE_RETAIL_STLP_COPY_BACKWARD_NOINLINE (in another
  scope). It is now FABLE_RETAIL_STLP_SORT_COPY_BACKWARD_NOINLINE. 0x005E1A40 preprocesses to the same text before and after the
  rename.

## Rule slips (also work/slips.txt)
- One Bash call exited 1, and one heredoc edit failed inside a call that exited 0.
- Two rows went past three failed forms and were kept ported:
  - 0x004B9540: three SRC_FAILs, matched on the 4th compile.
  - 0x004B4F30: six failed forms, matched on the 7th (8 grades with vland).
- The probe-after-grade ordering cost two regrades.
- Two g.sh calls ran without a file.
- One opt-in first reused an existing macro name; it was renamed.
- Generated families were graded in batches, and STATUS was written per family rather than per row.

## NAME_FIXES additions after the reloc_bindings check
NAME_FIXES.tsv now has 42 lines. reloc_bindings.tsv was checked for every symbol the ported q11007 families emit or bind:
- otherTarget: none. No new spelling is bound to a second address.
- otherLeaf: outside callers that bind these addresses under stand-ins were added to NAME_FIXES (0x004B9750, 0x004BA180,
  0x004BBB40, 0x0077E730, 0x0077E8F0, 0x0077E960, 0x0077F2C0, 0x009B5C70, 0x00A71370, 0x00AF3290, 0x00A32960). Bindings whose
  call site is one of my ported rows are stale and are fixed by those rows.
