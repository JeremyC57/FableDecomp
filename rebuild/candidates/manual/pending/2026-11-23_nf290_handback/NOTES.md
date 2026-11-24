# NF290 notes (2026-11-23): the last T1-access + T2 entries (1-28) and the first T1 signature entries (29-60)

Base: the repo working tree as of today (after landing nf288). I never wrote to it and ran no git.

**Numbering.** STATUS / APPLIED use my ENTRIES numbering (data lines, header excluded).
- 1 is NF288's skipped entry 3 (0x004962C0).
- 2..28 are NF288's 61..87 (NF286's 118..144).
- 29..445 are the signature batch. Reached in order: 1-60. Entries 61-445 are `open`. E2 was not used.

**Off-limits, none touched.** work/offlimits.txt has 328 addresses: NF289's ENTRIES, its locks, the RESERVED rows, T3_PORT_QUEUE and the held rows.
- My only ENTRIES line in that list is 258 (0x009F35D0, an NF289 row), which I did not reach.

**Locks.** Every compiled row was claimed with lock.sh before any compile.
- Landed tests were prechecked before their locks: clang, NOWCHAR, or D1 / D1b `cl /Zs`.
- The 5 rows I locked for re-checks (0x0087DE50 / 0x0087E250 / 0x0087E450, 0x00972CC0, 0x0093E760) had their tests prechecked first.
- Every control lock is released. rows.txt == my held locks (60).

## Method
Tools are in tools/: NF288's helpers plus defscan.py, pdbsets.py, sigdiff.py, medit.py, drectve_eq.py, pre_tests.sh, recheck.sh and gl.sh. gen_ren.py is fixed for `??` names.
1. **Exe census** (`nf.py census`): evidence/census_exe.tsv.
2. **Name sets.** Both PDB name sets hold every pdb_spelling of 1-58 and 60 as the exact string (evidence/pdb_namesets.tsv). 59 is FableWin-only, and it was skipped.
3. **Controls.** I locked and prechecked every census row and self, then compiled it at HEAD with VC7.1: 189 controls (the changed rows' HEAD objects included), all MATCH or RELOCATION_MATCH.
   - Rows whose HEAD source fails clang in its own lines were left out as controls: 0x00760020 (the C2DBox overload resolution).
   - 0x0087DE50 / 0x0087E250 / 0x0087E450 later became changed rows under D1c.
4. **Work list.** `bind.py` (evidence/binders_HEAD_*.txt) lists what each HEAD object binds at the entry, plus the self's DEF. The ENTRIES `spellings` column missed binders: 0x00A57BD0, 0x00AA0DC0, 0x00969470, 0x0097EC00 and seven GetRotationInAnimation rows.
5. **Same-TU copies.** `defscan.py` found 0x00F3D830's strong QAE StripUnusedVerts stand-in body (entry 12).
6. **Edits.** fix.py / medit.py: in place, zero line delta, orig/ = HEAD. No header, flag, catalog or test change. No changed row is `#include`d by another file (all 60 checked; the only filename hit is include/engine/RECONCILE.tsv, a list).
7. **Proof.** objsec HEAD v new is ALL_EQ for 60 / 60 rows (evidence/proof_a / b / c). Every non-debug section's bytes and relocations are equal under the renames, `.drectve` included, and the DEF sets are equal.
   - 0x0087E250 / 0x0087E450: `.drectve` differs only by the anonymous-namespace hash (`/alternatename`), because VC7.1 derives `?A0x...` from the compile path and HEAD and new compile in different work dirs. With the hash mapped it is equal (evidence/drectve_c.txt).
   - The same path hash also renamed CombatStressTestDebug in 0x00834760. That is not a real rename and is kept out of RENAMES.
8. **Links** (/FORCE, HEAD set v new set, with every control; evidence/link_a2, link_b, link_f, link_g):
   - New unresolved names are a subset of HEAD's: stand-in selves, and the decision entries 10 / 14.
   - No new duplicate.
   - One HEAD duplicate disappears: StripUnusedVerts, the self v 0x00F3D830.
   - Superseded evidence: proof_a / proof_c, link_a / link_b and recheck_final.log still include the two dropped selves 0x0049D8F0 / 0x0093E760. link_a2 (group A without 0x0049D8F0), link_f and link_g (0x0093E760 dropped, binders v the 0x00972CC0 COMDAT) supersede them.
   - 0x00760020 (entry 29) counts as binding the PDB spelling from the ENTRIES spellings column and the table, not from an object; its HEAD source fails clang in its own line, so it was not compiled.
9. **Recheck.** Two vland `--recheck-landed` runs, each with `timeout 900` on its launch line (recheck_final.log = recheck_a + recheck_b).
   - 62 / 62 MATCH / RELOCATION_MATCH, every test PASS, 0 RELOC_CONFLICT. No RELOC_NAME names a spelling I introduced.
   - The 62 include the two dropped selves 0x0049D8F0 / 0x0093E760, so 60 rows remain.
10. **Name census.** For every new self name I grepped `-w` over rebuild/src and rebuild/include and read every definition-shaped line, multi-line signatures included.
    - This found the two header-inline clashes below, and dropped those selves.
    - The other new self names are defined only by their self rows. 006ac390's new stand-in `?LowerBound@CInterfaceMap@@QBEPAUSEntry@1@ABK@Z` is unique.
    - Its pick-any `?GetInterface@CThing@@QBEPAVCTCBase@@J@Z` COMDAT has the same name as unreferenced stand-in COMDATs in 0x0076D460 / 0x0074E460. No landed row calls any CThing::GetInterface out of line: there are 0 table lines.

## Two selves dropped (a plain respell clashes with a header inline)
- **0x0049D8F0 SetAsPaused (entry 21)** and **0x0093E760 SetAsLookAtThing (entry 60).**
- **What happened.** Both self respells were MATCH and ALL_EQ. But engine/CWorld.h line 573 and engine/campfire_types.h line 95 (FABLE_RETAIL_CBRAINOUTPUT_INLINE) define the PDB symbol inline, and 0x004A2F10 carries its own local inline copy (line 185). Landed rows emit it as a pick-any COMDAT: 0x004A2F10 / 0x004A3740, and 0x00972CC0 (its HEAD object, compiled here).
- **Link result.** A strong self definition duplicates it: link_a showed 2 duplicates; link_f showed HEAD 0 v new 1.
- **What landed instead.** Only the binders moved, and they resolve against those COMDATs (link_a2, link_g: no unresolved, no duplicate). The selves need the nf86 / nf88 scaffolding port. Both edits and their HEAD twins are in evidence/prepared/.
- **A finding on the way.** Before this pass, binder 0x006292D0 bound `?SetAsPaused@CWorld@@QAEX_N@Z`. That is exactly the public symbol the libpk row 0x00C1C620 defines (the steering library's own CWorld). A whole-program link would have sent 0x006292D0's call to the wrong function. It now binds IAEX_N.

## Result (entries 1-60)
**29 done, 8 done-partial (entry 7 counted here: a land-or-drop unit), 1 done-already, 3 rejected (T3), 16 skipped-port, 3 decisions.** 58 rows are landing-ready unconditionally; 2 (0x006AC390, 0x00805890) depend on the entry-7 decision.
- 60 rows changed, with 7 self renames and 56 caller respellings (RENAMES.tsv).
- TABLE_CHANGES.tsv has 37 lines: 35 from the moved witnesses, the stale entry-5 delete, and the LowerBound stand-in replace.
- NAME_FIXES.tsv has 9 lines.

Signature entries (29-60): 15 done or done-partial (11 + 4), every one pointer↔reference, `const`, access, `void*`→`const void*`, `void`→`bool` return, or a PDB type behind a `void*`.
- Entry 35's `static_cast<CTVertexMatrixIndexedNormTex1*>(destination)` follows nf105: both sides are PDB-declared, C++ requires the conversion, and NF93 landed the same cast in that row.
- Entries 55 / 56 add `friend class CBankFileAsyncEntry;`. The ctor is private in the PDB and is called from the real class CBankFileAsyncEntry (NF281 D1).
- Entry 51 covers 11 binders in one entry. Three of them are anonymous-namespace STL helpers that clang rejects in unchanged own lines, so they took D1c `cl /Zs` (new + HEAD).

## Decisions I need
1. **Entry 7 (0x006AC390 + 0x00805890) is one unit, land or drop.**
   - The const self needs `const` on its local stand-ins GetInterface / CInterfaceMap::Find (pick-any) and on the strong same-TU stand-in body CInterfaceMap::LowerBound (0x004365B0).
   - LowerBound respells QAE→QBE. It stays a stand-in (0x004365B0 has 23 stand-in spellings), and the real Ego_r CVectorMap::LowerBound is const. TABLE_CHANGES has the `replace`.
   - 0x00805890's const must go with it, because HEAD resolved its QAE against the self.
2. **Header group: entries 10, 14, 27.** Each needs a new opt-in in a shared header, assert pins, and equiv_check plus a planted control. Not done in this pass.
   - 10: engine/CThreadedFileManager.h line 12 (only includer: the self).
   - 14: fable_user_profile.h `private: void Load();` at lines 152 / 305 (0x0040D400's copy).
   - 27: engine/CWorldMap.h, FABLE_RETAIL_CWORLDMAP_LOADFROMFILE (three users; all-or-nothing with the self and 0x00506F30).
   - Should I do them next pass, or should they go to the coordinator?
3. **Entry 2.** The prefix ruling makes `virtual` T3. R1's example ("virtual with the leading pad shrunk") would allow 0x006A98F0, whose pad 0x92 could shrink to 0x8E behind a vptr. Held; is that right?
4. **Entry 54 (self 0x009CC530 const).** It forces `const` on its local inline GetSymbol and the CArray accessor it reads. Queue it, or allow it as R1?

## Rule slips
- `sleep_never=1` appears once in a command line (a shell variable assignment, not a sleep call). There were no sleeps and no foreground waits.
- My clang prechecks ran in the foreground under `timeout 120`, as the prefix allows. Every cl / link ran in the background with `timeout 900` on its own line.
- No rm (clean.py only). Nothing was written under /home/user/FableDecomp. No donor bytes were used. No retail bytes are in the hand-back files (oracles are only in work/).
