# Landing nf168 (Account A, 2026-11-20): TC component names, CTC I-Z (31 rows)

Agent NF168 (round 10) handed back early (heavy context) with 32 entries: 12 done, 5 done-already, 3 blocked, 2 set aside for
locks, 10 open. Its files are in `naming/2026-11-20_agentNF168/`. The 12 unfinished entries go back to the queue unclaimed.

- 31 rows. q4183: the four subtitle rows now use the real STLport map (FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE), with self renames
  CTCInventoryTrade::FinalConstructSubtitles / FinalRefreshSubtitles / FinalConstructSubtitlesPC / FinalRefreshSubtitlesPC
  (0x0059D0B8 / 0x0059D1B1 / 0x0059D65A / 0x0059D761) and ConstructSubtitlesPC 0x0059E4D7, plus CTCInventoryExperience::GetXString
  0x0056EA4E. This removes HEAD's local CSubtitleMap::find / FinalRefreshSubtitles clashes (link below).
- Coordinator call: 0x005EE136 was catalogued GetAbilityLevel, a name that belongs to 0x005EDE93. Its body reads +0x148
  (ExperienceSpentOnAbilities), so it is ?GetExperienceSpentOnAbility@CTCInventoryAbilities@@QBEJW4EHeroAbility@@@Z (Ego_r.pdb
  0x0057CC91, FableWin.pdb 0x01EBC6E6). Renamed and ported (MATCH); its only referencer 0x005724CD is respelled with it.
- Headers (add-only declaration opt-ins, `orig` equals HEAD): CTCInventoryTrade.h (FINALSUBTITLES), CTCPerceiveThing.h (four
  flat-branch members), CTCInventoryAbilities.h (GETEXPERIENCESPENTONABILITY).
- Recheck (`logs/recheck.log`): 1 MATCH + 30 RELOCATION_MATCH, every test PASS. rebind multi-check 1 (baseline); split gate 0
  created; includers 280 good, 0 bad. tablecheck: no address gains a spelling; 0x007446A0 9 -> 4 and 12 more shrink.
- Link (`logs/link_landing.log`): 3 LNK2005 against 7 at HEAD (`logs/link_control_head.log`). The three GetSubDef clashes
  remain; FinalRefreshSubtitles and CSubtitleMap::find (x3) are gone.
- Queued: the two atexit-destructor stand-ins under the pair rule (q4223 / q4224), the 0x00811760 port (q5162), 0x00917DB0's
  non-const GetShopTC, and NAME_FIXES (0x00775350 UpdateStock, 0x00803540 UpdateTargetedPositionRandomWalk).
