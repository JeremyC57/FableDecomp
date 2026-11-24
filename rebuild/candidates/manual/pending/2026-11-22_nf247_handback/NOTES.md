# NF247 notes (2026-11-22, round 16b): CModeBase slot bindings, the 0x006A1620 binders, the held p27 lines, 0x00418DCA

Base: the repo tree with nf244 landed. NF246 runs beside me (0x005127A0 / 0x004AB4F0 / 0x00685BC0 DTOR_OUTOFLINE families, NF241's CODEXP27
lines, 0x0049D800, 0x01296F14); none of my rows is on its lists or in `pending/2026-11-22_codex_pass28/RESERVED.tsv` (grep before claiming).
NF246 has no hdr/ overlay. Locks: work/locks.log (the five q12682 rows were `released`; 0x008AC3F0 a new directory). Tools: NF244's set
re-pathed (tools/), plus rvt.py (a retail vtable dump with the landed Leaf at each slot) and ins36.py (the in-place form, kept as a fallback).
Flags lines added: none. Descriptive names added: none (every new spelling is a PDB name).

## Entry 1 (q12682): the CModeBase slot 3-6 bindings -- done, 6 rows, all header ports
### What was wrong (retail vtables read from Fable.exe, tools/rvt.py; the PDB names grep -a'd in Ego_r / FableWin / Ego_d, all three agree)
The five NF242 emitters 0x00741390 0x00743FE0 0x008AC4D0 0x00740420 0x008E31A0 (implicit dtors; each defines its class's ctor, so it emits
the class's ??_7) declared only GetType (+ NF242's slots 2 / 11 on the ice troll) in their local derived models, so slots 3-6 fell through
to the CModeBase names at the derived addresses. The PDBs list each class's own overrides:
| Class (vtable) | Overrides in all three PDBs (slots) | Retail targets |
|---|---|---|
| CModeScreamerAggressive (0x012681EC) | FrameUpdate, GetAnimFilter, OnActivate, OnDeactivate (3-6) | 0x008DA610 0x00741360 0x008DA410 0x008DA690 |
| CModeIceTrollGroundSmashIcicleAssault (0x01268774) | + AnimUpdate (2) | 0x008DE7A0 0x00743CA0 0x008D7630 0x008D9650 |
| CModeScreamerSuperAttack (0x01278338) | 3-6 | 0x008E16A0 0x008AC4A0 0x008D8330 0x008D79A0 |
| CModeJackOfBladesFloat (0x01267D10) | GetAnimFilter, OnActivate, OnDeactivate (4-6); no FrameUpdate (slot 3 = CModeBase 0x0073AC50) | 0x007403F0 0x008D9740 0x008D7730 |
| CModeDragonFireBeam (0x01285900) | FrameUpdate, OnActivate, OnDeactivate (3 / 5 / 6) | 0x008E0780 0x008D6A50 0x008DBB00 |
- 0x008E31A0 had more than the queue counted: its slots 3 and 5 bound the CModeDragonHover names at FireBeam's own 0x008E0780 / 0x008D6A50
  (DragonHover's own slot 3 / 5 are 0x008DB460 / 0x008D6A30). So 18 bindings move, not 16.
- A census of the other emitters of these vtables (work/vt_e0.txt; the ctor / Alloc rows 0x00740340 0x007412B0 0x0096DD10 0x00741B60 0x008D8580
  0x00743BE0 0x008C7C00 0x008D8620 0x008D86E0 0x00742FA0 0x0073A540, and the five header includers) found one more wrong one:
  **0x008AC3F0** (CModeScreamerSuperAttack ctor, on the header without FABLE_RETAIL_CMODEBASE_VIRTUALS) emitted a 5-slot ??_7 whose slots 2-4
  bound ?FrameUpdate / ?OnActivate / ?OnDeactivate@CModeScreamerSuperAttack at 0x00862EC0 / 0x008E16A0 / 0x008AC4A0 (each one slot off). It
  is fixed in the same unit (lock claimed). The ctor rows' 1-slot `?v00@` placeholder vtables are HEAD debt (NAME_FIXES), not touched.
  NF242's em1 census (134 other emitters) has no other mode vtable binding these names.
### The change (rows/; in-place drafts first, then the header ports; every row graded both ways)
- 0x00740420: ported off its local CAnimFilterBase / CAnimFilterOfGroup / CTCCreatureModeManager models onto CTCCreatureModeManager.h
  (CMODEBASE_VIRTUALS), CModeStandBase.h (+ PASS22_VTABLE), CAnimFilterOfGroup.h (INLINE_CTOR, which gives the implicit inline
  ~CAnimFilterOfGroup retail expands; needs FABLE_RETAIL_CCHARSTRING for its inline ctor), CIntelligentPointer.h, ETCInterfaceType.h.
  NCreatureMode::CModeJackOfBladesFloat has no header anywhere under rebuild/include: it stays the row's local class (HEAD token) on the
  shared base, with its three override declarations (the 0x00743FE0 / nf242 ruling 1 shape). Two cpp-level fixes (ETCInterfaceType.h, CCHARSTRING) before the first grade.
- 0x00741390: onto engine/NCreatureMode_CModeScreamerAggressive.h with the new add-only opt-in `_VTABLE` (ctor, GetType, GetAnimFilter: the
  header had FrameUpdate / OnActivate / OnDeactivate only); the row-local class model is gone.
- 0x008AC4D0: onto engine/NCreatureMode_CModeScreamerSuperAttack.h with `_IMPLICIT_DTOR` (the header's `virtual ~X();` under `#ifndef`) and
  `_GETANIMFILTER`; the local class model is gone.
- 0x008E31A0: onto engine/NCreatureMode_CModeDragonHover.h (the existing PASS27 opt-in + the new `_VIRTUALS`: the (CThing&) ctor and its
  slot 2-5 overrides); CModeDragonFireBeam (no header) stays local (HEAD token) on that shared base, with its three overrides.
- 0x00743FE0: onto the shared headers incl. stlport/fable_containers.h (its local `namespace std` allocator / _Vector_base / vector model is
  removed; the STLport ~vector<CIcicleData> / ~vector<C3DVector> copies are EQ to owners 0x00743A90 / 0x004DCFBB); its header-less local class
  declares slots 3-6 next to NF242's 2 / 11.
- 0x008AC3F0: defines CMODEBASE_VIRTUALS, CMODESTANDBASE_PASS22_VTABLE and the new `_GETANIMFILTER` (three #define lines; no other change).
### Proof
- vland --recheck-landed (work/r1.log; recheck_final.log): 6 / 6 RELOCATION_MATCH + PASS, no WARN, no RELOC_CONFLICT. The tests are the HEAD
  tests, unchanged (standalone host models that do not include the row: HEAD debt; decision 2).
- In-place forms (sources work/inplace/<addr>.cpp, regenerated by tools/ins36.py; objects were work/n1/) also graded RELOCATION_MATCH with only UND vtable-slot symbols changed; kept as the fallback.
- Symbol diffs (work/sym_[h0|n1|p1]_<addr>.txt): the ports add only shared-header pick-any COMDATs: ??0CAnimFilterBase, ??0CAnimFilterOfGroup
  (EQ 0x0073E8F0), ??_7CAnimFilterOfGroup, ??_GCAnimFilterOfGroup (EQ 0x0073E970), CCharString GetLength / GetAsCharArray (EQ 0x00403A00 /
  0x00403A10), CBasicString GetLength / PeekData (no owner), and on 0x00743FE0 the STLport __destroy / allocator / _STLP_alloc_proxy helpers (no
  owner); 0x008E31A0 and 0x008AC3F0 add none (UND only). pk.py: the only NEQ copies are ??_GCAnimFilterBase v 0x0041CCE4 (NEQ at HEAD too:
  nf240 ruling 3 debt) and the strong ctor copies ??0CModeScreamerAggressive (v 0x007412B0) / ??0CModeScreamerSuperAttack (v 0x008AC3F0), NEQ
  at HEAD with the same bytes (the rows emit their vtable through a member-init-only ctor: HEAD debt).
- vtmap (work/vt6_[hn].txt over the linked objects): name -> addresses over the 33-object set (the 6 rows, 9 ctor rows, 17 slot targets,
  ~CModeStandBase 0x0073D860):
  | Name | HEAD | After |
  |---|---|---|
  | ?FrameUpdate@CModeBase@CTCCreatureModeManager@@UAEXXZ | 0x0073AC50 0x008DA610 0x008DE7A0 0x008E16A0 | 0x0073AC50 |
  | ?GetAnimFilter@CModeBase@... | 0x007403F0 0x00741360 0x00743CA0 0x008AC4A0 | (base 0x00739E50 only, not in this set) |
  | ?OnActivate@CModeBase@... | 0x008D7630 0x008D8330 0x008D9740 0x008DA410 | (base 0x0073AAC0 only) |
  | ?OnDeactivate@CModeBase@... | 0x008D7730 0x008D79A0 0x008D9650 0x008DA690 0x008DBB00 | (base 0x00739E60 only) |
  | ?FrameUpdate@CModeDragonHover / ?OnActivate@CModeDragonHover | 0x008E0780 / 0x008D6A50 | none in vtables (self rows 0x008DB460 / 0x008D6A30) |
  | ?FrameUpdate / ?OnActivate / ?OnDeactivate@CModeScreamerSuperAttack | 0x00862EC0 / 0x008E16A0 / 0x008AC4A0 (0x008AC3F0) | 0x008E16A0 / 0x008D8330 / 0x008D79A0 |
  Every one of the 25 slot names in the after-table (work/vt_lk1_n.txt) binds exactly one address.
- Per-address spellings (self DEF from compiling each target row, work/t0/; binders = the vtables above; callers.py: no E8 / E9 caller of any):
  2 -> 1: 0x00741360 0x008DA410 0x00743CA0 0x008D7630 0x008D9650 0x008D8330 0x008D79A0 0x007403F0 0x008D7730 0x008D6A50 0x008DBB00;
  3 -> 1: 0x008E16A0, 0x008AC4A0 (the 0x008AC3F0 off-by-one names gone too); 0x00862EC0 loses ?FrameUpdate@CModeScreamerSuperAttack;
  2 -> 2 (stand-in self leaf, PDB binder replaces a wrong one; NAME_FIXES): 0x008DA610 (self QAE), 0x008DA690 (ModeScreamer::), 0x008D9740
  (no namespace, QAE), 0x008E0780 (QAE); 1 -> 1: 0x008DE7A0 (not landed). Nothing grows.
- Link (work/lk1.txt, the 33 objects): HEAD 1 LNK4006, new the same 1 (??0CModeScreamerSuperAttack defined by 0x008AC4D0 and 0x008AC3F0: HEAD
  debt), 0 LNK2005.
- Header: hdr/README.md (equiv_check over the 5 includers: 0 differ; pin_asserts 3 + 3 + 1; assert_names_check 0).
- Table (reloc_bindings): no line names a slot function this unit moves; the ??_7 lines (witnesses 0x00740340 0x007412B0 0x00743BE0 0x008D8620
  0x008AC3F0) keep their symbols. TABLE_CHANGES.tsv: nothing for q12682 (one delete for q12679).

## Entry 2 (q12679): the three blocked 0x006A1620 binders -- partial (count-reducing), 1 of 3
### 0x00664A10 CThingCreatureBase::UnsheatheRangedWeapon -- ported, binder respelled
- HEAD: a row-local CThingCreatureBase with 77 placeholder virtuals (slot 77 named AreWeaponsUnsheathed), `m_pDef` at +0xE0, a local
  CThingCreatureDef, the free stand-in `IsThingCarryingRangedWeapon(const CThingCreatureBase&)`; no Leaf line.
- Now: engine/CThingCreatureBase.h retail branch with FABLE_RETAIL_CTHING_ORIGINAL_VTABLE (slot 77 is the pure `IsPlayerCharacter() const`:
  CThingAICreature.h / CThingPlayerCreature.h override it at slot 77, 0x00835940 / 0x0052ABE0, and landed 0x00847DF0 calls slot 77 +0x134 by
  that name), _BEDSLEEP (SheatheAllWeapons) and the new add-only _UNSHEATHERANGEDWEAPON (this member + UnsheatheWeapon), PDef ->
  CThingCreatureDef.h `_MEMBERS` RangedWeaponDef (+0xE0, PDB +232), CTCCarrying.h PLAYERHIT_PASS26 (`CTCCarrying::IsThingCarryingRangedWeapon`).
  Leaf line added: ?UnsheatheRangedWeapon@CThingCreatureBase@@QAEXXZ (three PDBs; the HEAD object already defined that spelling).
  Two cpp-level fixes before the first grade (the CThingCreatureDef _MEMBERS opt-in). First full grade RELOCATION_MATCH.
- Symbol diff HEAD v new (work/sym_[h0|p1]_00664a10.txt): UND `?IsThingCarryingRangedWeapon@@YI_NABVCThingCreatureBase@@@Z` ->
  `?IsThingCarryingRangedWeapon@CTCCarrying@@SI_NABVCThing@@@Z`; new DEF the two shared-header CDefPointer inlines (`??C` / `Get`, 3 bytes, no
  owner). UnsheatheWeapon stays QAE (see the header note: the PDBs say IAE, but the self row 0x006648D0 and binders 0x006649C0 / 0x00664A50 spell
  QAE, so the opt-in declares it public: decision 3).
- Census of 0x006A1620 (callers.py): binders 0x005C2DF5 0x00664A10 0x00664A50 0x0068F9D0 0x006AB0E0 0x00890250 0x00890D80 0x0091EC30
  0x0091F050 (+ two sites in no landed row). Spellings 4 -> 3 (the PDB one, `?EntityWieldsRanged@@YI_NPAUEntity@@@Z` in 0x00890250,
  `?IsThingCarryingRangedWeapon@@YI_NPAUCThingNative@@@Z` in 0x00890D80). 0x006648D0 stays 1 (QAE).
- Link (work/lk2.txt): 0x00664A10 + 0x006648D0 0x006649C0 0x00664A50 0x006647F0, HEAD set and new set: 0 LNK2005 / 0 LNK4006 each. Header proof: hdr/README.md (995 includers, 0 real differences).
### 0x00890250 / 0x00890D80 -- blocked (a header port does not suffice)
- 0x00890250: the whole row is a stand-in (fake vtable struct, free `EntityWieldsRanged(Entity*)` leaf): re-authoring (NF240's finding stands).
- 0x00890D80: `#define CThingCreatureBase CThingCreatureBaseLayout` around the header include, then a local CThingCreatureBase /
  CThingNative subclass with a member-pointer slot-77 call, a local ScriptMethods with 30+ CScriptThing virtuals, local CBaseClassNative /
  CCPPointerInfo models: it needs CGameScriptInterface.h / CScriptThing.h ports before the binder can move. Not attempted.

## Entries 3-4 (q12676, q12659) -- not started
- q12676: of NF236's entry-5 blockers, none fell through this hand-back (0x006A1620's held line was already restored at nf240).
- q12659 (0x00418DCA, 469 includers): left for a pass that can carry the full proof.

## Rule slips
- My first lock claim used `0x`-prefixed directory names (the lock tree uses bare lowercase addresses); I removed those five new directories
  (literal paths; one variable-path `rm` was refused by the harness before it ran) and claimed the real (released) locks. No other slip:
  no git, no foreground sleep, every wait by PID, every grade under `timeout` (the 975-row cpp-only equiv_check ran under `timeout 1800`).

## Self-check
- Repo root listed before (work/root_before.txt) and after (work/root_after.txt): see the final line of this file.
- recheck_final.log: one vland --recheck-landed run over all 7 rows, overlay on: 7 / 7 RELOCATION_MATCH + PASS, no WARN.
- No __asm / naked / _emit. No new local model of a class that has a header (each port removes the HEAD local models of CAnimFilterBase,
  CAnimFilterOfGroup, CTCCreatureModeManager / CModeBase, std::vector and the ScreamerAggressive / SuperAttack / CThingCreatureBase models);
  the header-less classes CModeJackOfBladesFloat, CModeIceTrollGroundSmashIcicleAssault, CModeDragonFireBeam stay row-local (HEAD tokens) on
  shared bases.

## Decisions for the coordinator
1. q12682: all six rows are header ports; three new add-only opt-ins on the mode headers (hdr/README.md). The in-place forms (declarations
   added to the HEAD local models; UND-only diffs) are in work/inplace/ if you prefer the smaller change for any row.
2. Tests: the seven rows keep their HEAD tests (standalone host models that do not link the row). Queue linked header tests for them?
3. 0x006648D0 UnsheatheWeapon access: the three PDBs say protected (IAE); the self row and every landed binder spell QAE, so the new
   CThingCreatureBase.h opt-in declares it public. Queue the four-row family move (0x006648D0 self + 0x006649C0, 0x00664A50, 0x00664A10) and
   then flip the opt-in to `protected:`?
4. 0x008AC3F0 was not in the queue line (an extra emitter of the CModeScreamerSuperAttack vtable found by the census; lock claimed, three
   #define lines). Accept it in this unit?
5. 0x00890250 / 0x00890D80: re-queue as authoring / CGameScriptInterface + CScriptThing header ports (0x006A1620 then 3 -> 1).
- Not read this round: NF214's "Read first" list (COORDINATION.md, GOTCHAS.md, EXTRA_RULES.md, ASM_RULES.md, the NF92 / NF44 briefs and
  LESSONS); I worked from the brief chain, the nf244 README and the NF244 / NF242 / NF240 hand-backs.
- 0x00664A10's HEAD test still models slot 77 as AreWeaponsUnsheathed (host model; not changed).
- Root check: work/root_before.txt and work/root_after.txt list the same entries (nothing written into /home/user/FableDecomp).
