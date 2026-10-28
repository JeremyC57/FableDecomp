# Chunk 522 notes (re-queue, 20 rows)

9 RELOCATION_MATCH (src/), 11 DIFFER (differ/), 0 retail_asm, 0 SKIP.

## Names -- Proven (two sources: Ego_r.pdb / FableWin.pdb spelling + retail call shape / RTTI / donor order)
- 0x00533BC0 `?RemoveChild@CComponent@NUISystem@@UAEXK@Z` -- CComponent vtable slot 64; PDB virtual RemoveChild(unsigned long).
- 0x00535B10 STLport `vector<CCountedPointer<CComponent>>::insert(iterator, const value_type&)` -- callers AddChildFront 0x00533BA0 / 0x00546012.
- 0x00540620 `?Initialise@CEditBox@NUISystem@@UAEXXZ` -- CEditBox vtable; calls CChangingStateComponent::Initialise non-virtually.
- 0x005416B0 `?ResetPositions@CDynamicList@NUISystem@@UAEXXZ` -- CDynamicList vtable slot; writes PositionOffset/TopIndex, walks ScrollingChildren.
- 0x0057846F `??1CTCInventoryExperience@@UAE@XZ`, 0x0061A500 `??1CTCInventoryQuests@@UAE@XZ` -- RTTI vtables, tail to ~CTCInventoryBase 0x005C00A0.
- 0x005D87B6 STLport `_Rb_tree<unsigned long,pair<const unsigned long,long>>::insert_unique(iterator, const value_type&)` (hinted form, _tree.c).
- 0x005D94B0 `?CacheDef@CTCInventoryItem@@CI_NJ@Z` -- private static fastcall; statics 0x013B8C14/18.
- 0x0062D9E0 `??0CCreatureAction_GiveItemToThing@@QAE@AAVCThingCreatureBase@@AAVCThing@@1@Z` -- RTTI vtable 0x0125919C.
- Parked: 0x004FA0A9 TransferVectorOfSubComponents<CCardDef>, 0x005029A0 GetRelevantBuilding (fastcall free), 0x0054DF50 CFrontEndButton::GetDataFromDef,
  0x0055DC00 CTCPhotojournalCapture::CreateScreen (private), 0x005DCC00 CAnimationSet::GetRandomAnim, 0x005E1CB0 STLport __adjust_heap<pair<ulong,CAnimationEntry>>,
  0x00632B90 GFSerialiseMapBinaryIn<CCharString,unsigned long>, 0x0064A060 NPlayerGui::CDrawStatUpdates::Update, 0x0064D7A6 CDrawQuickAccessIcons::DrawChargeGraphic (private),
  0x00654438 CDrawQuestInfo::Draw(prepare, bool), 0x00663990 CThingCreatureBase::CreateWeaponForThingFromDef (static fastcall).
- Callees (exact PDB spellings in NAME_FIXES.tsv): CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (non-const) / 0x004365B0 (const);
  bitset<274>::test 0x00410DE0; CThingObject::Create 0x00703210; CTCInventoryItem::AddToInventory 0x005D8D50; CTCInventoryWeapons::SetThingAsActiveWeapon 0x005CB2B0;
  CDefClassBase::GetSubDef<CCarryableDef> 0x005CC10E; CTCCarrying Remove/Get/PutThingInCarrySlot 0x006A16A0/0x006A11F0/0x006A1F80; CTCHeroExperience::GetStatExperience 0x006D6510;
  CPlayerGui::PeekGuiDef 0x00438570; NPlayerGui::GFAnchorToTopRight 0x00643CA1; CEnginePrimitive2DClockSprite ctor 0x0065595C (17 pushes = PDB arg list);
  CDrawQuickAccessIcons::GetActivationTimeForButton 0x0064B5C1 (AAE); CCharString ctor/copy/operator= 0x0099E4B0/0x0099EC30/0x0099EFB0; CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0;
  CDataOutputStream::WriteSLONG 0x00993EB0; CPersistContext::CheckCRC 0x00404500; CWideString copy ctor / dtor 0x0099B720 / 0x0099B510 (global class).

## Names -- Probable (one strong source + shape)
- 0x00647457 `NPlayerGui::CMultiStatBar::AddStatUpdate(EStatUpdateType, long)` -- only (enum,long) member; body searches by type.
- 0x00643CDA `NPlayerGui::GFAnchorToBottomLeft` -- third of four GFAnchorTo* in donor order; adds height to Y only.
- 0x00437870 `CPlayerGui::IsMiniMapDrawing() const` -- returns PDrawMiniMap->Drawing.
- 0x00643EB2 `CDrawQuestInfo::DrawBox(prepare, const C2DBoxF&)`; 0x0064F6DC `CDrawQuestInfo::DrawOracleSymbols(prepare)`.
- 0x005BC6ED `NInventory::CTCInventoryBase::CaptureBackBufferToTexture()`.
- Bank vtable slot 24 GetGraphicMaxHeight; CIEngine slot 23 SetPersistentPrimitive.

## Names -- Descriptive (no PDB name exists)
- `DynamicListResetStates_01246dfc` -- const long[6] state-index table read by ResetPositions (data, no symbol).
- `GetFadedAlpha_00643d5d` -- CDrawBase helper at 0x00643D5D; landed under two different class names (see Review at landing).

## Label corrections (catalogue / landed names proven wrong; full list in NAME_FIXES.tsv, 65 rows)
- 0x004FA0A9 catalogued TransferVectorOfSubComponents<CAbilityButtonGuiDef>; element is CCardDef (CardPack, vtable .?AVCCardDef@@).
- 0x00663990 catalogued as a free function; it is static CThingCreatureBase::CreateWeaponForThingFromDef.
- 0x006924B0 bound as GetTableAnimation(CThingCreatureBase*, long); it is protected static CCreatureActionBase::GetRandomAnimFromName.
- 0x0064B5C1 bound QAE; PDB says private AAE.
- 0x005796AB / 0x00579FAA / 0x00579FC1 / 0x0057988C labelled `std::list::~list`; they are _Rb_tree dtors (map members of CTCInventoryExperience); 0x005792DA is _List_base<CSliderAnimator>.
- 0x00533CC0 bound `?AddChild@CComponent@@`; slot 60 is AddChildDynamically (NUISystem namespace).
- 0x00535B10 / 0x00485DBC bound on a CRetailVector stand-in; STLport std::vector is the real class (_M_insert_overflow is protected, IAE).
- 0x005E0E20 bound `??4SAnimationEntryPair`; it is std::pair<unsigned long,CAnimationEntry>::operator=.
- 0x00643CA1 bound without the NPlayerGui namespace.

## Header debt (SHARED_HEADER_AVAILABLE warnings)
CIVCountedPointeeBase, CObserver, CDefinitionManager, CTCInventoryItem, CCreatureActionBase -- local minimal declarations used because the
generated headers carry debug-STL layouts / missing retail-size bases; retype at landing.
