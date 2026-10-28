# Chunk 514 notes (agent514, 2026-10-28)

40 rows: 33 matched (4 MATCH + 28 RELOCATION_MATCH in src/, plus 1 retail_asm RELOCATION_MATCH in retail_asm/) and 7 parked in differ/.

## Proven
Each name is backed by Ego_r.pdb plus a second source: a vtable/RTTI slot, donor order or size, a caller, or the exact PDB layout.
- 00643e09 NPlayerGui::CDrawMiniMap::IsActive() const -- donor order ClearPrimitivesIfNecessary -> IsActive -> SetAsActive matches landed neighbours 0x00643DE5 / 0x00643E2E
- 00644334 NPlayerGui::CDrawStatUpdates::Draw(const CEnginePrimitivePrepareInfo&) -- retail order Crime::Draw, Crime::Clear, StatUpdates::Draw, StatUpdates::Clear (0x00644393 landed), AutoPickups::Draw, AutoPickups::Clear (0x0064440B landed) = donor order; it reads CPlayerGui::PStatUpdates
- 006443ac NPlayerGui::CDrawAutoPickups::Draw(const CEnginePrimitivePrepareInfo&) -- same ordering evidence; it reads CPlayerGui::PAutoPickups and CPlayerGuiDef::AutoPickupPos
- 00644587 NPlayerGui::CDrawTargetInfo::OnEventAttack() -- donor sizes: OnEventUnsheatheWeapon is 97 bytes and OnEventAttack 51 bytes; retail 0x00644526 is 97 bytes and this row 51 bytes
- 0064dd4d NPlayerGui::CDrawSpiritTargetingCursor::CDrawSpiritTargetingCursor(CPlayerGui&, vector<CDrawBase*>&) -- vptr 0x0125A5E8, RTTI .?AVCDrawSpiritTargetingCursor@NPlayerGui@@
- 0064e0a1 NPlayerGui::CDrawStatUpdates::CDrawStatUpdates(CPlayerGui&, vector<CDrawBase*>&) -- vptr 0x0125A5F4, RTTI; the PDB layout matches
- 00654f80 NPlayerGui::CStatBar::CChangeText::CChangeText(const C2DVector&, const CRGBColour&, const CWideString&, long, bool) -- PDB signature, `ret 14h`, the stores match the PDB layout
- 0065564b NPlayerGui::CStatBar::CChangeText::CChangeText(const CChangeText&) -- PDB; member-wise copy of the same layout
- 0065ffca std::find_if<list<CCountedPointer<CVillageTaskHorn>>::iterator, CIsHornTaskWithThisHorn> -- only caller is CTCVillage::UpdateHornTasks 0x0065BE6C; the predicate member is PHorn (PDB); the element copy ctor 0x0065D89B is bound to CCountedPointer<CVillageTaskHorn>; the Dinkumware donor has the same find_if at 0x006A00B8
- 00663340 CThingCreatureBase::GetPHeroCombatDef() const -- donor order IsCombatAnimationValid -> GetPHeroCombatDef -> ClearCurrentAction (both neighbours landed); reads CThingCreatureDef::HeroCombatDef
- 00686f10 CTCBase::AllocateTC(const CCharString&, CThing&) -- donor order AllocateTC -> GetPTCInfo (0x00686F60 landed); fastcall signature; caller CThing::AddTC
- 006f90d0 NAnimationEvents::CManager::AnimHasEmbeddedEvent(unsigned long, const CCharString&) const -- donor order -> GetEmbeddedEventTime 0x006F9130 (landed); callee order HasEvent -> GetEventTime
- 006472b2 NPlayerGui::CDrawDamage::AddDamageChange(float, bool) -- PDB signature; push_front 0x00655EB4 feeds the SDamageValue list; caller ChangeHeroHealthBy 0x00891720
- 00712b60 NTCScriptedControl::CActionDoCreatureAction::OnActivate() -- vtable 0x01265838 slot 2
- 006f4850 CDefendingCombatantInfo::CDefendingCombatantInfo() -- exact PDB member layout at STLport offsets; inlined at +0x34 of 0x006F4A00
- 006f4a00 CCombatantInfo::CCombatantInfo() -- PDB layout (DefenderInfo +0x34)
- 00710700 std::vector<short>::vector(const vector&)
- 006cda10 CGroundNavigatorExternalInterface::PeekNavigatorRadius(bool) const -- vtable slot 6
- 006cdb50 CGroundNavigatorExternalInterface::GetMinPathCost() const -- slot 18
- 006cdb90 CGroundNavigatorExternalInterface::GetMaxPathCost() const
- 006bd840 CMusicManager::IsDangerMusicEnabled() const -- the tail jump in CGameScriptInterface 0x00891330
- 006eab00 CDefendingCombatantInfo::CCombatRing::GetAllProngs(vector<CProng*>&) -- donor order after GetAllFreeProngs 0x006EAA90
- 006e8050 CDefendingCombatantInfo::CCombatRing::StretchRingThickness(const CThingCreatureBase&, const CThingCreatureBase&)
- 00692a50 CCreatureActionBase::IsUsingActionMovement() const
- 00692c30 CCreatureActionBase::GetActionSpeedParameter() const
- 006bbfa0 CRegion::IsMapIncluded(long) const
- 006b74e0 CGameMapCell::GetAtmosBlend(long) const
- 006e7690 CGameScriptInterface::DeregisterTimer(long)
- 00625160 (differ) NUISystem::CUIStateDef `scalar deleting destructor' -- vtable 0x0125871C slot 0, RTTI; PDB ??_GCUIStateDef@NUISystem@@UAEPAXI@Z
- 0067c520 (differ) CBlurredAttitudesDef `scalar deleting destructor' -- vtable 0x0125B8AC slot 0, RTTI
- 006d63a0 (differ) CTCHeroExperience::GetRunningNumHits() const -- caller CGameScriptInterface::GetPlayerCreatureCombatMultiplierRunningNumHits 0x00899D3E, donor order
- 0070c4e0 (differ) CAnimateThing::GetFirstCombinationAnimFromType(long)

## Probable
- 0064a37b NPlayerGui::CDrawQuestInfo::GetScriptSpriteBySymbol(const CCharString&) const -- catalog label (a PDB name) plus a body that reads CPlayerGuiDef::ScriptSprites
- 006ad220 CThingPlayerCreature::ResolveMovementBandType()
- 006af520 CMessageEvent::GetPTarget<CThingCreatureBase>() const
- 006b1900 CAtmosProcess::MixAtmos(const CCharString&, const CCharString&, float, float)
- 006bd780 CMusicManager::CState::GetDanger() const
- 0064a2db (differ) NPlayerGui::CDrawPerceivers::AddPerceivingThing(CThing&)
- 006af4e0 (differ) CMessageEvent::GetPOriginator<CThing>() const
- 006bb5b0 (differ) CGameTimeManager::IsTimeBetween(float, float, float) const

## Descriptive
No row's function name is descriptive. These modelling names are descriptive and flagged:
- 0x013BA860 g_fFramesPerSecond_013ba860 (006472b2) -- a file-static float that dynamic init 0x0121DF1C sets to (float)FramesPerSecond
- slot 1 of the CDrawBase vtables, EmptyVirtual_00442760 (0064dd4d, 0064e0a1) -- a folded empty `ret`, never called by name
- CDrawPerceivers +0x3c PerceivingThings (0064a2db, differ) -- the class is padding in Ego_r
- the helpers that are not retail functions: Create_*/Kill*_<addr> (differ dtors), CopyChangeText (none), FindDef_00663340 and the other stand-in callees used inside noinline stand-in bodies

## Label corrections (catalog -> evidence)
- 00644334 CDrawAutoPickups::Draw -> NPlayerGui::CDrawStatUpdates::Draw
- 00644587: the label is right, but the landed 0x00644526 carries ?OnEventAttack@... and is really OnEventUnsheatheWeapon
- 0064dd4d CDrawQuickAccessIconsPC ctor -> NPlayerGui::CDrawSpiritTargetingCursor ctor
- 0065ffca CGuiControlTree::SelectNextEntryByName -> std::find_if<..., CIsHornTaskWithThisHorn>
- 00663340 CEventKilledCreature ctor -> CThingCreatureBase::GetPHeroCombatDef
- 00686f10 CMeshVolumeMap::Find -> CTCBase::AllocateTC
- 00625160 / 0067c520 CCompressedMemoryDataInputStream `vector_deleting_destructor' -> CUIStateDef / CBlurredAttitudesDef `scalar deleting destructor'
- 006f4850 C3DAnimFileSequenceChunk ctor -> CDefendingCombatantInfo ctor; 006f4a00 C3DMeshMaterial ctor -> CCombatantInfo ctor
- 006eab00 CMusicManager::ResetMusicEntriesOffsets -> CCombatRing::GetAllProngs
- 006bbfa0 CHitParameters::IsDivineWrathStrike -> CRegion::IsMapIncluded
- 006b1900 CGameScriptInterface::SetQuestInfoName -> CAtmosProcess::MixAtmos
- 006af520 CGameScriptThing::GetPThing -> CMessageEvent::GetPTarget<CThingCreatureBase>
- 00710700 CAnimSet::CopyConstruct_Shorts -> std::vector<short> copy ctor
- 006d63a0 CEngine::GetEditorAnimation -> CTCHeroExperience::GetRunningNumHits
- 006bd780 CMusicManager::IsDangerMusicEnabled (propagated from its twin) -> CMusicManager::CState::GetDanger

## Header debt (local models where an engine header exists; vland SHARED_HEADER_AVAILABLE)
- Retail offsets or the namespace differ from the flat generated headers: CDrawMiniMap, CFadeInFadeOutBase, CPlayerGui and CPlayerGuiDef (STLport containers shrink the def; Draw/OnEventAttack/GetScriptSpriteBySymbol), CThingCreatureBase (PDef +0xe0, PCurrentAction +0x100, PBody), CCreatureActionBase, CDefinitionManager (GetDef stand-in bodies), CTCBase, CRegion, CGameScriptInterface, CCombatTypeDef, C3DAnimationInfo, CMeshDataBank, NAnimationEvents::CManager, CActionDoCreatureAction (flat struct vs namespaced class with a virtual base).

## Review at landing
- 0x00643E09: reloc_bindings has the non-const ?IsActive@CDrawMiniMap@NPlayerGui@@QAE_NXZ; Ego_r.pdb and this row use QBE.
- 0x00644526 (landed) is CDrawTargetInfo::OnEventUnsheatheWeapon, but it carries OnEventAttack. This row 0x00644587 now uses ?OnEventAttack@CDrawTargetInfo@NPlayerGui@@QAEXXZ, so the two collide (RELOC_DUPLICATE): rename 0x00644526.
- 0x00438570 PeekGuiDef: the PDB spelling is ?PeekGuiDef@CPlayerGui@@SIABVCPlayerGuiDef@@XZ (used here). Landed CDrawCrime::Draw 0x006442AE / CDrawMoney::Draw bind global YI spellings.
- 0x00655EB4 is the list<SDamageValue>::push_front, landed as invented ListWrapper_00655eb4::ListPushFrontWrapper_00655eb4. 006472b2 binds the STLport push_front.
- 0x0042B467 is _Rb_tree<CCharString, pair<const CCharString,long>, _Select1st, ...>::_M_find. 0064a37b uses the _Select1st spelling; reloc_bindings also holds one without _Select1st and several invented map finds.
- 0x00656D20 is vector<CIntelligentPointer<CThing>>::resize(size_type, const T&); its label is CArray<pair<long,long>>::push_back.
- 0x004433F0 is vector<long>::resize(size_type); it carries invented CPODVector/CLongVector spellings.
- 0x004A9730 is the CIntelligentPointer<CThing> copy ctor; several spellings exist (const-CThing variant, invented CIntelligentPointer).
- 0x004C7990 is CThing::PeekDefinitionManager() const (PDB); landed code binds many GetDefinitionManager spellings.
- 0x006697E0 is CDefinitionManager::GetDef<CHeroCombatDef>(long, CDefPointer<const CHeroCombatDef>*) const, with a stand-in body.
- 0x00663340: reloc_bindings has ?GetPHeroCombatDef@...@?AV?$CDefPointer@VCHeroCombatDef@@@@XZ (non-const T) from caller 0x007363A0; the PDB has $$CBV.
- 0x013BA89C / 0x006870C0: the landed invented g_RegisteredTCs (CRegisteredTCMap) / LowerBound are reused unchanged by 00686f10. The real one is the static CTCBase::RegisteredTCs (CVectorMap<CCharString, CRegisteredTCInfo, CKeyPairCompareLess>). Landed GetPTCInfo spells its callee FindRegisteredTCInfo.
- 0x006F9130 (landed GetEmbeddedEventTime) is public QBE; the PDB has private ABE (this row uses ABE).
- 0x00A4EAC0 C3DAnimationStats::HasEvent: no prior binding.
- Earlier rows: 0x005937E1 (landed as an extern "C" SetMiniMapRegionExitTextOffsetX, really _Rb_tree<long,...>::find); 0x006BCC10 (invented Find_006bcc10, really std::__find<const long*, long>); 0x006B2660 (invented GetDef_006b2660, reused); 0x013B8394 g_pSoundSystem (many conflicting spellings); landed caller 0x00710B80 binds an invented CVectorShort ctor for 0x00710700; landed caller 0x00744D00 binds CGameScriptThing::GetPThing for 0x006AF520; landed callers of 0x0070C4E0 call it FindCombinationAnim.
- vland reports only WARN RELOC_NAME (catalog labels) and SHARED_HEADER_AVAILABLE; no RELOC_IDENTITY or conflict errors.
