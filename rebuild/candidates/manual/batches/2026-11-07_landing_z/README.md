# Landing 2026-11-07z: authoring chunk 848 (4 of 6 landed, 2 parked)

| Row | Grade | Note |
|---|---|---|
| 0x00B2D9F0 CEngineSurfaceCompositionManager::UpdateDiskCache | **RELOCATION_MATCH + PASS, landed** | Codex pass-12 rebase; STLport find as a member template (D1); local CBankFile / CGraphicDataBank models (CBankFile.h on hold, header debt) |
| 0x009BC120 CPersistContext::TransferVectorOfSubComponentsLoadText | **RELOCATION_MATCH + PASS, landed** | Codex pass-12 rebase; GetAsCharArray as preset pointer + `if` |
| 0x00EA2720 NScript::CV_KnotholeGladeGatesScript::WatchForCutscene1 | **RELOCATION_MATCH + PASS, landed** | was placeholder KnotHoleGates_OnScript |
| 0x00D321C0 NScript::CQ_EndGameBossBattleScript::JackOfBladesBossFightCutscene | **RELOCATION_MATCH + PASS, landed** | was placeholder CScriptGameResourceObject_LoadEndGameSister; single-site do-while(0) round the cast-map assignments |
| 0x005253F0 CThingManager::LoadGameState | DIFFER(2263v2263) | text loop: UID low dword register v stack reload |
| 0x00DD4AD0 CQ_PrisonEscapeRescueMotherScript::CPrisonerAccomplice::Main | DIFFER(3467v3467) | ~CIsThingAlive expanded / ~CScriptThing called, reversed |

- **Headers landed:** CEngineResourceManager GETGRAPHICBANK, CGraphicFrame CTOR, fable_string GETASCHARARRAY_SELECT / REPLACE, and fable_map
  TREE_FIND_TEMPLATE / SET_FIND (three-way merged with chunk 849's SET_EMPTY; 5 landed fable_map includers rechecked RELOCATION_MATCH).
  The CDataInputStream / CStringParser / CThingManager / CEntityRuntimePersistence / CPersistContextReadSection opt-ins serve only the
  parked rows (`pending/2026-11-07_linux_hand848/hdr/`).
- **Names:** labels `labels_landing_2026-11-07z.tsv` (LoadFromGameStateBinary 0x00524EC0, LoadFromGameStateScript 0x005248B0,
  CGraphicDataBank::SetUnloadFrameDelay 0x009FD0F0, ...); queue `land0711z` (8, incl. 0x009BBBF0 bound as ~CPersistContext but the body is
  CheckUnknownSymbols). NEngineInternal::GPEngine / GPResourceManager rest on Codex evidence only: review.
