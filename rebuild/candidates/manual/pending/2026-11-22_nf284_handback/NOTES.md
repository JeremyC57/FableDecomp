# NF284 notes (2026-11-22, round 35b): T1 standin_alias continuation of NF282, hand-back 1 (CCountedPointer DeleteData, 31 addresses)

## What was done
- **31 addresses done** (work/done.tsv = NF282's work/sel_all.tsv lines 61-91, STATUS.tsv `done`): 0x00B2DE20, 0x00B36060..0x00B361B0 (22),
  0x00B4B020..0x00B4B060 (5), 0x00B94F90, 0x00B95000, 0x00BB6330. Each is a `CCountedPointer<T>::DeleteData` deleter whose only stand-in
  binder was a generated Reset row (local `SCountBlock` / `CCountedPointer_<t>` model, free `?Destroy_<t>@@YIXPAX@Z`). 31 binder rows
  respelled (rows/), one per address, with NF282's transformer tools/xf_ccp.py unchanged in its logic (coordinator ruling D1: the hybrid
  form). Every T here is a global class (`V<Name>@@`, no scope), forward-declared `class T;`.
- Each row gains a `// Leaf:` line on line 2 pinned from the HEAD object's defined function symbol (D3), the stand-in leaf
  `?Reset@CCountedPointer_<t>@@QAEXPAX@Z`, unchanged.
- No header changed (hdr/ empty), no `__asm` / `naked` / `_emit` / `volatile`; tests unchanged (none names the stand-in).

## Proof (evidence/)
- `ccp_identity31.tsv` (tools/ccp_ident.py): per target, the PDB spelling is in the FableWin.pdb name set (31/31; not in Ego_r's name
  set); the binder's retail extent holds the target as an imm32 (31/31, one store each); the real-typed caller binding of each binder in
  reloc_bindings.tsv names the same T (31/31 `agree`: 0x00B33B50 binds all 22 CEnginePrimitiveRenderer manager Resets, 0x00B4AC10 the five
  2D ones, 0x00B2C460 / 0x00B949A0 / 0x00B94BF0 / 0x00BB2540 the rest); 30 owner rows carry the PDB spelling as Leaf and a Names line
  naming T (FAM10). Exe scan of landed referencers: each target's only landed referencer is its binder, except 0x00B94F90, which
  CEngineSceneMap::ResizeLevels 0x00B94540 also stores (real-typed, shared engine/CCountedPointer.h, already the PDB spelling), plus one
  site in no landed row (0x00B959C6). Direct calls / jumps: `evidence/ccp_xsites31.tsv` (tools/xsites.py over the 31 targets) is
  empty, so the imm32 stores are every code reference.
- `rename_check31.txt`: per row exactly one symbol rename, `?Destroy_<t>@@YIXPAX@Z` = ENTRIES column 7 (31/31; checked by string, not
  only by the objects' own symbol diff).
- `proof_n31.txt` (tools/proof.sh): objsec HEAD v new object under that rename: every non-debug section, bytes and relocation lists,
  equal (ALL_EQ, .drectve included); rename-aware cmpobj: both function COMDATs SAME (leaf + pick-any `??0SCountBlock@@QAE@PAX@Z`);
  no `??_7` / `??_G` in either object (no table, vtcheck n/a). DEF lists identical; the only UND change is the one rename (31/31).
- `link_c31.txt`: 31 HEAD binders + 31 owner rows compiled at HEAD v 31 new binders + the same owners (LINK cleared, /FORCE): HEAD leaves
  all 31 `?Destroy_<t>` references unresolved; the new set resolves 30 of the 31 PDB references against their owners and leaves one,
  `?DeleteData@?$CCountedPointer@VCEngineSceneGrid@@@@SIXPAX@Z` (0x00B94F90's self row defines the plain stand-in
  `?DeleteData@@YIXPAUAutoTinyVirtualObject@@@Z`; T3, NAME_FIXES). Duplicates 0 in both sets.
- `ccp_name_census31.tsv` (tools/ccp_census.py): every reloc_bindings line of the stand-in and of the PDB spelling targets the entry's own
  address (31/31); a Leaf naming the PDB spelling exists only in the owner row (30) or nowhere (0x00B94F90). No object newly defines a
  symbol, so there are no new symbol names to census.
- Owner rows (the 31 targets) compiled at HEAD for the link check: 31/31 MATCH (work/hc/grade_<t>.txt).
- recheck_final.log: vland --recheck-landed over the 31 changed rows (runs rf_f1 16 rows + rf_f2 15 rows, each `timeout 900` on its own
  line; rf_f1 took under a minute), landed tests unchanged: see the tally at the end of this file.
- Staleness: orig/<b>.cpp equals the repo source found by address in build_candidates.ps1 (31/31, cmp, at the end of the pass); the 31
  binder locks read NF284 (rows.txt); the 31 owner locks I claimed for the link check are back to `released`.

## Spelling counts
- 30 addresses: table spellings 1 (stand-in) + owner Leaf (PDB) -> PDB only (2 -> 1). 0x00B94F90: table PDB (0x00B94540) + stand-in
  -> PDB only; its self row's Leaf is the plain `DeleteData` (the tracker counts it as agreeing; its object defines the AutoTinyVirtualObject
  stand-in, a T3 port, as NF282's 0x0067F650 / 0x00A110D0 / 0x00A99FE0 / 0x00A9CB20). No address grows; no new split.

## TABLE_CHANGES.tsv
31 stand-in lines deleted (every binder moved); 30 PDB lines added (the PDB spelling had no table line; witness = the respelled binder).
0x00B94F90 already has its PDB line (witness 0x00B94540).

## NAME_FIXES.tsv (T3 ports, not respells to apply at landing; D2)
31 binder self names (`?Reset@?$CCountedPointer@<T>@@@@QAEXPAV<T>@Z`, FableWin.pdb name set for all 31, real-typed callers bind it) and
0x00B94F90's self row. These are pre-existing self-v-callers splits (the binder's own leaf is the stand-in `CCountedPointer_<t>`).

## Open (APPLIED `open`)
- The deferred CCountedPointer entries (NF282 work/deferred.tsv): 0x004850F9 / 0x0048AFB0 (ReleaseData stand-ins), 0x0056C0F0 (6 binders),
  0x005ED2C0 / 0x0065D5A0 / 0x006E46F0 (destructor stand-ins), 0x006174F0 / 0x00AC3A10 (template T), 0x007180B0 / 0x007222B0 / 0x00B230E0
  (nested T), 0x0085A5A0 (2 binders). Not started.
- All non-CCountedPointer classes of ENTRIES (316 entries). Not started.

## Rule notes / slips
- Slip: one early command wrote `/tmp/x283` (a copy of NF283's address list, harmless) outside my folder.
- Every grade ran with `timeout 900` on its own line (hk.sh / nc.sh -> sc.sh; rf.sh for vland), launched in the background and checked
  with kill -0 in a later call; every compiled source (31 HEAD binders, 31 new binders, 31 owners, 31 unchanged tests) had its own passing
  clang precheck in an earlier call after its lock was mine (work/pre_head31.log, pre_new31.log, pre_own31.log, pre_test31.log).

## Recheck tally (recheck_final.log = rf_f1 + rf_f2 concatenated)
31 RELOCATION_MATCH PASS, 0 DIFFER / FAIL / RELOC_CONFLICT. Non-blocking: 31 WARN STANDIN_LEAF on the binders' own stand-in leaves (visible
through the pinned Leaf lines; T3), and 1 WARN RELOC_NAME on 0x00B95A30 (its callee 0x00B94F90's CATALOG label is
`_global::DeleteViaVtable0_00b94f90`; NAME_FIXES.tsv carries the PDB name for that row's T3 port).

## Why the hand-back stops at 31, and a scout of the next entry (ENTRIES line 1, 0x009E1DD0, not started)
Context was heavy after the reading list, so I stopped at the 31 the coordinator ruled on, with every file current. Scout of entry 1 for
the next agent (no file changed, no lock taken): `?GFAYToVY@@YIMM@Z` 0x009E1DD0 has 29 landed referencing rows (evidence/scout_009e1dd0_xsites.tsv:
119 E8/E9 call sites, 97 in landed rows, 22 in no landed row). A source grep finds TWO stand-in spellings, not the one ENTRIES lists: 0x0048F50C declares `float __stdcall
GFAYToVY_009e1dd0(float)` (`?GFAYToVY_009e1dd0@@YGMM@Z`, the table witness) and 0x00437210 / 0x0043E4D0 declare `float __fastcall
GFAYToVY_009e1dd0(float)` (`?GFAYToVY_009e1dd0@@YIMM@Z`, no table line). engine/CFontBank.h already declares `float __fastcall
GFAYToVY(float y);` (0x009E1DD0). The stdcall-to-fastcall move should be byte-neutral (a float argument goes on the stack and the callee
pops it in both conventions) but must be proven per object. Four landed tests (0x0048F50C, 0x0041A980, 0x0043E4D0, 0x00437210) define
their own stub under the stand-in name; they are standalone models and need no change unless the coordinator wants them respelled.
The other 26 landed referencing rows must be censused (source grep + object symbols) before the entry can close.

## Decisions needed
1. 0x00B94F90's self row (`DeleteData(AutoTinyVirtualObject*)`, catalog `_global::DeleteViaVtable0_00b94f90`) leaves the PDB reference of
   both its binders (0x00B95A30 respelled here, 0x00B94540 at HEAD) unresolved in a link: queue its port onto engine/CCountedPointer.h
   (explicit instantiation, as the other 30 owners) with NF282's four AutoTinyVirtualObject rows as one T3 unit?
2. Test-local stubs under stand-in names (the four GFAYToVY tests above, and generally): respell them with their rows, or leave
   standalone tests untouched (NF282 and I left them)?
3. Entry 1 (0x009E1DD0) has a second stand-in spelling with no table line (`?GFAYToVY_009e1dd0@@YIMM@Z`, 0x00437210 / 0x0043E4D0): the
   tracker sees only the table witness, so its spelling count is understated. OK for the next agent to treat both as one entry?
4. Order for the next agent: the 12 deferred CCountedPointer entries (each needs more than the transformer) or ENTRIES order from line 1?
