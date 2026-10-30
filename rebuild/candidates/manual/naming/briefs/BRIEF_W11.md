# Naming pass, wave 11 (one agent; a migration agent and a chunk agent share the lane)

Landed: operator=, waves 1-10, LowerBound. Records in `rebuild/candidates/manual/naming/`; wave 10 is `2026-10-30_wave10/`
(read its README.md, AGENT_README.md, DEFERRED.tsv and DECISIONS_W10.tsv first; its brief is `briefs/BRIEF_W10.md` and the
rules there are unchanged). Since wave 10, migration waves 6-8 (`rebuild/candidates/manual/headers/`), chunks 586-593 and the
asm re-author pass (`rebuild/candidates/manual/batches/2026-10-30_asm_reauthor/`) queued new NAME_FIXES. Owner decisions in
force: wide text is `unsigned short` (`G`), never `/Zc:wchar_t`; names right the first time (exact PDB spelling, two sources
per name, one retail address = one mangled name); retail's calling convention wins over a PDB spelling when they differ.

Workspace: `$S/naming/w11/` (S=/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad).
Prepared input: `$S/naming/w11/QUEUE_586_593_mig6_8_asmre.tsv` (201 NAME_FIXES rows incl. chunk 593; columns chunk, address, correct_symbol,
evidence, wrong_spellings_seen; the queue file itself is `rebuild/candidates/manual/naming/NAME_FIXES_QUEUE.tsv`).

## Scope, in this order (stop at about 600 edited rows)
1. **The mig8 entity nesting (55 rows + 3):** entity classes landed at global scope whose PDB name is nested
   `NScript::<Script>::<Entity>` (their `EAE` Main leaf already matches): wrap the row's entity class (and its binding words)
   in the script class / namespace so the leaf symbol becomes the PDB spelling; the mig7 three Main rows (0x00CE1280,
   0x00D51FC0, 0x00EF3200) are the same shape. CQuestInfoElementTimer -> nested in CDrawQuestInfo, timer by reference;
   00efbe20 RegisterFilter -> `?SetPredicate@CEntityScriptBase@@QAEXABVCPredicateBase@NScript@@@Z`. Unresolved in the
   queue: 00dd8ba0 CTortureDoorCourtyard (two PDB owners: pick by the vtable the row's Alloc stores, or DEFER with the
   evidence), 00dcf4c0 `CVtable_012DB4B8` (invented: find the real class from RTTI / the vtable's COL, or keep the
   descriptive `CVtable_<vt>` convention and say so).
2. **The asmre rows (22):** PDB access/mangling fixes on landed names (`CompileDefs` SIXXZ, `GetCurrentGameTime` IBENXZ,
   `SwapToHero` AAEXXZ, `GetMaximumNumberOfItems` in NInventory, `GetHighlightedItemMaxInCategory` 0x005BD993 for the
   invented GetMaxItemsPerSlot), the CStartupDefinitionManager::Get / GetDef<T> / GFAllocEngine / NResourceDirectoryNames /
   CTBaseSingleton<CConsole>::Get / GConsole* / CFrontEndManager / CIME::Get callee names. Several of their rows are still
   `asm_bake` files (0x005BD993, 0x0066FF20, 0x0042DF9E ...): respell the file name / catalog / bindings, never the asm body.
3. **Chunks 586-593 (~140 rows):** CString::operator<< for 0x00C0DEA0 / 0x00C0DEC0 / 0x00C0DF10; CEaxContext v CEAXContext
   (pick the PDB spelling); CAudioSound v CAudioObject (one object, two names: pick from the vtable's class); ImeUi
   allocator globals; the CUnknown PBD/PBG swap; 0x00CE4B70 recheck; 0x00C355E0 two spellings; CActivationReceptor part
   dtors; 00c54bd0 misnamed Reset; CHandleBasedMemoryPool::Free / InternalFreeHandle (520). Check every spelling against both
   PDB lists yourself; rows marked probable stay open without a second source.
4. **Wave 10 DEFERRED items** as budget allows (cheapest first; the mega families stay scoped only).

## Rules and hand-back (as BRIEF_W7..W10)
- Idempotent scripts plus verified file lists; a composed overlay (`ov_final/rebuild/...`) re-made from the CURRENT repo before
  the final recheck; BINDINGS.tsv (action/symbol/target/by_row/note); LABELS.tsv (proven names only); DEFERRED.tsv;
  RENAMES.tsv (git-mv proposals: the coordinator moves); DECISIONS_W11.tsv; recheck logs.
- A respelling that changes a row's leaf symbol keeps `// Leaf:` and the header comment in step; one that changes a
  relocation symbol is mirrored in BINDINGS.tsv (RETIRE the wrong spelling, ADD the right one, same target).
- **Recheck:** every changed landed row must stay MATCH/RELOCATION_MATCH + PASS under
  `tools/decomp_pipeline/linux_lane/lane_tools/vland.py --recheck-landed`, `LANDVERIFY_WORK=$S/naming/w11/lv` (delete lv dirs
  after each batch). `recheck.py <dir> <addrs>` builds a payload from the catalog (mkdir the dir first; it reads the repo, so
  run it on your overlay copy or build the payload yourself from the overlay). `bindings_multi_check.py` on your final
  bindings must print 0. Rows that are `asm_bake` or `toolchain_library` are not compiled: only their names/catalog change.
- Work only in `$S/naming/w11/`. Never run git. Never write under /home/user/FableDecomp. Never kill a process. No recursive
  find/glob over `/` or the scratchpad. `timeout` on every compile; never repeat a failing command more than twice. Leave no
  background job running.
- **Report:** rows edited per sub-wave, bindings changed, labels, renames, deferred items with reasons, recheck totals.
