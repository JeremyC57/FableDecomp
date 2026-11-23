# NF288 notes (2026-11-23): T1-access + T2, NF286's continuation, first hand-back

Base: the repo working tree as of today, after landing nf286. I never wrote to it and ran no git.

**Numbering.** STATUS / APPLIED use my ENTRIES numbering.
- 1 is the GetString fix in 0x00A64230.
- 2 and 3 are NF286's entries 10 and 11.
- 4..87 are NF286's 61..144: add 57 to get NF286's number.
- Reached in order: 1-60. Entries 61-87 (NF286 118-144: CThingCreatureBase SerialiseGameState through CombatDialogueManager) are `open`.

**Off-limits, none touched:** NF287's entries, rows.txt rows and locks; the Codex pass 28 RESERVED rows; T3_PORT_QUEUE; the held rows.
- work/offlimits.txt lists 371 addresses.
- Two census rows fell under it (NF287 entries) and were not compiled: 0x006D5850 (entry 8) and 0x008ED1F0 (entry 53).
- Two more were not compiled for other reasons: the self 0x0076CCE0 (entry 4) has no landed source, and the HEAD sources of 0x00766660 / 0x007677E0 fail clang in engine/CTCObjectAugmentations.h line 107, which is not D1.

**Locks:** every row was claimed with lock.sh before any compile (work/locks.log).
- Controls were released (work/release.log).
- The 70 changed rows stay NF288 (rows.txt == my held locks).

## Method (tools/; nf.py inherited from NF286, the rest new)
1. **Exe census.** `nf.py census` gives the landed rows calling or addressing each entry (evidence/census_exe.tsv). Both PDB name sets hold every pdb_spelling (evidence/pdb_namesets.tsv).
2. **HEAD controls.** The self row and every census row of an entry were locked, prechecked and compiled (VC7.1 HEAD, `grade_lane.sh`, 4 lanes). Every control graded MATCH / RELOCATION_MATCH.
3. **Binding read-out.** `bind.py` (evidence/binders_HEAD.txt) lists, for each object, the symbol bound at the entry address (xr against the retail bytes) and, for a self, the defined spelling. The work list came from this output, not from the reloc table or the source census.
4. **Same-TU definitions.** I checked every binder object for a same-TU definition of the entry's method (DEF scan). It found 0x00560D00, 0x0058509F and 0x0066BD00 / 0x0066BDF0.
5. **Edits** (fix.py, rep.py):
   - every edit is in place in each row's own local model, with zero line delta;
   - orig/ = HEAD copies;
   - no changed row is `#include`d by another file;
   - no header, flag or catalog change.
6. **Prechecks** (work/pre.log, work/pre_zs.log, work/cpp.log):
   - landed tests were prechecked before their locks;
   - sources were prechecked after their locks, in an earlier call than any compile.
   - clang passed everywhere except these:
     - NOWCHAR (row-local `wchar16` / `fable_string.h` wide compare): controls 0x00561980 / 0x005621B0 / 0x0041E6D3 / 0x0058578B, and the changed row 0x0059A238 (new + HEAD; entry 2);
     - D1 `cl /Zs`, after a gcc cpp `#error` pass: the unchanged tests of 0x006C3E80 (VC7.1 `<cwchar>`) and 0x0065B6DD (VC7.1 `<functional>` / `<cwchar>`). Also, under D1b, the unchanged tests of 0x0059A238 / 0x004962C0 (before their locks).
7. **Proof** (evidence/proof_p1/g2/g3/p4.txt): objsec HEAD v new is ALL_EQ for 70 / 70 rows.
   - Every non-debug section, `.drectve` included, is equal under the symbol renames, and the DEF sets are equal.
   - `gen_ren.py` paired every object's symbol changes with one method each. The only extra pair is the local CLongVector COMDAT of entry 19.
   - Fourteen objects emit tables (C3DMeshObject, CIntelligentPointer instances, CBuildingPointer, CMessageClearPrimitives). All are byte- and relocation-identical.
8. **Links** (evidence/link_g2.txt, link_g3.txt, link_g4.txt): HEAD set v new set, each with the unchanged controls. Entry 1 needs no link: HEAD resolves GetSymbol, but against the wrong function, so the proof is the xr read-out (evidence/entry1_xr.txt).
   - g4 (entry 2): HEAD leaves `...QAEJXZ` unresolved, and the new set resolves IAE against the self.
   - New unresolved names are a subset of HEAD's. They are stand-in selves, selves not compiled, and the untouched decision entries.
   - Duplicates are equal on both sides, except that g2 loses one: 0x0058509F's QAE copy v the old QAE self (entry 19).
9. **Recheck** (recheck_final.log; three vland `--recheck-landed` runs, `timeout 900` on the launch line): 70 / 70 MATCH / RELOCATION_MATCH PASS, 0 RELOC_CONFLICT.
   - No RELOC_NAME names a spelling I introduced.
   - The other warnings were already there: SHARED_HEADER_AVAILABLE, STANDIN_LEAF on the unchanged vfunc leaves of 0x007D4A60 / 0x007D4A90, and RELOC_ALIAS on import thunks.
10. **Name census** (evidence/name_census.txt): each of the 14 new self spellings is defined only by its self row.
    - The census is three checks: a grep for out-of-class `Class::Method(` definitions in rebuild/src and rebuild/include; a grep for in-class bodies of the 14 names (no hits); and the DEF scan of every census-row object.
    - The one exception is entry 19's name: 0x0058509F keeps a strong copy under the OLD spelling (QAE), so it is not a clash.
    - 0x005D95C0's GetDescription is a static overload with another signature.

## Result (entries 1-60)
**45 done, 6 done-partial (19 among them is a land-or-drop decision), 3 rejected (T3), 4 decisions, 1 skipped-precheck, 1 skipped-held.**
- 70 rows changed, with 14 self renames and 60 caller respellings (RENAMES.tsv).
- TABLE_CHANGES.tsv has 44 lines.
- NAME_FIXES.tsv has 12 findings.

### Entry 1 (the brief's first item)
- **The fix.** 0x00A64230's local CParsedItem declared only GetSymbol. It now declares GetString, and both calls are GetString (lines 11 / 22 / 24, comment corrected).
- **HEAD v new.** The HEAD object bound GetSymbol at both retail 0x009B9880 sites; the new one binds GetString x2, and objsec is ALL_EQ.
- **The test changed.** It is a row copy plus stubs: lines 200 / 211 / 213 and the stub at line 234 now say GetString. It still counts the calls, and its counter keeps the name `Symbols`. It passes clang and the recheck.
- **Census** (my own HEAD objects): the 9 other rows calling 0x009B9880 bind GetString, and all 8 rows calling 0x009B9870 bind GetSymbol. No other row binds GetSymbol at a GetString site. reloc_bindings has no GetSymbol -> 009b9880 line.

### Wider-than-access fixes (byte-identical, recorded per row)
- **Entry 7, 0x007E7AC0:** `const` removed, PDB QAE.
- **Entry 32, 0x00753C70:** `private:` + `const`.
- **Entry 60, 0x00830360:** `protected:` on a base-class declaration called by the derived class.
- **Entry 19, 0x00582027:** the const self body needs a const `operator[]` on the row's local CLongVector stand-in. I added it on the existing operator[] line. That pick-any local COMDAT respells (`??ACLongVector@@QAEAAJK@Z` -> `QBEABJK`); its bytes are equal and it has no table line.

## Decisions I need
1. **Entry 3 (NF286's 11).**
   - **Why it is still blocked.** D1b passed its unchanged test (`cl /Zs`), but the CHANGED self source 0x004962C0 fails clang in its own line 46, `unsigned int(...)` (a functional cast with a two-word type), exactly as at HEAD. NOWCHAR does not help there.
   - **Where the edit is.** It is ready in work/prepared/004962c0.cpp (line 78 `private: ... public:`, Leaf AAEXJ, HEAD copy beside it) and was not compiled.
   - **The question.** May a changed source whose only clang failure is an UNCHANGED own line, identical at HEAD, take `cl /Zs`?
   - **Entry 2 (NF286's 10) is done.** Its changed source fails clang only in line 369 (`CWideString + L" "`) and passes NOWCHAR, as 0x00561980 does.
1b. **Entry 19, land or drop.** The CLongVector const `operator[]` model change is wider than R1's examples. The spelling count stays 2 -> 2, because QAE shrinks to 0x0058509F's own copy.
2. **Entries 20 / 43 / 47 / 49 (`virtual`).** The QAE binders' local models either lack a vptr (43: adding one changes the layout) or carry placeholder slot runs (49), so `virtual` changes a table. 20 and 47 also have stand-in selves (`CTCInventoryBase_Retail` with a float parameter, `C005e9aa0`). T3, as NF286's 40 / 42?
3. **Entry 8.** The self 0x006D5260 defines QAE. The only QAE binder, 0x006D5850, is NF287's entry. Moving the self alone would leave it unresolved. Should the self move (line 85 `protected:`) land with NF287's row, or be queued?

## Rule slips
- **One `sleep 0`.** I ran `sleep 0 2>/dev/null` once, at the start of a status check (00:45, a no-op while prechecks ran). It is still a sleep call, against the rule. There were no other waits.
- **Control locks before test prechecks.** I first claimed the group-2 and group-3 rows as HEAD-only controls, which compile no test. When some of them became changed rows, I released those locks, prechecked their landed tests, and re-claimed the locks before any compile of the new sources.
- **Foreground prechecks.** My clang prechecks of landed tests (and the NOWCHAR retries) ran in the foreground, each under `timeout 120`. Every compile with cl / link ran in the background.
- None other that I know of:
  - every grade, `/Zs` and recheck ran in the background with `timeout 900` on its own line;
  - no rm (clean.py only);
  - nothing was written under /home/user/FableDecomp;
  - no donor-build bytes were used;
  - no retail bytes are in the hand-back files (the oracles are only in work/).
