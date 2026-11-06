# Landing 2026-11-06k: chunk 810 (0 of 6 rows landed, 6 parked)

One expert agent worked six unnamed gap-scan entity Mains (6.1-6.4 KB, 10+ attempts each, every attempt in
`pending/2026-11-06_linux_hand810/ATTEMPTS.log`). None matched, so nothing entered `src/compiled` and coverage is unchanged. All six
names are proven (slot 1 of the vtable the landed Alloc stores, plus both PDB name sets). The six best sources, their tests (PASS)
and the agent's header overlay (`hdr/`: new CQ_GuildTrainingMeleeScript.h and CQ_GuildTrainingWoodsDepartureScript.h; opt-ins
GANGLEADER / CFISHERMANSWIFE / CBORDELLOLADY in CQ_BountyHuntScript.h, CQ_HerosOldHouseScript.h, CV_BordelloScript.h) are in
`pending/2026-11-06_linux_hand810/`. As with chunk 793, the overlay is not copied into rebuild/include until a row that needs it lands.

## Parked
| Row | Grade | Residue |
|---|---|---|
| 0x00D8BCD0 NScript::CQ_HerosOldHouseScript::CFishermansWife::Main | DIFFER(6359v6343), 11 attempts | the CScriptThing copy 0x004ABE90 inside the expanded CIsThingAlive ctor (called in retail, inlined in ours), plus 3 loop pads |
| 0x00E3EB10 NScript::CV_BordelloScript::CBordelloLady::Main | DIFFER(6346v6330), 10 attempts | only the predicate copy (+16 bytes); a row-local inline CScriptThing copy fixed the four by-value Me copies |
| 0x00EC8300 NScript::CV_SickChildScript::CManInLove::Main | DIFFER(6268v6394), 10 attempts | the five by-value Me copies called where retail expands them; the inline-copy lever flips ebp across the whole body |
| 0x00D1B200 NScript::CQ_BountyHuntScript::CBH_GangLeader::Main | DIFFER(6180v6100), 11 attempts | the function-wide inline budget: retail expands the implicit movie dtor at all 7 sites |
| 0x00D56790 NScript::CQ_GuildTrainingMeleeScript::CMeleeOpponent::Main | DIFFER(6134v6151), 10 attempts | a scheduling split in the stage-3 counter load / inc / store (mostly offset shifts) |
| 0x00D62480 NScript::CQ_GuildTrainingWoodsDepartureScript::CArtifactThief::Main | DIFFER(6243v6195), 10 attempts | the predicate's nested ~CScriptThing expanded where retail calls 0x004AA840 |

**Shared blocker:** VC7.1 here cannot keep the copy CALLED inside an expanded CIsThingAlive / CIsThingConscious ctor while expanding
the top-level by-value copies (named local, `__forceinline`, IMPLICIT_COPY, definition order and position all tried). The next
chunk (812) takes quest-own Mains and jump-table Mains instead.

## Names
- Labels: `ghidra_out/labels_landing_2026-11-06k.tsv` (the six Mains); applied, the manifest sync dry run prints 0.
- 3 queue lines (tag `land0611k`): 0x00D1CAC0 is `?WatchForLeaderChargeGruntsKilled@CQ_BountyHuntScript@NScript@@QAEXXZ` (landed
  as WatchForAllLeaderChargeGruntsKilled, the name of the bool it sets); 0x009AC700 is the STLport
  `std::map<CCharString,CCharString>::operator[]` (bound under several CStringMap / CCutsceneArgMap / CTextArgMap spellings);
  0x0099EFB0 is `??4CCharString@@QAEABV0@ABV0@@Z` (bound as SymbolString and std:: variants).
