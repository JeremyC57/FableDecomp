# Hand chunk 2015 (CAIStateGroupBase .. CTCWillLightning): 2 of 2 byte-exact

Both rows grade RELOCATION_MATCH under VC7.1 RTM from the exact files in src/ (gall.sh with `FABLE_HDR_OVERLAY=hdr`), their behaviour
tests print `<addr>_TEST PASS` under g++ (tall.sh) and VC7.1 (tall.py), and the read-only verifier (vland.py, overlay on) reports 2 WINS.
Every call target / global was checked against retail (tmp/rel.py: each relocation of the built leaf against the retail target at the
same offset; all agree, the DIFF lines are binding-name differences filed in NAME_FIXES.tsv). Extents as in the prior outline (both real
function starts; 1533 and 1517 bytes correct; no SPLITS.tsv).

## What matched (2)
- **0x0097FEA0 CAIStateGroupBase::AIAction_FindAGuardAndYellForHelp** (1533 bytes; 7 grades in pass 1, plain C++, no device / flag).
  Looks up the STANDARD_MESSAGE_EVENT_YELL_FOR_HELP message def, finds the nearest guard (GetPBestLeafThing with
  GFFilter_Bind2(not1(CThingFilter_HasScriptedControl(true)), CThingFilter_AICreatureHasProperty(3)) and CThingCompare_Nearest), then
  loops while the proximity move (2.0, run) is running: stops with DONE when the village is hostile because of crime; once moving it
  plays TEXT_AI_CRIME_RUN_TO_GUARDS (retrying each frame, StopSpeaking + STOPPING when the group stops); an EXPRESSION_APOLOGY event from
  the hero removes the event, clears the apologisable crimes, notifies the village, posts APOLOGY_ACCEPTED / _REFUSED (IsStillValid) on
  the hero's deed log, clears the attacker when the hero is no criminal, and aborts; the yell message is re-sent every 0.8 lifespans
  (note: last_yell_frame starts at 0x80000000 and retail's 32-bit `frame - last > interval` wraps, so for non-negative frames the first
  yell never fires -- modelled as is). On DONE the guard's village bribe frame is reset and the creature's opinion reports all crimes to
  the guard's. Shape levers (each measured by a grade): the two literals as IMPLICIT `const char*` -> CCharString arguments (retail
  builds the temporary before pushing the other arguments); the village-member GetTC as a statement with its result unused under the
  DEFAULT GetTC body (not GETTC_EXPANDED): retail stores the HasTC bit (0x0097FFEF) into the byte the `moving` flag later reuses; the
  speech retry as `do { speech = Play(..); if (speech != RETRY) break; yield; } while (!IsStopping()); if (speech == RETRY) {stop}` (c2
  threads the retest away; a for(;;) / while form nested in the outer loop is rotated, the call duplicated -- measured with fragment
  probes); the outer loop as `do { } while (result == RUNNING)` (a for(;;) with a bottom break loses retail's aligned loop head `jmp ;
  lea ebx,[ebx]` at 0x0097FFD8); `last_yell_frame` initialised before `frames_between_yells` (its store sits between fild and fmul).
- **0x007A0D90 CTCWillLightning::GetTargets** (1517 bytes; 7 grades in pass 1). From the source thing: for the player caster the
  targeting component's targeted thing (must be alive, melee-vulnerable if a creature, lightningable) or the override thing, else
  `GetAllThingsInRoughArea<CThingCreatureBase>(box, GFFilter_Bind4(IsNot(source), IsNot(player), IsTargetable(false, true),
  In2DRange(pos, area)))`; sorted nearest first (std::sort, CThingCompare_NearestToPos); each candidate not yet targeted, not
  underground, lightningable and hostile is either assigned to every beam of the list (retarget mode) or gets one new beam while the list
  is under the limit; targets are appended to the targeted list. Shape levers: `else return false` when neither override nor targeted
  thing is alive (retail's single `xor al,al` exit); FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN (GetPItem then SetPItem for the beam's
  pointer members; the implicit copy writes PData directly); VISIBLE_CALLED ctor / dtor of the counted pointer.
  **Inline-budget device** (owner decision 2026-11-08, preference 3): `#pragma inline_depth(0)` around the new-beam arm's GFRandom
  statement and its list<CCountedPointer<CLightningBeam> >::push_back. Retail calls those two (0x004CDAE0 / 0x007A3870) but expands
  GFRandom in the retarget loop and list<CThing*>::push_back in both arms, and expands the first vector push_back (0x007A0EFC calls
  _M_insert_overflow) while calling the second (0x00436C80). c2 inline trace (tmp/inl.sh, c2.dll 0x1078D1AA): new-beam sites f 0.536
  (GFRandom c1 42 sc 47, CCP push_back c1 51 sc 57, list<CThing*> push_back c1 37 sc 41), first vector push_back f 0.199 sc 61
  (expanded): no /d2inlT<N> fits (needs N > 61 and N <= 47); retail's split needs 0.125 < f <= 0.158 at the new-beam arm. 16 plain-C++
  variants traced (no grade): for / while / for(;;) / do-while / flag-condition target searches, `||` and nested-if skip tests, if /
  else / continue arm forms, both size() spellings; the new-beam arm only ever took f 0.536 or 0.0715. With the device the only residue
  was `add eax,[edi+0DCh]` vs a register add, closed by forming the sum inside the region (`long onhit_every_frame = GFRandom(..) +
  m_OnhitEveryFrame;`).

## Parked for Fable 5.1 (0)
None.

## Skipped (0)
None.

## Grades
- 0x0097FEA0: pass 1, 7 grades (f1 1586v1533, f2 1565v1533, f3 1581v1533, f4 1533v1533 12 instr off, f5 1533v1533 3 off, f6 MATCH,
  final src gall MATCH). Plus 8 fragment probe compiles (speech-loop shapes) and 2 SRC_FAIL compiles (missing opt-ins), no diff.
- 0x007A0D90: pass 1, 7 grades (g1 1562v1517, g2 1594v1517, g3 1519v1517, g4 1519v1517, g5 MATCH, src gall MATCH, src gall after moving
  the predicate declarations into shared headers MATCH). Plus 16 c2 inline-decision traces (no diff). One compile hung to the 200 s
  timeout because I graded without the overlay (missing include); it produced nothing and is not counted.

## Names
All row names and callees are PDB-proven (two sources each, LABELS.tsv). Descriptive (no PDB enumerators on disk): the enumerator names of
EAIActionReturn / EAIActionRepeat / EMovementType / EOpinionPostDeedType / NSpeechGainManager::EDialogueLayer used in the FindAGuard
source (values from retail; the first three follow the landed AIAction rows); parameter and local names. Wrong landed / bound
spellings found: NAME_FIXES.tsv (12 entries: PeekDefinitionManager, GetDef<CMessageEventDef>, PlayAISpeech text overload, AddMessage def
overload, RemoveEvent, ClearAttackedByCreature, NCreatureAI::StopSpeaking, sort<CThing**, CThingCompare_NearestToPos>, nested
CLightningBeam ctor, CCountedPointer<CLightningBeam> ctor / dtor, the Bind4 GetAllThingsInRoughArea instance).
Verifier warnings for the coordinator (RELOC_NAME, expected until the fixes land): 0x007A3CA0 (landed as a stand-in), 0x007A3200
(catalog `_global::CCountedPointer_Release_007a3200`), 0x006691C0 (landed as `CAIStateGroupWithBuilding_006691c0::Cleanup`).

## Header debt
All header work is in hdr/ (README.md): 6 new headers (CThingFilter_HasScriptedControl, CMessageEventDef,
NEntityEvents_CEventExpressionPerformedTo, CThingFilter_IsTargetable, CFilter_Bind4, CThingCompare_NearestToPos) and opt-ins in 8 copied
repo headers (CAIStateGroupBase, CTCVillage, CTCEntityEvents, CThingCreatureBase, CTCCreatureOpinionOfHero [from the c2004 overlay],
CMessageEventManager, NCreatureMode, CTCWillLightning). NEntityEvents_CEventExpressionPerformedTo.h defines NEntityEvents::CEventBase with
slots 0..4 (the landed GetEvent rows' model); engine/NEntityEvents_CEventHitBase.h defines a smaller CEventBase -- the two must be merged
before a TU includes both. CTCWillLightning's retail class keeps m_tTimer / WeaponState opaque (CWillButtonHelperTimer exists under the
AICASTING opt-in; not needed here).

## Label corrections
0x006691C0 landed as `CAIStateGroupWithBuilding_006691c0::Cleanup` is CThingCreatureBase::ClearAttackedByCreature; 0x007A3CA0 catalog
`GetPBestThingInRoughArea<..IsAICreatureAwareOf..>` is GetAllThingsInRoughArea<CThingCreatureBase, vector<CThing*>, Bind4<..>>; 0x007A3200
/ 0x007A4210 are the CCountedPointer<CTCWillLightning::CLightningBeam> dtor / ctor (landed under unproven / address names).

## Tools (tmp/, copies or new)
md.py / rel.py (from agent2004), pq.sh (preflight + grade + summary, overlay set), ours.py (our leaf at retail addresses), mc.sh / fa.py
(fragment TU disassembly), inl.sh + inl/ (the agent2003 c2 inline-decision trace, pointed at this directory).
