# NF157 notes (round 10, PRIORITY reconciliation: NCreatureMode / NCameraModes)

Counts (from APPLIED.tsv / STATUS.tsv): 21 entries: 15 done, 4 done-already (q4536, q4537, q5035, q11670), 1 blocked (q11874),
1 deferred (q5037: 0x00871D30 held by NF155), 0 rejected (one claim spelling rejected in REJECTED.tsv, q11887, entry served by q11883),
0 open. 64 STATUS rows: 27 ported, 31 not-a-binder, 5 blocked (the five q11874 rows), 1 deferred (0x00871D30).
Final recheck: recheck_final.log, one vland --recheck-landed run over the 27 ported rows: 1 MATCH (0x008C2D60) + 26 RELOCATION_MATCH, every test PASS; selfcheck.log: 0 problems.

Census first: work/probe.tsv compiled every REFS.tsv row once (tools/probe.py) and recorded the symbol each binds at each entry address.
Every changed row was diffed against its HEAD object (tools/bdiff.sh: binding pairs + defined external symbols); STATUS notes give the diff.

## Per entry (non-obvious ones)
- q3943 (0x008E31A0 ~CModeDragonHover): the binder 0x008E3860 now calls the dtor as
  `NCreatureMode::CModeDragonHover::~CModeDragonHover` through a NEW header-less local model `NCreatureMode::CModeDragonHover : CModeStandBase`
  (no header for the class anywhere under rebuild/include; the landed sibling 0x008E31A0 models it the same way on the shared
  CModeStandBase.h; base from the shared header). Difference from the sibling: there the dtor is implicit; here it is declared (not defined)
  so the qualified call stays out of line and the object defines only the leaf.
  0x008E3860's own leaf stays the free stand-in ?CModeDragonFireBeam_CompositeScalarDeletingDestructor@@YIPAXPAXHI@Z. The real name
  ??_GCModeDragonFireBeam@NCreatureMode@@UAEPAXI@Z (FableWin.pdb 0x02865520) grades RELOCATION_MATCH as a compiler ??_G
  (work/008e3860_f1.cpp), but VC7.1 emits that vtable/??_G only where a FireBeam ctor body is compiled, and both FireBeam ctors are owned by
  other rows (0x008D8620, 0x008D86E0), so the inline-ctor scaffolding would define 0x008D8620's name with a made-up body; the implicit
  copy-ctor form (work/008e3860_f2.cpp) emits vtables/??_G of four base classes. NAME_FIXES carries the ??_G name. DECISION NEEDED: either a
  FireBeam-ctor-owning TU (0x008D8620) emits the ??_G, or the dragon family {0x008D8620, 0x008D86E0, 0x008E31A0} moves first.
- q3987 (0x0084DC10 CBase::FrameUpdate, parked): 0x0069B790 ported (its CCameraBase stand-in removed; CurrentMode typed
  NCameraModes::CBase* from the shared header with the NF157 opt-in). 0x0084E110 ported whole onto engine/NCameraModes_CBase.h CYLINDER +
  engine/CGameCameraManager.h's retail SINGLECUT branch (the pairing landed 0x00878BE0 already uses; SingleCut is public there and
  SetSingleCut is its inline body), first form RELOCATION_MATCH; its own leaf moved from the stand-in ?Init@CCameraModeBase@@QAEXXZ to the PDB
  ?Init@CBase@NCameraModes@@QAEXXZ that both landed callers (0x00697E20, 0x00699B00) already bind, which also ends that existing split. Mode
  type 8 is compared as an integer (enumerator name unknown; landed 0x00699E60 writes the same).
- q4538 / q5036 / q5037 / q4539 (CProjectileWeapon / GetObservedThing): in-place respells in each row's existing local models (accepted
  form). 0x00878B60's own leaf moved QBE -> ABE with its only referencer 0x00878DB0 (family; else a new split at 0x00878B60).
  0x00878DB0 / 0x00879560 / 0x00878B60 own leaves QBE -> ABE: leaf-only self renames, both PDBs agree; 0x00878DB0 and 0x00879560 have no
  landed referencer (exe scan: callers in no catalog row).
  Base cast (flag for the coordinator): 0x00878DB0, 0x00872490 and 0x008FAA80 keep the default (non-retail) CThing chain, where
  engine/CThingGameObject.h is not usable, so CThingGameObject is only forward-declared and the call site writes
  `((const CThing*)GetObservedThing())->GetPos()` (single inheritance, offset 0; no address constant; same bytes). The alternative,
  `class CThingGameObject : public CThing { };` as landed siblings 0x00871D50 / 0x00872050 / 0x0087B690 do, is a new local model of a
  shared-header class (not allowed). 0x0075DAA0 already had the retail chain and uses the real class.
- q11649 (0x005ECF50 DeleteData): 0x005ED270 binds the header's static member (its pick-any copy of DeleteData is identical to the owner's
  explicit instantiation; both LINK_ONCE_DISCARD). Finding: 0x0069C3D0 (NF115) instantiated the same DeleteData COMDAT with an INCOMPLETE
  NCameraModes::CScriptModuleBase (body `call operator delete` only, no virtual destructor call) under 0x005ECF50's retail name; fixed by
  including engine/NCameraModes_CScriptModuleBase.h (bindings and symbols unchanged). Other rows that store DeleteData of a forward-declared
  T may carry the same defect (grep `class X;` + Reset/ctor instantiations).
- q11678 / q11875 / q11876 / q11882 / q11883 / q11884: leaf-only self renames (pointer stand-ins -> PDB references; bodies pass &x to the
  rows' unchanged pointer stand-ins, same bytes) with every landed referencer moved in the same hand-back.
- JoB family (q11880, q11883, q11884, q11887): {0x00740450, 0x008D7370, 0x008C2D60, 0x00742880, 0x008C0D10} move together.
  - 0x008C0D10: NF147's queued in-place move (class token now `class NCreatureMode::CModeJackOfBladesScream`, PDB key V).
  - 0x00742880: self rename from the free stand-in to the PDB Alloc (0x0073B130 already binds it), CModeBasic::Alloc 0x00742460 shape,
    NEW header-less local model on the shared CTCCreatureModeManager::CModeBase (no header for the class; landed sibling 0x008C0D10 has the
    same shape).
  - 0x008C2D60: self rename + catalog fix (_global::DeleteViaVtable0_008c2d60 ->
    CCountedPointer<NCreatureMode::CModeJackOfBladesScream>::DeleteData) as an explicit instantiation of the header body (pick-any), NEW
    header-less local model as in 0x008C0D10. Test rewritten on engine/CCountedPointer.h (PassPattern DELETEDATA_OK kept).
  - Link proof: link/job.log (the five new objects, LINK= cleared, /DLL /NOENTRY /NODEFAULTLIB /FORCE:UNRESOLVED): no LNK2005; control
    link/ctl.log (one object twice) shows LNK2005 + LNK1169, so detection works.
- q11881 (0x008AC3F0): ported onto engine/NCreatureMode_CModeScreamerSuperAttack.h. The vtable 0x01237F1C type descriptor in Fable.exe
  is `.?AV?$CIntelligentPointer@$$CBVCThing@@@@`: the header's CIntelligentPointer<const CThing> is right, the old row model was wrong.
- q11874 (0x008E1150 CModeDragonFlying): blocked whole (see APPLIED): 5 rows with 4 different vector/counted-pointer stand-ins, and the move
  cascades into the own leaves of 0x00950C30 (StartFlying) and 0x008E1580 (CModeDragonStrafeBurninate ctor), both PDB-proven with the real
  std::vector, so their callers would follow.

## New local models (narrow rule; list required)
- 0x008E3860: NCreatureMode::CModeDragonHover : CModeStandBase (sibling 0x008E31A0).
- 0x00742880, 0x008C2D60: NCreatureMode::CModeJackOfBladesScream : CTCCreatureModeManager::CModeBase (sibling 0x008C0D10).
None of these classes has a header under rebuild/include (grep `class|struct <name>` over all of rebuild/include: only a forward
declaration of CModeJackOfBladesScream in engine/CCreatureAction_JackScream.h).

## Self renames on rows outside the entry addresses (both PDBs, referencers checked)
- 0x00878B60 (referencer 0x00878DB0 moves with it), 0x00878DB0 and 0x00879560 (no landed referencer), 0x00742880 (referencer 0x0073B130
  already binds the new spelling), 0x0084E110 (referencers 0x00697E20 / 0x00699B00 already bind the new spelling).

## New PDB / header spellings replacing stand-ins on callees (no address's spelling count grows)
- 0x008AC3F0: 0x00861E00 ??0CModeStandBase@@QAE@AAVCThing@@@Z, 0x00A01B90 ?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z,
  0x0129AAF4 ?NullChar@CCharString@@1DB, vtables 0x01237F1C ??_7?$CIntelligentPointer@$$CBVCThing@@@@6B@ and 0x01238C6C
  ??_7?$CIntelligentPointer@VCThing@@@@6B@: each spelling is already in reloc_bindings.tsv at that address; the row's old stand-ins
  (??0CModeBase_00861e00 also used by 0x00740160; ?SetPItem@..@@QAEXPAX@Z; ??_7CIntelligentPointerCreature / ..Thing) only leave.
- 0x0084E110 / 0x0069B790: 0x0084DC10 ?FrameUpdate@CBase@NCameraModes@@QAEXXZ (bound by 0x00873DB0 already).
- 0x00878DB0 / 0x00879560 / 0x0075DAA0 / 0x00872490 / 0x008FAA80 / 0x005ED270 / 0x00892xxx / JoB / dragon / sleep-bed rows: the entry spellings
  (each the self row's own leaf after this hand-back).

## Descriptive names
- None new. The stand-in own leaves kept (0x008E3860, 0x005ED270) are HEAD's; their PDB names are in NAME_FIXES.

## Flags lines
- None added.

## Funclets
- No EH funclet row under rebuild/src/compiled/01 includes any changed row (grep of the parents' file names and addresses).

## Header debt seen (not changed)
- 0x008E31A0 defines ??0CModeDragonFireBeam@NCreatureMode@@QAE@AAVCThing@@@Z (0x008D8620's PDB name) strongly with a made-up body, and models
  CAnimFilterBase / CAnimFilterOfGroup / CTCCreatureModeManager locally although shared headers exist.
- 0x008E78C0 still models CModeBriarRoseConcentrate locally although engine/NCreatureMode_CModeBriarRoseConcentrate.h exists (leaf respelled
  in place only).
- 0x00892410 / 0x00892530 / 0x00892610 / 0x00892B50 / 0x00892C20 model CCountedPointer, CGameCameraManager, CAIGameCameraBase, CWorld
  locally (vland WARN SHARED_HEADER_AVAILABLE); only the deleter name was respelled in place.

## grades_used
- STATUS grades_used counts every compile of the row in work/grades.log (g.sh grades, failed compiles and the bdiff.sh HEAD compile; vland
  rechecks are not counted). Maximum 6 (0x008E3860: the probe, the three ??_G forms and the final form, plus bdiff).

## Rule slips
- Four Bash calls exited non-zero: the first reading-list `wc` (a brace-expanded path that does not exist), an `ls` of NF147's repo rows/
  directory, a trailing `grep -m2 error` with no match after the 0x00878DB0 / 0x00879560 grades, and the 0x005ED270 census loop (a final
  `cat` of a missing lock owner file).
- Queue order: q11881 was finished after the JoB unit (q11883 / q11884 / q11887), which I took as one unit at q11880.
- 0x0084E110 was first recorded blocked on a wrong reason (a private SingleCut); the landed 0x00878BE0 pairing showed the public branch, and
  the row was then ported in one form.
