# 00d7e0c0 final handoff

Final status: **VERIFIED_MATCH**. Selected grade: 4. 

ROW.json and root STATUS.tsv are authoritative. Earlier status claims below are historical; they may have been superseded.

## rows/00d7e0c0/NAMING_EVIDENCE.md

# 00d7e0c0 naming evidence

Both original PDBs identify private NScript::CQ_HeroSoulsArenaScript::CHOHGuard::Main. Ego_r nested TPI10fde has size1c, protected ParentClass reference14 and PMasterData pointer18. Native own factory00d7dff0 allocates1c, copies Me and installs vtable012d3ca4; slot1 is00d7e0c0. Exact Leaf is extracted independently and checked by objdump. Return00d7ebd1 plus14CC proves full2834-byte extent, including cleanup paths.

Original owner TPI10f73 is fully modeled with all ten nested classes, exact methods and ARENA_LEVEL_REACHED enum from TPI10f61. Native owner factory00d80ba0 allocates68, initializes Game40/MasterScriptData44, GuardChatTimer58 and vector words5c/60/64. Original owner fields CreatureCount5c, booleans60..66, ArenaLevelReached68, GuardChatTimer6c, Rounds70 contract to48,4c..52,54,58,5c with established base/vector retail layout. Own owner Init00f26b90 (slot3 of012d3994) independently clears these flags, initializes vector5c and timer58. Main uses PlayerHitSummoner1/2 at4d/4e and PlayerAttacksGuard4f.

Original function-local records prove SummonerA, SummonerB, Fred, seh_me, _Fail_Val, five distinct long HLStyleConversationIndex locals, speech_movie and PauseMyEntitiesPlease. Native/own donor independently confirm combat while(SummonerA.IsAlive()||SummonerB.IsAlive()) with NewScriptFrame at its start, post-combat constant for-frame loop, and aggression target/indefinite wait inside that loop. The source does not move aggression actions outside the original loop. Fred is GetNearestWithScriptName(Me,HOHGuard). Combat timer comments are gated by distance<30; distance>15 uses Fred's alive state, while near comments depend on hero-hit-summoner flags. After combat comments use distance<15. Each timer reset is30. Talk and hit checks are independent.

CScriptThing::IsAlive is an original authored virtual method; native004ab130 proves return !PImp.IsNull() && PImp->IsAlive(), including original virtual slot75 and false-null result. It stays ordinary inline. CIsThingAlive has genuine authored ctor copying private Thing and empty destructor. Original STB/Movie special members have raw generated flags0x103/0x107 in both PDBs; all remain implicit or external declarations. Native calls Movie ctor006e7b60 and destructor006e7b80, matching selected operands.

pDSTGame is original CGameScriptInterfaceBase* at0143e8f8; timer helper GetTime/== bodies and assignment use original private long TimerIndex and Game slots89/90. Native first and second timer assignments expand SetTimer. Ordinary attempts1/2 leave second assignment out of line, so selected3 expands only the exact genuine authored assignment body. Native pause ctor expansion and donor0359fd38 prove its exact authored body; ordinary1 fails. Original stronger keywords are unproven; the clarified Owner original-helper category applies. No generated member body is authored or strengthened.

Every emitted relocation is checked at its exact native operand against original symbol/type and independent native target/string/float data; all patched bytes equal full native extent. Fourteen Game slots and five Thing slots are checked from their actual vtables. Original enum spellings HIGHEST4,GROUP_SELECT_NONE0,HEAL_LIFE14 are retained. The exact selected Main passes3497 scenarios per compiler on GNU64 and nativeVC7.1 /GR-.
