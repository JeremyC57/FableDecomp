# NF242 notes (2026-11-22, round 14): the held CModeSitBench unit with the vtable slot swap, then the queue

Base: the repo tree with nf240 landed. NF241 ran beside me (list<CComponent*> family); none of my rows is on its lists or in
`pending/2026-11-22_codex_pass28/RESERVED.tsv` (grep of every address before claiming). NF241's overlay is stlport/fable_containers.h only.
Locks: 0x00740040 / 0x00739E70 / 0x00739E90 were pre-assigned NF242; the nine slot rows claimed with plain mkdir + `NF242` (work/locks.log),
all new directories. Tools: NF240's set re-pathed (tools/), plus swap911.py (the slot-9 / 11 rewrite), vtmap.py (every vtable COMDAT of an
object mapped slot by slot onto its retail vtable by RTTI name, with the landed self leaf at each target), vm.sh / emc.sh / lk.sh /
nameaddr.py (HEAD v new objects, vtable census, link sets, name -> addresses summary).
Flags lines added: none. Descriptive names added: none (every new spelling is a PDB name).

## Entry 1 (q12677): the held q12674 unit + the slot-9 / 11 swap -- done, 12 rows, one landing
### What NF240 left, and why the unit grew
- NF240 held 0x00740040 / 0x00739E70 / 0x00739E90 (rows + `NCreatureMode_CModeSitFloor.h` IMPLICIT_DTOR) because nine local-model rows
  declare CModeBase slot 9 = IsRemoveOnDeactivate and slot 11 = IsAllowingPlayerSpecialMovementMoves (retail: the reverse).
- Compiling those nine emitters (NF240 read their sources only) showed more: their CModeStandBase comes from engine/CModeStandBase.h
  WITHOUT its slot overrides, so slot 7 binds `?GetMovementSpeed@CModeBase@CTCCreatureModeManager@@...` at retail 0x00861900 (seven
  rows) and slot 2 binds `?AnimUpdate@CModeBase...` at 0x00862EC0 (five rows) / 0x008D6850 (0x00743FE0). So NF240's census
  "0x00739E70: 2 -> 1" was wrong: respelling 0x00739E70's self leaf to the CModeBase name would have put that name at two addresses
  (0x00739E70 and 0x00861900), and the swap alone would have moved the IsAllowing name to 0x0073DA00 (slot 9 of the seven
  CModeStandBase-derived vtables), not to 0x00739E90. The unit therefore also gives those seven rows CModeStandBase's overrides.
- Retail proof of the slot order (all three bodies 0x00739E80 / E90 / EA0 are `xor al,al ; ret`, so the order comes from overrides):
  vtable 0x012673CC slots 8 / 9 / 10 / 11 = 0x00739E80 / 0x00739E90 / 0x0073A9F0 / 0x00739EA0; CModeCarryingWeaponStand
  ::IsAllowingPlayerSpecialMovementMoves 0x008E35F0 (`mov al,1 ; ret`) sits at slot 9 of 0x012856E4; CModeStandBase's 0x0073DA00 at
  slot 9 of every CModeStandBase vtable read here (0x012857A8 0x01285860 0x012681EC 0x01267D10 0x01278338 0x01285900 0x01268774);
  CModeIceTrollGroundSmashIcicleAssault's own IsRemoveOnDeactivate (0x00743CB0, `mov al,1 ; ret`) at slot 11; FableWin.pdb code order
  GetMovementSpeed 0x01DD6780 then IsAllowing 0x01DD67A0; the header CMODEBASE_VIRTUALS order.
### The change (rows/)
- The three held rows: NF240's hand-back files unchanged (0x00740040 ctor onto NCreatureMode_CModeSitFloor.h + IMPLICIT_DTOR;
  0x00739E70 / 0x00739E90 self leaves onto CTCCreatureModeManager.h), plus a NEW linked test for 0x00740040 (below).
- Nine rows (0x008E3720 0x008E31A0 0x008E37C0 0x00741390 0x00740420 0x008AC4D0 0x0073B0D0 0x0073AEA0 0x00743FE0): in their local
  CTCCreatureModeManager::CModeBase model (the same class token at HEAD) the slot-9 and slot-11 declarations trade names
  (tools/swap911.py; both `bool () const`, so no layout or slot-count change), with one comment line. In-place respell (the
  nf214 rule); NOTE: CTCCreatureModeManager has a shared header, so a port onto it stays the long-term fix (each row also models
  CAnimFilterBase / CAnimFilterOfGroup locally).
- Seven of them (all but 0x0073B0D0 / 0x0073AEA0, which emit no mode vtable) also `#define FABLE_RETAIL_CMODESTANDBASE_PASS22_VTABLE`,
  the existing CModeStandBase.h opt-in that the landed header-based siblings 0x00743600 / 0x008E3A20 use: CModeStandBase's
  AnimUpdate / GetMovementSpeed / IsAllowingPlayerSpecialMovementMoves overrides (retail 0x00862EC0 / 0x00861900 / 0x0073DA00; all three PDBs).
  No header edit.
- 0x00743FE0 only: its header-less local `NCreatureMode::CModeIceTrollGroundSmashIcicleAssault` (no header anywhere under
  rebuild/include) declares its own `virtual void AnimUpdate(CAnimateThing&)` and `virtual bool IsRemoveOnDeactivate() const` (Ego_r /
  FableWin / Ego_d: ?AnimUpdate@CModeIceTrollGroundSmashIcicleAssault@NCreatureMode@@UAEXAAVCAnimateThing@@@Z,
  ?IsRemoveOnDeactivate@...@@UBE_NXZ; retail slots 2 / 11 of 0x01268774 = 0x008D6850 `ret 4` / 0x00743CB0 `mov al,1 ; ret`).
  Without the AnimUpdate line PASS22 would bind ?AnimUpdate@CModeStandBase at 0x008D6850 (a new split); without IsRemoveOnDeactivate the
  swap would bind ?IsRemoveOnDeactivate@CModeBase at 0x00743CB0. Decision 1 asks about these two member declarations.
### Proof
- vland --recheck-landed (work/b1.log = recheck_final.log): 12 / 12 RELOCATION_MATCH or MATCH + PASS. WARNs: 23 SHARED_HEADER_AVAILABLE
  (the HEAD local models), nothing else.
- Symbol diffs (objdump -t, HEAD v new, work/sy_[hn]_<addr>.txt): the nine rows differ only in UND vtable-slot symbols (3 or 4 per
  emitting row; 0x0073B0D0 / 0x0073AEA0 identical). No DEF changes.
- Vtables (work/vm2.vt, tools/vtmap.py over the compiled HEAD and new objects): name -> retail addresses bound by the emitters.
  Census of other emitters first: every landed row whose source defines FABLE_RETAIL_CMODEBASE_VIRTUALS or models GetMovementSpeed /
  IsIdleMode (134 rows) compiled at HEAD (work/em1.vt): only 0x00743600 / 0x008E3A20 bind these names, all at the right addresses.

  | Name | HEAD addresses | After |
  |---|---|---|
  | ?IsAllowingPlayerSpecialMovementMoves@CModeBase@CTCCreatureModeManager@@UBE_NXZ | 0x00739EA0 (6 rows), 0x00743CB0 | 0x00739E90 (self, bench) |
  | ?IsRemoveOnDeactivate@CModeBase@CTCCreatureModeManager@@UBE_NXZ | 0x00739EA0, 0x0073DA00 (7 rows) | 0x00739EA0 |
  | ?GetMovementSpeed@CModeBase@CTCCreatureModeManager@@UBEM... | 0x00861900 (7 rows) | 0x00739E70 (self, bench) |
  | ?AnimUpdate@CModeBase@CTCCreatureModeManager@@UAEX... | 0x00862EC0 (5 rows), 0x008D6850 | neither (self row 0x00739E40 not graded here) |
  | ?AnimUpdate / ?GetMovementSpeed / ?IsAllowing...@CModeStandBase | 0x00862EC0 / 0x00861900 / 0x0073DA00 | the same (+ 7 rows) |

  | Address | HEAD spellings | After |
  |---|---|---|
  | 0x00739E70 | 1 (?vfunc_7@CClass_00739e70 self) | 1 (PDB, self + bench vtable) |
  | 0x00739E90 | 1 (?...@CModeBase@@QAE_NXZ self) | 1 (PDB) |
  | 0x00739EA0 | 2 (PDB self + ?IsAllowing@CModeBase from 6 rows) | 1 |
  | 0x0073DA00 | 3 (stand-in self, CModeStandBase PDB, ?IsRemove@CModeBase) | 2 (stand-in self, PDB) |
  | 0x00861900 | 3 (stand-in self, CModeStandBase PDB, CModeBase) | 2 (stand-in self, PDB) |
  | 0x00862EC0 | 2 (PDB self, CModeBase) | 1 |
  | 0x008D6850 | 2 (stand-in self, CModeBase) | 2 (stand-in self, PDB) |
  | 0x00743CB0 | 2 (gapscan self, CModeBase IsAllowing) | 2 (gapscan self, PDB) |
  | 0x00740040 / 0x007400E0 / 0x01267BFC | NF240's census | unchanged from NF240 (2 -> 1 / 2 -> 2 / 1 -> 1) |
  Nothing grows. New spellings (PDB, replacing a wrong one, count-neutral): ?AnimUpdate@CModeIceTroll...@NCreatureMode@@ at 0x008D6850,
  ?IsRemoveOnDeactivate@CModeIceTroll...@NCreatureMode@@ at 0x00743CB0 (their self rows are stand-ins: NAME_FIXES).
- Remaining HEAD debt in these nine vtables (not moved by this unit; NAME_FIXES): slots 3 / 4 / 5 / 6 bind the CModeBase FrameUpdate /
  GetAnimFilter / OnActivate / OnDeactivate names at derived overrides (16 slot bindings over 5 rows), because the derived local models
  do not declare those overrides.
- pk.py (work/own_740.tsv): 0x00740040's copies as NF240 found them: EQ ??_GCModeSitBench (0x007400E0), ??_GCAnimFilterOfGroup
  (0x0073E970), ??1CAnimFilterOfGroup (0x00739DB0), ??0CAnimFilterOfGroup (0x0073E8F0), CCharString GetLength / GetAsCharArray; NEQ
  ??_GCAnimFilterBase v 0x0041CCE4 (landing nf240 ruling 3: debt). The nine rows define no new symbol.
- Link (work/lnk1.txt): the 12 rows + 0x00739E60 / E80 / EA0, 0x0073A9F0, 0x0073DA00, 0x00742790, 0x00938380, 0x007400E0, 0x008D71D0,
  0x0073E8F0, 0x00739DB0, 0x00743600, 0x0073E970, 0x008E3A20, 0x00862EC0, 0x00861900, 0x008D6850, 0x00743CB0 (30 objects): HEAD set 5
  LNK4006, new set the same 5 (RemoveAllModes 0x0073B0D0 v 0x0073AEA0; four inline ??0CAnimFilterOfGroup copies v 0x0073E8F0), 0 LNK2005.
- Header: hdr/README.md (equiv_check over today's 22 includers: 0 differing; assert_names_check 0; pin_asserts 0).
- Table: NF240's two reloc_bindings lines (the vtable symbol of 0x01267BFC); no table line names a slot function changed here.
### Test (0x00740040, NEW, links the row)
- On the shared headers with the row's opt-ins plus FABLE_RETAIL_CMODEBASE_CTOR / _PARENTTHING (declarations / an inline accessor, test
  only); stubs for every symbol the row object leaves undefined (25). A test-only accessor class CBasicStringProbe : CBasicString<char>
  (never constructed) fills the stub string data (CBasicString's members are protected). 13 checks: base ctor gets the thing; the
  "SIT_BENCH" temporary destroyed; GetAnimFilter at +0x2C; group text / length 9; CRC from Calc(0, text, 9); slots 1..11 dispatch to the
  stub named by their PDB names (slot 9 IsAllowing, slot 11 IsRemoveOnDeactivate); delete through the base runs ~Group, ~CModeSitBase,
  ~CModeBase, operator delete in that order. PASS (work/t1.log, b1.log).

## Entry 2 (q12678): the CEngine unit -- step 1 prepared and proven but HELD (one row); steps 2 / 3 not started
- Step 1 (landing nf240 ruling 5): engine/CIEngine.h with the four PDB slot declarations as the default (Render / ConvertScreenPosToWorldPos /
  ConvertScreenPosToWorldPosAtViewDistance / ConvertWorldPosToScreenPos; their four macros become no-ops; the CEngineInterpolationInfo
  forward declaration unguarded; pin_asserts pinned 4; assert_names_check 0), and the ten tests that override the VirtualNN_ placeholders
  respelled to the PDB signatures (`{}` bool bodies -> `{ return false; }`). Drafts in work/q12678/ (hdr/, orig/, rows/, README_CIEngine.md);
  NOT in rows/ / hdr/ because one row fails. Their ten locks are released again.
- Proof: include closure 2 headers (CIEngine.h, CEngine.h), 149 rows; equiv_check (work/equiv2.tsv): 122 differing files over 108 rows.
  vland --recheck-landed over all 108 in six batches (work/c00..c05.log, each under timeout 900): 107 MATCH / RELOCATION_MATCH + PASS
  (the ten respelled tests all PASS); RELOC_CONFLICT WARNs on 0x0064BFBA / 0x0064E978 / 0x006B42F0 (callees unrelated to CIEngine; not checked at HEAD; those rows are not in the hand-back).
- Blocker: 0x0058578B CTCInventory::OpenInventory (8,081 bytes) goes RELOCATION_MATCH -> DIFFER: one two-instruction order swap at
  0x00585D7A (retail `mov ecx,[ebp+68h] ; mov esi,[ebp+40h]`, ours the reverse; work/h578_ovl.txt). It is not the edit's form: the HEAD
  header with the four EXISTING opt-ins gives the same DIFFER, the same single swap: once with the macros added to a source copy
  (work/v578a.cpp / v578a.txt) and once with the HEAD source unchanged and the macros as /D flags (no line shift; work/v578b.txt). So the PDB
  declarations themselves perturb VC7.1's scheduling in this row (the source only calls Enable3DSceneRendering through the interface).
  Decision 2.

## Entries 3-5 (q12679, q12676, q12659) -- not started (context; handed back after q12678 step 1 as the brief allows).

## Self-check
- `$S/w22/nftools/selfcheck.py` does not exist in this lane (nftools holds absrefs / apply_rows / callers / mkpay / xref / xrow.sh): not run.
- Repo root listed before and after (root_before.txt): unchanged. No git run.

## Rule slips
- One wait on the census job used a bounded `while kill -0 <pid>; do sleep 5; done` in the foreground (PID-based, but a foreground loop).
- One scratch cleanup used `rm -rf` on a variable path (`$A/work/o_vm1`, inside my work dir); later cleanups use tools/clean.py.
- One single foreground `sleep 240` (it timed out to the background); later waits used background PID loops / Monitor.

## Decisions for the coordinator
1. 0x00743FE0: its header-less local model of NCreatureMode::CModeIceTrollGroundSmashIcicleAssault gains two override declarations
   (AnimUpdate, IsRemoveOnDeactivate; three PDBs + the retail slots) so that PASS22_VTABLE and the swap create no split there. Accept as an
   in-place addition to an existing header-less local class, or hold the row (and so the unit) for a port?
2. 0x00740040's new linked test uses three devices: a test-only accessor class CBasicStringProbe : CBasicString<char> (never
   constructed; NF240's 0x00B3AAA0 derived-probe precedent), a replaced global operator delete (to log the delete order), and two
   test-only opt-ins FABLE_RETAIL_CMODEBASE_CTOR / _PARENTTHING (existing declaration / inline accessor). Accept?
3. q12678 step 1 is blocked by 0x0058578B only (107 / 108 rechecked rows pass). Options: (a) re-tune 0x0058578B (authoring: find the
   spelling that schedules 0x00585D7A as retail under the PDB declarations) and land step 1 with it; (b) a cheaper probe first: bisect the four existing opt-ins on 0x0058578B (/D flags, HEAD header, four compiles); if one
   declaration alone perturbs it, the other three placeholders can retire now; (c) keep the opt-ins and the placeholder default for now;
   (d) another ruling. Steps 2 / 3 wait on this.
4. The nine rows still model CTCCreatureModeManager / CAnimFilterBase / CAnimFilterOfGroup locally, and 16 slot bindings (slots 3-6) in
   five of their vtables bind CModeBase names at derived addresses (NAME_FIXES): queue header ports of those rows?
5. Self rows with stand-in leaves at the unit's addresses (0x0073DA00, 0x00861900, 0x008D6850, 0x00743CB0; NAME_FIXES): queue their
   leaf respells (each has two PDB sources and every landed vtable binder already on the PDB spelling after this unit)?
- STATUS / APPLIED were written per entry (after the batch grade), not after every row.
