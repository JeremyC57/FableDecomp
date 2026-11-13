## 0x00956060

FABLE_RETAIL_CAISTATEGROUP_MAZEBASE -- engine/_quarantine/CAIStateGroup_MazeBase.h: polymorphic class over CAIStateGroupBase (PTargetThing, EnemyKnockDownFrame).
FABLE_RETAIL_CAISTATEGROUP_MAZETUTORIALBATTLETELEPORT -- engine/CAIStateGroup_MazeTutorialBattleTeleport.h: class over MazeBase with Update() slot 12, FrameOfLastTeleport, LastTeleportPos.
FABLE_RETAIL_CMAZEBATTLEDEF -- engine/_quarantine/CMazeBattleDef.h: CMazeBattleDef : CParentDefClassBase with StatGetTypeName and retail-offset members.
FABLE_RETAIL_STLP_COPY_NOINLINE -- stlport/fable_containers.h: random-access __copy kept out of line (__declspec(noinline), body visible).

## 0x00F39730

engine/CRenderStateCaptureBlock.h: FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_CAPTURE_INLINE -- inline Capture() { Open = true; StateManager->StartCaptureBlock(this); } (row 0x00F39730).
engine/CDisplayManager.h: FABLE_RETAIL_CDISPLAYMANAGER_RENDERTARGET -- declares bool SetRenderTarget(CRenderTarget&) 0x009BF160 and inline IsRendering() const (row 0x00F39730).

## 0x00523CF0

# Header overlay for row 0x00523CF0 (CThingManager::RuntimeSaveGamePersistentEntitiesInLoadedMaps)
Each file is a full copy of the current repo header with additions only (diff shows only `>` lines), each behind its own opt-in.

- engine/CClassFactory.h: FABLE_RETAIL_CCLASSFACTORY_CLASSLIST -- `std::list<CClass*> ClassList` (+0x18, Ego_r type) in place of the opaque `unsigned long ClassList[1]`.
- engine/CThingManager.h: FABLE_RETAIL_CTHINGMANAGER_RUNTIMESAVE -- public SaveEntitiesToScript 0x00521EB0 / SaveEntitiesBinary 0x00522160 (Ego_r QAEX...), private RuntimeSaveGamePersistentEntitiesInLoadedMaps 0x00523CF0 (AAEX_N); forward `class CMemoryDataOutputStream`.
- engine/NThingManager_CEntityRuntimePersistence.h: FABLE_RETAIL_CENTITYRUNTIMEPERSISTENCE_SETGAMESCRIPT -- `void SetGamePersistentScript(long,const CCharString&)` 0x005237F0 (Ego_r QAEXJABVCCharString@@@Z).
- engine/CWorldMap.h: FABLE_RETAIL_CWORLDMAP_GETNOMAPS -- inline `long GetNoMaps() const { return Maps.size(); }` (Ego_r + FableWin ?GetNoMaps@CWorldMap@@QBEJXZ).
- engine/CMemoryDataOutputStream.h: FABLE_RETAIL_CMEMORYDATAOUTPUTSTREAM_RESERVESIZE -- `void ReserveSize(unsigned long)` 0x00996BC0 (Ego_r QAEXK@Z).
- engine/CThing.h: FABLE_RETAIL_CTHING_GETSERIALISATIONSECTIONNAME -- inline `const CCharString& GetSerialisationSectionName() const` (FableWin 0x017BE742).
- engine/CCompressorZlib.h: FABLE_RETAIL_CCOMPRESSORZLIB_INTERNALCOMPRESS -- `unsigned long InternalCompress(const void*,unsigned long,CArray<unsigned char>&)` 0x00A3ADD0 (Ego_r QAEKPBXKAAV?$CArray@E@@@Z).
- fable_string.h: FABLE_RETAIL_CCHARSTRING_RESERVE -- CCharString::Reserve(long) 0x0099EF90 and SetUseFastExtend(bool) 0x0099EF70 (Ego_r). NB: a root-dir overlay; do not include fable_game.h (it pulls the repo fable_string.h by relative path -> C2011).
- stlport/fable_containers.h: FABLE_RETAIL_STLP_VECTOR_RANGE_INSERT -- STLport 4.6 vector::insert(pos, first, last) -> `__declspec(noinline)` _M_range_insert(forward_iterator_tag) real body (vector<CCharString> 0x0052B6A0), the input-iterator __copy overload and forward declarations of distance/advance.

## 0x00918B90

engine/C3DVector.h: FABLE_RETAIL_C3DVECTOR_RANDOMPOINTINSPHERE -- static __fastcall GetRandomPointWithinUnitSphere(unsigned long*) decl (0x00A146C0, Ego_r).
engine/CAIStateGroup_Gaze.h: FABLE_RETAIL_CAISTATEGROUP_GAZE_UPDATE -- FindFreeGazePos (0x00918880) + virtual Update decls.
engine/CCreatureNavigationDef.h: FABLE_RETAIL_CCREATURENAVIGATIONDEF -- CDefClassBase-derived real class (same member offsets) + inline FableWin StatGetTypeName (for the visible GetSubDef body).

## 0x00972990

CAIStateGroup_JackOfBladesStage1.h: FABLE_RETAIL_JACK_STAGE1_ONACTIVATE declares virtual OnActivate (0x00972990)
CTCBoss.h: FABLE_RETAIL_CTCBOSS_SETPHASESTARTPOS inline SetPhaseStartPos
CThingSearchTools.h: FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTROUGH declares GetPFirstThingInRoughArea<Filter,T> (0x007FCEF0)
CTCHitLocations/NHitLocations/C2DBoxI etc: repo opt-ins only (FABLE_RETAIL_CHITLOCATION_SETENABLED, FABLE_RETAIL_C2DBOXI_CTORS).
CThingSearchTools.h: FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTROUGH_BODY defines the real GetPFirstThingInRoughArea<Filter,T> body (0x007FCEF0 algorithm) as a noinline template (needs _BESTROUGH, _BESTROUGH_BODY, CTHING_STATTYPEFLAGS, CTCMAPWHO(_SEARCHID)).
CTCHitLocations.h: FABLE_RETAIL_CTCHITLOCATIONS_WITHDEFNAME declares GetPHitLocationWithDefName (0x00815820, Ego_r/FableWin PDB spelling; replaces the descriptive _FROMNAME GetPHitLocationFromName).

## 0x006C8E90

- engine/CTCPerceiveThing.h: FABLE_RETAIL_CTCPERCEIVETHING_THINGFLAGS (nested in _AWARENESS): inline IsThingIgnoreFOVLineOfSight / IsThingSmelt / IsThingHeard + GetSightRadius decl 0x007448F0 (row 0x006C8E90).

## 0x00839100 (not landed)

engine/CThingGameObject.h: FABLE_RETAIL_CTHINGGAMEOBJECT_WEAPONTRAIL -- declares GetWeaponTrailGraphicIndex() const (0x00839100).
engine/CTCBerserk.h: FABLE_RETAIL_CTCBERSERK_WEAPONTRAIL -- declares GetWeaponTrailAttack/GetWeaponTrailKnockDown const (0x0078BED0/0x0078BEE0) + StatGetInterfaceType.
engine/CTCCombat.h: FABLE_RETAIL_CTCCOMBAT_WEAPONTRAIL -- declares IsMeleeFlourishValid() const (0x00733EE0) + inline GetIsInMiddleOfFlourishAction() (FableWin).
stlport/fable_map.h: FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE -- _Rb_tree_iterator::operator-- with its STLport body, noinline (retail COMDAT 0x0083AEA0).
stlport/fable_map.h: FABLE_RETAIL_STLP_MAP_REVERSE -- _Rb_tree / map const_reverse_iterator rbegin()/rend() const (STLport 4.6; needs FABLE_RETAIL_STLP_LIST_REVERSE for reverse_iterator).
engine/CWeaponDef.h: FABLE_RETAIL_CWEAPONDEF_WEAPONTRAILS -- FlourishSpecialWeaponTrail (+0x7c) typed as the Ego_r CWeaponTrailGraphicSet (default path keeps long[2]); WeaponTrails (+0x84) typed as the real std::map<EObjectAugmentationType,CWeaponTrailGraphicSet> (struct_layouts_egor.tsv) + CWeaponTrailGraphicSet {Attack, Knockdown} (Ego_r, no header of its own).

Repair 3 (2026-11-13): all five overlay files re-checked against the CURRENT repo headers (diff <repo> <overlay> | grep -c '^<' = 0 for each; additions only).

## 0x00AD9F10 (not landed)

- stlport/fable_containers.h: FABLE_RETAIL_STLP_LIST_ASSIGN_POPBACK_DECL -- list operator= / pop_back declared only (out of line in retail, 0x00ADD020 / 0x00ADC790).
- engine/CVertexGroupBlock.h: FABLE_RETAIL_CVERTEXGROUPBLOCK_REMOVELAST -- RemoveLast(unsigned long) declaration (0x00AD9470).
- stlport/fable_map.h: FABLE_RETAIL_STLP_TREE_CONSTFIND_NOINLINE -- tree const find body visible but __declspec(noinline) (0x006699C0).
- stlport/fable_containers.h: FABLE_RETAIL_STLP_LIST_POPBACK_NOINLINE (with ..._ASSIGN_POPBACK_DECL) -- pop_back body visible, __declspec(noinline).
- engine/CVertexGroupBlock.h: FABLE_RETAIL_CVERTEXGROUPBLOCK_HASGROUP_DECL -- HasGroup declared only; the row defines its body (Ego_r has it out of line = defined in the .cpp).
- stlport/fable_containers.h: FABLE_RETAIL_STLP_SORT2_BODY (+ _NOINLINE) -- two-arg sort with the STLport body visible (landed 0x00ADC890 text).
- stlport/fable_containers.h: FABLE_RETAIL_STLP_LIST_ASSIGN_BODY -- list operator= body visible out of class (landed 0x00ADD020 text).
- engine/CVertexGroupBlock.h: FABLE_RETAIL_CVERTEXGROUPBLOCK_ADDGROUP -- declares ?AddGroup@CVertexGroupBlock@@QAEXK@Z (landed 0x00AD91F0) so row 0x00AD9F10 can carry InsertBlock's real body (wf20 attempt 3).

## 0x008C12D0 (not landed)

- engine/CTCBoss.h (full copy of the repo header): `FABLE_RETAIL_CTCBOSS_GETGENERICCOUNTER` adds the inline FableWin.pdb accessors ?GetGenericCounter@CTCBoss@@QBEJXZ / ?SetGenericCounter@CTCBoss@@QAEXJ@Z (row 0x008C12D0 reads GenericCounter +0x10 through it). Default path unchanged.
- engine/CCombatAction_GenericStrikeResponseBase.h (full copy): `FABLE_RETAIL_CCOMBATACTION_GENERICSTRIKERESPONSEBASE_TYPED` types PThingThatHit (CIntelligentPointer<const CThing>, Ego_r.pdb) / PointOfHit / DirectionOfHit (C3DVector) and declares virtual Initialise (0x00865F40); same layout. Default path unchanged.
- engine/CCombatAction_StrikeResponseJackOfBladesIntoRush.h (full copy of the generated header): `FABLE_RETAIL_CCOMBATACTION_STRIKERESPONSEJACKOFBLADESINTORUSH` gives the real class over CCombatAction_GenericStrikeResponseBase with virtual Initialise and CauseRush +0x148 (sizeof 0x14c). Default (generated) path unchanged.
- Repair 3 (2026-11-13): all three overlays re-based check vs the current repo headers -- `diff repo overlay | grep -c '^<'` = 0 for each
  (only the #ifdef blocks above are added). engine/CIntelligentPointer.h is NOT overlaid (the repo copy is used as is).

