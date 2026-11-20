# NF129 header overlay (hdr/engine/)

Base copies: orig/engine/<header> = the repo working tree copy at the start of the session (still byte-identical to the
repo at hand-back). Every change is a new `#ifdef` opt-in; the default path is token-identical (cpp -P of the header with the
overlay vs without it, both with no opt-in and with the header's main FABLE_RETAIL_<CLASS> opt-in: identical md5 for all three).
`tools/decomp_pipeline/pin_asserts.py` pinned 0 asserts in each (they use FABLE_STATIC_ASSERT_AT with explicit lines).
No landed row defines any of the new opt-in macros (grep over rebuild/), so none of the landed includers listed below changes.

## engine/CStringParser.h
- New opt-in `FABLE_RETAIL_CSTRINGPARSER_CALCINQUOTES_DECL` (only when FABLE_RETAIL_CSTRINGPARSER_MOVESTRINGPOS is not also
  defined, which already declares it): declares `bool CalcInQuotes(bool,const char*,const char*) const;` (0x009B9570, Ego_r.pdb +
  FableWin.pdb ?CalcInQuotes@CStringParser@@IBE_N_NPBD1@Z) in the protected section after `Error`. Used by row 0x009B9BA0.

## engine/CSystemManager.h
- New partial mode `FABLE_RETAIL_CSYSTEMMANAGER_ESYSTEMRETURN_ONLY` (ignored when FABLE_RETAIL_CSYSTEMMANAGER is defined): the
  header then declares only `enum ESystemReturn { };` (own include guard) and nothing else, so a row's existing local
  CSystemManager model can spell Update as `ESystemReturn Update();` (?Update@CSystemManager@@QAE?AW4ESystemReturn@@XZ, Ego_r +
  FableWin) without a new local enum. Used by rows 0x004189C2, 0x00496CF0, 0x00499880.

## engine/CParsedItem.h
- New opt-in `FABLE_RETAIL_CPARSEDITEM_GETVALUE_OUTOFLINE`: GetFloat / GetInteger become declarations (out-of-line retail
  copies 0x009B98A0 ?GetFloat@CParsedItem@@QBEMXZ / 0x009B9890 ?GetInteger@CParsedItem@@QBEJXZ, Ego_r + FableWin); the inline
  bodies stay in the `#else` branch (the default). Used by row 0x00A641A0.

## Landed includers (grep -rlE '<header>' rebuild/src/compiled) -- none defines the new opt-ins

### engine/CStringParser.h (78 rows)
- rebuild/src/compiled/00/40/CPersistTraitsCCharString_TransferIn_0040e510.cpp
- rebuild/src/compiled/00/40/CPersistTraitsfloat_TransferIn_0040f7e0.cpp
- rebuild/src/compiled/00/40/CStringParser_CStringParser_004047a0.cpp
- rebuild/src/compiled/00/40/CStringParser_Init_00403c20.cpp
- rebuild/src/compiled/00/40/CUserProfileManager_LoadGameState_0040bca0.cpp
- rebuild/src/compiled/00/41/CStringParser_CStringParser_00414e30.cpp
- rebuild/src/compiled/00/46/CStringParser_CStringParser_0046381b.cpp
- rebuild/src/compiled/00/47/CTCInGameMenu_AddRegionAndTimeInfo_0047f58b.cpp
- rebuild/src/compiled/00/48/CTCInGameMenu_GetInterfaceType_0048616c.cpp
- rebuild/src/compiled/00/49/CFrontendGameComponent_GetProfileNames_004959e0.cpp
- rebuild/src/compiled/00/49/CProgressDisplay_InitialiseTextures_0049a0b0.cpp
- rebuild/src/compiled/00/4a/CWorld_LoadGameStateInternal_004a21f0.cpp
- rebuild/src/compiled/00/4a/CWorld_LoadQuestsFromFile_004a0d90.cpp
- rebuild/src/compiled/00/4b/CQuestManager_LoadGameState_004b5500.cpp
- rebuild/src/compiled/00/4b/NQuestManagerCRuntimeQuestPersistence_LoadFromGameStateScript_004b5080.cpp
- rebuild/src/compiled/00/4c/CSubtitledMessage_AddMasterShotCameraTagFromText_004ce1b0.cpp
- rebuild/src/compiled/00/4c/CThing_ConstructFromLevelScript_004caa00.cpp
- rebuild/src/compiled/00/4c/CThing_Save_004c9120.cpp
- rebuild/src/compiled/00/4f/CWorldMap_LoadAllLoadableGlobalThingsFromSingleFile_004fe2a0.cpp
- rebuild/src/compiled/00/50/CWorldMap_LoadFromFile_00507c30.cpp
- rebuild/src/compiled/00/51/CThingManager_LoadSingleThing_0051fd80.cpp
- rebuild/src/compiled/00/51/NThingManagerCEntityRuntimePersistence_ClearPersistenceDataForSectionInEntityScript_0051f480.cpp
- rebuild/src/compiled/00/52/CThingManager_LoadEntitiesFromScript_00520d00.cpp
- rebuild/src/compiled/00/52/CThingManager_LoadGameState_005253f0.cpp
- rebuild/src/compiled/00/52/NThingManagerCEntityRuntimePersistence_LoadFromGameStateScript_005248b0.cpp
- rebuild/src/compiled/00/59/NUISystemCFrontEndManager_AddRegionAndTimeInfo_00597228.cpp
- rebuild/src/compiled/00/5d/CPersistContext_TransferMaplongunsignedlong_005d8978.cpp
- rebuild/src/compiled/00/5f/CTCInventoryMap_GetPlayerMode_005fa8e0.cpp
- rebuild/src/compiled/00/5f/CTCInventoryMap_GetScreenType_005fa8f0.cpp
- rebuild/src/compiled/00/5f/CTCInventoryMap_IsInventoryScreen_005fa590.cpp
- rebuild/src/compiled/00/61/CPersistTraitsCTCInventoryMapCUIMiniMapMarker_TransferIn_006159a0.cpp
- rebuild/src/compiled/00/61/CPersistTraitsCTCInventoryMapCUIRegionInfo_TransferIn_00615e70.cpp
- rebuild/src/compiled/00/61/CTCInventoryMap_HasFrameUpdate_00619f40.cpp
- rebuild/src/compiled/00/67/CPersistTraitsstdlistCReactionMatch_TransferIn_00678760.cpp
- rebuild/src/compiled/00/68/CTCCreatureOpinionOfHero_OnSerialise_00684610.cpp
- rebuild/src/compiled/00/6a/CThingPhysical_ConstructFromLevelScript_006a5d90.cpp
- rebuild/src/compiled/00/6c/CPersistTraitsCOpinionDeed_TransferIn_006c9490.cpp
- rebuild/src/compiled/00/6d/CPersistTraitsCSimVoice_TransferIn_006d41f0.cpp
- rebuild/src/compiled/00/75/CPersistContext_TransferMappairlonglongCCharString_00756d00.cpp
- rebuild/src/compiled/00/7e/CPersistTraitsCDefString_TransferIn_007e3820.cpp
- rebuild/src/compiled/00/81/CTCBoastingPodium_OnInitialActivate_00814260.cpp
- rebuild/src/compiled/00/81/NHitLocationsCPhysicalPrimitiveSphere_vfunc4_00816000.cpp
- rebuild/src/compiled/00/83/CThingAICreature_ConstructFromLevelScript_008315c0.cpp
- rebuild/src/compiled/00/83/CThingPhysicalSwitch_ConstructFromLevelScript_0083d990.cpp
- rebuild/src/compiled/00/83/CThingTrackNode_ConstructFromLevelScript_00837e40.cpp
- rebuild/src/compiled/00/99/CAFile_GetRelativeFilePath_00998570.cpp
- rebuild/src/compiled/00/99/CAFile_ParserIsDriveLabelNext_00998210.cpp
- rebuild/src/compiled/00/9a/CDefinitionManager_ParseEnumForSymbols_009acee0.cpp
- rebuild/src/compiled/00/9b/CPersistContext_CheckUnknownSymbols_009bbbf0.cpp
- rebuild/src/compiled/00/9b/CPersistContext_TransferVectorLoadText_009bb110.cpp
- rebuild/src/compiled/00/9b/CPersistContext_TransferVectorOfSubComponentsLoadText_009bc120.cpp
- rebuild/src/compiled/00/9b/CStringParser_NextItemExists_009ba4f0.cpp
- rebuild/src/compiled/00/9b/CStringParser_PeekNextItem_009ba8c0.cpp
- rebuild/src/compiled/00/9b/CStringParser_ReadAsFloat_009ba650.cpp
- rebuild/src/compiled/00/9b/CStringParser_ReadAsStringUntilPastString_009ba3e0.cpp
- rebuild/src/compiled/00/9b/CStringParser_ReadAsStringUntilString_009ba330.cpp
- rebuild/src/compiled/00/9b/CStringParser_ReadAsStringUntilWhitespace_009ba4a0.cpp
- rebuild/src/compiled/00/9b/CStringParser_ReadAsStringUntilWholeString_009ba440.cpp
- rebuild/src/compiled/00/9b/CStringParser_ReadAsUHUGE_009ba5b0.cpp
- rebuild/src/compiled/00/9b/CStringParser_ReadLineAsString_009ba390.cpp
- rebuild/src/compiled/00/9b/CStringParser_ReadNextItemAsQuotedString_009ba1d0.cpp
- rebuild/src/compiled/00/9b/CStringParser_ReadNextSymbol_009b99c0.cpp
- rebuild/src/compiled/00/9b/CStringParser_SetStringPos_009b9940.cpp
- rebuild/src/compiled/00/9b/CStringParser_SkipPastNewline_009b9d70.cpp
- rebuild/src/compiled/00/9b/CStringParser_SkipPastNextItemIfIs_009b9e00.cpp
- rebuild/src/compiled/00/9b/CStringParser_SkipPastWholeString_009b9c00.cpp
- rebuild/src/compiled/00/9b/CStringParser_SkipUntilWhitespace_009b9ce0.cpp
- rebuild/src/compiled/00/9b/CStringParser_SkipUntilWholeString_009b9aa0.cpp
- rebuild/src/compiled/00/9b/CStringParser_SkipWhitespace_009b9c60.cpp
- rebuild/src/compiled/00/9e/CPersistTraitsC2DVector_TransferIn_009e81e0.cpp
- rebuild/src/compiled/00/9e/CPersistTraitsCRGBColour_TransferIn_009e8c10.cpp
- rebuild/src/compiled/00/9e/CPersistTraitsSLONG_TransferIn_009e2590.cpp
- rebuild/src/compiled/00/a6/CGenericVarRef_ReadFromParserAsBool_00a63db0.cpp
- rebuild/src/compiled/00/a6/CGenericVarRef_ReadFromParser_00a63fc0.cpp
- rebuild/src/compiled/00/a6/CGenericVar_ReadFromParserAsBool_00a64230.cpp
- rebuild/src/compiled/00/ae/NParticleEngineCParticleEmitterDatabase_Load_00ae8d50.cpp
- rebuild/src/compiled/00/ae/NParticleEngineCParticleEmitter_DatabaseSerialise_00aef1f0.cpp
- rebuild/src/compiled/00/af/NParticleEngineCParticleSystem_DatabaseSerialise_00af4900.cpp

### engine/CSystemManager.h (45 rows)
- rebuild/src/compiled/00/40/CUserProfileManager_LoadGameState_0040bca0.cpp
- rebuild/src/compiled/00/40/CUserProfileManager_SetDefaultValuesVideo_004093e0.cpp
- rebuild/src/compiled/00/40/CUserProfileManager_SetDefaultValues_0040c790.cpp
- rebuild/src/compiled/00/42/CNewFrontendGameComponent_Run_0042ec7c.cpp
- rebuild/src/compiled/00/43/CDisplayEngine_DoRender_00435530.cpp
- rebuild/src/compiled/00/49/CFrontendGameComponent_Output_00494900.cpp
- rebuild/src/compiled/00/49/CProgressDisplay_DisplayCenteredText_00497fd0.cpp
- rebuild/src/compiled/00/49/CProgressDisplay_DrawRetailDisplay_00498490.cpp
- rebuild/src/compiled/00/49/CProgressDisplay_GetPTextBank_00497b30.cpp
- rebuild/src/compiled/00/49/CProgressDisplay_InitialiseTextures_0049a0b0.cpp
- rebuild/src/compiled/00/49/CProgressDisplay_StartProgress_00499aa0.cpp
- rebuild/src/compiled/00/4a/CWorld_Init_004a6e30.cpp
- rebuild/src/compiled/00/4a/CWorld_Load_004a1840.cpp
- rebuild/src/compiled/00/54/NUISystemCIconText_GetDataFromDef_00541fd0.cpp
- rebuild/src/compiled/00/55/NUISystemCText_vectordeletingdestructor_00550010.cpp
- rebuild/src/compiled/00/55/NUISystemCText_vectordeletingdestructor_00550020.cpp
- rebuild/src/compiled/00/58/CTCInventory_OpenPCOptionsMenu_0058af10.cpp
- rebuild/src/compiled/00/59/CFrontEndManager_CanStartAttractMode_00595a03.cpp
- rebuild/src/compiled/00/59/NUISystemCFrontEndManager_BuildComponents_00598a1c.cpp
- rebuild/src/compiled/00/62/global_GFPlayFullScreenXBoxMovie_006286f0.cpp
- rebuild/src/compiled/00/6b/CMusicManager_Update_006bdc60.cpp
- rebuild/src/compiled/00/9a/CStreamingFontBank_PrepareToRender_009a1720.cpp
- rebuild/src/compiled/00/9a/CStreamingFontBank_Render_009a25d0.cpp
- rebuild/src/compiled/00/9a/globalanonnamespace_InitializeStreamingFontTextureCache_009a3640.cpp
- rebuild/src/compiled/00/9b/global_DisplayCenteredText_009bca20.cpp
- rebuild/src/compiled/00/9d/CStateBlockFunctionAlphaAdditive_Apply_009df730.cpp
- rebuild/src/compiled/00/9d/CStateBlockFunctionAlphaMultiply_Apply_009dfda0.cpp
- rebuild/src/compiled/00/9e/CStateBlockFunctionAlphaBoolean_Apply_009e0410.cpp
- rebuild/src/compiled/00/9e/CStateBlockFunctionAlphaText_Apply_009e0ae0.cpp
- rebuild/src/compiled/00/9e/global_GFABoxToVBox_009e2020.cpp
- rebuild/src/compiled/00/9f/CGraphicBankManager_ProcessLoading_009f8230.cpp
- rebuild/src/compiled/00/9f/CTexture_BltToRenderTargetWithGPU_009fa780.cpp
- rebuild/src/compiled/00/9f/CTexture_GenerateMipmapsWithGPU_009faf70.cpp
- rebuild/src/compiled/00/9f/CTexture_GetNextValidDimensions_009fa620.cpp
- rebuild/src/compiled/00/9f/CTexture_LoadFromDataStreamToPreallocatedSurface_009fb750.cpp
- rebuild/src/compiled/00/a6/CVolumeTexture_Lock_00a685e0.cpp
- rebuild/src/compiled/00/ab/CMouseDX_ResetNonExclusiveMouse_00ab5030.cpp
- rebuild/src/compiled/00/ab/CStaticFontBank_CStaticFontBank_00ab8e10.cpp
- rebuild/src/compiled/00/ab/CStaticFontBank_PrepareToRender_00ab83a0.cpp
- rebuild/src/compiled/00/b2/CEngine_Initialise_00b26360.cpp
- rebuild/src/compiled/00/b8/CEngineOcclusionManager_FrameEnd_00b8df50.cpp
- rebuild/src/compiled/00/f3/CSurfaceCompositor_CompositeQuad_00f39a50.cpp
- rebuild/src/compiled/00/f3/CSurfaceCompositor_SetBaseBlendModes_00f38ff0.cpp
- rebuild/src/compiled/00/f3/CSurfaceCompositor_SetBlendMode_00f38a10.cpp
- rebuild/src/compiled/00/f3/CSurfaceCompositor_StartComposition_00f39730.cpp

### engine/CParsedItem.h (6 rows)
- rebuild/src/compiled/00/61/CPersistTraitsCTCInventoryMapCUIRegionInfo_TransferIn_00615e70.cpp
- rebuild/src/compiled/00/99/CAFile_GetRelativeFilePath_00998570.cpp
- rebuild/src/compiled/00/9b/CParsedItem_InitAsFloat_009b9770.cpp
- rebuild/src/compiled/00/9b/CParsedItem_InitAsInteger_009b9700.cpp
- rebuild/src/compiled/00/9b/CPersistContext_CheckUnknownSymbols_009bbbf0.cpp
- rebuild/src/compiled/00/a0/CWParsedItem_InitAsFloat_00a0ca90.cpp
