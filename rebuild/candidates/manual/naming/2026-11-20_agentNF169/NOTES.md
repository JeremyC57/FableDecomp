# NF169 notes (round 10: creature action / AI state group naming; resume of NF161 / NF164 returned entries)

Hand-back on context before the slice was done. Counts (APPLIED.tsv / STATUS.tsv / recheck_final.log): 22 entries: 8 done (q11657
q11667 q11680 q11695 q11779 q11783 q11930 q11941), 6 blocked (q4750: 0x006EAC50 after 3 forms; q11715: decision; q11937-q11940: port,
3 forms on 0x00917330), 8 open (q4967 / q4968 partial with exact remaining binders; q5101 q5102 q11714 q11934 q11895 q11896 not
attempted). Rows: 26 ported (recheck_final.log: 1 MATCH + 25 RELOCATION_MATCH, every test PASS; one changed test, 0x008B0610),
26 blocked (only 0x006EAC50 and 0x00917330 were graded, 3 forms each; 3 more InitCreaturePosition rows share 0x00917330's shape;
0x0096C9A0 waits on a decision; 20 are "not attempted (entry open)"), 12 not-a-binder. selfcheck.log: 0 problems. Header opt-ins: 1
(hdr/README.md).

Reading: CLAUDE.md, COORDINATION.md, NF92 BRIEF/LESSONS/NOTES, NF44 BRIEF, NF88/89/90 LESSONS, EXTRA_RULES (whole), ASM_RULES
(rules section), GOTCHAS (greps + lines 420-475), NF161 NOTES/census, NF164 APPLIED/NOTES, landing READMEs nf118 nf122 nf123 nf127
nf131 nf137 nf148 nf154 nf155. The chunk 732 / 755 / 764 NAME_FIXES were read only through the queue lines' evidence.

## Per entry
- q4750 GetDefenderZone: 0x008B0610 respelled in place (const&, ECombatZone from engine/ECombatZone.h; test mirrors the signature).
  0x006EAC50 blocked/port after 3 forms: its CCombatManager lives in namespace CombatDebug (the nf95b case: moving it to the global
  token of a shared-header class is not in place), so the row was re-authored on engine/CCombatManager.h with two new opt-ins
  (work/hdr_006eac50/: CCOMBATMANAGER_GETCOMBATDEBUGINFO, CTHINGCREATUREBASE_ISINMELEEASATTACKED; NOT in hdr/ because no finished row
  uses them). Best form work/006eac50_blocked.cpp: DIFFER(451v451), binds the PDB GetDefenderZone, the D1 `_M_find` 0x006F0250 and the
  shared vector<C3DVector> copy 0x006F0810; only a frame residue (0x38 v 0x34: retail shares the UniqueID key slot with the GetName
  temp). Forms 1-2 failed because my copy of the self row's define block was cut short (CONSTFIND missing): a rule slip in spirit.
  Next idea: `Combatants.find(creature.GetUniqueID())->second` written in the row with CONSTFIND + MFIND_DECL, or a named key local.
- q4967 / q4968 (AIAction_MoveToThingProximity / LookAtThing): 13 binders moved (11 in place with tools/aiact.py, 0x0095EDF0 and
  0x0094F000 onto the shared CIntelligentPointer.h). Remaining wrong binders are listed in APPLIED/STATUS (not attempted).
  0x0093A080's MoveToThingProximity stays on its stand-in: the argument is built by an out-of-line converting ctor 0x00930D20 that must
  move with it.
- q5101 / q5102 / q11934: not attempted; the remaining binders model CCombatActionBase / CDefinitionManager locally (NF164 analysis holds).
- q11657, q11680, q11930, q11779, q11695 (= q11783), q11941, q11667: done (self renames in RENAMES section 1; every REFS row ends on
  the new spelling). q11667: holding 0x006ED8C0 holds all five rows (the self rename needs every referrer).
- q11714: blocked (family must move whole; 0x0092B3E0 needs a full header port). q11715: blocked on a coordinator decision (below).
- q11937-q11940: blocked/port (3 forms on 0x00917330; APPLIED has the forms and the next one). q11895 / q11896: open (not started).

## Self renames (files keep their names; no funclet includes any changed row: grep of every changed basename, 0 includers)
0x0088B760, 0x0088B810, 0x006ED510, 0x0093DC10, 0x00954220, 0x0097F5B0, 0x0090FC50 (RENAMES section 1). File names already carry the
right class/method, so nothing moves.

## In-place respells on existing local models (same class token; symdiff vs the HEAD object: only the respelled callee / own leaf)
0x008B0610 (CCombatManager), 0x0090EEC0 0x0090F470 0x0091E750 0x009357F0 0x009574D0 0x00957750 0x00958000 0x00959A60 0x0096EE90
0x0094BB20 0x0093A080 (CAIStateGroupBase), 0x0088B810 (CAIBrain), 0x006ED510 0x006ED8C0 0x006ED9C0 0x006ED9E0 0x006EDA80
(CCombatManager), 0x0097F5B0 0x0097F690 (CAIStateGroupBase), 0x00954220 (CAIStateGroup_MazeRangedCombat), 0x0090FC50.
Local empty `enum EAIActionReturn { };` / forward lines were replaced by the shared engine/EAIAction.h (Round-1 lesson); 0x0094F000's
descriptive local enums were replaced by the shared ones (same values: MOVEMENT_TYPE_FOLLOW 5 = AI_MOVEMENT_RUNNING,
AI_ACTION_REPEAT_ONCE 1 = ACTION_REPEAT_UNTIL_FINISHED, AI_ACTION_RETURN_SUCCEEDED 2 = AI_ACTION_FINISHED).

## Ports (row moved onto shared headers)
- 0x0088B760 onto engine/CAIBrain.h + CAIStateGroupBase.h (retail); EScriptAIPriority enumerators defined in the row (the header's
  forward enum; the 174-row shape). MATCH.
- 0x0093DC10 onto a new retail-class opt-in (hdr/README.md); the CMeleeCloseMethods stand-in is gone.
- 0x0095EDF0 / 0x0094F000: stand-in structs CIntelligentPointerThing / CIntelligentPointerConstThing replaced by the shared templates
  (+ FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_INLINE). Their objects now define the shared template COMDATs
  (??$?0VCThing@@@?$CIntelligentPointer@$$CBVCThing@@@@..., ??1 / ??_7 / ??_G of CIntelligentPointer<const CThing>) instead of the stand-in
  ones; vtable 0x01237F1C moves from ??_7CIntelligentPointerConstThing@@6B@ to ??_7?$CIntelligentPointer@$$CBVCThing@@@@6B@ (RENAMES s2).

## Decisions for the coordinator
- 0x006ED8C0 keeps a same-TU `__declspec(noinline)` STAND-IN body of DeregisterAttackingCombatant (NearSink_s7_0, not the owner's body),
  so its object strongly defines 0x006ED510's symbol, now in the PDB spelling. It did the same under the old spelling at HEAD
  (pre-existing one-definition debt, not created here); the nf96a rule wants the owner's body verbatim. Hold or accept as before.
- q11715: may a retail-class opt-in go into engine/_quarantine/CAIStateGroup_EatAtTable.h (nf155 did _quarantine/CThing.h), or does the
  quarantine header not count as a shared header (then the self row may model the class locally like the landed caller 0x0096CD70)?
  The same question decides CCombatSequenceBase in 0x006EAC50 (only _quarantine/CCombatSequenceBase.h; ten landed siblings model it
  locally, and CCombatSequenceGeneric.h says "the includer supplies CCombatSequenceBase").
- q11937-q11940: no decision needed after all: slot 3 confirmed (rtti-vtables.tsv) and the full-register epilogue is VC7.1's bool
  `a && b` materialisation (landed bool sibling 0x0090FC50). Next form for 0x00917330 is work/00917330_d.cpp plus fable_compiler.h.

## One definition (not link-tested)
- 0x0095EDF0 / 0x0094F000 now emit the shared-header COMDATs of CIntelligentPointer<const CThing> (converting ctor from <CThing>, ??1,
  ??_7, ??_G). The landed owners 0x007C2930 / 0x00930D20 define the same-type copy ctor (a different symbol) through a local template
  model plus `template class CIntelligentPointer<const CThing>;` (explicit instantiation: its ??1 / ??_7 / ??_G are template COMDATs too).
  No landed row defines ??$?0VCThing@@@?$CIntelligentPointer@$$CBVCThing@@@@QAE@ABV?$CIntelligentPointer@VCThing@@@@@Z (reloc_bindings
  has no line). A landing link test with LINK= cleared is still advised (nf140).
- Vtable 0x01237F1C: binds.py on both pS objects shows ??_7?$CIntelligentPointer@$$CBVCThing@@@@6B@ at +1c/+49 (0x0095EDF0) and +49
  (0x0094F000); reloc_bindings already has that spelling and the stand-in at 0x01237F1C (2 spellings; these rows leave the stand-in).

## Locks claimed this session (plain mkdir or released -> NF169)
0x0090EEC0 0x0090F470 0x0091E750 0x009357F0 0x009574D0 0x00957750 0x00958000 0x00959A60 0x0096EE90 0x0092B3E0 0x009352C0 0x0095EDF0
0x009621F0 0x0096FCD0 0x006ED8C0 0x006EDA80. Claimed but unchanged: 0x0092B3E0 0x009352C0 0x009621F0 0x0096FCD0. No acctB or HELD row
touched.

## Casts / stand-ins
- No cast added. No new local model of a shared-header class (selfcheck 0). No Flags lines added. No descriptive names added.

## Rule slips
- Several early Bash calls did not start with the brief's env line (read-only greps).
- Queue order: q11680 was finished before q11667 (both done).
- 0x006EAC50 forms 1-2 were spent on a truncated define block (see above); 0x00917330 form 3 was a SRC_FAIL (missing include).
- q11895 / q11896 were left open while later entries (q11930 .. q11941) were worked (queue order); q5101 / q5102 / q11714 / q11934 were
  skipped as large ports (open).
- The 0x008B0610 test edit was not needed (standalone model); kept, PassPattern unchanged.
