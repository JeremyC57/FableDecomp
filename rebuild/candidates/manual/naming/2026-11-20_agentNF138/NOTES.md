# NF138 notes (round 10, PRIORITY slice 7, sixth pass)

Counts (from APPLIED.tsv / STATUS.tsv / recheck_final.log):
- Entries (3): 1 done (q4747), 0 done-already, 0 rejected, 1 blocked (q4802, rows left), 0 deferred, 1 open (q6408, partial).
- Rows changed: 9 (3 MATCH + 6 RELOCATION_MATCH, 9 test PASS in one recheck_final.log run): 0x00733480, 0x007334B0, 0x00733290,
  0x0064BE45, 0x0068ECA0 (q4747 family); 0x00B20B60, 0x00A6CB20, 0x00A12840 (q4802); 0x00848FD0 (q6408).
- Not-a-binder: 4 (0x0075B8D0, 0x008C1900, 0x0073A680, 0x008EACB0). Blocked: 7 (q4802: 0x00762E10, 0x00B21DD0, 0x00B21E90, 0x00764E90,
  0x00BCD850, 0x00BCD8F0, 0x00BCD950). Not reached (STATUS `none`, q6408): 0x0073ACC0, 0x008676A0, 0x008A08A0, 0x008A1A10, 0x008C1E80, 0x00950C30.
- Header opt-ins: 4 (engine/CTCCombat.h x3, engine/CPhysicsMesh.h x1; hdr/README.md). selfcheck.log: 0 problems.
- Locks claimed (all were free): 0x007334B0, 0x00733290, 0x0068ECA0 (q4747 family), 0x00A12840 (q4802 family).
- grades_used in STATUS counts every compile of the pass (g.sh forms, bindchk, HEAD selfsym / pdiff, the per-batch vland) except the
  final recheck; it agrees with work/grades.log plus the pdiff / vland runs.

## q4747 (done): a five-row family, every address ends on one spelling
- 0x00733480 GetPMeleeFlourishAbility: ported onto engine/CTCCombat.h (retail branch) + CDefPointer.h; own leaf -> the claim (Ego_r 0x0074B600 /
  FableWin 0x025005A0). MATCH on the first form. Its only exe referencer 0x0064BE45 is respelled here.
- 0x007334B0 (NF135 finding) CThing::GetPBaseDef -> CTCCombat::GetPMeleeBlockCounterAbility (self rename; Ego_r 0x0074B630 / FableWin
  0x02500790; Ego_r order = retail order with the same 0x30 spacing; body returns +0x48 = retail PBlockCounterAbility). MATCH.
- 0x00733290 (new finding, an ADD-ON beyond the brief) fn_operator / IsShowingBaseAbility / IsBreakBlockValid -> CTCCombat::IsMeleeBlockCounterValid
  const (Ego_r 0x0074B420 / FableWin 0x02500780; Ego_r order IsMeleeBlockCounterValid / SetPossibleQueuedAction = retail 0x00733290 /
  0x007332A0; both callers use it as the block-counter gate). Neither old name is in any PDB. Name uniqueness: IsMeleeBlockCounterValid and
  GetPMeleeBlockCounterAbility appear nowhere in reloc_bindings.tsv, the catalog, rebuild/src, rebuild/include or rebuild/tests.
- **Separable for the coordinator:** the core (0x00733480, 0x007334B0, 0x0064BE45) lands alone without growing 0x00733290's spelling count
  (0x0064BE45's IsShowingBaseAbility -> IsMeleeBlockCounterValid swaps one stand-in for the PDB name: 3 spellings before, 3 after). The
  add-on (self row 0x00733290 + 0x0068ECA0) must land as a pair WITH 0x0064BE45; together they take 0x00733290 from 3 spellings to 1.
- 0x0064BE45: the local CDefPointer / CAbilityDef / CRefCounted / CDefVirtual models are replaced by engine/CDefPointer.h; the local
  CTCCombat model (present at HEAD) is respelled in place (four declarations to the PDB const members, including IsMeleeFlourishValid QAE
  -> QBE = 0x00733EE0's self leaf; the other 0x00733EE0 referencers are untouched). `ability = combat->Get...()` on the shared operator=
  gives retail's AddRef-only copy (the local is known null). RELOCATION_MATCH on the first form. Its other local models (CThing,
  bitset, CVectorMap specialization, IS_THING_ALIVE stand-in, CPlayer, CThingPlayerCreature) are unchanged (SHARED_HEADER_AVAILABLE warnings
  as at HEAD). Its test is unchanged (a standalone model that still uses the old stand-in names; PASS; not blocking).
- 0x0068ECA0: in-place respell of its local CTCCombat declaration only. Its test is unchanged (PASS).
- Gate scan (work/gate/gate_targets.json): 0x00733480 / 0x007334B0 referenced only by 0x0064BE45; 0x00733290 only by 0x0064BE45 / 0x0068ECA0.
- Existing split left: 0x006445BA (CDrawBase::ClearEffects static) is still bound by 0x0064BE45 as ?ClearEffects@CEffectVector@@QAEXXZ
  (NAME_FIXES; needs a full port of 0x0064BE45 off its local std::pair / bitset models).

## q4802 (blocked, rows left): 2 binders ported, 7 rows blocked
- Census (tools/bindchk.sh, work/bindchk_q4802.txt) of the 9 rows NF126 left: all still bound a stand-in (CArray x3, CPointVector, CKeyVector,
  CElement12 x4).
- 0x00B20B60: the local CKeyVector stand-in (no header) -> STLport std::vector<C3DVector> with FABLE_RETAIL_STLP_VECTOR_ASSIGN. Own name
  unchanged (stand-in CSplineData_00b20b60::CopyFrom; 0x00B22310 binds it as CSplineRandomOffsetData::operator=, one source, NF126).
- 0x00A6CB20 + 0x00A12840 (family): 0x00A6CB20 moved onto engine/CPhysicsMesh.h (new opt-in GETRIGIDBODYBOUNDINGPOINTS); both PDBs spell the
  parameter std::vector<C3DVector>&, so its own leaf changes and its only referencer 0x00A12840 (which bound a third stand-in) moves too;
  0x00A12840's own leaf becomes the PDB spelling its referencer 0x0075B000 already binds. Both addresses go from 2 spellings to 1.
  Hand back both or neither.
- Blocked (STATUS has each reason): 0x00762E10 (the CTCShapeManager family: its CreateShape stand-in 0x007624E0, also bound by 0x00762D80, is
  probably Ego_r ?GetInstantiationFromEnum@CTCShapeManager@@QAE?AV?$CCountedPointer@VCShape@NShape@@@@W4EShape@NShape@@AAV?$vector@... -- one
  source, not in NAME_FIXES); 0x00B21DD0 (copy-construct by zero + assign: needs a CCardinalSpline copy-ctor decision = needs header claim;
  its own ctor's PDB parameter is const CCardinalSpline&, HEAD's local model takes a pointer); 0x00B21E90 (inline-budget residue: one
  /d2inlT100 try, DIFFER 568v619); the four CElement12 rows (STLport __copy / __copy_backward / fill instances over 12-byte vectors whose
  correct element type changes their stand-in self leaves, which the landed callers 0x00BCDB10 / 0x00BCDCA0 bind; 0x00764E90 / 0x00BCD950
  are byte twins whose template arguments must be proven distinct; 0x00764E90's callers 0x0076530C / 0x0076557A are not landed).

## q6408 (open, partial)
- 0x00848FD0 ported (form 1): the local CTCCreatureModeManager / CCountedPointer_CModeBase / SCountBlock / CCreatureActionBase /
  CCreatureAction_SitDown models -> the shared headers (CCreatureAction_SitDown.h retail branch exists). The singleton spelling
  ...QAE_NUCCountedPointer_CModeBase@@@Z disappears; no ctor callee in this row. 10 of 34 REFS rows now bind the claim.
- Next (not reached): the other three singletons 0x008676A0 / 0x008A08A0 / 0x008A1A10 are dozen-local-model rows; each port also moves
  the CCountedPointer<CModeBase>(T*) callee 0x00718A00 and its mode ctor (0x00868220 / 0x008AC3F0 / 0x00861E00 + 0x0073E8F0): run xr.sh on
  those addresses first and port every row that binds the same stand-in in the same batch.

## Header opt-ins (hdr/README.md)
- engine/CTCCombat.h: FABLE_RETAIL_CTCCOMBAT_GETPMELEEFLOURISHABILITY, _GETPMELEEBLOCKCOUNTERABILITY, _ISMELEEBLOCKCOUNTERVALID (declarations).
- engine/CPhysicsMesh.h: FABLE_RETAIL_CPHYSICSMESH_GETRIGIDBODYBOUNDINGPOINTS (declaration + the STLport / C3DVector includes it needs).
- No existing member changed; nothing needs a header claim from these opt-ins.

## Tests
- Rewritten to link the real row on the shared headers (PassPatterns kept): 0x00733480, 0x007334B0, 0x00733290 (self rows whose identity
  or own leaf changed). All other tests unchanged (standalone models; PASS).

## Descriptive names
None new. The q4747 family replaces the descriptive / stand-in names IsShowingBaseAbility, IsBreakBlockValid, GetPBaseAbility, fn_operator,
CThing::GetPBaseDef (on 0x007334B0) with PDB names. CSplineData_00b20b60 (0x00B20B60's own stand-in) is unchanged.

## Flags lines
None added to any handed-back row. (0x00B21E90's /d2inlT100 try stays a draft in work/q4802.)

## Funclets
No row under rebuild/src/compiled/01/21 (nor any other compiled row) names any of the nine changed source files (grep of each file name).

## Local class models
None new (selfcheck 0 NEWLOCAL). Kept from HEAD and respelled in place only: CTCCombat (0x0064BE45, 0x0068ECA0), CPhysicsMesh (0x00A12840).
Row-local stand-ins with no header that remain: CSplineData_00b20b60 (0x00B20B60), CDrawMeleeActionButtonEffects / CEffectVector (0x0064BE45).

## Self renames (RENAMES section 1) and their referencers
Every self address was exe-scanned for E8/E9 callers (gate.py scan / xr.sh) and for 4-byte absolute references anywhere in Fable.exe
(.rdata vtables included): no data or vtable reference for any of them, so none is virtual.
- 0x00733480 (entry q4747): referencer 0x0064BE45, in this hand-back.
- 0x007334B0 (outside the entries, q4747 family): referencer 0x0064BE45, in this hand-back.
- 0x00733290 (outside the entries, add-on): referencers 0x0064BE45 + 0x0068ECA0, both in this hand-back.
- 0x00A6CB20 (q4802 binder): referencer 0x00A12840, in this hand-back.
- 0x00A12840 (outside the entries, q4802 family): referencer 0x0075B000 already binds the new PDB spelling (not changed).

## One definition
Every new external definition in the changed objects is the row's own leaf or a shared-header inline / pick-any COMDAT (CDefPointer<const
CMeleeCombatAbilityDef> members, CIVCountedPointeeBase::AddRef, C3DVector::operator-=, STLport vector size / begin / end / [], the
CCountedPointer<CModeBase> converting ctor / dtor, CIntelligentPointer<CThingCreatureBase>::Get); the local-class COMDATs HEAD emitted are gone
(objdump -t diffs in work/q4747, work/q6408).

## Decisions for the coordinator
- q4747: the 0x00733290 add-on (self row + 0x0068ECA0) is separable from the core but must land with 0x0064BE45 (above).
- q4802: 0x00A6CB20 and 0x00A12840 land together. 0x00B21DD0 needs a CCardinalSpline copy-construction decision (needs header claim).
- q4802 / CElement12: the four STLport instance rows need their element type (CArray<C3DVector> by the 0x00BCDB10 / 0x00BCDCA0 callers) and
  their callers in one family hand-back.
- 0x00762E10: a CTCShapeManager naming family (GetInstantiationFromEnum candidate for 0x007624E0) would unblock it.

## Rule slips
- GOTCHAS.md (135 KB) and ASM_RULES.md were read only in part (GOTCHAS grepped for the relevant lines; ASM_RULES first 60 lines);
  EXTRA_RULES.md was read in full with long lines cut at 450-500 characters.
- Several Bash calls (file reads, greps, objdump, cp) ran without the required `export S ...; source env.sh; cd` preamble; every compile ran
  through g.sh / bindchk.sh / selfsym.sh / pdiff.sh or the vland call, which source it.
- The first RENAMES.tsv draft had a column-header line after `# Section 2`; removed before gate.py scan.
- 0x00B21DD0 (0 new forms) and 0x00B21E90 (1 form) are marked blocked: port below the 3-failed-form budget (structural reasons, NF126
  drafts; context).
- One form on 0x00A6CB20 was a SRC_FAIL for a missing prerequisite define (FABLE_RETAIL_C3DVECTOR_SUBTRACT_ASSIGN): counted.
