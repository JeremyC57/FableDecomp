# NF118 notes (round 10, PRIORITY reconciliation slice 6; 30 queue lines over 27 addresses)

Counts come from APPLIED.tsv / STATUS.tsv / recheck_final.log.
- Entries (35 queue lines): 12 done, 0 done-already, 0 rejected (two posed claims corrected; REJECTED.tsv lists the posed
  GetAbilityMutexLock for q6369 / q6388, both still done under the proven name), 0 blocked, 1 deferred (q4490, held by NF115), 22 open.
- Rows: 12 ported (rows/; 9 self renames in RENAMES s1, 12 callee respellings in s2), 15 not-a-binder (STATUS), 0 blocked.
- recheck_final.log: one vland --recheck-landed run over the 12 rows/ rows: 2 MATCH + 10 RELOCATION_MATCH, every test PASS.
  selfcheck.log: 0 problems.
- Final-log WARNs, all reviewed:
  - SHARED_HEADER_AVAILABLE: on local models the landed rows already had (in-place respells).
  - STANDIN_LEAF on 0x006AF220 / 0x0072A860: their catalog names change at landing (RENAMES s1).
  - RELOC_NAME 0x0072A860: the same catalog relabel.
  - Two pre-existing RELOC_NAME lines that pdiff shows unchanged: 0x00687D40 GetEventScrollAmount vs the catalog
    GetMouseWheelMovement (row 0x00690030), and 0x0051ED80 GetThingByUID vs the catalog GetPThingWithUID (row 0x00729BC0).
  - No RELOC_CONFLICT.
- Header opt-ins: 5, over three overlay headers (hdr/README.md).

## Claims settled or corrected
- q4018 (0x00718EC0): the queue called it "PDB-proven"; no PDB has a list<pair<long, EConversationAttitude> > member. The name is
  descriptive-by-library (STLport 4.6 `_List_base::~_List_base`, the spelling the only caller 0x00719C70 already binds; Ego_r layout
  CTCScriptedControl::AttitudeTags). Retail references the address only from 0x00719C70 and the ~list jmp thunk 0x007191A0, so it is
  not a linker fold.
- q6369 / q6388 (0x0072A860): `GetAbilityMutexLock` rejected as posed (REJECTED.tsv). Applied instead:
  ?IsChargingUpAbility@CTCSpecialAbilities@@QBE_NXZ (Ego_r 0x00741700 + FableWin 0x0254F790). Evidence:
  - Ego_r order: the donor run IsAbleToCastRaiseDead 0x007416D0 (private, bool) / AddHarvestedSoul 0x007416E0 (void) /
    SetupRaiseDeadSpell 0x007416F0 (private, bool) / IsChargingUpAbility 0x00741700 is retail's TU head: 0x0072A830 `xor al,al ; ret`,
    0x0072A840 `ret` (called by CCreatureAction_CastWillHarvestSoulSpell::PerformAction 0x008BA970), 0x0072A850 `xor al,al ; ret`, then
    0x0072A860. The other const bool getter of the run is private; the 0x0072A860 callers are other classes.
  - Ego_d.pdb carving (ghidra_out/egod_carved_functions.tsv): IsChargingUpAbility is a tc_special_abilities.obj member (.cpp body);
    GetAbilityMutexLock is a header-inline COMDAT (tc_hero_stats.obj), FableWin lists it only at mid-function 0x01E67417, and retail
    expands it in place (0x0062C001).
  - Body form: `return AbilityMutexLock;` grades DIFFER(4v9); `if (AbilityMutexLock) return true; return false;` MATCH (`// Inferred:`).
  - Callers: 0x0068F2A0 (the will-charge-up watcher keeps player mode 0x23 while it is true), 0x00690030 (no ability cycling while
    charging), 0x008E9B70 AnimUpdate (not landed; with GetCurrentAbilityStrafeAnim, its FableWin neighbour).
  - Interface 0xA7 = TCI_SPECIAL_ABILITIES (167). The stand-ins CTCHeroState (0x00690030) and CASuspendableProcess (0x0068F2A0) are
    CTCSpecialAbilities.
- Needs header claim: engine/CTCSpecialAbilities.h FABLE_RETAIL_CTCSPECIALABILITIES_STRAFE_CALLS_PASS24 declares an out-of-line
  `GetAbilityMutexLock() const` for the pending pass24 draft 0x008E9B70 (pending/2026-11-18_codex_pass24_open). That draft's 0x0072A860
  call should use IsChargingUpAbility. I left the opt-in alone; its only user is the unlanded draft.

## Callee self rows still on stand-in spellings (NAME_FIXES.tsv; pre-existing splits, not created)
- 0x006D3A30 / 0x006D4B40: GFSerialiseVectorBinaryOut / In<pair<CCharString, long> >. Their only caller 0x006D4C00 now binds the PDB
  spellings; the self rows' local vector model drops the allocator.
- 0x0088B760: CAIBrain::GetCurrentStateGroupScriptPriority. Its only caller 0x006D5AD0 now binds the PDB spelling; the self row is
  the derived stand-in CAIBrain_Methods returning int.
- The split gate counts only rows that reference a target, never the target's own self row. The final scan (work/gate,
  19 targets) lists 16 other referencing rows, and each already binds the new spelling (probe or callers.py): 0x004A5A40,
  0x004FD040, 0x005FF190, 0x006D2BD0, 0x006D4A20, 0x006D4AA0, 0x006AF270, 0x00719C70, 0x00729F60, 0x00746260, 0x0089B5B0, 0x00910CF0,
  0x0092C8A0, 0x0095DB90, 0x00969FB0, 0x009793D0. No split is created. Three are closed: 0x008332E0 (0x006AF270 already bound the PDB
  spelling), 0x00885010 and 0x006D44F0.

## Small entries settled by probe (work/probe_2r.tsv)
- q4619 0x006BBF70: both callers already bind ?GetNameGraphicOffset@CRegion@@QBE?BVC2DVector@@XZ. The self row returned a non-const
  C2DVector; respelled in place (MATCH).
- q5143 0x006A8930: proposal verified (Ego_r 0x005A48A0 + FableWin 0x01F95940; both callers bind it). Self parameter CThing* ->
  const CThing&, respelled in place.
- q5144 0x006A8A30: self row right. Binder 0x0092E540 ported from the local CThingFilter_BedOccupant stand-in to the shared
  CThingFilter_IsOccupiableOccupied (engine/CThingFirePointFilters.h). Its `not1` needs std::unary_negate, so the row also defines
  FABLE_RETAIL_STLP_REMOVE_COPY_IF (templates only, no code). First form: C2143 on unary_negate.
- q5078 0x006AF220: self row ported from the descriptive CDistanceFilter_006af220 to
  CMessageEventFilter_CanBeSeenOrHeard::operator() on engine/CMessageEventFilters.h (YELLFORHELP_UPDATE class) +
  CThingAICreature.h MESSAGERANGE. Reading the reference member twice reloads it after the call (DIFFER 49v49); the named local
  matches (`// Inferred:` line).

## Found in passing (not applied)
- 0x00729900 is a twin of the old 0x00718EC0 stand-in (free SortTreeRecursively_00436900). Its only caller is the
  CTCCreatureExpression scalar deleting destructor 0x00729A00, plus a jmp thunk 0x007299B0. It is another STLport _List_base dtor
  whose element type I have not proven.

## Open: the CTCHeroExperience family (q4486, q4762, q4764, q4765, q4766), analysed but not started
probe (work/probe_hx.tsv) of every REFS row:
- 0x006D6520 GetTrainableStatLevel: wrong binders 0x0057BE71 (CTCInventoryAbilities::GetAbilityLevel_006d6520 stand-in), 0x005EE887
  (QAEJJ), 0x0057C239 (QBEKJ); 14 rows already right. The self row is a free __fastcall GetAbilityLevel(S*) stand-in.
- 0x006D6510 GetStatExperience: wrong 0x0056FAD7 (QAEJJ), 0x0056E93D / 0x0056F4BA / 0x0056F597 (CTCHeroExperience_Calls),
  0x005724CD (_Calls, EHeroStat); 10 right. The self row is a free stand-in.
- 0x006D64A0 GetExperienceSpentOnStat: 0x005724CD binds ?GetTrainableStatLevel@CTCHeroExperience_Calls.. here. The self row is a
  free stand-in.
- 0x006D67B0 GetCostToUpgradeTrainableStat: wrong 0x005721FA / 0x005746F7 / 0x005724CD (_Calls); 3 right.
- The SExpSpendUndo rows (0x0056E93D SaveBefore, 0x0056F4BA RevertExperiencePoints, 0x0056F597 SaveAfter) also define their OWN leaves
  with CTCHeroExperience_Calls* (Ego_r ?SaveBefore@SExpSpendUndo@@QAEXPAVCTCHeroExperience@@@Z). Porting them renames those self
  rows, so their callers join the family. They also define a local enum EHeroExperienceStatCategory that clashes with
  engine/EHeroExperienceStatCategory.h.
- Header plan: the self rows need a vector layout without BLOCK_THRESHOLDS' inline GetTrainableStatLevel body, plus
  ExperienceSpentOnStats as vector<long> (+0x44, Ego_r layout). That means a new layout opt-in in the `#elif` chain plus a
  GetExperienceSpentOnStat declaration.

## Flags lines
None.

## Inferred lines (for the batch README)
- 0x0072A860: the if / return true / return false body. `return AbilityMutexLock;` DIFFER(4v9); `!= false` DIFFER(11v9).
- 0x006AF220: the named reference local for Creature (an extra-local tie-breaker); the member read twice is DIFFER(49v49).

## Descriptive names
None new. Library spelling (descriptive-by-library, STLport 4.6): 0x00718EC0 _List_base<pair<long, EConversationAttitude> >::~_List_base.

## Funclets
No EH funclet row includes any changed row: a grep of rebuild/src/compiled for the changed file names found only build_candidates.ps1.
The self renames move these parent files, none of which has funclets: 0x00718EC0, 0x006D4C00, 0x006B32A0, 0x00729BC0, 0x006D5AD0,
0x0072A860.

## Rule slips
- Brief-listed reading skipped or skimmed: EXTRA_RULES.md (read only the Inferred / bracket sections), ASM_RULES.md, NF92 NOTES,
  the PRIORITY table and the batch READMEs behind the entry tags.
- One Bash call tried a foreground `sleep` (blocked by the harness, nothing ran).
- Several early Bash calls skipped the lane preamble (`source $S/wine/env.sh`). The grade calls went through tools/g.sh, which
  sources it.
- One grade included the nonexistent engine/CCharString.h without a `cpp` precheck. VC7.1 spun at 99% CPU; I killed it with pkill
  after about 3 minutes, inside the 240 s timeout window.
