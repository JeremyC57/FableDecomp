# header overlay opt-ins (one line each: header, opt-in, what, evidence)
# hand2c entries below up to the hand3c marker are LANDED (commits 039a0c8c02, e7b244de39); the overlay files themselves were removed.

- engine/CTCBuyableHouse.h  FABLE_RETAIL_CTCBUYABLEHOUSE_GETRENTMONEYPOS  declares private `bool GetRentMoneyPos(C3DVector&, long, long)` (0x006C06D0) inside the GETSIGNPOSTPOS private block. Evidence: Ego_r.pdb 0x006F8940 and FableWin.pdb 0x024B2D50 `?GetRentMoneyPos@CTCBuyableHouse@@AAE_NAAVC3DVector@@JJ@Z`; retail caller 0x006C0C90. Declaration only, no layout change; no static assert moved.
- engine/CPerlinNoise.h  FABLE_RETAIL_CPERLINNOISE_GETNOISE  declares public `float Get2DNoise(const C2DVector&) const` (0x00F36010) and `float Get3DNoise(const C3DVector&) const` (0x00F361D0). Evidence: Ego_r.pdb ?Get2DNoise@CPerlinNoise@NFractal@@QBEMABVC2DVector@@@Z 0x00BC77B0 / ?Get3DNoise@...@QBEMABVC3DVector@@@Z 0x00BC79E0, donor order Get2DNoise, Get3DNoise, GetGaussian2DNoise = retail 0x00F36010, 0x00F361D0, 0x00F36540; Ego_d.pdb records both. Declarations only. NOTE: the FABLE_STATIC_ASSERT(sizeof==4096) line moves from line 13 to line 19 (the class body is one block; no way to add members without moving it).
- engine/CTCWobble.h  FABLE_RETAIL_CTCWOBBLE_ONINITIALACTIVATE  (inside FABLE_RETAIL_CTCWOBBLE_PASS22) declares `virtual void OnInitialActivate();` (0x007E2DF0), the CTCBase slot-6 override. Evidence: Ego_r.pdb 0x007DEA30 / FableWin.pdb 0x027954E0 ?OnInitialActivate@CTCWobble@@UAEXXZ; CTCWobble vtable 0x0123DF34. Declaration only.
- engine/CEngineScene.h  FABLE_RETAIL_CENGINESCENE_GETMAXAVERAGEPRIMITIVESPERCELL  public inline `float GetMaxAveragePrimitivesPerCell() const`. Evidence: FableWin.pdb 0x02E1C840 ?GetMaxAveragePrimitivesPerCell@CEngineScene@@QBEMXZ; Ego_d.pdb CEngineScene field list 0x647F4 (public, const, float); retail 0x00B9480A `fdiv [ecx+54h]` (+0x54 = MaxAveragePrimitivesPerCell in the existing layout). No layout change.
- engine/CEngineSceneMap.h  NEW HEADER (no opt-in; no repo header existed)  CEngineSceneMap (vfptr, Area +4, MaxLevels +0x14, GridLevels CArray<CCountedPointer<CEngineSceneGrid> > +0x18, Scene +0x24; sizeof 0x28) and a method-only CEngineSceneGrid (GetCellTotalPrimitiveCount, private, friend CEngineSceneMap). Evidence: Ego_d.pdb class record 0x648BB / field list 0x648BA (members, access, methods), Ego_r.pdb names ?Update@CEngineSceneMap@@QAEXXZ, ?ResizeLevels@CEngineSceneMap@@AAEXJ@Z, ?GetCellTotalPrimitiveCount@CEngineSceneGrid@@AAEKKK@Z, FableWin ?GetCurrentLevels@CEngineSceneMap@@QBEKXZ; retail offsets from 0x00B947E0 / 0x00B91920. The landed CEngineSceneMap rows (0x00B91920, 0x00B949A0, 0x00B94A40) use local models and are not affected.
- engine/CPackedUIntArray.h  FABLE_RETAIL_CPACKEDUINTARRAY_CALCULATEBIAS  private `CalculateBias(const void*, unsigned long, long, unsigned long) const` (0x00A62AE0), private static inline `ReadNBytes(unsigned long&, const unsigned char*, unsigned long)` (n == 4 word load, else memcpy for n > 0) and private static `unsigned long BitMasks[]` (0x013979E8); also `#include <string.h>` under the same opt-in (top of the header). Evidence: Ego_r.pdb ?CalculateBias@CPackedUIntArray@@ABEKPBXKJK@Z, ?ReadNBytes@CPackedUIntArray@@CIXAAKPBEK@Z (written without its __fastcall: inline at its only site, keeps g++ builds), ?BitMasks@CPackedUIntArray@@0PAKA; Ego_d.pdb field list (private / private static); retail 0x00A62B15 inline `cmp edx,4 ; mov ecx,[ebx]` else `rep movsd ; rep movsb`; FableWin 0x033AC5D0 ReadNBytes (later revision: byte loop). No layout change.
- engine/CPerlinNoise.h  (opt-in FABLE_RETAIL_CPERLINNOISE_GETNOISE, CHANGED after landing 039a0c8c02): SCurve / Interp bodies are now the
  outer-bracketed `return (t*t*(3.0f-2.0f*t));` / `return (a+(b-a)*t);` (same instructions; one no-code PAREN tuple fewer per call in
  the right places for c2's 81-tuple regions; measured on Get3DNoise 0x00F361D0 with c2 DAG dumps + the sim). Landed users 0x00F36010 /
  0x00F36540 re-graded RELOCATION_MATCH on the new text (g.py) -- the coordinator should --recheck-landed them when applying the header.
- engine/CEngineInternalPrimitive2DViewport.h  FABLE_RETAIL_CENGINEINTERNALPRIMITIVE2DVIEWPORT_CLEARCHILD (inside ..._MEMBERS)  virtual `ClearChildPrimitive(unsigned long)` (0x00BE2120), public inline const `HasChildPrimitive(unsigned long)` (the CEngineInternalPrimitiveGroup body) and `UpdateRenderLayerMask()` (0x00BE1D80); includes engine/CEnginePrimitiveHandle.h under the opt-in. Evidence: Ego_r.pdb / FableWin.pdb ?ClearChildPrimitive@CEngineInternalPrimitive2DViewport@@UAEXK@Z, FableWin ?HasChildPrimitive@...@@QBE_NK@Z, ?UpdateRenderLayerMask@...@@QAEXXZ; landed twin 0x00BC62D0. No layout change.
- engine/CCachedFile.h  FABLE_RETAIL_CCACHEDFILE_COPY  private `void Copy(const CCachedFile& rhs)` (0x00A5B650). Evidence: Ego_r.pdb / FableWin.pdb ?Copy@CCachedFile@@AAEXABV1@@Z; Ego_d.pdb parameter `rhs`; retail caller Clone 0x00A5B710. Declaration only, no layout change.
- engine/CThingPhysicalSwitch.h  FABLE_RETAIL_CTHINGPHYSICALSWITCH_GETCURRENTGRAPHIC  protected `const CEngineGraphic& GetCurrentGraphic() const` (0x0083DB60). Evidence: Ego_r.pdb / FableWin.pdb ?GetCurrentGraphic@CThingPhysicalSwitch@@IBEABVCEngineGraphic@@XZ. Declaration only, no layout change.

## hand3c (2026-11-19) -- batch 1 LANDED 58b05aba99 (overlay copies removed)
- engine/CPSCBase.h  FABLE_RETAIL_CPSCBASE_CTOR  declares public `CPSCBase();` (out of line 0x00AF2620). Evidence: Ego_r.pdb 0x00AB5910 / FableWin.pdb 0x02F101B0 ??0CPSCBase@NParticleEngine@@QAE@XZ; reloc_bindings 0x00AF2620 called from every CPSC* ctor (0x00B012E0, 0x00B103F0 ...). Declaration only, no layout change; the two FABLE_STATIC_ASSERT_AT lines move down 5 lines (their pinned names are unchanged).
- engine/CPSCBase.h  FABLE_RETAIL_CPARTICLEUPDATEPARAMDESC_CTOR  public inline `CParticleUpdateParamDesc():Callback(0){}`. Evidence: FableWin.pdb 0x02F4E710 ??0?$CParticleUpdateParamDesc@KK@NParticleEngine@@QAE@XZ (/Od: CBaseClass(), vptr, Callback = 0); retail inline at 0x00B10407..0x00B10417 (CPSCEmitterGeneric ctor). No layout change.
- engine/CPSCEmitterGeneric.h  FABLE_RETAIL_CPSCEMITTERGENERIC_CTOR  public `CPSCEmitterGeneric();` (0x00B103F0). Evidence: Ego_r.pdb 0x00AD1150 / FableWin.pdb 0x02F47CE0 ??0CPSCEmitterGeneric@NParticleEngine@@QAE@XZ. Declaration only; the FABLE_STATIC_ASSERT_AT(53) line moves down 5 lines.
- engine/CPSCDecalRenderer.h  FABLE_RETAIL_CPSCDECALRENDERER_CTOR  public `CPSCDecalRenderer();` (0x00B1DFD0). Evidence: Ego_r.pdb 0x00ADE5E0 / FableWin.pdb 0x02F55400 ??0CPSCDecalRenderer@NParticleEngine@@QAE@XZ. Declaration only (the plain FABLE_STATIC_ASSERT line moves down 5 lines).
- engine/CParticleEmitterManager.h  FABLE_RETAIL_CPARTICLEEMITTERMANAGER_DEFAULTALIGNMENT  public inline `const long GetDefaultAlignmentMode()const{return 0;}`. Evidence: FableWin.pdb 0x02F26960 ?GetDefaultAlignmentMode@CParticleEmitterManager@NParticleEngine@@QBE?BJXZ; retail folds it to 0 (0x00B1E34A `and [esi+20h],0FFFF003Fh`; landed CPSCRenderSprite ctor 0x00AFB7C0 spells the same body locally). No layout change.
- engine/CPSCSplineBase.h  FABLE_RETAIL_CPSCSPLINEBASE_GETCOMPONENTCOST  public `virtual void GetComponentCost(CParticleSystemCost&,const CParticleSystem&,const float,const float);` (0x00B202C0). Evidence: Ego_r.pdb / FableWin.pdb 0x02F5EFB0 ?GetComponentCost@CPSCSplineBase@NParticleEngine@@UAEXAAVCParticleSystemCost@2@ABVCParticleSystem@2@MM@Z; retail 0x00B05C85 `call 0xB202C0`. Declaration only.
- engine/CParticleSystem.h  FABLE_RETAIL_CPARTICLESYSTEM_SCALEPARTICLEDIMENSIONS_FLOAT_INLINE (nested inside FABLE_RETAIL_CPARTICLESYSTEM; needs ..._SCALE)  turns the existing `ScaleParticleDimensions(float&) const` declaration into its in-class body `if (ScaleParticles) size *= max(max(X,Y),Z)` spelled with the inner max written twice. Evidence: Ego_r.pdb / FableWin.pdb put the float& / C2DVector& / unsigned long& overloads in the first psc_*.obj that uses each (COMDATs; none in particle_system.obj); FableWin /Od 0x02F26DC0 evaluates the inner max twice (macro); landed 0x00AF8F80 has the same body. VC7.1 declines to inline this body (retail calls it); with the declaration only the row gets one extra frame word. Default path unchanged.

## hand3c batch 3
- engine/CMatrix4x4AlignedSSE.h: a copy of the current repo header with a new block appended at the end, outside the include guard like
  AlignedSetMultiply. FABLE_RETAIL_CMATRIX4X4_ALIGNEDPRESCALE_BODIES holds the shared bodies of CMatrix4x4::AlignedPreScale and
  AlignedPreScaleTransposed: Ego_d locals, FableWin accessor form, and the g_SSEAvailable / GEnableSIMD switch through
  FABLE_RETAIL_CMATRIX4X4_SIMD_ORIGINAL_FLAG. The nested FABLE_RETAIL_CMATRIX4X4_ALIGNEDPRESCALE_FORCEINLINE makes them __forceinline.
  The default path is unchanged. Used by row 0x00A9D750.
  The same text gives RELOCATION_MATCH for the landed 0x00A9D480 / 0x00A9D580 (diagnostic only; their row sources still carry their own bodies).

## hand3c batch 5
- engine/CMatrix4x4AlignedSSE.h: a copy of the repo header at b8a046277f with per-member out-of-line opt-ins added to the ALIGNEDPRESCALE block.
  FABLE_RETAIL_CMATRIX4X4_ALIGNEDPRESCALE_OUTOFLINE defines only AlignedPreScale, and FABLE_RETAIL_CMATRIX4X4_ALIGNEDPRESCALETRANSPOSED_OUTOFLINE
  defines only AlignedPreScaleTransposed. Both are plain (not inline). BODIES / FORCEINLINE and the default path are unchanged.
  Used by the re-pointed 0x00A9D480 / 0x00A9D580.
- engine/CAnimatingObjectDef.h  NEW HEADER (batch 7; only the generated flat model in engine/_quarantine/ existed)  CAnimatingObjectDef :
  CSubDefClassBase, StatGetTypeName ("CAnimatingObjectDef"; FableWin ?StatGetTypeName@CAnimatingObjectDef@@SI?AVCCharString@@XZ 0x02676280),
  _pad_0x25[3], Animation CAnimationSet +0x28. Evidence: RTTI bases (ghidra_out/rtti_bases.tsv), Ego_r struct_layouts_egor.tsv Animation +40,
  retail 0x007F8C20 `add eax,28h` before AnimExists / GetAnim. Modelled on engine/CAppearanceDef.h. Row 0x007F8C20.
- engine/CTCAnimatingObject.h  FABLE_RETAIL_CTCANIMATINGOBJECT_ADDQUEUEDANIMATION (batch 7)  declares
  ?AddQueuedAnimation@CTCAnimatingObject@@QAEXABVCCharString@@_N@Z (public, Ego_r + FableWin). Declaration only; default path unchanged.
- engine/CGameNavigatorManagerExternalInterface.h  FABLE_RETAIL_CNAVIGATORMANAGEREXTERNALINTERFACE_GETNAVIGATIONINFORMATION (batch 7)
  replaces the base class's slot-1 placeholder `Initialise()` with Ego_r's pure `GetNavigationInformation(const C2DBoxF&,
  CNavigationAreaUpdate&) const` (field list 0x32FC4: vftable offset 4) and declares the derived override (retail vtable
  0x01238C58 slot 1 = 0x006B2960). Default path unchanged. NAME_FIXES: the default `Initialise` slot-1 name is wrong.
- engine/CEngineScene.h  FABLE_RETAIL_CENGINESCENE_GETMAPAT (batch 8)  MapList typed as map<long, map<long, CCountedPointer<CEngineSceneMap> > >
  (12 bytes; Ego_r CEngineScene::MapList) and the public GetMapAt declaration. No layout change. Row 0x00B94BF0.
- engine/CEngineSceneMap.h  FABLE_RETAIL_CENGINESCENEMAP_CTOR (batch 8)  CBaseClassNonCopyable base (RTTI), the inline ctor
  (scene, max_levels, area) -> Initialise (0x00B949A0; out-of-line copy 0x00B94A40 landed). Same layout. Row 0x00B94BF0.
- engine/CAIStateGroup_BirdStartled.h  NEW HEADER (batch 8)  : CAIStateGroup_FlyingThingBase (RTTI); IsRunnable slot 11 / Update slot 12
  (vtable 0x012896C0). Row 0x00914650.
- engine/CThingFilter_ScaresBirds.h  NEW HEADER (batch 8)  Bird (CThingAICreature*) +0 (Ego_r), inline ctor (FableWin 0x02B8D8A0), declared
  operator()(const CThingCreatureBase&) const (FableWin 0x02B8E030; the row defines the inline body). Row 0x00914650.
- engine/CThingDeadCreature.h  FABLE_RETAIL_CTHINGDEADCREATURE_STATGETTYPE (batch 8)  static StatGetType() { return 13; } (FableWin 0x01D3E930).
- (MERGED 8a0b6a1f3d) engine/CMiniMapDisplay.h  FABLE_RETAIL_CMINIMAPDISPLAY_GETDRAWPOSITION (batch 9, nested in DRAWMARKERS)  the +0x54 bytes typed as
  CRegionMinimap RegionMinimap (0x64, same layout; needs FABLE_RETAIL_CREGIONMINIMAP), the FableWin inline GetRelativePosOnMap (0x027D3920),
  GetRelativeMapSize declared (0x00829C70) and GetDrawPosition declared (0x0082A020). Nested FABLE_RETAIL_CMINIMAPDISPLAY_RELATIVEMAPSIZE_BODY:
  the FableWin 0x027D3850 GetRelativeMapSize body, visible and __declspec(noinline) under _MSC_VER (needs C2DVECTOR_INITIALISE).
  Default path unchanged. Row 0x0082A020.
- (MERGED 8a0b6a1f3d) engine/CRegionMinimap.h  FABLE_RETAIL_CREGIONMINIMAP_EXTENTS_NOINLINE (batch 9)  __declspec(noinline) before the existing inline
  GetRegionMapExtents body (retail calls 0x0066C6A0 from 0x0082A035). Default path unchanged. Row 0x0082A020.
- Batch 8 overlays were merged (7030ce2d2e) and moved to keep/stale_overlays/b8 (identical to the repo copies).
- engine/CEngineSceneGridPos.h  FABLE_RETAIL_CENGINESCENEGRIDPOS_INLINE (batch 10)  the FableWin bodies of the two ctors (0x02E1CDA0
  Level = -1; 0x02E1B520) and ConvertToLevel(long lev) (0x02E1B340), inline. Default path unchanged. Row 0x00B93B30.
- engine/CEngineInternalPrimitiveBase.h  FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_SETGRIDPOS (batch 10, nested in OCCLUSION)  inline
  SetGridPos(const CEngineSceneGridPos& pos) (FableWin 0x02E1CDC0). Default path unchanged. Row 0x00B93B30.
- engine/CEngineSceneMap.h  FABLE_RETAIL_CENGINESCENEMAP_ADDPRIMITIVE (batch 10)  includes CEngineSceneGridCell.h; CEngineSceneGrid gets the
  public GetCellCreate decl (0x00B937A0) and the private inline AddPrimitive(x, y, prim) (FableWin 0x02E1A450); CEngineSceneMap gets the
  public inline GetGridLevel (FableWin 0x02E1CE10) and private CalculateLevel (0x00B91920) / AddPrimitive (0x00B93B30). Same layout.
- engine/NPlayerGui_CDrawQuestInfo.h  FABLE_RETAIL_CDRAWQUESTINFO_DRAWMINIGAMEINFO (batch 10)  forward class C2DBoxF; public declarations
  DrawBox(const CEnginePrimitivePrepareInfo&, const C2DBoxF&) 0x00643EB2 and GetElementByIndex(long) const 0x00644FC5 (Ego_r / FableWin
  QAE / QBE). Declarations only; default path unchanged. Row 0x0064D404 (which defines GetElementByIndex's body in its own TU).
- (MERGED de5ba369a2) batch 10 overlays (CEngineSceneGridPos, CEngineInternalPrimitiveBase, CEngineSceneMap, NPlayerGui_CDrawQuestInfo).
- engine/CTCCreatureNavigation.h  FABLE_RETAIL_CTCCREATURENAVIGATION_FRAMEUPDATE (batch 11)  Agents typed std::vector<const CSteeringAgent*>
  (+0x40, same size), private inline GetParentCreature() const (FableWin ?GetParentCreature@CTCCreatureNavigation@@ABEAAVCThingCreatureBase@@XZ),
  the slot-9 FrameUpdate override (0x006CC800); includes CThingCreatureBase.h. Default path unchanged. Row 0x006CC800.
- engine/CThingCreatureBase.h  FABLE_RETAIL_CTHINGCREATUREBASE_GETCURRENTSPEED (batch 11)  public GetCurrentSpeed() const declaration
  (0x00661CD0). Row 0x006CC800.

- engine/CRegionMinimap.h  FABLE_RETAIL_CREGIONMINIMAP_BUILDTGA (batch 12, needs _POPULATE)  includes stlport/fable_containers.h, CSurface.h,
  CRGBColour.h; RegionMinimapSurface typed CSurface (+0x24), RegionMinimapCellList std::vector<RegionMinimapCellData> (+0x48),
  RegionMinimapThemeColour std::vector<CRGBColour> (+0x58), same offsets; CellAt indexes the vector; inline ValidateRegionMinimapPosition
  (empty, Ego_r 0x006A1760 `ret 8`), GetRegionMinimapThemeValue / GetRegionMinimapThemeLighting (FableWin text, Ego_d local `idx`),
  ReleaseMinimapThemeData (local `temp` swapped), declaration BuildRegionMinimapTGA 0x0066CD50. Default path unchanged. Row 0x0066CD50.
- engine/CSurface.h  FABLE_RETAIL_CSURFACE_SETPIXEL_COLOUR (batch 12)  declaration SetPixel(long, long, const CRGBColour&) 0x009F4120.
- engine/CRGBColour.h  FABLE_RETAIL_CRGBCOLOUR_INTERPOLATE (batch 12)  includes GFFloatToLongNear.h and declares GFGetInterpolatedColour
  (0x00A44250) above the class; inline Interpolate(CRGBColour col, float interp) const (FableWin 0x02188AA0, float 256.0f). Row 0x0066CD50.
- engine/C2DCoordI.h  FABLE_RETAIL_C2DCOORDI_INITIALISE (batch 12, default-class branch)  inline Initialise(long x, long y) (FableWin
  0x021875A0). Row 0x0066CD50.
- engine/C3DAnimation2.h  FABLE_RETAIL_C3DANIMATION2_CALCLOCALRELATIVE (row 0x00A4D770, OPEN)  declarations CalcLocalTransformations
  (0x00A4CEC0), CalcLocalTransformationsRelative (0x00A4D770), private GetKeyframeInterpolateFixPos (FableWin 0x03227270).
- engine/C3DMesh2.h  FABLE_RETAIL_C3DMESH2_GETBONEKEYFRAME (row 0x00A4D770, OPEN)  inline GetBoneKeyframe(unsigned long) const (FableWin
  0x03227EE0, &BoneKeyframes[bone]).
- engine/CEngineMap.h  FABLE_RETAIL_CENGINEMAP_PEEKWATERHEIGHT (batch 12, after PEEKWATER)  declaration PeekWaterHeight(long, long) const
  (0x00B41A80). Row 0x00B41A80.
- engine/NCameraModes_CScriptedControl.h  NEW header (batch 12)  NCameraModes::CScriptedControl : CBase (retail 0xE8: PScriptModule +0xA4,
  ObservedThingMeshHeightOnInit +0xAC, InitCamera +0xB0, Ego_r.pdb names), ctor decl 0x0069DFE0 (Ego_d.pdb params cam_man,
  cam_mode_def, pbase), Update / GetModeType declared (concrete). Needs CBase MEMBERS + STRING_CAGE + TYPED_COMPOSITES. Row 0x00699E60.
- engine/CGameCameraManager.h  FABLE_RETAIL_CGAMECAMERAMANAGER_SCRIPTEDCONTROL (batch 12, FULL class)  inline GetCameraMode() (FableWin),
  declarations SetCameraMode 0x00697E20, SetScriptedCameraMode 0x00699E60, private PreloadScriptedCamera 0x00699B00,
  SwitchIntoScriptedControl (FableWin; the row defines the body), UpdateCleanupFlags 0x00697C60. No layout change. Row 0x00699E60.

## hand3c batch 14
- engine/CGameMap.h: FABLE_RETAIL_CGAMEMAP_RESIZE (row 0x006B75A0): CGameMap::Resize declaration (public), CGameMapCell::Clear
  declaration (0x006B7490), and `private:` before the cell's four data members (FableWin.pdb access). Default path unchanged.
- engine/CTCWallMountEffects.h  NEW header (row 0x007DF770): CTCWallMountEffects : CTCBase (0x24; PDef +0x0C, WaitingForConfirmation +0x10,
  CreaturePerformingAction +0x14, FrameEffectLastUsed +0x1C, ObjectWasGamePersistant +0x20; Ego_r names), enum
  EWallMountEffectsConfirmationType (FableWin), FrameUpdate (slot 9) and private DoEffect 0x007DF6A0 declared.
- engine/CCreatureAction_UseWallMount.h  FABLE_RETAIL_CCREATUREACTION_USEWALLMOUNT_CLASS (row 0x007DF770): replaces the generated pad
  class with the polymorphic CCreatureActionBase-derived class (ctor 0x00843610, virtual dtor, Initialise / PerformAction / Clone,
  PObjectToUse +0xA8). Default path unchanged.

## hand3c batch 15
- engine/NFire_CFirePoint.h  NEW header (row 0x00780D80): NFire::CFirePoint (Ego_r 0x2C layout), inline IsOnFire / IsBurning (FableWin),
  SetAsBurning decl 0x00780540.
- engine/CFlammableDef.h  NEW header (row 0x00780D80): CFlammableDef : CSubDefClassBase, FireResistance +0x28, MaxDamageCaused +0x2C,
  FireDamagePerCycle +0x30, FireDamagePeriod +0x34 (no size claimed).
- engine/CTCFlammable.h  FABLE_RETAIL_CTCFLAMMABLE_FRAMEUPDATE (row 0x00780D80; with FIREPOINTS): typed PDef, Burning .. HasFirePoint
  (+0x1C..+0x31), inline IsOnFire (FireResistance <= 0), FrameUpdate (slot 9), ExtinguishFire 0x00780C80. Default path unchanged.

## Overlay housekeeping (after b14 landed, 2026-11-19)
Landed overlays were removed from hdr/ (backed up in hdr_landed_backup/); the repo copies now carry them with pinned asserts:
CGameCameraManager, CThingCreatureBase, C2DCoordI, CCreatureAction_UseWallMount, CRegionMinimap, CGameMap, CTCWallMountEffects,
CRGBColour, NCameraModes_CScriptedControl, CTCCreatureNavigation, CSurface, CEngineMap.
Remaining overlays are pure additions on the current repo copy: C3DAnimation2.h (rebased), C3DMesh2.h (row 00a4d770),
CTCFlammable.h, plus new CFlammableDef.h and NFire_CFirePoint.h (row 00780d80).
- engine/CPlayerGui.h  FABLE_RETAIL_CPLAYERGUI_UPDATEMONEY (batch 15, row 0x00439BB0): private UpdateMoney() and UpdateStat(float x5,
  NPlayerGui::CStatBase&) const declarations (Ego_r/FableWin AAE / ABE, Ego_d parameter names), plus a NPlayerGui::CStatBase forward
  declaration near the top. Pure additions; pinned asserts unmoved in meaning.
- engine/NPlayerGui_CStatBar.h  FABLE_RETAIL_NPLAYERGUI_CSTATBAR_CHANGETEXTS (batch 15, row 0x00439BB0): public UpdateChangeTexts()
  0x00646B12 and AddChangeText(const CRGBColour&, const CWideString&, long, bool) 0x0064DAE0 declarations (Ego_d parameter names).
- engine/CTCInventoryQuests.h  FABLE_RETAIL_CTCINVENTORYQUESTS_RESETCATEGORYLIST (batch 16, row 0x00621A20; needs
  FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22): inline ResetCategoryList() { Categories.clear(); } (Ego_r / FableWin
  ?ResetCategoryList@CTCInventoryQuests@@QAEXXZ; FableWin body = vector<CCategory>::clear).
- engine/NInventory_CTCInventoryBase.h  FABLE_RETAIL_CTCINVENTORYBASE_ADDCATEGORY (batch 16; not with ORIGINAL_COMPLETE_METHODS_PASS22,
  which declares the same line): protected AddCategory(CTCInventoryBase*, long, CThing*) 0x005C024A.
- engine/CCategory.h  FABLE_RETAIL_CCATEGORY_SETASSELECTED (batch 16): SetAsSelected(bool) 0x005BC934. Inserted after the pinned/
  unpinned assert block (CCategory.h's asserts sit above the class body, lines unchanged).
- engine/CNavQuadTree.h  FABLE_RETAIL_CNAVQUADTREE_GETBLOCKEDNODE (batch 17, row 0x00A770B0): inline static __fastcall
  GetBlockedNode() { return &BlockedNode; } (FableWin ?GetBlockedNode@CNavQuadTree@@SIPAVCNavBlockedQuadTreeNode@@XZ). Asserts are
  pinned (_AT) in the repo copy; insertion moves none in meaning.

## hand3c batch 23
- engine/CCodaVector.h  FABLE_RETAIL_CCODAVECTOR_ORTHOGONALRIGHT (row 0x00C40590 CCodaListener::Commit): in both class branches, the
  declaration float Normalize() (0x00C402E0) and the header inlines Dot(const CCodaVector&) const and OrthogonalRight(const CCodaVector&
  vFront, const CCodaVector& vTop) (COMDAT copy 0x00C40370). Evidence: GameCODA 2.10 CODAApi.lib / CODAApiDbg.lib member CodaListener.obj
  (relocations and COMDATs); bodies as the SDK Toolkit twin CCtkVector (CtkVector.h). Pure additions; the file has no asserts.
## Overlay housekeeping (2026-11-19, wrap-up)
b16..b22 overlays are in the repo (CCategory, CTCInventoryQuests, NInventory_CTCInventoryBase, CNavQuadTree, CEnginePrimitiveRenderer,
CEngineScene, CDecal, CPSCDecalRenderer: identical to the repo copies) and were moved to hdr_landed_backup/. Not landed: CCodaVector.h (b23),
CAIStateGroup_OpinionReactionRoot.h (OPEN row 00986210, CALLINGCHILDREN), CQ_HerosOldHouseScript.h (DIFFER row 00d8a240, WATCHFORBOOTY),
C3DAnimation2.h + C3DMesh2.h (OPEN row 00a4d770).
