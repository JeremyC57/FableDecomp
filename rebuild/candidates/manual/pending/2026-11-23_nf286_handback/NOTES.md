# NF286 notes (2026-11-23, restarted): T1-access + T2, first hand-back after the container restart

The batch was redone from the repo tree after the 2026-11-23 restart wiped the first run; nothing from that run is on disk. Base: the
repo working tree (never written, no git).

**Off-limits, none touched:**
- NF287's entries and locks;
- the Codex pass 28 RESERVED rows;
- the T3_PORT_QUEUE addresses;
- the held rows the standing brief lists.

work/offlimits.txt has 360 addresses; intersected with the 147 rows I locked, it is empty. 0x006345C0 and 0x007310C0 are NF287 entries,
so they were not used as controls; I settled their binding by source instead.

**Locks:** every row was claimed with the shared lock.sh before any compile (work/locks.log). The 95 controls were then released
(work/release.log). The 52 changed rows stay NF286 and are listed in rows.txt.

## Reading list
The following were read in full:
- the prefix and BRIEF_NF_BATCH.md;
- CLAUDE.md, docs/pipeline/ASM_RULES.md, EXTRA_RULES.md and docs/COORDINATION.md;
- the nf281_282, nf283, nf284 and nf285 landing READMEs;
- NF283's and NF281's NOTES / LESSONS / STATUS.

GOTCHAS.md was not read in full. See the rule slips at the end.

## Method
1. **Census before any edit, with no unlocked compile.**
   - Exe scan of every E8/E9 and absolute reference to each entry address, mapped to landed rows (`tools/nf.py census` ->
     work/census.tsv).
   - Source census of the self row and every referencing row (`nf.py scen` -> work/scen_1_60.txt). It records each declaration line,
     the access in effect (a label on the same line first), and const / virtual. For a row that only calls the method, it records the
     shared-header declaration instead.
   - Both PDB name sets agree on every pdb_spelling reached.
   - Object level: the VC7.1 HEAD half of every changed row, plus 95 locked controls (other referencing rows and the self rows), built
     with `nf.py hgrade`. Their relocations were resolved against the retail bytes (`nf.py xr` -> work/xr_ctl.tsv). No control binds any
     old spelling.
2. **Edits** (tools/fix.py, rep.py): in place, zero line delta. Every changed row has HEAD's line count. orig/ holds the HEAD copies, each
   equal to the repo file at its catalog path. No changed row is `#include`d by another file.
3. **Prechecks** (work/pre.log, work/pre_zs.log). Landed tests were prechecked before their locks; sources after their locks, in an
   earlier call than any compile.
   - **clang:** every new and HEAD source of the 52 rows and all 95 controls, except those below.
   - **clang NOWCHAR:** 0x00A5B710 / 0x00A5B760 (new and HEAD) and controls 0x0044A2A0 / 0x0048F939.
   - **D1 `cl /Zs`, sources:** 0x00A5B710 / 0x00A5B760 (new and HEAD), as well as NOWCHAR. Controls 0x00859EC0 / 0x00859EF0, whose only
     clang error is the CTCFlourishTarget.h FABLE_STATIC_ASSERT typedef.
   - **D1 `cl /Zs`, landed tests:**
     - 0x00BCFDE0 / 0x00BCFCB0 / 0x00B9DE90: the only clang errors are `_assert`;
     - 0x00B82100 / 0x00B619B0 / 0x0069A990 / 0x009EB430 / 0x00634190 / 0x0043A080 / 0x00659C7E: the only clang errors are in VC7.1
       `<cwchar>` / `<functional>` / iostream.
   - **All other landed tests** passed clang.
   - **The /Zs tool** (`nf.py zs`, tools/zsq.sh) feeds the verifier's vc71() text with the row's flags, or /Od /W3 for tests. Each check
     runs under `timeout 900` on its own line, in the background. It was validated: a planted syntax error gives ZS_FAIL.
4. **Grades** (`nf.py grade`, tools/run_grades.sh): four background lanes, each grade command on its own `timeout 900` line.
   - 104 HEAD / new objects of the 52 rows: all RELOCATION_MATCH.
   - 95 controls: all MATCH or RELOCATION_MATCH.
5. **Proof** (`nf.py proof` -> evidence/proof_all.txt): every non-debug section's bytes, flags and relocations are equal under the
   symbol renames, `.drectve` included, and the DEF sets are equal. ALL_EQ for 52 / 52.
   - Each object's rename set equals the RENAMES lines for that row exactly (0 mismatches).
   - Six rows emit tables: 0x00A5B710 / 0x00A5B760 (CAFile, CCachedFile), 0x008BA3C0 / 0x00915890 / 0x0048A210 (CIntelligentPointer
     instances), 0x00686E10 (CStringParser). Their ??_7 sections are byte- and relocation-identical under objsec. No table appears or
     changes.
6. **Links** (evidence/link_g1..g3.txt; `nf.py link`, tools/links.sh).
   - Each HEAD set leaves the wrong spellings unresolved, and the new set resolves them.
   - Duplicates are equal on both sides (g1 3/3, g2 3/3, g3 1/1). All are pre-existing same-TU copies.
   - Left unresolved in the new sets, as at HEAD, because the definer is a stand-in self or outside the batch: ShouldSpawn /
     DatabaseClone (NParticleEngine), (Un)InitActiveMode AAE (the selves define `...QAEXJ@Z`), Close IAE (CTCDoor_Methods), IsQuestActive
     QBE (0x004AF610), CPhysicsMesh::IsInContainmentVolume, ??0CEngineClothSimulationMeshBase, InitialiseInput.
   - IsMainPlayer QBE resolves in both sets only through 0x0048A210's pick-any in-class copy (HEAD debt, NAME_FIXES).
7. **Recheck** (recheck_final.log; tools/recheck_final.sh, one vland --recheck-landed run, `timeout 900` on its line).
   - 52 / 52 RELOCATION_MATCH PASS, 0 RELOC_CONFLICT.
   - No RELOC_NAME names a new spelling.
   - The other warnings were already there: 260 SHARED_HEADER_AVAILABLE and 1 STANDIN_LEAF (0x006A7B20's own leaf, unchanged).
8. **Name census** (evidence/name_census.txt): each of the 9 new self spellings is defined only by its self row. The one in-class hit,
   0x0076CC00, is a declaration whose object has the symbol UND.

## Result (ENTRIES 1-60 reached in order; 61-144 open, not started)
- **49 done, 2 done-already (41 by source, 44), 5 rejected (9, 13, 14, 21, 50; all T3), 2 decisions (40, 42), 2 skipped-precheck
  (10, 11).**
- **The 12 retries:**
  - done: 1-8 and 12;
  - rejected: 9;
  - skipped: 10 and 11. Their landed tests fail clang in their own lines, so neither D1 nor E1 covers them.
- **Changes:**
  - 52 rows changed, with 9 self-leaf renames and 53 caller respellings (RENAMES.tsv);
  - TABLE_CHANGES.tsv has 45 lines, including 5 stale-witness lines (entries 2, 27, 28, 41, 44);
  - NAME_FIXES.tsv has 13 findings outside these rows;
  - no header, flag, test or catalog change.

## Per-row decisions (land or drop)
- **Entry 1 (0x00A5B710 / 0x00A5B760):** both rows rest on D1's `short wchar_t` covering their row-local typedef. They pass clang NOWCHAR
  and `cl /Zs`. Drop both if that reading is wrong.
- **Entry 23 (0x0042E3EE, T2 UAE -> AAE): a `virtual` removal, not one of R1's kinds.**
  - The local model's last virtual (slot 1) becomes `private: void Input(); public:`.
  - The TU has no ctor or dtor, and neither object has a ??_7.
  - Fable.exe holds no absolute reference to 0x0042E3EE, so it is in no vtable; both PDBs say AAE.
- **Entry 43 (0x00B63010): access widened**, private -> public; both PDBs say QAE. The census covers all 54 referencing rows (STATUS).
- **Wider fixes as ruled:**
  - entries 3 / 4 / 6: the ctor parameter goes `unsigned int` -> `unsigned long` (both PDBs K, and the selves define K);
  - entry 38: `const` on the local callee IsQuestActive, which the PDB-const body forces (both PDBs QBE; already bound there).

## Decisions I need
- **Entries 40 / 42 (script base FollowPreCalculatedRoute UAE / IsPerformingScriptTask UBE):** the binders' local models emit ??_7
  (0x00DDC980's inline ctor), so `virtual` changes a table. Entry 40's self is also an int-parameter stand-in. Port the script rows onto
  one shared model with the retail slot order (T3)?
- **Entries 13 / 21 (CKeyboardDX / CMouseDX):** confirm they join the D2 T3 queue (the ctor models emit ??_7 with no base slot).

## Rule slips
- **GOTCHAS.md** was not read word for word in this run. In the first run, before the restart, I read lines 1-3 and 433-510 in full,
  lines 4-432 cut to their first 110-140 characters, and the keyword lines in full.
- Otherwise none that I know of:
  - every compiled source and every changed row's landed test had a passing precheck in an earlier call, after the lock for sources and
    before it for tests;
  - every grade, /Zs and recheck ran in the background with `timeout 900` on its own line;
  - no rm (clean.py only);
  - no git; nothing written under /home/user/FableDecomp;
  - no donor-build bytes used.
