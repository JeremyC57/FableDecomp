# Landing 2026-11-08d: authoring chunk 853 (1 of 6 matched + a two-function row recut and landed; 4 parked) and one respelling

| Row | Grade | Note |
|---|---|---|
| 0x0043A570 CPlayerGui::~CPlayerGui | **RELOCATION_MATCH + PASS, landed** | was placeholder NHeroInformationScreens_Destructor; CPlayerGui vtable store 0x0123177C, sole caller `??_G` 0x00444F20, `??1CPlayerGui@@UAE@XZ` in both PDBs |
| 0x00CE6CF0 NScript::CGameflowScript::Init | **MATCH + PASS, landed** | the catalogue row (2237 B) held two functions: Init is 16 bytes and ends `jmp +0` into the next |
| 0x00CE6D00 NScript::CGameflowScript::DeclareGossipCategories | **RELOCATION_MATCH + PASS, landed** | new row (2221 B), added to `rebuild/manifest/boundary-starts.tsv` and the manifest; Ego_r + FableWin |
| 0x00D3B410 NScript::CQ_GuildTrainingScript::GossipSetup | **RELOCATION_MATCH + PASS, re-landed** | respelling of the landed `CGameflowScript::DeclareGossipCategories` (that name is 0x00CE6D00); removed with `repair_remove.py` and re-landed under the PDB name, bytes unchanged |
| 0x0050A650 CWorldMap::GetMapNavigationAreaInit | DIFFER(2233v2233), 6 lines | chunk 850's SRC_FAIL now compiles; the 4-byte register residue Codex pass 13 also hit (ebx v eax at the last vector free) |
| 0x00E19560 NScript::CQ_WhiteBalverineWWScript::SpawnBalverines | DIFFER(2159v2078), 59 lines | inline budget: one ctor inlined where retail calls 0x007E72A0, one ~CScriptThing expanded |
| 0x00DF5000 NScript::CQ_TentacleKrakenBossFightScript::PhaseOne | DIFFER(3039v3071), 92 lines | ~CScriptThing split in the loop, ~map expanded where retail calls 0x00CDBFB0, one frame-slot pair swapped |
| 0x0066B370 CTCSoundPlayer::PlayAllSoundEvents | DIFFER(1805v1808), 439 lines | was gap-scan placeholder; register allocation only (this in edi v esi) |

- **Headers landed:** CPlayerGui (MEMBERS), NHeroInformationScreens_CManager (LAYOUT), CDefPointer (nested RELEASE_COUNTREF),
  CSubtitleRenderer (DTOR). Each is an `#ifdef` / `#else` branch that keeps the original lines on the default path, and no landed file
  defines the macros, so the 2,982 landed includers (CDefPointer is everywhere) compile the same text; the agent regraded the 10 nearest
  siblings with the overlay (`pending/2026-11-08_linux_hand853/SIBLINGS.tsv`). The other opt-ins (C2DBoxF COPY, CNavigationPosition,
  CSymbolMap, CTCSoundPlayer, CThingSearchTools, CThingVillage / CThingAICreature, new CQ_TentacleKrakenBossFightScript.h) serve only
  parked rows and stay in the pending dir.
- **Names:** labels `labels_landing_2026-11-08d.tsv` (PlayAllSoundEvents 0x0066B370, GetAllCombinationAnims 0x0070D100,
  GetSoundEventManager 0x006F90C0, GetAnimEventsBetweenTimes 0x006F99B0, the two NPlayerGui list `clear`s 0x0043FE40 / 0x0043FDF0).
  Queue `land0811d` (9; UpdateSounds -> PlayAllSoundEvents needs the landed FrameUpdate 0x0066BBF0 respelled).
