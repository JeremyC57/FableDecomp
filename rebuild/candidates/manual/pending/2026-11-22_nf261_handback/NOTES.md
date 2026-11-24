# NF261 notes (2026-11-22, round 23b): seven CCombatActionBase slot rows, AllowsCombinationAnims, the pooled 0.5f; entries 4-6 left open

Base: the repo tree with nf259 landed. NF260 runs beside me (none of its locks or entries touched: every row I compiled was `released` or
unlocked before I claimed it, evidence/locks.log). None of my rows is in codex_pass28 RESERVED.tsv (checked). Tools: NF259's set re-pathed
(tools/), plus tools/lnk2.py (lnk.py that also prints the unresolved lines naming watched symbols). Read-first list: the brief chain NF261 ->
NF259 -> ... -> NF215, NF214 and its list (COORDINATION.md first, NF92 BRIEF / LESSONS / NOTES, NF44 BRIEF, NF88-90 LESSONS, GOTCHAS.md and
EXTRA_RULES.md every line but with long lines cut at 330-700 characters (see slips), ASM_RULES.md), the nf259 landing README and hand-back, the nf258 landing README / NOTES (entry 5).
grades_used in STATUS.tsv counts q.py forms; every changed row also ran in three vland --recheck-landed payloads (evidence/vland_e123.log,
evidence/vland_final16.log = the 10 rows + 6 controls, recheck_final.log = the 10 rows after the last comment edits).

## Entry 1 (q12740): 0x0062EA40 .. 0x0062EAB0 -- done (7 rows, all MATCH on the first form)
- Spellings: ?IsToUseRotationInAnimation / IsToAlignWithGroundNormal / IsToFaceTarget / IsToFaceTargetAtStart / IsToPerformActionOnStrike /
  IsToHitProneThings@CCombatActionBase@@UBE_NXZ and ?GetActionMovementVector@CCombatActionBase@@UBE?AVC3DVector@@XZ.
- Sources: FableWin.pdb only (0x01D91710 .. 0x01D91750, 0x01D91880 / 0x01D91890: the header-inline CCombatActionBase bodies, which Ego_r / Ego_d
  do not record) + the retail CCombatActionBase vftable 0x01259E44 slots 71-77 (read directly: each slot holds exactly that address) + the bodies
  (`mov al,1 ; ret`, `xor al,al ; ret`, the hidden-result C3DVector(0, 1, 0) store). ONE PDB: the second source is the retail vftable / body
  (nf228 ruling), as the queue line says. engine/CCombatActionBase.h MELEE_OWN_VIRTUALS already declares all seven: no header change.
- Rows: 0x0062EAC0's define block (MELEE prelude + MELEE_OWN_VIRTUALS), one body each. The HEAD rows were free __fastcall stand-ins
  (`?CCombatActionBase_<Name>@@YI_NXZ`) with no Leaf line, and 0x0062EA80 a row-local non-virtual non-const CCombatActionBase model
  (`QAE`). New tests link the real row object (raw storage + qualified call), catalog PassPatterns kept (AUTO_TINY_..., MOVEVEC_OK).
- vtmap, as measured (the queue's "DIFF -> SAME on 0x00955620" is not what happens: the HEAD rows had no Leaf, so vtmap said NOLEAF):
  0x00955620 (CombatIdle table): slots 71-73 / 75-77 NOLEAF -> SAME; slot 74 there is CombatIdle's own override 0x006F5940. Slot 74 checked
  on 0x008B83C0 (CCreatureAction_SummonCreatures table): slots 71-77 NOLEAF -> SAME (evidence/vtmap_*_head.txt / _rows.txt). 0x00857E40
  (??_7CCombatActionBase@@6B@) emits a 1-slot stub table: no information.
- Census: 0 code references to the seven addresses (vtable slots only). Every other landed declaration of these members is the PDB
  `virtual ... () const` form (grep); the non-const ones belong to derived classes' own overrides (other addresses). Definer grep: no other
  landed source or header defines any of the seven for CCombatActionBase (CCreatureAction_ScreamerIntoAggressive.h defines its own
  IsToFaceTarget / AtStart overrides). Link (evidence/link_e123.txt): with the HEAD rows the emitters 0x00955620 / 0x008B83C0 leave 13
  references to the seven PDB symbols unresolved; with the new rows 0. So each address goes from 2 spellings (self stand-in + emitters'
  PDB spelling) to 1.
- Objects (evidence/symdiff_*.txt): only the leaf changes; 0x0062EA80 also defines the header-inline ??0C3DVector@@QAE@MMM@Z, as at HEAD
  (NO-OWNER, pk.py). No ??_7 in any of the seven.

## Entry 2 (q12741): 0x00692890 AllowsCombinationAnims -- done (RELOCATION_MATCH, first form)
- ?AllowsCombinationAnims@CCreatureActionBase@@MBE_NXZ: Ego_r 0x005191F0 + FableWin 0x01DB9540 (protected virtual const) + retail
  CCreatureActionBase vftable 0x01259D24 slot 66 = 0x00692890 (between slots 65 / 67 = 0x00692880 / 0x006928B0). Body
  `(PAnimEntry->GetFlags() & 0x8) == 0` gives retail's `shr eax,3 ; not eax ; and eax,1`. Row = the nf259 0x00692880 define block
  (FULL_MEMBERS + CANIMATIONENTRY_GETFLAGS); emitted leaf confirmed MBE (objdump). Test = the 0x00692880 header test with bit 3 cases.
- The HEAD stand-in ?AllowsCombinationAnims@CCreatureActionBase_Methods@@QAEDXZ (a struct derived from CCreatureActionBase + a local
  CAnimationEntry + a cast of PAnimEntry's Data) is gone; GetFlags binding unchanged (?GetFlags@CAnimationEntry@@QBEJXZ, 0x005DC210). New
  pick-any ??C?$CCountedPointer@VCAnimationEntry@@@@QBEPAVCAnimationEntry@@XZ (NO-OWNER, as in 0x00692880 / 0x006928B0).
- vtmap: slot 66 of the CCombatActionBase-derived tables is the CCombatActionBase override 0x0062EA10 (0x01259E44 slot 66, read directly), so
  0x00955620 cannot show it; on 0x006DF800 (CCreatureAction_UnsheatheToShowToSheatheTrophy, a direct CCreatureActionBase class) slot 66 flips
  DIFF -> SAME.
- Pre-existing bindings found (NAME_FIXES; not changed, rows not mine):
  - 0x006F5C10 (HEAD) binds slot 66 of its 70-slot CCombatAction_* local-model tables as ?AllowsCombinationAnims@CCreatureActionBase@@MBE_NXZ at
    0x0062EA10: one symbol bound at two retail addresses (0x006DF800 binds it at 0x00692890). Neither has a table line, so vland shows no
    RELOC_CONFLICT (recheck_final.log: 0x006F5C10 RELOCATION_MATCH PASS, no conflict line). At HEAD nothing defined the symbol (LNK2001 in
    the HEAD link for 0x006DF800 and 0x006F5C10); now 0x00692890 defines it, so in a full link 0x006F5C10's slot 66 would resolve to
    0x00692890's body where retail has 0x0062EA10. DECISION 1.
  - 0x00693B30 (the CCreatureActionBase ctor) emits ??_7CCreatureActionBase@@6B@ with the public UBE spellings at slots 65 / 66 / 67 (PDB: MBE).
    nf259 counted the emitters as all on MBE; this one is not.

## Entry 3 (q12742): the pooled 0.5f at 0x0122F59C -- done (2 rows)
- Census (evidence/census_0122f59c.txt): 1,568 references, 498 landed rows (absrefs.py); table lines for 0x0122F59C: ?g_scale@@3MA (0x00661F90)
  and ?g_cosTableScale@@3MB (0x00B0A750) only; every extern float / double declared in those 498 sources resolved against the table: only these
  two land on 0x0122F59C. (Externs a row gets from a shared header are not in this grep; the table has no other line for the address.)
- 0x00661F90: `t * g_scale` (twice) -> `t * 0.5f`. RELOCATION_MATCH; xref: the 0x0122F59C site binds __real@3f000000. The HEAD test carried
  its own copy of the body with g_scale = 2.0f (the wrong value); the new test links the row (HEAD local layout structs in place, cases
  that separate 0.5 from other scales). g_thresh (0x01230A5C) left as is (NAME_FIXES candidate).
- 0x00B0A750: NOT the claim as written. Retail multiplies by 0x0122F59C (0.5f) then 0x01230010 (1024.0f, 94 references: also a pool literal)
  at 0x00B0A832 / 38 and 0x00B0A8B6 / BC. The HEAD row wrote `x * 0.5f * g_cosTableScale` and its comment put g_cosTableScale at 0x01230010,
  but VC7.1 emitted the extern first, so the extern bound the 0.5f site and the literal 0.5f bound the 1024.0f site (crossed;
  evidence/xref_00b0a750_head.txt). The fix replaces the extern with the literal 1024.0f and keeps retail's order:
  - form 1 `x * 0.5f * 1024.0f`: folded to `* 512.0f` (__real@44000000), DIFFER 444v456 (evidence/xref_00b0a750_f1.txt);
  - form 2 `float scaled = x * 0.5f; scaled = scaled * 1024.0f;` (the two-statement spelling of the landed GFSmoothFadeValue 0x00AF9000, whose
    body this helper is): RELOCATION_MATCH, every 0x0122F59C site __real@3f000000 and both 0x01230010 sites __real@44800000
    (evidence/xref_00b0a750_new.txt). Behaviour is unchanged (both scales are powers of two).
  The HEAD test modelled the scale as 2048; the new test links the row and checks values a 2048 / 512 scale would fail.
  DECISION 2: the queue line called g_cosTableScale a second name for 0.5f; it was a crossed binding of the 1024.0f literal.
- TABLE_CHANGES: both lines deleted. The two rows still model CThingCreatureBase / CPSCUpdateNormal locally (HEAD; vland
  SHARED_HEADER_AVAILABLE, unchanged). 0x00B0A750's helper TableCos01 is the expanded NParticleEngine::GFSmoothFadeValue (made-up helper
  name, NAME_FIXES); renaming it is a port, not done here.

## Entry 4 (q12743): CastAssassinRush local models in 0x0072CAA0 / 0x008CD470 -- open (not started; scoped, no compile)
Advisor call: the entry needs new header opt-ins, so stop at its boundary rather than hand back a half-done overlay. What it takes:
- Header (engine/CCreatureAction_CastAssassinRush.h, retail branch, add-only declarations; then pin_asserts, whole-tree equiv_check,
  recheck of its includers 0x0072D990 / 0x0096AE50 / 0x009735D0 / 0x008C2550 / 0x008CD470 / 0x0072CAA0 and the opt-in's users):
  - `virtual void Initialise();` ?Initialise@CCreatureAction_CastAssassinRush@@UAEXXZ (Ego_r 0x009047C0 + FableWin 0x029F9410) for 0x008CD470;
  - the (creature, priority, final action) ctor ??0CCreatureAction_CastAssassinRush@@QAE@AAVCThingCreatureBase@@JV?$CCountedPointer@VCCreatureActionBase@@@@@Z
    (Ego_r 0x009055B0 + FableWin 0x029F91B0; retail 0x008CE100) for 0x0072CAA0 (TARGETCTOR already declares the other one).
- 0x008CD470 must shed: the renaming-macro includes of CCreatureAction_CastAssassinRush.h / _quarantine CCombatActionBase.h /
  _quarantine CThing.h, its local CThing / CCombatActionBase / CCreatureAction_CastAssassinRush / CRightHandedSet / CPhysicsMethods models
  and the local CCountedAction. Needs CThing.h (RemoveTC, AddTC, the physics pointer +0x60, Type +0x10), CTCPhysicsBase.h slot 74
  SetFacingAnglesFromRHSet, CRightHandedSet.h, CCombatActionBase.h MELEE + INITIALISE, the header Dir as a packed C3DVector (the HEAD row
  memcpy's three dwords). CTCAssassinRush has no header (may stay local, in place). Its callee 0x008CEAE0 SetFinalAction moves from the
  stand-in ?SetFinalAction@CTCAssassinRush@@QAEXUCCountedAction@@@Z onto the PDB / self-row spelling (0x008CD470 is its only binder, callers.py):
  count-reducing.
- 0x0072CAA0 must shed: its local CThing (bitset / CVectorMap HasTC / GetTC), CPlayer (ZTargeting +0x20A), CThingCreatureBase, CTCTargeter
  (slot 33 = GetPTargetedThing per GOTCHAS wf7) and CTCSpecialAbilities models, plus the CastAssassinRush model with its conversion operator.
- Related stand-ins found (NAME_FIXES): the self leaves of 0x008CE040 / 0x008CE100 spell pointer parameters and a stand-in counted pointer
  while 0x0072CAA0 binds the PDB spellings (a self-v-binder split each). The 0x008CE040 row also emits ??_7CCreatureAction_CastAssassinRush@@6B@.

## Entry 5 (q12744) and entry 6 (q12659): open (not started)
- q12744 caution for the next agent: with TREE_DTOR_DECL_ONLY in all four rows, nothing may define
  ??1?$_Rb_tree@KU?$pair@$$CBKJ@std@@...: the table has no line for it (no retail owner known), and today 0x00AD9770 / 0x00AD9980's pick-any
  copies are what resolve 0x00ADCE10 / 0x00ADC310's reference. Link the four objects before and after, unresolved lines included.
- q12659: needs the full 469-includer proof; not started.

## Whole-pass checks
- Headers: none changed (hdr/README.md; orig/ empty).
- Link (evidence/link_e123.txt; LINK unset, /FORCE /NODEFAULTLIB): the 10 new objects + 11 HEAD controls: 0 duplicates; positive control (a
  renamed copy of the 0x0062EA40 object): LNK4006. The only watched unresolved line is 0x00692890's GetFlags call (owner 0x005DC210, not in the
  set; 0x00692880 has the same).
- Recheck: evidence/vland_final16.log (the 10 changed rows + controls 0x0062EA10, 0x006F5C10, 0x00693B30, 0x00955620, 0x008B83C0, 0x006DF800):
  8 MATCH + 8 RELOCATION_MATCH, all PASS, 0 RELOC_CONFLICT (WARN lines are HEAD's: local models, RELOC_NAME labels). After it the witness
  comments of 0x0062EA70 / 0x00692890 were corrected (0x008B83C0 / 0x006DF800, not 0x00955620), so recheck_final.log reruns the 10 changed rows
  alone (the controls were already released). pk.py: evidence/pk_e1.tsv .. pk_e3.tsv (every non-leaf function COMDAT of the new objects:
  NO-OWNER header inlines; 0x00B0A750's TableCos01 / GFFloatToLongNear COMDATs are defined at HEAD too).
- Funclets / folds: no `../src/compiled/` include names any of the 10 changed files (grep of src, tests, include).
- Descriptive names added: none. Flags lines added: none (no changed row has CompilerFlags; none needs one).
- Locks: the 10 changed rows stay NF261 (rows.txt); every control was set back to `released` (evidence/locks.log). 0x0062F230 was claimed
  and released without a compile (its table is a 1-slot stub).

## Rule slips
- The first link attempt ran without sourcing the lane env (FileNotFoundError for link.exe); rerun at once with env.sh, the evidence file
  overwritten with the real run.
- Two Bash calls exited non-zero: the first reading call (`wc` on a mistyped NF89 / NF88 LESSONS path) and a `kill -0` on a finished PID as
  the last command of a precheck call.
- GOTCHAS.md and EXTRA_RULES.md were read line by line but with each line cut at 330-700 characters, and EXTRA_RULES non-empty lines
  251-261 (the 2026-11-05 asm summary, whose canonical text ASM_RULES.md I read in full) were not displayed: not a full read.
- The two `<cstdio>` tests (0x00661F90, 0x00B0A750) were first prechecked with the stdio / math.h error lines filtered out; rerun unfiltered
  before the last recheck: no errors. 0x0062EA80's header comment was re-joined after its first precheck (comment only; prechecked again).
- The HEAD tests of the controls were not prechecked (their sources were; the controls were graded / rechecked as landed).
- Nothing written in the repo (work/root_before.txt v root_after.txt identical); no git; no foreground sleep; every grade under an outer
  `timeout 900`, in a call after its passing clang precheck, after its lock was mine; no rm -rf (tools/clean.py).

## Decisions for the coordinator
1. 0x00692890 now defines ?AllowsCombinationAnims@CCreatureActionBase@@MBE_NXZ; 0x006F5C10 (HEAD) binds that symbol at slot 66 where retail has
   0x0062EA10 (CCombatActionBase::AllowsCombinationAnims, FableWin). Pre-existing mis-binding (0x006DF800 and 0x006F5C10 already disagree), so I
   did not hold the row. Land as is with 0x006F5C10's slot-66 respelling and the 0x0062EA10 self row (?AllowsCombinationAnims@CCombatActionBase@@UBE_NXZ)
   queued, or hold 0x00692890 until 0x006F5C10 is fixed?
2. 0x00B0A750: accept the fix as a crossed-binding repair (made-up extern -> 1024.0f literal, two statements as the landed GFSmoothFadeValue),
   rather than the queue's "second name for 0.5f" reading?
3. One-PDB evidence for the seven 0x0062EA40 .. 0x0062EAB0 spellings (FableWin + the retail slot + the body): accept, as queued?
4. NAME_FIXES: 0x0062EA10; the 0x00693B30 UBE slots 65-67; 0x008CEAE0 / 0x008CE040 / 0x008CE100 (CastAssassinRush family); the
   GFSmoothFadeValue helper name in 0x00B0A750; the pool-literal candidates 0x01266300 (0.001f) and 0x01230A5C (0.01f). Queue?
5. q12743 / q12744 / q12659 to the next agent (scoping above).
