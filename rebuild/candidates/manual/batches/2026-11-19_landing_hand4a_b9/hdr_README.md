# hand4a header overlay (one line per opt-in)
Merged into the repo by the coordinator, and dropped from this overlay:
- b1: stlport/fable_containers.h
- b2: engine/CTCInGameMenu.h
- b3: engine/C3DVector.h and the three NCameraModes_CScriptModule*.h headers
- b4: engine/NCameraModes_CBase.h, engine/CGameCameraManager.h and engine/NCameraModes_CRotateAround.h
- b5: engine/CHeroCombatDef.h, engine/CTCCombat.h and engine/CTCHeroExperience.h
- b8: engine/CCombatManager.h (SETUPPLACEMENTWHEEL) and engine/CDefendingCombatantInfo.h (CCOMBATWHEEL_UPDATE); landed 44bcf8321a
- b6: engine/C2DBoxI.h (FABLE_RETAIL_C2DBOXI_SETBR and FABLE_RETAIL_C2DBOXI_COPY_FORCEINLINE; landed 7a5e8341da)
The history of each is in NOTES.md and batches/b1..b8/hdr.

Current (batch 9, row 0x00649E8B NPlayerGui::CDrawFishingGame::Draw; a copy of today's repo header, default path unchanged):
- engine/CTCFishing.h FABLE_RETAIL_CTCFISHING_DRAW_GETTERS (nested in FABLE_RETAIL_CTCFISHING_PASS22): FableWin.pdb inline const
  GetStage / IsStageInitialised / GetDistance / GetStrain, and the Ego_r + FableWin ?GetPFishingRodDef@CTCFishing@@QBEPBVCFishingRodDef@@XZ
  declaration (0x007B0880). No layout change; no landed users.
