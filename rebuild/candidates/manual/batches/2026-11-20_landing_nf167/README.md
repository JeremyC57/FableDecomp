# Landing nf167 (Account A, 2026-11-20): TC component names, CTC A-H (23 rows)

Agent NF167 (round 10) handed back 25 entries (11 done, 3 done-already, 9 blocked, 2 set aside for locks, returned to the queue);
its files are in `naming/2026-11-20_agentNF167/`.

- 23 rows. Self renames (5): CThingFilter_EnemyCreature::operator() 0x006A7550, CTCHeroStats::GetMoralityPercentage 0x0057A690
  (both tests rewritten on the shared headers), CTCCreatureModeManager::CModeBase::FrameUpdate 0x0073AC50,
  CTCHeroRangedAttacks::ContinueAimingRangedWeapon 0x00789FD0, CTCDExperienceOrb::CombineWithExperienceOrb 0x0077A130.
  0x008A7F60's existing static_cast from the component map's void* now targets the real CTCBob instead of a deleted stand-in.
- Headers (add-only opt-ins, `orig` equals HEAD, existing asserts line-pinned): CTCDrunkenness.h (DRANKFRAMES: inline
  DrankFrames / GetDrunkenness / SetDrunkenness and the private static DrunkennessDeltaPerFrameOfDrinking), CTCCarryable.h
  (GETCARRYSLOTPASSIVE), CTCEnemy.h (ARETHINGSENEMIES), CTCCreatureOpinionOfHero.h (CLEARPENDINGDEEDS).
- Coordinator call, q4151 / q4152: the statics take the private spelling (`@@0MB` / `@@0JB`). Ego_d.pdb PUB32 lists them that way,
  the headers declare them private, and only members read them. The queue's public `@@2` claim is recorded in REJECTED.
- Recheck (`logs/recheck.log`): 23 RELOCATION_MATCH, every test PASS. rebind multi-check 1 (baseline); split gate 0 created;
  includers 201 good, 0 bad. tablecheck: 11 addresses shrink (0x0057A67E 4 -> 1). 0x0057BC6E shows a second spelling from
  0x006D0180, whose text has it at HEAD too (the binding was missing from the HEAD table). Queued.
- Link (`logs/link_landing.log`): 3 LNK2005, the same three at HEAD (`logs/link_control_head.log`).
- Queued: the 22-row CTCCreatureModeManager mode family (q11669), the CTCHeroRangedAttacks.h GetShotBuildupTime header item and
  0x00899C10 (q11687), the 0x0089A070 / 0x0089A0E0 tests (q11768), 0x0057BC6E, and NAME_FIXES (0x007F6DF0, 0x0057C2E6,
  0x00686D30).
