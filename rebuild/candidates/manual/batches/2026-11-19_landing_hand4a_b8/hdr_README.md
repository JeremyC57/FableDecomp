# hand4a header overlay (one line per opt-in)
Merged into the repo by the coordinator, and dropped from this overlay:
- b1: stlport/fable_containers.h
- b2: engine/CTCInGameMenu.h
- b3: engine/C3DVector.h and the three NCameraModes_CScriptModule*.h headers
- b4: engine/NCameraModes_CBase.h, engine/CGameCameraManager.h and engine/NCameraModes_CRotateAround.h
- b5: engine/CHeroCombatDef.h, engine/CTCCombat.h and engine/CTCHeroExperience.h
- b6: engine/C2DBoxI.h (FABLE_RETAIL_C2DBOXI_SETBR and FABLE_RETAIL_C2DBOXI_COPY_FORCEINLINE; landed 7a5e8341da)
The history of each is in NOTES.md and batches/b1..b6/hdr.

Current (batch 8, row 0x006ECEA0 CCombatManager::SetupDefenderPlacementWheel; copies of today's repo headers, default path unchanged):
- engine/CCombatManager.h FABLE_RETAIL_CCOMBATMANAGER_SETUPPLACEMENTWHEEL: private inline GetWorld() (FableWin ?GetWorld@CCombatManager@@ABEAAVCWorld@@XZ
  0x024128A0), private non-const inline GetCombatantInfo (Ego_r + FableWin ?GetCombatantInfo@CCombatManager@@AAEAAUCCombatantInfo@@ABVCThingCreatureBase@@@Z
  0x02415CC0, expanded in retail) and ?SetupDefenderPlacementWheel@CCombatManager@@AAEXAAUCCombatantInfo@@_N@Z (0x006ECEA0).
- engine/CDefendingCombatantInfo.h FABLE_RETAIL_CCOMBATWHEEL_UPDATE: ?Update@CCombatWheel@CDefendingCombatantInfo@@QAEXXZ declaration (0x006E8560).
