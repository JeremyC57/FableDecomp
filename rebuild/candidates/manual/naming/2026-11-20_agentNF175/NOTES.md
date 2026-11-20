# NF175 notes (2026-11-20, round 10; handed back early at an entry boundary for context)

Counts (from APPLIED.tsv / STATUS.tsv / recheck_final.log): 41 entries: 7 done (q4390 q4989 q6254 q6255 q11895 q11896 q11935), 8 blocked
(q4967 q4968 q4983 q12067 q5101 q5102 q11934 q11714; no grade was attempted on any of their blocked rows this session), 2 rejected (q4780
q5120), 1 deferred (q6190, acctB), 23 open (the five cut-scene cast-map lines q6368 q6493 q6504 q6509 q6510, then q11936 onward: not started).
93 STATUS lines: 11 ported (rows/), 14 not-a-binder, 28 blocked, 1 deferred, 39 open (untouched rows of open entries). One final recheck over
the 11 ported rows: 1 MATCH + 10 RELOCATION_MATCH, every test PASS. selfcheck: 0 problems.

## Self renames (RENAMES section 1; every landed referencer already binds the new spelling, so no split grows)
- 0x0082E4E0 CSurface::GetHeight stand-in -> CScriptedMapBrush::GetThingsMovement (referencer 0x00820840 already binds it).
- 0x00DCEC70 / 0x00DCED10 -> NScript::CQ_OrchardFarmRaidScript::CCrateTeamMember::GoOnPatrol / IsThingCarryingCrate (referencer 0x00DCE230).
- 0x00956720 / 0x00956730 -> CCreatureAction_ThrowShotObject::IsToFaceTarget / GetIsToFaceTargetCone (no referencer: vtable only).
- 0x006929A0 free stand-in -> CCreatureActionBase::Update (referencer 0x006643C0 already binds it).
- A self rename moves the row's file: no EH funclet includes any of these six (grep of rebuild/src/compiled/01/21 for the basenames: 0 hits).
- Catalog names: 0x0082E4E0's catalog CSurface::GetHeight is wrong (RENAMES s1 carries module / name); 0x006929A0's catalog
  CCreatureActionBase::Update is already right (leaf-only change). Gate measured, not read: reloc_bindings.tsv maps each of the six new leaves
  only to its own address (GetThingsMovement / GoOnPatrol / IsThingCarryingCrate / Update; the two ThrowShotObject leaves are unbound), and a
  HEAD compile of 0x006643C0 binds 0x006929A0 as ?Update@CCreatureActionBase@@QAEXXZ. gate.py was not run.
- Acceptance (objdump -t defined-symbol diff, HEAD vs NF175, work/pd_<addr>.txt for all 11 rows): only the row's own leaf, shared-header
  inline / pick-any COMDATs (CIntelligentPointer<const CThing> ctor / dtor / vftable / ??_G, map / tree find and iterator operators,
  vector size / begin / operator[], CIntelligentPointer<CThingCreatureBase> Get / ->), __real@3f800000, and dropped implicit members of
  header-less local stand-ins (CIntelligentPointerConstThing, CMoveOptions, CRetailVector, CCrateTeamMember::Return, CSpeechGroupMapIterator)
  that no landed row binds.

## Binder respells (RENAMES section 2)
- q4968 / q4967: 0x00956380, 0x009621F0, 0x00984A20 (LookAtThing), 0x00961790 (MoveToThingProximity) now bind the PDB CAIStateGroupBase
  spellings, declared in place on each row's existing local CAIStateGroupBase model (accepted in-place respell). The by-value argument
  is the shared CIntelligentPointer<const CThing> (CONVERT_INLINE); local stand-ins CIntelligentPointerThing / ConstThing / Ctor and the
  local enums were dropped (engine/EAIAction.h / EMovementType.h). 0x00984A20 also moved off its local CBaseIntelligentPointer model, so
  0x00A01B10 / 0x00A01B50 / 0x00A01B90 now bind the shared spellings (s2). 0x00961790 keeps its legacy local CIntelligentPointer template
  (in place; no new token); its CMoveOptions stand-in became the PDB `bool&` local and its last argument the PDB float.
- q4989: 0x008AF1E0 moved onto the shared CombatDialogueManager.h / CMainGameComponent.h (0x008AF6E0's define block); its local
  CombatDialogueManager / CMainGameComponent / map / iterator models were removed. Note: 0x008AF6E0 defines IsSpeechGroupAllowed in its own
  TU too (pre-existing same-TU strong copy, not changed here).
- q11935: 0x006929A0 bound GetPItem / ClearCurrentAction as extern "C" @getSub@4 / @runSub@4; now the PDB spellings.

## Header opt-ins (hdr/README.md): all add-only, member-specific, default paths token-identical
- engine/CScriptedMapBrush.h FABLE_RETAIL_CSCRIPTEDMAPBRUSH_THINGSTOMOVE (nested CThingPosAndOrientation, typed ThingsToMove, GetThingsMovement).
- engine/CCombatActionBase.h FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_OWN_VIRTUALS (the 11 own virtuals in the MELEE branch).
- engine/CCreatureAction_ThrowShotObject.h FABLE_RETAIL_CCREATUREACTION_THROWSHOTOBJECT_FACETARGET (two overrides).
- engine/CCreatureActionBase.h FABLE_RETAIL_CCREATUREACTIONBASE_UPDATE (public Update()).
- For the coordinator: the three TUs that emit ??_7CCreatureAction_ThrowShotObject@@6B@ (0x009551A0 / 0x009566C0 / 0x00956960) define neither
  MELEE_OWN_VIRTUALS nor FACETARGET, so their vtable COMDAT is unchanged (70 slots, as at HEAD). If a later pass adds the two macros to them,
  it must add them to all three together (one vtable).

## Inferred statements (EXTRA_RULES 2026-11-12)
- 0x00DCEC70: the discarded `IsActiveThreadTerminating();` on the non-idle path (retail tail `jmp 0x00F35B30`).
- 0x00DCED10: `if (IsActiveThreadTerminating()) return false;` before each return (retail calls 0x00F35B30 there; the stand-in's
  Return(value) helper cannot live on the shared nested class). Both marked `// Inferred:` in the row.

## Decisions for the coordinator
- q4780 / q12029 (GameCODA voice family): the SDK proves 0x00C59710 = CSalBaseVoice::Release, 0x00C3F3D0 = ~CSalBaseVoice and 0x00C4CF40 =
  ~CCodaObject (NAME_FIXES; byte comparisons against CODAApi.lib). The queued descriptive CCodaVoice::Stop is REJECTED. Applying the SDK names
  is one family: 0x00C59710, 0x00C5A1F0, 0x00C3F490, 0x00C3F3D0 and 0x00C3F3D0's 7 referencers (0x00C3F470 0x00C3F490 0x00C564B0 0x00C56850
  0x00C568E0 0x01215200 0x012162F0, two of them EH funclets), with the ~CCodaObject split on 0x00C4CF40 (10 referencers, 5 spellings) to settle
  first. EXTRA_RULES 2026-11-12 asks only light renaming of hand-written vendor rows; I left it for q12029 (open).
- q5120: the PROPOSAL UpdatePitchEnvelopeStages is REJECTED (SDK CSalSwBaseVoice::CalcMultiFuncEnvelope; the only binder already binds it).
  The self row 0x00C5A700 keeps its landed descriptive leaf: a pre-existing split (self vs binder) that only a 715-byte port onto
  CSalSwBaseVoice.h fixes.
- Cut-scene cast map (q6368 / q6493 / q6504 / q6509 / q6510, open): a 40-row family plus RunCutsceneMacro 0x00CBFB7D / operator[] 0x00CD3D2E
  respells and the two self rows; not started (it hands back whole). Row list: APPLIED / work/cutscene_wrong_nf158.tsv.
- Rows blocked: port without attempt (each models a dozen-plus shared-header classes locally, or needs the retail CThing /
  CThingGameObject chain so that CIntelligentPointer<T>(T*) can static_cast): q4967 / q4968 0x00911630 0x00916910 0x0093A080 0x009804A0
  0x0092A8A0 0x0092B3E0 0x009352C0 0x0096FCD0; q5101 / q5102 / q11934 (11 rows; NF164's analysis holds); q4983 / q12067 (4 rows, NF107 /
  NF110 analysis). Classes that must move first are named per row in STATUS.

## Tests
- Rewritten on the shared headers (linking the real row object): 0x0082E4E0, 0x00956720, 0x00956730 (PassPatterns kept).
- Unchanged (standalone, PASS): the AI rows, 0x008AF1E0, 0x00DCEC70, 0x00DCED10, 0x006929A0. 0x008AF1E0's and 0x006929A0's existing tests
  model CombatDialogueManager / CCreatureActionBase standalone (as at HEAD; they do not link the row). Not rewritten for context.

## Descriptive names
- None new. All names applied come from Ego_r.pdb / FableWin.pdb (or the STLport library spelling already in reloc_bindings).

## Grades
- grades_used in STATUS counts q.py compiles (g.sh, work/grades.log) plus the row's vland payloads. Five extra HEAD compiles (hb.sh, into
  pH/) read old leaves / bindings: 0x0082E4E0, 0x00984A20, 0x00DCEC70, 0x00DCED10, 0x006929A0. No row needed more than 2 forms; 0 failed forms.

## Locks
- Claimed this session (owner was `released`): 0x0092B3E0, 0x009352C0, 0x0096FCD0, 0x009621F0 (ported); the first three are untouched
  (blocked: port). Claimed 0x006643C0 for a HEAD probe compile only (untouched).
- No stand-in was cast to a real type to reach a PDB spelling; no new local model; no forceinline / asm / volatile added.

## Rule slips
- One Bash call exited 1 (a grep of a missing NAME_FIXES_QUEUE path, then a cat of a missing lock owner) early on.
- Several early calls skipped the lane preamble (`$S` unset), so the first callers.py of a loop printed nothing; re-run with the preamble.
- GOTCHAS.md (137 KB) and ASM_RULES.md were grepped, not read in full, before the first edit (no asm or new token used in any row).
- q4390's old leaf was first written from the source; confirmed by a HEAD compile afterwards (same symbol).
- Queue order: the five cut-scene cast-map lines (q6368 q6493 q6504 q6509 q6510) were passed over (left open) and q6510 onward worked; they
  were first recorded as blocked and relabelled open at hand-back.
- RENAMES section 2 was first written from filtered binding pairs; a full HEAD-vs-new pair diff then added the 0x01237F1C vftable and
  0x0122DED8 lines (hb.sh had also truncated its pair list at 30 lines; fixed).
