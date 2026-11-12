## 0x009FF220

# hdr overlay (row 0x009FF220) -- all under the new macro FABLE_RETAIL_CFRAMEDATA_UPLOADFRAME (default path unchanged)
- engine/CGraphicFrame.h: CGraphicFrameData public inline ~CGraphicFrameData (IsInitialised -> Uninitialise 0x009FCBA0) + Uninitialise decl + friend CGraphicFrame; CGraphicFrame ctor, FABLE_FORCEINLINE ~CGraphicFrame (`--rc; if(rc==0)`) and operator= (`if(--rc==0)`), destructor + ::operator delete spelled out; adds #include "fable_compiler.h".
- engine/CManagedTexture.h: decls ??4CManagedTexture (0x00A69B20) and bool Release() (0x00A69AD0).
- engine/CGraphicDataBank.h: forward class CManagedTexture; private decls LoadVolumeGraphic (0x009FD9B0) and LoadGraphicFrame (0x009FED30).
- engine/CGraphicInfo.h: inline IsBump() (==2) and IsVolumeTexture() (==4) on Bank->GetEntryDataType(Index).
- engine/CGraphicInfoFrame.h: CGraphicInfoFrame private decls IsVolumeTexture / UploadToVolumeTexture / UploadToTexture; CFrameData::UploadFrame(bool) decl.

## 0x0082B7E0

engine/CMiniMapDisplay.h: FABLE_RETAIL_CMINIMAPDISPLAY_DRAWMARKERS -- typed handle vectors, MarkerFlashFade..UpdateRequired (+0x24..+0xBC), protected AddAutoMarker/DrawMarker/DrawMarkers decls, CMarkerInfo::IsHeroMarker decl (row 0x0082B7E0).
engine/CFadeInFadeOutBase.h: FABLE_RETAIL_CFADEINFADEOUTBASE_OPEN -- ToColour typed CRGBColour; inline Open (body of 0x00434840), Update decl 0x00434870, inline IsOpen/Close (row 0x0082B7E0).
engine/CPlayerGuiDef.h: FABLE_RETAIL_CPLAYERGUIDEF_MINIMAPMARKERGRAPHICS -- MiniMapMarkerGraphics +0x958 typed std::vector<long> (row 0x0082B7E0).

## 0x0084D910

engine/CGameCameraManager.h: FABLE_RETAIL_CGAMECAMERAMANAGER_GETOBSERVEDTHING -- inline GetObservedThing() const (FableWin inline; LOOKAROUND branch) for row 0x0084D910. Overlay = current repo header + only this 7-line block (DEBUGPOINTS blocks of row 0x006992A0 kept).
engine/CThingCreatureBase.h: FABLE_RETAIL_CTHINGCREATUREBASE_MESHSIZE -- declares GetNominalMeshRadius 0x006625C0 / GetNominalMeshHeight 0x006625E0 / GetCurrentMeshRadius 0x00664BA0 (Ego_r PDB names) for row 0x0084D910.

## 0x007AF490

- engine/CTCBoss.h: FABLE_RETAIL_CTCBOSS_KILLHELPERS -- declares ?KillHelpers@CTCBoss@@QAEXXZ (0x007AF490).
- engine/CCombatAction_GenericStrikeResponseKnockdown.h: FABLE_RETAIL_CCOMBATACTIONGENERICSTRIKERESPONSEKNOCKDOWN_IMPLICIT_DTOR -- AnimName / PThingThatHit with real types, implicit inline dtor.

## 0x007F9960

engine/CThingFilter_IsNotList.h: FABLE_RETAIL_THINGFILTER_ISNOTLIST_INLINE_CTOR -- inline body PNotList(&list),PNotList2(0) for the const-CThing* ctor (retail 0x007F9960 inlines it; landed 0x007F6D70/0x00788617 spell the same).
engine/CTCTargetingSpirit.h: NEW header (no repo header existed) -- CTCTargetingSpirit : CTCTargetingBase, Ego_r members MaxTargetingDistance +0x38, Targets +0x3C, TargetedPos +0x48, retail sizeof 0x54 (ctor 0x004DEBD4 / Alloc 0x004E1006); for rows 0x007F9960, 0x007F9B90, 0x007F98E0.

## 0x0096CB40

FABLE_RETAIL_CTCTAVERNTABLE_GETSEAT (engine/CTCTavernTable.h): declares CTCTavernTable::GetSeat 0x007B8840 (Ego_r/FableWin ?GetSeat@CTCTavernTable@@QAE_NAAVCThing@@AAVC3DVector@@AAVCRightHandedSet@@AAJ3PBVCCharString@@@Z); forward-declares CCharString (guarded by the same opt-in).

## 0x007BDE31

engine/CFireballSpellLevelDef.h: FABLE_RETAIL_CFIREBALLSPELLLEVELDEF -- polymorphic def deriving CSubDefClassBase, SpellLevel +0x28 (row 0x007BDE31).

## 0x007D2CC0

- engine/CTavernGameCoinGolfDef.h: FABLE_RETAIL_CTAVERNGAMECOINGOLFDEF_SUBDEF -- polymorphic class : CSubDefClassBase (as row 0x007D4BD0 declares it) with Ego_r members from +0x28 (Courses +0x50).
- fable_string.h: FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE -- CBasicString<T>::operator==(const T*) body visible but __declspec(noinline) (retail COMDAT 0x004115A0 called out of line).

## 0x004085F0 (not landed)

engine/CUserProfileManager.h: FABLE_RETAIL_CUSERPROFILEMANAGER_RESETASSIGNEDINPUTS (nested in PASSIVEINPUTS) adds AssignedInputsAggressive +0x60 and ResetAssignedInputs().


## 0x004085F0 (landing ex, after repair 3)

engine/CUserProfileManager.h: FABLE_RETAIL_CUSERPROFILEMANAGER_RESETASSIGNEDINPUTS (nested in PASSIVEINPUTS) adds AssignedInputsAggressive +0x60 and ResetAssignedInputs().
engine/CStartupDefinitionManager.h: FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER_ISALLOCATED (nested in CSTARTUPDEFINITIONMANAGER) adds static __fastcall bool IsAllocated() (FableWin ?IsAllocated@CStartupDefinitionManager@@SI_NXZ 0x018377B4/0x01888A60), inline `!g_StartupDefinitionManagerInstance.IsNull()` over the counted pointer 0x013B8760 (descriptive global name). (Repair 3; the row also defines the existing FABLE_RETAIL_CGAMEDEFINITIONMANAGER to hide the non-retail CGameDefinitionManager::GetDef -- no overlay copy needed.)

