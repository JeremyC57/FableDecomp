# NF58 round 7 notes: STLport tree / list stand-in self names (queue entry q10437; 76 self rows, 163 rows)

Every number below is counted from STATUS.tsv, APPLIED.tsv, RENAMES.tsv, NAME_FIXES.tsv, work/grades.log and recheck_final.log.

## Outcome
- **Rows (STATUS.tsv, 163):** 121 ported, 25 not-a-binder, 17 blocked, 0 deferred, 0 pending.
- **Entries (APPLIED.tsv, 76):** 61 done, 3 done-already (0x00A84110, 0x00485E45, 0x00630FE0: the self row already carried the STLport
  spelling NF50 gave it), 1 `done (NF56, landing nf56)` (0x0069D520, coordinator note; its caller 0x0069D900 is not-a-binder: the probe
  binds exactly NF56's rows/0069d520.cpp Leaf), 11 blocked.
- **Final recheck (recheck_final.log):** one vland run over all 121 changed rows through hdr/: 121 RELOCATION_MATCH, 121 tests PASS. No
  test file changed (the row tests are self-contained copies).
- **Self renames (RENAMES.tsv section 1):** 113 rows (every changed row except the 8 hinted-insert rows whose only change is the
  private `_M_insert` access). Section 2 lists 160 changed relocation bindings of the changed rows (probe of the repo copy vs the new
  compile).
- **NAME_FIXES.tsv:** 84 lines: (a) 35 landed out-of-slice binders that bind a different spelling than a slice row's own symbol;
  (b) 49 landed callee rows (outside rows.txt) whose own leaf is a stand-in for the symbol the re-authored rows now bind (mostly the
  per-instance `_M_create_node` "buyNNNN" helpers and the per-instance iterator `operator--` / `operator++` COMDATs).
- **Grades:** work/grades.log has 400 lines: 257 probe compiles of unchanged repo copies (work/probe/), 143 grades of changed or
  attempted rows (including the held-back and blocked attempts).
  The most on one row is 3 (0x00869CA0; 0x00CD34C5, 0x0085DED0, 0x00801EC0, 0x006F4D00 also 3 counting the probe).
- **EH funclets:** no row under rebuild/src/compiled includes the file of any changed row (work/eh_includers.txt is empty).
- **Locks:** NF58 holds exactly the 121 changed rows; locks on unchanged rows (not-a-binder / blocked) were released.

## How the family was ported
- **Size-TU CRetailTree pairs (`CRetailTree_{long,ulong}key_buyNNNN::Insert` + `CRetailTree_*key_insNNNN::insert`, 43 groups:
  42 from work/groups.tsv plus the hand-ported 0x0042B98B):**
  `work/port.py` writes both rows from `work/groups.tsv`. The `_M_insert` row is the explicit instantiation of stlport/fable_map.h
  `FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY` (landed 0x0042CB08 pattern; the size TU keeps `_M_create_node` out of line at the
  buyNNNN address); the insert_unique row is the explicit instantiation of the `FABLE_RETAIL_STLP_MAP` body with
  `FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ` (out-of-line `_Rb_global<bool>::_M_decrement`; landed 0x0042CEE3 pattern). Hand-proved on
  0x0042B98B / 0x0042BA13 / 0x0042BA99 / 0x0042BB21, then scripted; nearly every row graded RELOCATION_MATCH in one grade.
- **Hinted insert_unique callers (H):** 8 of them bound the public `QAE` `_M_insert` that no header declares;
  `work/privm.py` only wraps their local `_M_insert` declaration in `private:` (STLport access). The others already bound the new
  spelling (not-a-binder). 0x00CD34C5 changed its two typedefs long -> int (see REJECTED.tsv).
- **/O2 `_M_insert` rows (Insert_CRetailTree_*, CRbTree::_M_insert, ...):** `work/port2.py` (NF56's 0x0069D520 pattern, `_M_create_node`
  inline). Their stand-in insert_unique callers 0x00632990, 0x006AE9F0, 0x006DB610, 0x0070AFE0, 0x0085E140 moved onto fable_map.h's
  insert_unique with `FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE` (the per-instance `operator--` COMDAT) via `work/portH.py`; 0x00739B00 onto
  `FABLE_RETAIL_STLP_TREE_INSERT_EQUAL_BODY`; 0x00801EC0 / 0x00801CB0 onto `FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY` with
  `DEC_OUTOFLINE` and the new overlay opt-in `INC_OUTOFLINE`.
- **Instance types:** each from two sources: the Ego_r.pdb member of that exact type (work/pdb_maps.tsv, from struct_layouts_egor.tsv)
  and/or the landed callers' own STLport bindings (probe compiles, work/bindings.tsv), checked against the retail node size
  (`push NNh` in `_M_create_node`), the copied value and the key compare (`jb` unsigned / `jl` signed). Where no PDB member has the
  exact type the second source is named in the row: map<EClothingSuitPart, bool> (CTCInventoryClothing::ConstructScreen/PC call its
  operator[] 0x005BBC4E; node copies a byte value at +4), map<void*, float> (Ego_r CTCHaste::Multipliers; three landed callers bind it),
  set<CDisplayModeDesc> (CDisplayManager::EnumerateDisplayModes via 0x009C2800; comparator 0x009C1860 is landed as
  std::less<CDisplayModeDesc>::operator()), map<long, CEngineTextureDecal> (PDB name sets carry `?$map@JVCEngineTextureDecal@@`; node
  0x28), map<int, CCharString> (PDB PlayCutsceneSound(map<int,CCharString>&); map<long,CCharString> is already 0x00472A05).
- **One address, one name:** work/uniq.py checks every new Leaf against rows/, every landed Leaf and reloc_bindings (minus the old
  bindings of the changed rows), and every symbol the new compiles bind: 0 problems at hand-back.

## Entries (instance, PDB member, rows of the entry, status)
| entry | instance | Ego_r.pdb member(s) | callers in the slice | status |
|---|---|---|---|---|
| 0x0042BA99 | map<NUISystem::CManager::EUsedKeys, long> | CManager::Keys | 0042bb21 0042c1c1 | done |
| 0x0042BBA7 | map<long, EEngine2DLayer> | CManager::Layers | 0042bc2f 0042c300 | done |
| 0x004313E9 | map<unsigned long, unsigned long> | CEngineScenePrimitiveStatsEnumerator::Destination, CMorphingSprite::GraphicIndices | 00431363 005537f0 | done |
| 0x0045CA8F | map<unsigned long, long> | CSpecialEffectsStringMap::ContainmentMap, CTCInventoryStats::CreaturesKilled | 0045ca09 005d87b6 | done |
| 0x0045E0FA | map<EOpinionReactionType, bool> | COpinionReactionManagerDef::BlockFurtherReactions, COpinionReactionManagerDef::AllowSpeechOnNonPureAISpeaker | 0045e074 | done |
| 0x0045E4B4 | map<EOpinionDeedType, bool> | COpinionDeedMaskDef::DeedEnabled | 0045e42e | done |
| 0x0045EE0E | map<EOpinionAttitudeType, bool> | COpinionPersonalityDef::AttitudeEnabled | 0045ed88 | done |
| 0x0045F4FD | map<EOpinion, float> | COpinionSourceDef::ProducedOpinion | 0045f477 | done |
| 0x0046180C | map<EObjectAugmentationType, float> | CArmourDef::AugmentationResponse | 00461786 | done |
| 0x00461A40 | map<EDamageAttribute, float> | CArmourDef::DamageTypeResponse, CArmourDef::ArmourThresholds | 004619ba | done |
| 0x00463EB4 | map<EHeroTrainableStatType, long> | CHeroAbilityStatPackage::StatThresholds | 00463e2e | done |
| 0x004647B9 | map<EWorldMapNameGraphic, unsigned long> | CUILocaleGraphicsDef::WorldMapRegionName | 00464733 | done |
| 0x00465DBC | set<EWeaponClass> | CCombatAbilityBlockingData::ValidBlockWeaponTypes | 00465d36 | done |
| 0x004669BF | map<EHeroTrainableStatType, float> | CHeroAbilityStatWeight::StatWeights | 00466939 | done |
| 0x00469639 | map<long, CBlendedParticleEffectSet> | CMaterialDef::BlendedSwordHitEffects | 004695b3 | done |
| 0x0046FBC9 | map<ECombatCreatureType, long> | CCombatTypeDef::AttackPatterns | 0046fb43 | done |
| 0x00472A05 | map<long, CCharString> | CCreatureActionBase::CutSceneEvents, CGlobalDef::ConversationCameras | 0047297f | done |
| 0x004E51F6 | map<long, float> | CExperienceDef::ParticleScalesPerExperiencePoints, CExperienceDef::LifeTimeForDistanceThresholds | 004e5170 | done |
| 0x004E5425 | map<long, long> | CExperienceDef::PickupParticleEffectsPerExperiencePoints, CGameScriptInterface::ScriptTimers | 004e539f 005b2a37 | done |
| 0x004E57AC | map<EClothingSuitPart, long> | CHeroSuitDef::SuitParts, CTCInventoryClothing::CurrentClothingSelected | 004e5726 005bb5ef | done |
| 0x004E5BA7 | map<CDefString, long> | CFireEffectCreationSet::ContainmentMap | 004e5b21 | done |
| 0x004E6466 | map<EIdleStateGroup, long> | CIdleSchedulerDef::Period, CTCIdleScheduler::IdleStatesWeight | 004e63e0 00801ec0 | done |
| 0x004E699E | map<EQuakeStrength, float> | CRumbleDef::QuakeIntensities | 004e6918 | done |
| 0x004E6C39 | map<EQuakeLength, float> | CRumbleDef::QuakeDurations | 004e6bb3 | done |
| 0x004E7C3B | map<EQuestCardPrerequisite, long> | CQuestCardDef::Prerequisites | 004e7bb5 | done |
| 0x004E9299 | map<EObjectAugmentationType, CWeaponTrailGraphicSet> | CWeaponDef::WeaponTrails | 004e9213 | done |
| 0x004E94C8 | map<EObjectAugmentationType, long> | CWeaponDef::ArrowTrails | 004e9442 | done |
| 0x004EC79D | map<EObjectAugmentationType, CDefString> | CObjectAugmentationsDef::InitSounds, CObjectAugmentationsDef::LoopingSounds | 004ec717 | done |
| 0x004ECA59 | map<EObjectAugmentationType, CObjectAugmentationParticleSet> | CObjectAugmentationsDef::AugmentationParticles | 004ec9d3 | done |
| 0x004ECEE6 | map<EOpinionAttitudeType, float> | COpinionReactionManagerDef::ToleranceToBeingHit, CShopDef::AttitudeBuyPriceMultiplier | 004ece60 | done |
| 0x00511430 | set<long> | CEditWorldMap::MapsAlteredByPen, CGameTimeManager::StopTimeIndexes | 00512da0 | done |
| 0x00567AF0 | map<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity, float> | CTCHeroOnlineScoreboard::LocalScoreboard | 00567bb0 00568230 | done |
| 0x00579FD8 | map<EHeroTrainableStatType, SExpSpendUndo> | CTCInventoryExperience::UndoableStatBuys | 0057a124 | done |
| 0x0057A060 | map<EHeroAbility, SExpSpendUndo> | CTCInventoryExperience::UndoableAbilityBuys | 0057a1aa | done |
| 0x005808A0 | map<void*, float> | CTCHaste::Multipliers | 00580d21 | done |
| 0x0059B1BE | map<FRONTEND_SCREEN, NUISystem::CComponent*> | CFrontEndManager::m_mapScreens | 0059b246 0059b35c | done |
| 0x005B2337 | map<NUISystem::CComponent*, std::pair<long, long>> | CTCInventoryTrade::ItemsToBuy, CTCInventoryTrade::ItemsToSell | 005b2872 005b2efe | done |
| 0x005B23BF | map<NUISystem::CComponent*, NUISystem::CList*> | CTCInventoryTrade::SellListsByCategory, CTCInventoryTrade::BuyListsByCategory | 005b2447 005b28f8 | done |
| 0x005B24CD | map<NUISystem::CComponent*, std::pair<CWideString, long>> | CTCInventoryTrade::NumBuyableThings, CTCInventoryTrade::NumSellableThings | 005b2555 005b2b76 | done |
| 0x005B25DB | map<NUISystem::CComponent*, NUISystem::CComponent*> | CTCInventoryTrade::ItemBuyLists, CTCInventoryTrade::ItemSellLists | 005b2663 005b2cb5 | done |
| 0x005BB220 | map<EClothingSuitPart, bool> | - | 005bb2a8 005bb85a | done |
| 0x005BBCBE | map<long, CEnginePrimitiveHandle> | CHeroPostcardGenerator::PrimitiveHandles, CTCInventoryClothing::MorphPrimitiveHandles | 005bbd46 005bbe32 | done |
| 0x005F926B | map<EHeroAbility, NUISystem::CComponent*> | CTCInventoryAbilities::UnassignedIcons, CTCInventoryAbilities::AbilitiesMenuEntries | 005f92f3 005f9853 | done |
| 0x00632A30 | map<NUISystem::ETableSprites, long> | CUIDef::Sprites | 00632990 | done |
| 0x0067B320 | map<EOpinionReactionType, EOpinionAttitudeType> | COpinionReactionManagerDef::AttitudeCondition | 0067ba30 | done |
| 0x0067B3E0 | map<EOpinionReactionType, EOpinionTargetingConditionType> | COpinionReactionManagerDef::TargetingCondition | 0067baf0 | done |
| 0x0067B4A0 | map<EOpinionReactionType, float> | COpinionReactionManagerDef::PreReactionDelay | 0067bbb0 | done |
| 0x006AE900 | map<EGameAction, std::pair<C3DVector, long>> | CThingPlayerCreature::RelativeMovementAccelerationComponents | 006ae9f0 | done |
| 0x006DAFA0 | map<long, CDefString> | CHeroExperienceDef::ComboMultiplierIncrease, CHeroExperienceDef::ComboMultiplierLock | 006db450 | done |
| 0x006DB390 | map<EHeroExperienceStatCategory, float> | CHeroStatIncreaseData::StatIncreases | 006db610 | done |
| 0x006DCB20 | map<long, CComboMultiplierData> | CHeroExperienceDef::ComboMultiplierMap | 006dcdd0 | blocked |
| 0x006F1280 | map<unsigned __int64, CThingCreatureBase*> | CDefendingCombatantInfo::Attackers | 006f1ef0 | done |
| 0x006F4D00 | map<unsigned __int64, CCombatantInfo> | CCombatManager::Combatants | 006f4f30 | done |
| 0x0070AEA0 | map<long, CEngineTextureDecal> | - | 0070afe0 | done |
| 0x00739670 | map<ECombatAbilityDataType, std::pair<CCountedPointer<CCombatAbilityData>, CCombatAbility >> | - | 00739b00 | done |
| 0x00783F00 | map<unsigned __int64, CIntelligentPointer<CThing>> | CTCBoastingCrowdControl::HeroCrowdThings | 00784040 | done |
| 0x00801730 | map<EIdleStateGroup, CAIStateGroupBase*> | CTCIdleScheduler::IdleStates | 008017f0 00801cb0 | done |
| 0x0085DED0 | set<CTCMapwho*> | - | 0085e140 | done |
| 0x00869CA0 | map<CDefPointer<const CArmourDef>, float> | - | 00868e50 00869020 | blocked |
| 0x009C2350 | set<CDisplayModeDesc> | - | 009c2800 | done |
| 0x009EF750 | map<CCharString, CCountedPointer<CConsoleInputBase>> | CConsole::RegisteredConsoleInputs | 009efb80 | blocked |
| 0x00CD300B | map<int, CCharString> | - | 00cd3093 00cd34c5 | done |

The 14 entries not in the table: 0x0042B98B map<NUISystem::CComponent*, long> (Ego_r CTCInventoryClothing::ClothingItemMap /
CTCInventoryWeapons::WeaponMenuItemMap; hand port; done); 0x00A833B0 (done, below); 0x00A84110, 0x00485E45, 0x00630FE0 (done-already);
0x0069D520 (NF56); 0x00A81470, 0x00A82230, 0x006311D0, 0x00631420, 0x00817D70, 0x008B29A0, 0x008B2A60, 0x008B2B20 (blocked, below).
In the table, 0x00632A30's caller 0x00632990 was re-authored too: its old leaf used a one-parameter `_Rb_tree_iterator<V>`, while its
own caller 0x00632830 already bound the STLport spelling.

## Local definitions in changed rows (disclosed)
- **CBlendedParticleEffectSet** (0x00469639, 0x004695B3): Ego_r.pdb layout (Particle1Index, Particle1Value, Particle2Index,
  Particle2Value; 16 bytes); no shared header defines it (engine/CMaterialDef.h only forward-declares it).
- **CCompareDefString<T>** (0x004E5BA7, 0x004E5B21): PDB-named comparator, no shared header; reads CDefString::TablePos through the
  overlay opt-in FABLE_RETAIL_CDEFSTRING_COMPARE_FRIEND.
- **std::less<CDisplayModeDesc>** explicit specialisation, declaration only (0x009C2350): the out-of-line comparator 0x009C1860 (the
  0x004759AD device).
- **Forward enum declarations** (`enum E;`, the MS extension the shared headers use) where the enum's shared header is heavy or needs a
  def-model macro: EOpinionReactionType, EIdleStateGroup, EWorldMapNameGraphic (its header is a def model), EQuestCardPrerequisite (same),
  EOpinionTargetingConditionType, NUISystem::ETableSprites; forward `class` declarations for pointer values (NUISystem::CComponent,
  NUISystem::CList, CThingCreatureBase, CAIStateGroupBase, CTCMapwho, CConsoleInputBase, CCombatAbilityData). No class with a shared
  header is modelled.

## Descriptive names (NOTES duty)
- **0x00A833B0 CNavRegion::`scalar deleting destructor'** (was CCountedPointerListOwner_00a833b0): an unreferenced COMDAT; named by
  layout. The body destroys exactly a list<CCountedPointer<CNavRegionConnection> > at +4 (clear 0x00A80A20, nf41 e175) after a trivial
  word at +0, and Ego_r.pdb's only class with a member of that type is CNavRegion (RegionID +0, Connections +4). The row now deletes a
  CNavRegion through engine/CNavRegion.h (the local model is gone).
- **CCompareDefString<T>::operator()** in 0x004E5BA7 / 0x004E5B21: the class is PDB-named (`??R?$CCompareDefString@J@@QBE_NABVCDefString@@0@Z`)
  but has no shared header; the rows define it (an empty class whose operator() compares the TablePos words, which is what retail inlines).

## Blocked rows (17) and entries (11)
- **0x006311D0** (0 grades): blocked/authoring (evidence): unreferenced COMDAT (no E8/E9 caller, no 4-byte data reference anywhere in Fable.exe); 0x006311D0 and 0x00631420 are identical 66-byte map<unsigned long, CCharString> destructors (_M_erase 0x00630D90, free the header) of one instance at two addresses, so ~_Rb_tree vs an owner's implicit destructor cannot be told apart. 0 changed-row grades (1 probe)
- **0x00631420** (0 grades): blocked/authoring (evidence): unreferenced COMDAT (no E8/E9 caller, no 4-byte data reference anywhere in Fable.exe); 0x006311D0 and 0x00631420 are identical 66-byte map<unsigned long, CCharString> destructors (_M_erase 0x00630D90, free the header) of one instance at two addresses, so ~_Rb_tree vs an owner's implicit destructor cannot be told apart. 0 changed-row grades (1 probe)
- **0x006DCB20** (1 grades): blocked/port: map<long, CComboMultiplierData> _M_insert (Ego_r CHeroExperienceDef::ComboMultiplierMap; H 0x006DCDD0 binds the STLport spelling). Retail calls this instance's _M_create_node out of line in the right arm (0x004F9763, landed as the stand-in CRetailList_node0x28::Buynode) but expands it in the left arm (malloc + the map<EDamageAttribute,float> copy constructor 0x004F97B3, landed as NPairLongMapTree::_Rb_tree::_Rb_tree); the shared ORIGINAL_INSERT_BODY expands both (1 grade: DIFFER 227v198). Needs those two callee rows named first (_M_create_node and _Rb_tree copy ctor of the instances) and an asymmetric-inline device; the NF50 0x0076A110 mixed shape is the same open problem
- **0x00817D70** (0 grades): blocked/authoring (evidence): /O2 _M_insert of the global handler map at 0x013BAF2C filled only by RegisterThingHandlers 0x008162F0 (four (id, CThing member-function handler) pairs, via insert_unique 0x00818000). Neither PDB names the map or the handler type (no member, no name-set entry), so the instance's template arguments (key long? value a pointer-to-member type?) cannot be proven from two sources; 0 changed-row grades (1 probe). Needs the handler map's declared type
- **0x00818000** (0 grades): blocked with 0x00817D70 (same unproven global handler map 0x013BAF2C); binds the stand-in CRetailTree_00818000::Insert, not the self row's current name, so nothing is split
- **0x00869020** (0 grades): blocked/port: CHitParameters::SetAsModifiedByArmour (955 bytes) models nine classes locally (CArmourDef, CDefPointer<const CArmourDef> as CArmourDefPointer, the multimap<CDefPointer<const CArmourDef>, float, CCompareDefPointer<..> > as CArmourMap with its node / pair / iterator, map<EDamageAttribute,float> as CDamageMap, vector<EObjectAugmentationType> as CAugmentationArray, CTCWeapon, CThing, CBaseIntelligentPointer, CHitParameters), so its _M_insert call binds the stand-in ?_M_insert@CArmourMap@@... These must move onto engine/CArmourDef.h, engine/CDefPointer.h, engine/CCompareDefPointer.h, stlport/fable_map.h, engine/CThing.h and engine/CHitParameters.h first (well over 10 grades). Its callee 0x00869CA0 is held back with it (ready re-author in work/ready/). 0 changed-row grades (1 probe)
- **0x00869CA0** (3 grades): blocked/port with its in-slice caller 0x00869020 (which models nine classes locally, see its line). A ready re-author is in work/ready/00869ca0.cpp: the explicit instantiation of fable_map.h ORIGINAL_INSERT_BODY for multimap<CDefPointer<const CArmourDef>, float, CCompareDefPointer<const CArmourDef, float> > (Ego_r CArmourDef / the landed 0x00868E50 GetArmourWeightsFromThing binds exactly that spelling), RELOCATION_MATCH in 3 grades; it is held back because 0x00869020 cannot move with it in this pass
- **0x008B29A0** (0 grades): blocked/authoring (evidence): /O2 _M_insert of the global int -> handler map at 0x013BAFA8, filled only by the propagated-label row 0x008B08B0 (catalog CScreenEffectPriorityStack::Process; the body registers handlers). No PDB member or name-set entry gives the map type (Ego_r CScreenEffectPriorityStack holds only set<CStackEntry*> and lists, 4-byte unsigned nodes, while this tree has 8-byte values and signed keys), so the template arguments are unproven; 0 changed-row grades (1 probe)
- **0x008B2A60** (0 grades): blocked/authoring (evidence): /O2 _M_insert of the global int -> handler map at 0x013BAFB4, filled only by the propagated-label row 0x008B08B0 (catalog CScreenEffectPriorityStack::Process; the body registers handlers). No PDB member or name-set entry gives the map type (Ego_r CScreenEffectPriorityStack holds only set<CStackEntry*> and lists, 4-byte unsigned nodes, while this tree has 8-byte values and signed keys), so the template arguments are unproven; 0 changed-row grades (1 probe)
- **0x008B2B20** (0 grades): blocked/authoring (evidence): /O2 _M_insert of the global int -> handler map at 0x013BAFC0, filled only by the propagated-label row 0x008B08B0 (catalog CScreenEffectPriorityStack::Process; the body registers handlers). No PDB member or name-set entry gives the map type (Ego_r CScreenEffectPriorityStack holds only set<CStackEntry*> and lists, 4-byte unsigned nodes, while this tree has 8-byte values and signed keys), so the template arguments are unproven; 0 changed-row grades (1 probe)
- **0x008B2DD0** (0 grades): blocked with its callee 0x008B29A0 (same unproven global map 0x013BAFA8); binds a CRetailTree stand-in that is not the self row's current name
- **0x008B2E90** (0 grades): blocked with its callee 0x008B2A60 (same unproven global map 0x013BAFB4); binds a CRetailTree stand-in that is not the self row's current name
- **0x008B2F50** (0 grades): blocked with its callee 0x008B2B20 (same unproven global map 0x013BAFC0); binds a CRetailTree stand-in that is not the self row's current name
- **0x009EF750** (1 grades): blocked/port with its in-slice caller 0x009EFB80 (needs a fable_string.h opt-in, see its line). A ready re-author is in work/ready/009ef750.cpp: the explicit instantiation of fable_map.h ORIGINAL_INSERT_BODY for map<CCharString, CCountedPointer<CConsoleInputBase> > (Ego_r CConsole::RegisteredConsoleInputs), RELOCATION_MATCH in 1 grade; held back because its only caller cannot move with it in this pass
- **0x009EFB80** (1 grades): blocked/port: insert_unique of map<CCharString, CCountedPointer<CConsoleInputBase> > (Ego_r CConsole::RegisteredConsoleInputs). On fable_map.h (STLP_MAP + STLP_TREE_DEC_OUTOFLINE) VC7.1 reloads v's key around each out-of-line CBasicString<char>::operator< 0x00429950 call and merges the two _M_insert tails (1 grade: DIFFER 202v254); retail keeps the key in ebp, which needs that operator's body visible but not inlined (the FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE device). fable_string.h is a root #pragma once header that cannot be overlaid (NF41 lesson): it needs a FABLE_RETAIL_CBASICSTRING_LESS_NOINLINE opt-in in the repo header first. Its callee 0x009EF750 is held back with it (ready re-author in work/ready/)
- **0x00A81470** (0 grades): blocked/authoring (evidence): unreferenced COMDAT (no E8/E9 caller, no 4-byte data reference anywhere in Fable.exe); 0x00A81470 and 0x00A82230 are identical 25-byte list<CCountedPointer<CNavRegionConnection> > destructors (clear 0x00A80A20, free the sentinel) of one instance at two addresses, so ~_List_base vs ~list vs an owner whose first member is the list cannot be told apart. 0 changed-row grades (1 probe). Needs a caller or an EH/unwind reference to settle
- **0x00A82230** (0 grades): blocked/authoring (evidence): unreferenced COMDAT (no E8/E9 caller, no 4-byte data reference anywhere in Fable.exe); 0x00A81470 and 0x00A82230 are identical 25-byte list<CCountedPointer<CNavRegionConnection> > destructors (clear 0x00A80A20, free the sentinel) of one instance at two addresses, so ~_List_base vs ~list vs an owner whose first member is the list cannot be told apart. 0 changed-row grades (1 probe). Needs a caller or an EH/unwind reference to settle

## Not-a-binder (25)
- 0x0042C1C1: repo copy already binds the new _M_insert (AAE) and insert_unique spellings exactly (probe compile)
- 0x0042C300: repo copy already binds the new _M_insert and insert_unique spellings exactly (probe compile)
- 0x00485E45: self symbol is already the STLport spelling (NF50 rename, probe compile confirms); no change. Its out-of-slice callers still bind stand-ins: NAME_FIXES
- 0x00512DA0: repo copy already binds the new _M_insert spelling exactly (probe compile)
- 0x00567BB0: repo copy already binds the new _M_insert spelling exactly (probe compile)
- 0x0059B35C: repo copy already binds the new _M_insert and insert_unique spellings exactly (probe compile)
- 0x005B2A37: repo copy already binds the new _M_insert and insert_unique spellings exactly (probe compile)
- 0x005BB5EF: repo copy already binds the new _M_insert and insert_unique spellings exactly (probe compile)
- 0x005BB85A: repo copy already binds the new _M_insert and insert_unique spellings exactly (probe compile)
- 0x005BBE32: repo copy already binds the new _M_insert and insert_unique spellings exactly (probe compile)
- 0x005F9853: repo copy already binds the new _M_insert and insert_unique spellings exactly (probe compile)
- 0x00630FE0: self symbol is already the STLport spelling (NF50 rename, probe compile confirms); no change. Its out-of-slice callers still bind stand-ins: NAME_FIXES
- 0x0067BA30: repo copy already binds the new _M_insert spelling exactly (probe compile)
- 0x0067BAF0: repo copy already binds the new _M_insert spelling exactly (probe compile)
- 0x0067BBB0: repo copy already binds the new _M_insert spelling exactly (probe compile)
- 0x0069D900: repo copy already binds exactly the STLport _M_insert leaf NF56's re-authored 0x0069D520 defines (probe compile vs NF56 rows/0069d520.cpp Leaf); entry 0x0069D520 is NF56's (coordinator note)
- 0x006DB450: repo copy already binds the new _M_insert spelling exactly (probe compile)
- 0x006DCDD0: repo copy already binds the STLport _M_insert spelling of map<long, CComboMultiplierData> (probe compile); no change needed (its callee 0x006DCB20 is blocked)
- 0x006F1EF0: repo copy already binds the new _M_insert spelling exactly (probe compile)
- 0x006F4F30: repo copy already binds the new _M_insert spelling exactly (probe compile)
- 0x00784040: repo copy already binds the new _M_insert spelling exactly (probe compile)
- 0x008017F0: repo copy already binds the new _M_insert spelling exactly (probe compile)
- 0x00868E50: repo copy already binds the STLport _M_insert spelling of multimap<CDefPointer<const CArmourDef>, float, CCompareDefPointer<..> > (probe compile); no change needed. Its callee 0x00869CA0 is blocked (ready re-author in work/ready/), so no landed row defines that spelling yet
- 0x009C2800: repo copy already binds the new _M_insert spelling exactly (probe compile)
- 0x00A84110: self symbol is already the STLport spelling (NF50 rename, probe compile confirms); no change. Its out-of-slice callers still bind stand-ins: NAME_FIXES

## For the coordinator
- **Held back (work/ready/):** 0x00869CA0 (multimap<CDefPointer<const CArmourDef>, float, CCompareDefPointer<..> >::_M_insert, 3 grades)
  and 0x009EF750 (map<CCharString, CCountedPointer<CConsoleInputBase> >::_M_insert, 1 grade) graded RELOCATION_MATCH as explicit
  instantiations, but their in-slice callers 0x00869020 / 0x009EFB80 cannot move in this pass, so both stay blocked/port (no self
  rename without its callers). Each can land together with its caller's port; neither caller binds the row's current stand-in name.
- **File moves:** the 113 section-1 rows change name; their sources (and tests, whose content is unchanged) move with the new catalog
  names. No EH funclet includes any of them.
- **NAME_FIXES.tsv (a):** none of these out-of-slice binders bound the old stand-in self spelling (they already bound something else:
  a map::insert / CUndoMap / CKillMap / CPauseSet ... stand-in), so the renames split nothing new; respelling them joins them up.
  Includes NF50's leftovers 0x0047E07F (-> 0x00485E45) and 0x00A7A7F0 / 0x00A84640 / 0x00A84BE0 (-> 0x00A84110).
- **NAME_FIXES.tsv (b):** the landed `_M_create_node` rows carry `QAE` where STLport has `IAE` (protected) even when their template
  arguments are right (0x00466255, 0x004EC441); the rest are stand-ins (BinarySearchTreeNode_Create, TreeNode_AllocData, Buynode,
  CRetailTreeIterator::Increment / Decrement, ...).
- **engine/CWeaponDef.h:** `struct CWeaponTrailGraphicSet` is `class` in both PDBs; the overlay adds an opt-in, the default is unchanged.
- **0x006DCB20 / 0x0076A110 shape:** an `_M_insert` that expands `_M_create_node` in one arm and calls it in the other cannot be
  produced from the shared header body; both rows wait for a device.

## Rule slips (work/slips.txt)
- 1. cmpb.py on group 1 before its attempts/*.rel existed: FileNotFoundError, the Bash call exited 1.
- 2. a trailing grep -m1 Leaf: on a file with no Leaf line exited 1 (buy-row check).
- 3. portH.py failed on 0x006AE9F0 / 0x006DB610 (regex), and the chained q.py grades ran on the missing rows (2 wasted grade log lines, both rows).
