## 0x006A5610

FABLE_RETAIL_CTHINGPHYSICAL_CYLINDERCOLLISION (engine/CThingPhysical.h): declaration of GetFirstCylinderCollisionBetweenPoints (row 0x006A5610)

## 0x007D71E0

- engine/CTavernGameCardBaseDef.h: FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_CARDORDERS -- CardOrders typed as std::vector<CCardPositionsDef> (row 0x007D71E0).

## 0x00B4AC10

engine/CEnginePrimitiveRenderer2D.h: FABLE_RETAIL_CENGINEPRIMITIVERENDERER2D_MEMBERS -- named Ego_r members at retail offsets (+0x0c..+0x320) + private AddPrimitiveManager decl (0x00B4ABB0), nested in the retail branch.
engine/CEngineDef.h: FABLE_RETAIL_CENGINEDEF_FOV_2D -- public float FOV_2D at retail +0x5c (Ego_r +0x60), rest stays opaque.
engine/CEnginePrimitiveManager2DSprites.h: FABLE_RETAIL_CENGINEPRIMITIVEMANAGER2DSPRITES_MANAGER -- class over CEnginePrimitiveManager with ctor(CArray<CEngineScene*>&) 0x00BAD040, own members opaque (sizeof 0x38).
engine/CEnginePrimitiveManager2DParticleGroup.h: FABLE_RETAIL_CENGINEPRIMITIVEMANAGER2DPARTICLEGROUP_MANAGER -- base CEnginePrimitiveManagerParticleGroup (ctor 0x00BBBF40, no header exists) + derived with inline ctor and virtual dtor (vtable 0x012A22D0), sizeof 0x28.
- 0x00B4AC10 repair: also defines repo FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS (no overlay change) to get the class path of engine/CEnginePrimitiveManagerMeshBase.h / CEnginePrimitiveManagerStaticMeshesBase.h.

## 0x00973DC0

- engine/CAIStateGroup_JackOfBladesBase.h: FABLE_RETAIL_JACKOFBLADESBASE_SUMMONPILLARS declares protected `void SummonPillars();` (row 0x00973DC0).

## 0x00875B10

engine/C3DVector.h: FABLE_RETAIL_C3DVECTOR_SCALE_NAMED_RESULT -- operator*(float) builds the result in a named local (x87 scheduling tuple; row 0x00875B10). Copied from current repo header + this opt-in only; the one changed existing line: the repo's `#ifdef FABLE_RETAIL_C3DVECTOR_SCALE_X_PAREN` (repo line 152) became `#elif defined(FABLE_RETAIL_C3DVECTOR_SCALE_X_PAREN)` after the new `#if defined(FABLE_RETAIL_C3DVECTOR_SCALE_NAMED_RESULT)` branch, so NAMED_RESULT takes precedence when both are defined (default build unchanged).
engine/NCameraModes_CTools.h: FABLE_RETAIL_NCAMERAMODES_CTOOLS_MESHTYPE -- namespace enum NCameraModes::EMeshType + CTools::TestMeshForOccupationAndGetType declaration (row 0x00875B10). Copied from current repo header + this opt-in only.

## 0x005EC270

- engine/CInputProcessTavernGame.h: FABLE_RETAIL_INPUTPROCESS_TAVERNGAME_FRIEND -- `friend class CTCTavernGame;` so ProcessInput 0x005EC270 can call the protected IsEventButtonPressed (access only).
- engine/CTCTavernGame.h: FABLE_RETAIL_CTCTAVERNGAME_GETGAMESTATE -- inline GetGameState() (FableWin.pdb name; body = return TavernGameState).

## 0x004093E0 (not landed)

fable_user_profile.h: FABLE_RETAIL_CUSERPROFILEMANAGER_VIDEODEFAULTS (row 0x004093E0) -- names EffectsDetail +0x70 / ScreenAspectRatio +0x74 / NewProfileToUseWASD +0x78 / CameraResetting +0x79 / CameraSensitivity +0x7C / DisplayModes (std::vector<CDisplayModeDesc>) +0x80 in the +0x52..+0xCC pad (Ego_r.pdb, retail -8 after the two input vectors); declares SetDefaultValuesVideo, GetDisplayModes (0x00406880), protected ApplyValues, private ClampVideoOption(float&,...)/ClampAntialiasing/ClampResolution/ClampRefreshRate (unsigned long&, long) per the PDB manglings
engine/CDisplayManager.h: FABLE_RETAIL_CDISPLAYMANAGER_VIDEODEFAULTS (row 0x004093E0) -- inline FableWin ?GetBitDepth@CDisplayManager@@QBEJXZ (BackBufferFormat.GetColourDepth()) and ?GetRefreshRate@CDisplayManager@@QBEJXZ (RefreshRate); needs FABLE_RETAIL_CPIXELFORMAT + _COLOURDEPTH

