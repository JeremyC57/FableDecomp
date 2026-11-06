# Hand chunk 786 (MurderTwist brothers, PrisonEscape Main / CPrisoner, KGG villager, OrchardFarm crate member): 4 of 6 byte-exact

All 4 matched rows are RELOCATION_MATCH. vland (read-only, empty overlay) finds no RELOC_IDENTITY. It warns RELOC_NAME on
0x00DD2CE0 only because the manifest labels of 0x00DD9520 / 0x00DD9900 are wrong (see Label corrections), and RELOC_ALIAS on the
rand import thunk. Every test passes under g++ and VC7.1, the differ/ tests included. hdr/ is empty: no header changes.

## What matched (4)
- **0x00EACF00 CV_MurderTwistScript::CMurderTwistBrotherOlderFriend::Main** (first grade):
  - the landed CMurderTwistBrotherFriend::Main 0x00EAC5B0 body with this row's four lines;
  - MurderTwistEvent = MTE_FIND_HIDDEN_GEM (7).
- **0x00EADBB0 CV_MurderTwistScript::CMurderTwistBrotherYounger::Main** (first grade):
  - the Friend top, then the FIGHTING block: `++ParentClass.MurderTwistItemsFound`, two surrender lines, MTE_MEET_LEADER,
    MTBS_NEARLY_DEAD, `ModifyThingHealth(Me, 10.0f, false)`;
  - the OnPredicateFail 0x00EADB60 erase loop over MagicRuneRumbleList (first match, then break), with
    FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE;
  - the BrotherOlder 0x00EABB30 NEARLY_DEAD loop (health 1 -> MTBS_DEAD), then RemoveThing(Me, true, true).
- **0x00DD2CE0 CQ_PrisonEscapeRescueMotherScript::Main** (second grade; the first failed on a missing CEntityScriptBase include):
  - the CQ_TraderEscortScript::Main 0x00E00DB0 model: SetTeleportingAsActive(false);
    SetQuestCardObjective(GetActiveQuestName(), "TEXT_QUEST_PRISON_ESCAPE_OBJECTIVE_02", "PrisonCourtyard", "");
  - 15 bindings with flags 0 / 1 x8 / 0 x6; the four key doors (StoreRoomDoor, TortureChamberDoor, BarracksDoor, PrisonKeyDoor)
    share CPrisonKeyDoor::Alloc;
  - PostAddScriptedEntities x2, AddLogbookStoryEntry(220), four spawned threads. The relocation order was checked by hand.
- **0x00EA0090 CV_KnotholeGladeGatesScript::CKGG_VillagerOutside::Main** (first grade, with and without /Gr):
  - the Friend defines and includes. A local CV_KnotholeGladeGatesScript (the chunk 782 layout) holds the nested class
    (ParentClass +0x14, PMasterData +0x18, Moved +0x1c, Attacked +0x1d).
  - The male test is `Me.GetDefName() == "CREATURE_KN_VILLAGER_MALE"`. This is the inline CCharString text equality: the null arm
    uses the strcmp intrinsic against "" and the other arm uses Compare 0x00411570. It needs both FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
    and FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE.
  - There are three arms per frame:
    - talked to: movie, pause and intro line;
    - near the gate with rand() % 200 == 0: a panic conversation;
    - otherwise, while the gates are shut: look at the gate, attack it and, if the hero killed a villager, panic about the hero.

## Parked (2)
- **0x00DD5A60 CQ_PrisonEscapeRescueMotherScript::CPrisoner::Main**: DIFFER 2246v2250, 20 difflines, 12 attempts.
  - The first half, the conversation waits and every call and literal match.
  - The function-final GUARDS loop's exit block schedules `push ebp` before the pDSTGame loads. Retail loads first and pushes after.
    Two side effects follow:
    - the cell-door cold exit (retail 0x00DD5DDA: out-of-line ~CScriptThing, inline ~CTimer) is cross-jumped into our final block;
    - the COURTYARD-comment terminate exit gets its own block, where retail jumps to 0x00DD6319.
  - The loop-head macro (lever 4) is what made the GUARDS exit hot, as retail has it.
  - None of these moved the residue: do-while(0) at four other checks, bottom-tested loops, or an inner door scope.
- **0x00DCE230 CQ_OrchardFarmRaidScript::CCrateTeamMember::Main**: DIFFER 2605v2589, 82 difflines, 14 attempts. The whole control flow
  is decoded and every call, literal and branch is in retail order.
  - The blocker is the prologue `SetPredicate(CIsThingAlive(Me))`. Retail calls the CScriptThing copy 0x004ABE90 inside the inline
    CIsThingAlive ctor, but expands the same copy at the two hot by-value sites (SetCombatNearbyBreakOffRange(Me, ...) in member
    states 2->5 and 5->0). VC7.1 expands it everywhere hot, the prologue included.
  - This is the still-unexplained INLINE_DEPTH_REVISIT family.
  - Two probes bracket retail:
    - `: CBaseClass(other) { assign; if (Info) ++RefCount; }` (a6) keeps the prologue copy inline and leaves dead stores at the hot
      sites;
    - `: CBaseClass(other), PImp() { PImp = other.PImp; }` (a7) makes the prologue exact but takes every copy out of line.
  - The rest follows from the prologue's register choice: &Me is not in edi from entry, the two RemoveThing-path decrements are not
    split into load / dec / store, and the DEFENDING block's eax/edx are swapped.
  - IsBeingCarriedBy and GetPos have to be __forceinline. These are the CScriptThing wrapper bodies; retail expands both inline.

## Names
- Proven: all six rows (PDB name sets + the single vtable reference of each entity Main, or the binding chain for 0x00DD5A60;
  0x00DD2CE0 is slot 2 of quest vtable 0x012DB86C).
- Descriptive (marked in the sources): the locals (comment_timer, cell_door, conversation, seh_me, speech_movie, male,
  fetching, near_crate, crate, carrier, exit_marker). The CCrateTeamMember AI-state numbers (1, 2) and member states (0..5) are
  written as numbers because no enumerator names are known.

## Label corrections
- 0x00DD9520 is CQ_PrisonEscapeRescueMotherScript::SplittingUpCutscene, not ::PrisonEscape.
- 0x00DD9900 is CQ_PrisonEscapeRescueMotherScript::ProcessRaces, not CV_ChapelOfEvilScript::EndMission.
- 0x00DCEC50 is CCrateTeamMember::SetState, not CHandleBasedMemoryPool::GetPointerFromHandle.
- The landed GoOnPatrol 0x00DCEC70 and IsThingCarryingCrate 0x00DCED10 use global stand-in classes. Their PDB spellings are nested
  in NScript::CQ_OrchardFarmRaidScript (NAME_FIXES.tsv).
- Catalog modules for 0x00EADBB0, 0x00DD5A60, 0x00EA0090 and 0x00DCE230 lack the enclosing script class.

## Header debt
- None changed. The CScriptThing copy-ctor size question (above) would be a CScriptThing.h / CCountedPointer.h change if a spelling
  is found. It should be tried on the whole CIsThingAlive family first.
- The CQ_OrchardFarmRaidScript retail layout (Ego_r - 0x14 before CrateList, - 0x18 after it) and CCrateTeamManager are local models
  in four landed rows plus this differ. They are candidates for engine/CQ_OrchardFarmRaidScript.h.
