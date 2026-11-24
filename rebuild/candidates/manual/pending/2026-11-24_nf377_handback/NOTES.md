# NF377: NF374 follow-ups (Account A), hand-back notes

Ten rows changed. All ten ACCEPT: nine in batches/b1 (eight binders plus the 00bfd0b0 self) and one in batches/b2 (binder 00bfc770).
Compiles: 43 grades. That is runpair r1 (10 rows HEAD + new = 20, plus 9 callee and context rows HEAD) plus 14 HEAD census compiles (c1 11, c2 3).
Nothing was written to the repo and no git was run.

## Batches

### batches/b1: land with land_scripted

| Row | Kind | Old symbol | New symbol | Table |
|---|---|---|---|---|
| 0046b0d6 | binder, target 0044fc00 | TransferBinaryIn(CMemoryDataInputStream&) | `?TransferBinaryIn@CDefString@@QAEXAAVCDataInputStream@@@Z` | HAND rewitness to 0046b545 |
| 00786700 | binder, target 00a455a0 | ctor(C3DSkeletalMorphTarget*) | `??0C3DSkeletalMorphParameter@@QAE@ABVC3DSkeletalMorphTarget@@@Z` | delete (last binder; PDB line exists) |
| 00752640 | binder, target 005dc340 | GetAnimation (QBEPBV) | `?GetAnim@CAnimationSet@@QBEABV...` | HAND rewitness to 007502d0 |
| 007fdf60 | binder, target 00a01b90 | SetPItem(CThing*) | `?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z` | HAND rewitness to 008c0900 |
| 0052ac10 | binder, target 005296b0 | `??0CThingPhysical@@QAE@J@Z` | `??0CThingPhysical@@QAE@K@Z` | HAND rewitness to 0052ac90 |
| 00a9b740 | binder, target 00a98910 | `??1CTemporaryBuffer@@QAE@XZ` | `??1CTemporaryBuffer@@UAE@XZ` | HAND rewitness to 00bde860 |
| 00950c30 | binder, target 0097e200 | `?AIAction_DoNothing@CAIStateGroup_JackDragon@@QAEJXZ` | `?AIAction_DoNothing@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@XZ` | delete (last binder; PDB line exists) |
| 00bf8c80 | binder, target 00bf6e20, own ctor too | TreeNode(JJJ); own SplitBranch(JJJ) | the PDB `(CEngineLandscapeMap*, CLandscapeBackgroundTreeNode*, long)` forms of both | replace (last binder) |
| 00bfd0b0 | self (NAME_FIXES of NF374) | SaveVerts<CFanBaseVertex> AAE | `??$SaveVerts@UCWaterFanBaseVertex@...AAEXAAV...AAVCDataOutputStream@@@Z` | none |

- Every change is a declaration change: a parameter or return type, a cast at the call (+0), or a member moved to its PDB class. Every row
  keeps its local model and follows the precedent of a landed sibling. NF374 handled 004671ca / 0046abd6, 007864c0, 007fdeb0, 00a99510 and
  009224f0 / 00950950 the same way.
- **00bf8c80** moves its own ctor as well, because it forwards its three arguments (NF304 Q6, as NF374's 007bb150). The PDB SplitBranch
  spelling is already a table line, bound by its caller 00bf8030, and the link now resolves it. Both retail callers (00bf8030, 00bf84c0)
  compiled. Both bind the PDB form and neither binds the JJJ self.
- **00bfd0b0** passes the lander's own checks:
  - the new symbol is in both PDB name sets (grep -Fx, 1/1);
  - `V.obj_text(new obj, new symbol)[2]` returns it.

  At HEAD, the 00bfd0b0 and 00bfcf90 objects both define the CFanBaseVertex spelling. It is a silent COMDAT double, and the HEAD map
  resolved it to 00bfd0b0.obj. In the new build only 00bfcf90 defines it (evidence/self_00bfd0b0.txt).
- **Test:** batches/b1/tests/00bfd0b0_test.cpp renames the test's struct to CWaterFanBaseVertex. The new vland PASS used it. It is a name-only
  change; the landed test passes too. land_scripted does not carry tests, so copy it by hand if you want it.

### batches/b2: binder 00bfc770 (land after b1's 00bfd0b0)
- The row calls all four SaveVerts by the PDB private by-reference spelling.
- The four QAE/PAV table lines become `replace` lines. 00bfc770 is the only retail caller of 0x00BFCF00 / CF90 / D020 / D0B0 (exe scan).
- It depends on 00bfd0b0: until that lands, 00bfd0b0's COMDAT still defines the CFanBaseVertex spelling, and nothing defines the
  CWaterFanBaseVertex one.
- **.drectve** (advisor ruling, met in derived form; evidence/drectve_00bfc770.txt). The section is not byte-equal; it is 1398 bytes either way.
  - Lines: 6 at HEAD and 6 new, in the same order. The two /DEFAULTLIB lines are byte-equal.
  - Left operands: each of the four /alternatename left operands is the row's objsec rename pair.
  - Right operands: these are VC7.1's derived non-template fallbacks, not object symbols. On both sides each one equals its left operand
    with `??$SaveVerts@<arg>@@` collapsed to `?SaveVerts@`.
  - None of the eight fallback names is a DEF in any of the 86 compiled objects, a reloc_bindings symbol or a self_symbols symbol.
  - The HEAD link resolved none of the four, because the old primaries and fallbacks are undefined. The new link resolves each new
    primary to the object at the target the HEAD table bound the old one to (00bfcf00 / 00bfcf90 / 00bfd020 / 00bfd0b0); xr agrees.

## Gates (every row)
- **vland:** HEAD and new both RELOCATION_MATCH with the test PASS, at one LANDVERIFY_WORK path (logs/ in each batch).
- **objsec374:** ALL_EQ under the intended renames only. `.drectve` is BYTE_EQUAL in all rows except 00bfc770 (above), and the COMDAT counts
  are equal.
- **pk_all + pickany_vs_retail:** HEAD equals new under the renames for 10/10 rows, with 0 reach flips (evidence/pkdiff_r1.txt,
  reachflip_r1.txt).
- **/FORCE /MAP link** (evidence/link_all.txt): the 10 changed rows, plus the callee rows 0044fc00 00a455a0 005296b0 00a98910 00bf6e20
  00bfcf90 00bfcf00 00bfd020 00bf8030, plus 00bf84c0 and 00bdc180.
  - 0 duplicates, and nothing HEAD resolved becomes unresolved.
  - The JJJ SplitBranch self is gone, but nothing references it.
  - Three new names stay unresolved, each only on the PDB name where no owner row is landed (NF302 Q2): GetAnim 005dc340, SetPItem 00a01b90,
    AIAction_DoNothing 0097e200. Their old names were unresolved at HEAD too.
  - These now resolve: TransferBinaryIn (0044fc00), the C3DSkeletalMorphParameter ctor, CThingPhysical(K), ~CTemporaryBuffer UAE, the TreeNode
    ctor, the SplitBranch ctor (00bf8030's call) and the four SaveVerts.
- **apply_table.py --dry** (evidence/apply_table_dry.txt): run on b1 TABLE, b1 HAND, b2 TABLE and all three combined. 0 misses;
  multi-target 1 before and 1 after, with no new one.
- **Names:** every new spelling is in the Ego_r and FableWin name sets. Every new callee spelling except GetAnim, SetPItem and DoNothing is also
  the landed self of its target.

## Census (compile, not grep) for each old spelling
Candidates are the landed rows whose retail body calls the target (tools/callers377.py exe scan), narrowed by a scoped declaration scan
(tools/decl377.py). Every candidate was compiled at HEAD (tools/headonly.sh), and its external symbols decide the result
(evidence/census_c1.txt). Witness relocations were then resolved against the retail bytes (evidence/xr.txt).

| Old spelling | Compiled | Binds old | Table action |
|---|---|---|---|
| TransferBinaryIn(CMemoryDataInputStream&) | 0046b545 | yes (xr 0044fc00) | rewitness (004e9e0b, 006dba50 also declare it; not compiled) |
| C3DSkeletalMorphParameter(PAV) | 007864c0 (only other caller) | no | delete |
| GetAnimation | 007502d0 | yes | rewitness (0041e6d3 007306c0 00730c00 00730dd0 also declare it) |
| SetPItem(CThing*) | 008c0900 (read-only; NF375 holds its lock) | yes, x2 | rewitness. Of 42 global-scope declarers, it is the only one that also calls 0x00A01B90 in retail; 007cc060 declares it and does not bind it |
| CThingPhysical(long) | 0052ac90 | yes | rewitness (0052ad50, 00666090 also declare it) |
| JackDragon::AIAction_DoNothing | 00950950 00952890 00950cf0 0094feb0 0094fe30 | none | delete; no include/ file declares it on JackDragon |
| ~CTemporaryBuffer QAE | 00bde860 | yes | rewitness (00be8920 via a candidates header) |
| TreeNode(JJJ) | 00bdc180 (only other caller) | no (binds PAX) | replace |
| SplitBranch(JJJ) self | 00bf8030, 00bf84c0 (only callers) | no | none (no table line) |

## Lander holds: coordinator action needed
land_scripted holds any address in column 2 of T3_PORT_QUEUE.tsv, and two b1 rows are there. Both lines were queued by NF374 for the
address as a *target*:
- **0x00A9B740** (line `nf374 0x00A9B740 port`) is about binder 0x00A25D20's stand-in pool type. It is unrelated to this change, and that line
  stays open. Land 00a9b740 by hand, or with the line moved aside for this landing.
- **0x00BFD0B0** (line `nf374 0x00BFD0B0 standin_alias port`, binder 0x00BFC770) is resolved by b1 + b2. Close it when both land.

b2 (00bfc770) needs 00bfd0b0 landed first. If 00bfd0b0 is held, hold b2 too.
None of the ten rows is in an open claim other than NF377's own, or in NF376's set.

## Hand steps
1. Land b1. Then apply b1/HAND_TABLE_CHANGES.tsv, keeping the rewitness lines whose binder (named in the note) landed. Land b2 after
   00bfd0b0.
2. NAME_FIXES.tsv covers two things:
   - 0x00BFD0B0: the catalog label and the source and test file names still say CFanBaseVertex.
   - 0x005DC340: the GetAnimation spelling is still declared by five landed rows.
3. Optional: copy b1/tests/00bfd0b0_test.cpp.
4. QUEUE_PROPOSALS.tsv proposes two T1 binder targets: 005dc340 GetAnimation declarers and 00a98910 non-virtual dtor binders.
   Targets 0044fc00, 005296b0 and 00a01b90 are already in T3; their remaining old-form binders are listed in the census table above.

## Tools (tools/)
- **NF374 copies, retargeted to NF377:** nf.py, objsec374.py, pk374.py, pkdiff374.py, runpair.sh and others.
- **ed377.py:** exact once-only source edits.
- **callers377.py:** retail caller census.
- **decl377.py:** scoped declaration scan.
- **headonly.sh:** HEAD-only vland.
- **census377.py:** which objects bind a symbol.
- **link377.py:** /FORCE /MAP link with per-symbol defining object. It needs /OPT:NOREF; without it the map is empty.
- **mkrun377.py:** builds the run dirs.

## Closing checks
- **PDB exactness:** every new symbol in evidence/objsec/*.renames.txt (14 of them) is an exact line (grep -Fx) in both name sets
  (evidence/pdb_exact.txt).
- **Shipped equals graded:** each batches/b*/rows/<a>.cpp equals the source_cpp of work/p_r1n/payload.json, and so does the 00bfd0b0 test.
- **Claims screen:** checked COORDINATION_CLAIMS.tsv (every line), UNMATCHED_ROWS.tsv and pending/*acctB* for the ten rows and the five
  witnesses. Apart from NF377's own claim, the only lines are Account A `done` lines from 2026-11-20. No Account B line names any of them.
- **Locks:** all twelve NF377 locks are released (evidence/locks_final.txt).
