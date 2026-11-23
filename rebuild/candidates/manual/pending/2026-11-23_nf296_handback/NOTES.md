# NF296 notes (2026-11-23): T1 "other" made-up leaf names, continuation of NF292 (hand-back 1)

## Summary
- **75 rows changed** (rows.txt), all RELOCATION_MATCH PASS in vland `--recheck-landed` (recheck_final.log = rc1 + rc2 + rc3: 47 + 2 + 28
  PASS; rc1's two LINK_FAIL rows are fixed in rc2, and rc1's 0x0069F670 / 0x0069FC40 are superseded by rc3, which carries their unit-2
  and unit-4 respells together). No header changes (hdr/ empty), no new opt-ins.
- **Entries reached: 60 of 894** (STATUS.tsv): done 33, done-partial 18, skipped-held 9. Read but not attempted (not in STATUS; listed
  under "Read, not attempted" below): 343, 557, 661. Every `done` entry checked with tools/tblafter.py: after TABLE_CHANGES its target
  keeps only the PDB spelling (616 was downgraded to done-partial by that check: 0x007AA9E0 binds the PDB leaf on CThing).
  Landing-ready: all 75 rows. Two tests changed (rows/006cd970_test.cpp, rows/00892f10_test.cpp: they link the row and defined the
  callee stub under the stand-in name, so the respelled row LINK_FAILed; stub respelled to the PDB name, rc2 PASS).
- Table: TABLE_CHANGES.tsv (52 delete, 22 add, 6 rewitness, NF292 layout: bare lowercase addresses, rewitness = the new witness; all 6
  rewitnesses point at held NF295 rows and are CONDITIONAL: those rows name the stand-in in code (HasMode: three of them declare
  HasMode(unsigned long)) but were not compiled here). RENAMES.tsv section 2: 81 caller respellings (NF292 layout). APPLIED.tsv: one
  line per ENTRIES address (tools/mkapp.py; the per-binder table is work/applied_perbinder.tsv). NAME_FIXES.tsv: 5 owner self rows.
  REJECTED.tsv: none. recheck_final.log: 0 RELOC_CONFLICT.

## Method (order E2: units 1-4)
1. Triage (tools/cls1.py -> cls2.py -> filt.py -> tri5.py, work outputs re-creatable): SAMESCOPE SIGEQ / ACCESS lines, CLEAN or PRIV.
2. Every exe caller per target with xsites (tools/xsites.py), bucketed by what each landed caller's code names (tools/bk.py:
   PDB / table stand-in / neither; evidence/binder_buckets.txt). Stay-off set: NF293 / NF295 / NF297 targets and spelling witnesses,
   their locks, the standing held rows, codex p28 reserved; T3 queue rows (column 2).
3. **Second source per binder from the donor FableWin.exe** (names only, evidence/donor_calls.txt): tools/dcalls.py lists a donor
   function's direct callees, tools/rcalls.py the donor callers of a function; dchk.py checks each binder's donor counterpart calls the
   exact PDB spelling. Every respell below has that (or a one-level donor chain where retail inlined a wrapper: CVolume ctor, ~CThreadedFile
   -> Close, ~CShaderDataBank, CTCBerserk OnKill / OnRemove -> FinalCleanup). This is the guard for tiny bodies (4..17 bytes: entries
   130, 287, 385, 484, 529, 541, 600, 607, 609, 610) where an ICF fold could otherwise make the address ambiguous.
4. Units: 1 = 14 single-binder CLEAN entries; 2 = 10 multi-binder CLEAN entries; 3 = 12 PRIV entries (binder is a member of the class:
   declaration put under `private:` / `protected:` on its own line, `public:` reopened on the same line, line count kept; 885 is a free
   function); 4 = 15 tiny / multi-binder entries.
5. Per row: test precheck before the lock (D1 cl /Zs for 6 tests failing clang only in VC7.1 iostream headers: work/zs_*.log), lock,
   HEAD precheck, HEAD grade (all RELOCATION_MATCH / MATCH), respell by explicit line list on the code part (tools/nfix.py, asserts every
   line changes; the declaration comment gets "NF296: PDB name, was <old>"), new precheck, new grade, objsec under an explicit map
   (tools/proof2.py, extended with exact full-symbol pairs for const / access changes), link HEAD v new with the HEAD owner rows
   (tools/lkset.sh), vland.

## Proof
- objsec: evidence/proof_u1..u4 (+ proof_u2c for the const pairs): every changed row ALL_EQ under its map, every new symbol is the
  ENTRIES pdb_spelling exactly; vtable counts unchanged.
- Wider (R1) fixes, byte-identical: `const` added where the PDB is QBE / IBE (entries 130, 287, 382, 420, 440, 456, 529, 610,
  616, 663); access labels (unit 3); 0x006CD970's second parameter `int` -> `bool` (PDB ?GetHeightZBelowAt@CThingCreatureBase@@QBEMABVC3DVector@@_N1@Z;
  the call passes 0; objects equal).
- Links (evidence/link_u1..u4): every HEAD-unresolved stand-in disappears; no new duplicate (the LNK4006 lines are the same CVectorMap
  LowerBound folds in both sets). Still unresolved after, each explained: owner not landed (0x009F73C0, 0x0098E7D0), owner not linked
  (0x00666D40: its HEAD source fails clang inside engine/CThingSearchTools.h, not D1), owner defines another name (NAME_FIXES and
  decision 2).
- Wrong-scope guard: three respells came out on the wrong class (0x0093E5E0 / 0x0096EE90 declare IsInCombatMode on a local CThing,
  0x0079EAA0 declares ClearMeshEffect outside the NGraphicAppearance namespace); reverted, locks released, STATUS skipped-port.
- Census (evidence/name_census.txt, rerun over all four units from RENAMES): each new name is defined only by its owner row, except
  pre-existing same-TU copies I did not add and that carry the PDB name already: GFGetAngleYZTo in 0x00753E90, GetCombatType in
  0x00664D40, GFIntToWideString (regex hits in declaring rows). No NF296 row defines any of the new names.
- objsec note: proof_u2.txt's `bad` blocks (name-only map on const changes) are superseded by proof_u2c.txt; 0x0069F670 / 0x0069FC40 are
  proven in their final form (both respells) in proof_u4.txt.
- D1: the six `/Zs` passes (work/zs_u1t / u2t / u3t / u4t.log) ran without a planted failing control; vland later compiled those tests
  (all PASS), so the risk is low.

## Decisions / questions
1. **Owner self rows on folded tiny bodies.** 0x006925A0 (4 bytes) defines ?M@C_006925a0@@QAEEXZ (and the table also binds
   ?HasPerformedAction@CCombatActionBase@@ to it), 0x00662170 (13 bytes) defines ?CPixelShader_CountReferences@@YIKPAVCPixelShader@@@Z,
   0x00661E70 (12) ?CTCPhysicsStandard_CanCollideWithLandscape@@..., 0x006621A0 (17) ?M@S@@QAEDXZ, 0x0079DCB0 (11) ?IsSet@T@@QAE_NXZ,
   0x004BFFC0 (5, jmp) _Forward. The binders now bind the PDB names their donor callers call; whether each owner is the same function or
   another ICF-fold member is identity research (GOTCHAS 302 / 458), so I did NOT put them in NAME_FIXES. Queue as owner self ports?
2. **NAME_FIXES** (non-folded owners defining made-up names): 0x00A67770, 0x00CB7D60, 0x0073A840, 0x005DAA00, and an
   access slip on 0x0077CCA0 (DoStopRunning defined public, PDB IAE). Owner self ports (ruling 2)?
3. **0x0082FE00** (catalog CThingCreature::EndCutFade, 22 bytes) calls ForceAIReset through a local struct CThingCreature: the class and
   the catalog name both look wrong (the donor's only EndCutFade is CGameScriptInterface's; donor ForceAIReset callers include
   CThingAICreature::OnNonKillingLevelUnload). Left as a port; worth an identity look.
4. Split gate: entries 22, 23 and 501 gain a PDB table line (or keep it) while a held NF295 row keeps the stand-in (rewitness CONDITIONAL);
   their self rows define the PDB name, so the remaining split is the held row's (GOTCHAS 476).

## Read, not attempted
- entry 343: the donor caller of 0x008FADB0 (CPhysicsForceModelBase::DoGroundEvasion) does not call GetPointsList: identity research before any respell (0x008FAF70 is an NF295 entry; 0x008FAC30 names neither).
- entry 557: only 0x007C6DA0's donor caller calls StopAttachedSound directly; the CTCDivineWrath / CTCUnholyPower / CTCBulletTime callers do not (a wrapper inlined in retail, or another identity): left for a pass that follows the donor chains. 0x00794C10 / 0x007B0950 are NF295 entries.
- entry 661: four spellings (QAE 0x0099B560, AAE 0x0099B720, made-up 0x0099BDB0, Release 0x0099B7D0); the made-up binders' donor counterparts (SetSubstring / operator=) were not resolved in the donor name set. Not attempted.


## Tools (tools/)
- New: dcalls.py, rcalls.py, dchk.py (donor evidence, names only), bk.py (caller buckets), tri5.py (triage), nfix.py (respells, with the
  access wrap), rec.py (APPLIED / RENAMES / TABLE_CHANGES from a spec), lines.py, census.py, tblafter.py (post-change table check).
- Modified: proof2.py (exact full-symbol pairs), mkpay.py (patches rows/<addr>_test.cpp too), cls1.py / cls2.py (start at entry 1).

## Rule notes
- Locks: claimed only after each test precheck; every compiled source prechecked in an earlier call after its lock; owner locks released
  after their links; locks held now = the 75 changed rows. 0x009624D0 was HELD by NF297 at claim time: skipped-held (NF297 appeared
  mid-run; its entries were then added to the stay-off set; no other overlap).
- Every grade / link / vland ran in the background with `timeout 900` on its own launch line (gr.sh, lk.sh, rf.sh, zsrun.sh); completion
  checked with kill -0 / log tails in later calls. No sleep, no rm (clean.py), no git, nothing written under the repo, no __asm / naked /
  _emit. One mistake: a `while read` loop without its input file waited on stdin and was stopped with TaskStop (no compile involved).
- orig/: HEAD copies of the 75 changed rows and the two changed tests.
