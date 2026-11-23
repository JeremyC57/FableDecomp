# NF292 notes (2026-11-23): the six generated-branch opt-ins, then the T1 "other" made-up-name batch (hand-back 1)

## Summary (STATUS.tsv / APPLIED.tsv)
- **47 rows changed** (rows.txt), all RELOCATION_MATCH PASS in three vland `--recheck-landed` runs (recheck_final.log: 47 PASS, 0 DIFFER,
  0 RELOC_CONFLICT; the RELOC_NAME warnings are pre-existing stand-ins of other callees). Tests unchanged (standalone copies, NF284 D2; none
  `#include`s a changed row).
- **Landing-ready: 46 rows; 1 row (0x009CB960, entries 71 / 72) pending decision 1** (it is itself a T3_PORT_QUEUE address).
- **Entries reached: 39 of 933** (STATUS.tsv): done 23, done (pending decision 1) 2, done-partial 7, skipped-held 4, skipped-port 3.
  Entries 1-6 (opt-ins) all done. Read but not attempted (not in STATUS): 57 (table witness 0x008E9710 stale; 0x008D70A0 / 0x008D88A0 /
  0x008E97B0 declare FindCombinationAnim on local CAnimateThing models) and 195 (WriteData binders differ in signature, two of them
  virtual; the WriteBuffer binders 0x004B64E0 / 0x00A2EAA0 / 0x00A75400 are SIGEQ).
- **New table state**: TABLE_CHANGES.tsv (34 delete, 15 add, 1 rewitness; CONDITIONAL: the RemoveBank delete, the WriteUShort rewitness and the four 0x009CB960 lines), RENAMES.tsv section 2 (53 lines),
  NAME_FIXES.tsv (2 owner self rows defining non-PDB names; a third, 0x00B59760, is decision 3). REJECTED.tsv: none.
- **Four header overlays** (hdr/, HDR_README.md): add-only declaration opt-ins in GENERATED flat branches (ruling NF291 Q4) plus one
  assert pin. equiv_check 0 differ over 1,693 rows; planted control flags 80.

## Order (E2)
1. Opt-in unit (entries 1-6, binders 0x006E11F0, 0x00A99840, 0x00A9BA70, 0x00895ED0, 0x007311F0): one unit, four headers.
2. B1: the SAMESCOPE / SIGEQ entries whose binder declares the made-up name on a local model with the real class name (tools/cls1.py,
   cls2.py, filt.py: no header declaration, no same-TU body, binder still names it, PDB not virtual where the binder is not).
3. B2: the next group of the same shape, including two const additions (PDB QBE where the binder declared QAE).
- Worked by binder: every exe caller of each target was found with xsites.py (evidence/xsites_*.tsv), not only the table witness;
  e.g. entry 54 had 7 binders (table: 1), entry 188 had 9 (table: 2), entry 61 had 2.

## Proof per changed row
- objsec HEAD v new (evidence/proof_o1.txt, proof_b1.txt + proof_b1door.txt, proof_b2.txt + proof_b2_const.txt): every non-debug section
  (`.drectve` included) equal in bytes and relocation lists under an EXPLICIT map built from the respell itself (tools/proof2.py:
  `?old@` -> `?new@` on the HEAD-only symbol, required to be among the new object's new symbols; const changes mapped by hand), never the
  sorted pairing. Symbol sets otherwise identical, so COMDAT sets SAME; vtable counts unchanged (tables H=N) and their bytes/relocs equal.
- Three CTCDoor rows (0x007306C0, 0x00730C00, 0x00730DD0) also differ in the per-TU anonymous-namespace hash of SDoorOpenAnim /
  SDoorCloseAnim, because VC7.1 hashes the compile path and qq.py compiles a copy in its own work directory (separate H / N dirs; vland
  printed a third hash for the same row) (ruling 2026-11-20
  "per-TU anonymous-namespace hashes are one spelling"). Mapped explicitly; ALL_EQ.
- Every new symbol is in both PDB name sets except `?SetCurrentGroupNames@CAnimFilterOf2Groups@@QAEXABVCCharString@@0@Z` (FableWin only;
  second source: the 7 retail call sites and the landed self row 0x0073E040) (evidence/pdbq.txt).
- Links (evidence/link_*.txt, LINK cleared, binders + HEAD owner rows): every HEAD-unresolved stand-in disappears; no new duplicate (the
  LNK4006 lines are the same identical-body folds in both sets). Still unresolved after, as for the existing PDB binders: 0x00A4CEC0
  (no landed row), 0x0070D920 / 0x00B59760 / 0x006925E0 (self rows define other names: NAME_FIXES), 0x009D6420 (owner not linked: its
  HEAD source fails clang in its own line), `?GetMouseWheelMovement@CGamePlayerInterface@@` (referenced by the owner row itself).
- Census (evidence/name_census.txt, all opt-in, B1 and B2 names; evidence/codegrep_oldnames.txt for the old names): each new name is
  defined only by its owner row, except pre-existing definitions I did not add: 0x00AB0260 (the non-const FindFirstSequence overload,
  another address and signature), 0x00B31160 (a same-TU CEngineCamera::SetCameraState body, PDB spelling, already landed), and three
  rows defining CMemoryDataOutputStream::Clear besides its owner 0x00996B60 (0x00A66860, 0x00A66C00, 0x009E42C0: decision 5). Old names survive only in standalone tests (D2) and in
  the rows left (STATUS).
- vland recheck: recheck_final.log (rc1 opt-in unit 5, rc2 B1 19, rc3 B2 23).

## Opt-in unit (entries 1-6)
- 0x006E11F0: its `#define` is merged onto comment line 7 (lines 1-3 hold the row title, Flags and Leaf; line count kept). Only this
  entry's line was touched; its other stand-in (free X_006e11f0::Create for 0x004CA1D0) is NF291 entry 20, T3.
- 0x00A99840 / 0x00A9BA70: the Methods ctor/dtor stand-ins (NF289 entry 10) stay; only GetLength / PeekData moved to the header.
- 0x007311F0: the opt-in declares the member protected (PDB IAE); the call from the derived CTCDoor_Methods::FrameUpdate on `this` is legal.

## Left for later / not mine (STATUS notes)
- Binder rows in NF290's ENTRIES (stayed off): 0x004B3A20 (entries 99, 421), 0x009C1050 (208), 0x00BF8030 (247, 315), 0x00BFC770 (188),
  0x00BABB50 / 0x00BAD040 (237). Each is a one-line local-model respell for whoever holds the row.
- Ports: entry 14 / 20 / 21 (same-TU strong bodies under the made-up name), 153 (three binders model the member on the derived
  CCombatActionBase: tried, objsec showed the wrong scope, reverted), 222 (pointer v reference), 31 (CAIStateGroupBaseCalls).
- Candidates already filtered for the next agent (work outputs re-creatable with tools/cls1.py -> cls2.py -> filt.py): about 70 more
  CLEAN SIGEQ entries (e.g. 105 SetName -> SetCombatAnimation with ~30 binders, 32 FinishUpdate -> Terminate, 365, 380, 391-393, 396,
  402, 405, 419, 423, 430, 458, 465, 473, 478, 480, 493-494, 506, 514, 522, 528-531, 539, 562-579, 595-620, 639-705, 927, 929) and 19 PRIV
  ones where the binder is a member of the same class (37, 38, 44, 52: put the respelled member under protected/private).
- Identity doubts to check before respelling (advisor list): 618/619 (one entry's stand-in is the other's PDB name), 646, 649, 152's
  GetCanItemBeQuickAssigned, 278, 380.

## Decisions / questions
1. **0x009CB960 is itself a T3_PORT_QUEUE address** (nf285: CBankFile_Retail port), unlike NF291's 0x00896A30 (only mentioned inside
   another line). I changed only its two CBankFileManager member lines (entries 71 / 72), proven like the rest, but its RENAMES and
   TABLE_CHANGES lines are CONDITIONAL. Land it now, or hold it for the port?
2. **Owner self rows defining non-PDB names** (NAME_FIXES): 0x0070D920 (`?op_del@@YGXPAX@Z`, 17 bytes) and 0x006925E0 (GetMovementRatio,
   57 bytes). The binders now bind the PDB names, unresolved in the link exactly as for the existing PDB binders. Queue as owner self ports?
3. **0x00B59760** (20 bytes): 27 landed exe callers; the table binds `?SetupEnableConsoleVariable@CEngineComponent@@QAEXXZ` (both PDB
   sets) with witness 0x00B29930, and now 0x00B2A010; the self row defines `?GetControlAngleXY@CAIGameCameraBase@@QAEXXZ` while both PDB sets hold only the virtual const
   `...@@UBEMXZ`. A possibly shared trivial body with two PDB names: I left it out of NAME_FIXES (GOTCHAS NF30 "reject rather than pick").
4. **CMemoryDataOutputStream::Clear is defined by four landed rows** (0x00996B60 owner, plus 0x00A66860, 0x00A66C00, 0x009E42C0, all
   catalog-named CMemoryDataOutputStream::Clear): pre-existing, found by the census, not caused here (0x00ADFDC0 only references it).
5. **Conditional table lines**: entry 319's delete is CONDITIONAL on 0x009D6490 (T3-queued, not compiled) not binding the same RemoveBank spelling.
6. **0x0088BA40 / 0x0088B6F0** carried an unused declaration of the made-up IsEnabledAndRunnable; I respelled them (objects identical,
   no symbol either way) so no landed code keeps the made-up name on CAIStateGroupBase. Keep, or drop as noise?

## Rule notes
- Coordinator note (2026-11-23): tools/flags.sh and srcof.sh read past a row's own catalog block. Fixed with NF294's copies (and the same
  bound in tri4.py, look.py, look2.py, filt.py). Re-run over all 80 rows I compiled: only two owner rows, 0x009D5A30 and 0x00B30B50
  (link-only, unchanged), had picked up a neighbour's `/Gr`; both regraded with their own empty CompilerFlags (RELOCATION_MATCH) and
  their links (b2_319, b2_222) rerun with the corrected objects: same results. No changed row was affected (H and N used the same,
  correct flags; the vland recheck reads the catalog itself).
- 0x0094FEB0 (NF294's IsStopping): I did not touch that row (entry 32, Terminate, not attempted), so nothing to fold in.
- Locks: claimed before any compile; tests prechecked before their locks (work/pre_t1.log, pre_t2.log, pre_t3.log; five D1 `cl /Zs`
  passes in work/zs_t1.log, zs_t3.log with a failing planted control); HEAD sources after their locks (pre_srcH*.log); new rows before
  their grade (pre_srcN*.log). Every grade / link / vland has `timeout 900` on its own launch line (gr.sh, lk.sh, rf.sh, zsrun.sh);
  equiv_check under `timeout 3000` (not a grade). Completion of background jobs was read from their DONE markers / log
  tails in later calls, not with `kill -0`; the lk_b2, rc3 and zsrun t3 launches saved no PID file. The B1 / B2 new-row prechecks ran
  without the hdr/ overlay that their grades used (those rows define none of its four macros; equiv_check shows the overlay is
  token-identical for non-definers). No sleep, no foreground waits, no rm (clean.py), no git, nothing written under the repo,
  no `__asm` / `naked` / `_emit`.
- Locks held now: the 47 changed rows (stay-off scan over rows.txt: T3 hits only the five opt-in binders, which the prefix sends me
  to, and 0x009CB960; no NF290 ENTRIES / rows hit). Released: every owner row compiled only for links, 0x00A4CEC0 (not landed), 0x00BF84C0 and
  0x009D6420 (precheck failed in their own lines), and the three reverted entry-153 binders.
- orig/: HEAD copies of the 47 changed rows and orig/hdr/engine/ of the four headers.
- Reading: BRIEF.md prefix, the standing brief, GOTCHAS.md (all 511 lines), the nf289 and nf291 hand-backs, COORDINATION.md and the
  naming sections of EXTRA_RULES.md (names right the first time, stand-in names, anonymous-namespace hashes, relocations aside).
