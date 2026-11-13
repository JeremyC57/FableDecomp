# Chunk 2051 header overlay (all changes opt-in; default text of every repo header unchanged)

New headers (no repo copy existed; no landed includers):
- engine/CGameNavigatorManagerExternalInterface.h -- CNavigatorManagerExternalInterface root + CGameNavigatorManagerExternalInterface
  (vtable 0x01238C58, World +4; inline ctor and empty inline virtual dtors: CWorld::Init builds a stack temporary with no dtor call).
- engine/CScriptConversationManager.h -- Ego_r.pdb members (map<long,CScriptConversation> +0, NextConversationIndex +0xC,
  AlwaysShowText +0x10), no user dtor (checked_delete 0x004AE810 expands the map dtor).
- engine/CSkeletalMorphResourceManager.h -- CTBaseSingleton<CSkeletalMorphResourceManager>, CachedMorphs map +4, sizeof 0x10.
- engine/CStopWatch.h, CXMVPlayerBuffers.h, CEngineInterpolationInfo.h, CMainGameComponentInit.h, CFrontendGameComponent.h,
  CFrontEndDef.h (retail: CParentDefClassBase + vAttractModeMovie +0x3C), CMovieInfo.h, CXMVPlayer.h (statics descriptive) --
  Ego_r.pdb layouts; used by the parked 0x0042EC7C.

Opt-in blocks in copies of repo headers (each documented at the block with its evidence):
- CWorld.h FABLE_RETAIL_CWORLD_TYPED_MEMBERS: +0x14..+0x7C typed as Ego_r.pdb boost::scoped_ptr<T> / CCountedPointer<T> under the
  PDB names (same offsets), protected InitThingManager / InitMeshBank declarations, the seven always-on accessors respelled, the
  flattened-name offset asserts swapped for typed ones.
- CWorldMap.h _CTOR, CEnvironment.h _CTOR (inside _VIRTUALS), CNavigatorManager.h _VIRTUALS (real vptr, ctor, ~ slot 0,
  AddNewClassType slot 1), CFactionManager.h _CTOR, CCombatManager.h _CTOR (ctor + Init), CMessageEventManager.h _CTOR,
  COpinionReactionManager.h _CTOR, CThingSearchTools.h _IMPLICIT_DTOR, CGameTimeManager.h _IMPLICIT_DTOR, CGameCameraManager.h _CTOR
  (+ Update / GetRenderCamera override declarations), CGameCameraBase.h _CTOR (+ Update), CBulletTimeManager.h _CTOR,
  NUISystem_CManager.h FABLE_RETAIL_CMANAGER_WORLDINIT (SetMeshBank, inline SetInputManager, SetInput), CCreatureActionBase.h
  _STATREGISTER, CMainGameComponent.h _ENGINEGRAPHICS (PMainGraphicBank typed + inline PeekEngineGraphics).
- For the parked Run: CNewFrontendGameComponent.h _RUN, CSystemManager.h _FORCEMUSICPLAY (inline Get/ResetForceMusicPlay, FITTED
  bodies), CFrontEndManager.h _RUN, CTBaseSingleton.h _RELEASE, CFontManager.h _ASYNC, CInputManager.h _CLEAREVENTS,
  fable_retail_banks.h _CLOSERETAILBANK, NUISystem_CUIScreenUnitConverter.h _RESETDIMENSIONS, CRGBColour.h
  _FORCEINLINE_COPY (same copy body, FABLE_FORCEINLINE; size TU refuses the plain inline).
No shared inline body text was changed on a default path, so no landed row needs a regrade.

Continuation agent (2026-11-12): no header text changed. The matched Run uses FABLE_RETAIL_CRGBCOLOUR_FORCEINLINE_COPY with its
`// Inline-budget:` line (c2 inline trace: copy ctor size 31, savings 5 -> 26 > size-TU limit 15, refused with no score). CWorld::Init
regraded RELOCATION_MATCH under the same overlay.

# Chunk 2055 additions (scout, row 0x00821A80 CMap::LoadFromFile; all opt-in, default text of every repo header unchanged)
Everything above (chunk 2051) is kept unchanged. New / changed files (evidence at each block; details in scout/headers.md):
- engine/CCachedFile.h -- NEW here, a byte-identical copy of chunk 2054's overlay header (/opt/fable_lane/w22/agent2054/hdr; matched
  0x00A7AF60 with it): CCachedFile : CAFile, inline `virtual ~CCachedFile() { Close(); }` (fwsize /EHsc SAME as FableWin 34i/126b).
- engine/CHeightMap.h -- NEW (no repo copy), under FABLE_RETAIL_CHEIGHTMAP; merges the pending hand1017 + handQ3 overlays (reconcile at
  landing): : CBaseClassNonCopyable (RTTI), friend CMap, private Clear 0x008F6050; nested opt-in FABLE_RETAIL_CHEIGHTMAP_CELLINDEX
  (GetExtraCells, GetIndexIntoCells -- both fwsize SAME as FableWin -- and GetCellIncludingExtraCellsAt), FABLE_RETAIL_CHEIGHTMAP_GETCELL.
- engine/CGameMap.h -- NEW (no repo copy), under FABLE_RETAIL_CGAMEMAP: CGameMapCell (7 bytes; SetAtmosTheme / SetAtmosBlend /
  SetSoundTheme fwsize SAME) and CGameMap : CBaseClassNonCopyable (sizeof 0x24; GetSizeX/Y fwsize SAME; GetCell index from retail).
- engine/CMap.h -- copy + nested opt-in FABLE_RETAIL_CMAP_LOADFROMFILE (LoadFromFile declaration, static inline
  HeightMapUnitToGameMapUnit, CThemePaletteEntry::SetThemePaletteIndexUsed; both bodies fwsize SAME).
- engine/CFileFormatBrush.h -- copy + nested opt-in FABLE_RETAIL_CFILEFORMAT_MAPHEADER (CFileFormatMapHeader 0x10822,
  CFileFormatHeightMapCell 21, CFileFormatGameMapCell 11; implicit ctors fwsize SAME as FableWin's).
- engine/CHeightMapCell.h -- copy + nested opt-in FABLE_RETAIL_CHEIGHTMAPCELL_SETTERS (SetEngineThemePaletteIndex / SetEngineBlend;
  FableWin sizes are assert builds: bodies UNVERIFIED beyond retail's plain byte stores).
No landed includer reaches any new block (all `#ifdef`), so no landed row needs a regrade. Proof the overlay compiles: scout/stub.cpp
(DIFFER 3v2648, no SRC_FAIL), scout/fw_helpers.cpp (every helper instantiated; /O2 /GS DIFFER 431v2648, /Od fwsize OK),
scout/offsets.cpp (26 compile-time offset checks pass).

# Chunk 2055 additions (worker, row 0x00821A80 CMap::LoadFromFile RELOCATION_MATCH; all opt-in, default text unchanged)
Three more copies of repo headers, each with one new opt-in block (evidence at the block):
- engine/CDefPointer.h -- FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD: SetDynamicCast reads the source handle's pointee again for
  the store (the text of the proven copy assignment, 0x007355F0) instead of caching it in Assign's by-value parameter. Keeps the
  destination's release null test when the source is an address-taken local: retail 0x00821F5B `test edi,edi ; je ; dec dword ptr
  [ebp]`. FITTED by the retail bytes (FableWin's debug SetDynamicCast is 34 instructions with its debug type check; ours is 24).
  The default (`Assign(other.InternalGetObject())`) is untouched.
- engine/CDefinitionManager.h -- FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_INLINE: the by-name GetDef<T> as a plain `inline`
  template (expanded at 0x00821E95), text `def.Get()` test + `out->SetDynamicCast(def)`. PROVEN by size: fwsize /EHsc gives 40
  instructions / 139 bytes = FableWin.pdb 0x02247E20 exactly (the IsNull / `*out = def` text of ..._GETDEF_NAME_BODY is 41 / 142).
- stlport/fable_containers.h -- FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE: both vector::_M_insert_overflow bodies visible but
  `__declspec(noinline)` (macro FABLE_STLP_OVERFLOW_OUTOFLINE, empty by default). Retail calls vector<long>::_M_insert_overflow
  0x00440990 three times and keeps _M_finish in a CPU location across push_back's store; the declaration-only opt-in reloads it and
  grows the frame by 8. The row includes "stlport/fable_containers.h" first so the overlay copy is the one every header sees.
Regraded under this overlay (all unchanged): 0x00821010 LoadBrushesFromFile, 0x00820B50, 0x0063E590, 0x00826C90, 0x008F5FA0,
0x008F6BB0 (vector<CHeightMapCell>::_M_insert_overflow), 0x008338D0, 0x00835B90, 0x0083D280, 0x008CB250, 0x008C79B0.
