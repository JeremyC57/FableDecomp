# Landing nf196 (Account A, 2026-11-20): NF191's returned families: IS_THING_ALIVE, CTCVillage, CCombatActionBase (17 rows)

Agent NF196 (round 10) handed back early (context) at q6530 with 50 entries: 6 done, 4 done-already, 9 blocked, 4 deferred and 27
open. Its files are in `naming/2026-11-20_agentNF196/`, with the q6449 census and drafts and the ungraded q6532 draft
(`work/00e2e440.cpp`). The open entries, the partial families with rows left to port (q4411 / q6405, q4492, q4522, q6321, q6457,
q6500) and the entries deferred for rows since landed (q6465 / q6467, q6492) go back to the queue unclaimed (37 entries). q6396 is
closed by nf192.

- 17 rows land: 2 MATCH + 15 RELOCATION_MATCH, every test PASS.
- IS_THING_ALIVE instances: q6447's claimed spelling was wrong (REJECTED.tsv). 0x00436380 is the PBV instance
  `??$IS_THING_ALIVE@PBVCThing@@@@YI_NABQBVCThing@@@Z`: a self rename, with its binders 0x0057D5A0, 0x0064A1AE and 0x007BDAC7. The four
  CTCVillage rows 0x0065B2C4 / 0x0065B349 / 0x0065BFFA / 0x0065C45A move onto engine/CTCVillage.h + CTCDummyVillager.h and bind the
  PAV instance at 0x0040DCE0 (q4522). 0x005C3A95 / 0x0065C781 move onto CIntelligentPointer<CThing> (q4492).
  - The CTCVillage rows set `#pragma optimize("s")` / `("y")` after their shared includes (their TU regime) and use
    FABLE_RETAIL_STLP_BITSET_TEST_DECL.
- Others: 0x00956380 onto the shared CastWillLightningSpell MAZE opt-in (q4411), 0x0093C6D0 GetRandomAnimFromName (q6456; new
  FLAT_GETRANDOMANIMFROMNAME opt-in), q6522 / q6523 / q6524.
- Self renames (5, RENAMES section 1): 0x00836BE0, 0x00436380, 0x00CF0C00 (CEntranceGuard::GetParentScript), 0x00E53CB0 / 0x00E362B0 (the
  CBB_BeardyBaldyMan / CAC_Owner GetRandomSpeech). Their files move; no funclet includes them.
- Headers: declaration-only `#ifdef` opt-ins, no layout change, default paths token-identical, no landed includer defines them. `orig`
  equals HEAD for three. pin_asserts pinned 1 (CTCDummyVillager.h line 45).
  - engine/CTCVillage.h: TRYTORESPAWNDEFNAMED, ENABLEGUARDS, ISREPORTEDORUNREPORTEDCRIMEKNOWN, DETACHFROMDUMMYVILLAGER (the
    DetachFromDummyVillager guard becomes `#if defined(FABLE_RETAIL_CTCWIFE_MARRY) || defined(...)`).
  - engine/CTCCreatureOpinionOfHero.h: DECRIMINALISEALLKNOWNCRIMES, FINDMOSTRECENTDEED_FILTER.
  - engine/CTCDummyVillager.h: ISGUARD_INLINE.
  - engine/CThingCreatureBase.h: FLAT_GETRANDOMANIMFROMNAME.
  - B landed FABLE_RETAIL_CTCVILLAGE_PATROLTASK in CTCVillage.h (p25M) after the agent copied it. The landed header is a clean
    three-way merge keeping both; the asserts below are already pinned (`logs/header_merge_CTCVillage.txt`).
- Pick-any (`logs/pickany_check.txt`): every same-TU copy in the CTCVillage rows equals its owner (IS_THING_ALIVE<CThing*> 0x0040DCE0,
  IS_THING_ALIVE<CThingAICreature*> 0x004A88F0, IS_THING_DEAD 0x0065D639, the list iterator helpers 0x0041C780 / 0x0041C7A0 / 0x0041C7C0).
  By hand: the PBV IS_THING_ALIVE copies in 0x0057D5A0 / 0x0064A1AE equal the self row 0x00436380 (21 = 21); `?HasTC@CThing@@`
  (0x004118C0) has no landed row.
- Blocked with decision items: q6449 (ConstantFPS is protected; the four rows left differ through the GetConstantFPS() accessor, which
  reorders the load), q6513 (0x0122AE60 NullString atexit thunk: TU-emitter family of 296 rows, as q4411's ??_G rows).
- Casts: 0x0093C6D0 keeps the C-style downcast `(CThingCreatureBase*)GetCreature()` from HEAD (debt queued) and its local model until
  engine/CCreatureAction_PlayAnimation.h gets the 8-parameter PDB ctor (header item queued). 0x0065BFFA's `static_cast<const CThing*>`
  is an upcast.
- Link (`logs/link_landing.log`): 1 LNK2005 against 4 at HEAD (`logs/link_control_head.log`). The CreateVillager,
  `CAC_Owner::GetRandomSpeech` and `IS_THING_ALIVE<CBaseObject*>` clashes are gone, and no new one appears.
- Recheck (`logs/recheck.log`): rebind multi-check 1 (baseline); split gate 0 created. Includers and referencing rows 141 good, 0 bad;
  owner recheck good.
- tablecheck (`logs/tablecheck.txt`): twelve addresses shrink (0x00436380 4 -> 1, 0x0040DCE0 6 -> 5, 0x0062E900 3 -> 2, 0x004CC340
  5 -> 4, ...). One grows, 0x0076D2F0 2 -> 3 from 0x0064A1AE: SAME (an unchanged binding the table lacked). No out-of-image oracle line.
