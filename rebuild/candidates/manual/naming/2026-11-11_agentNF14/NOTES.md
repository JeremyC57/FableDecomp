# agentNF14 notes (w22, 2026-11-11)

62 rows changed, all **RELOCATION_MATCH** in a final regrade (overlay on, `--all` bindings, catalog PassPatterns kept). VC7.1 /Od
tests pass on all 62. The g++ tests pass on 41; the other 21 are **PREFAIL** (listed below). No row went over 4 grades plus the
final regrade. Nothing in BUSY.txt or agentH40 was touched. Overlay: `hdr/` (11 headers, additions only, see `hdr/README.md`).

## Item 1: GetDef<CThingBaseDef> 0x0042AF3C / CTCBase::GetDefinitionManager 0x006869D0 (48 of 48 moved)

Every row now calls the def lookups `const` through `GetDefinitionManager()` on a CTCBase base. The repo has two CTCBase models:

- The shared `engine/CTCBase.h` (`FABLE_RETAIL_CTCBASE`, which needs `engine/ETCInterfaceType.h` first), used where the
  component's own header builds on it.
- A local `class CTCBase { virtual ~CTCBase(); CThing* ParentThing; protected: GetDefinitionManager() const; GetWorld() const; }`
  plus `class CGameDefinitionManager : public CDefinitionManager {}`. This follows NF13's landed rows (007498e0 and others) for
  classes whose header is still flat. It is not a pad struct, and it binds the same Ego_r symbols.

The def lookups are modelled as `__declspec(noinline)` explicit specialisations of the const member templates:
`GetDef<T>(long, CDefPointer<const T>*) const` and `GetSubDef<T>(CDefPointer<const T>*) const`. They have no stand-in bodies.
`CDefPointer<const CDefClassBase>` is specialised for the return of GetPDefFromGlobalIndex / GetPDefFromInstantiationName.

Callee identities proven on the way are in NAME_FIXES.tsv (95 addresses). The main ones:
- GetPDefFromGlobalIndex has two byte-identical overloads, QBE 0x009AD9E0 and IAE 0x009ADA10. The IAE one is non-const and
  protected, and 00811470 / 008115a0 bind it.
- GetSubDef<CTattooDef> 0x005B1EFC, <CStockItemDef> 0x0059350C, <CShopDef> 0x005130A0, <CExpressionSubDef> 0x005DA240,
  <CIdleSchedulerDef> 0x00801670, <CCreatureDef> 0x00485815.
- GetDef<CExpressionDef> 0x005F81BE.
- NResourceDirectoryNames::GetTattooUserDir 0x0041A650 and GetTattooDataDir 0x0041A6A0, proved by the strings they return.
- CShopDef::Test 0x007773A0 and CTCActionUseBase::OnActionUse 0x007EAB90.

### Descriptive (made-up) names used
These have no PDB, string, FSE or EgoCore name. Each is used for one address and is marked descriptive in its row:
- `MoralityChangeOnTheftFromHome` (007ee8f0): CHeroStatsDef +0xE0, the theft delta. Ego_r has several candidate deltas, so this
  is a descriptive pick.
- `PHeroStatsDef` (0057d1a4, 007ee8f0): the CGameDefinitionManager +0xDC member.
- `CStockMap::operator[]` (007744a0): left as landed (STLport map instance, invented spelling).
- `CShopItemDefVector` (00772250, 007744a0) / `CThingDefVector` (0071da70): local STLport vector<> models.
- `InMapwho` bit and the slot-96 `UpdateOldPosition` (both 006b0c10): kept as landed, marked descriptive in the row.

### STL and callee spellings left as landed
These are outside this brief's def-system scope. The verifier warns on them (`WARN RELOC_NAME`):
- 004c9560: `CThingManager::OnThingCreated` -> 0x0051E2E0; `CComponentVector` copy ctor -> 0x004CC290;
  `CThing::RegisterWithManager` -> 0x00A36F80.
- 004ca010: `CThingManager::GetThingTypeFlags` -> 0x0051E580.
- 005a30a1: `CTCInventoryTrade::GetShopThing` -> 0x005BC4C4; `CTCShop::GetStockMap` / `CLongMap` dtor -> 0x00774610 / 0x004DD2B1.
- 005b43a2:
  - `CLongVector::push_back_005bba94` -> 0x005BBA94. This is an address-suffixed spelling. The retail name is
    `std::vector<EClothingSuitPart>::push_back` (FableWin 0x01E30907; landed 005b76d1 binds it), and it is in NAME_FIXES.
    Moving the row needs a vector<EClothingSuitPart> / std::find / map-iterator remodel.
  - `FindLong` -> 0x005BA7F9; `Successor` -> 0x004292C0; `list<long>::erase` -> 0x00429B02; `SActionParamPair` ctors.
- 005bdf08 / 005bdfb1: `NInventory::FindCategoryWithDefIndex` / `find_if` -> 0x005C08C0 / 0x005C08D3.
- 005bec92: `IsAugmentation` / `GetMaxNumberOfItems` / `CPerItemData` ctor.
- 005bf77d: `CTCInterface1A::Notify` -> 0x005D8E50.
- 005bf9e3: `CCountedPointer<vector<..>>::ReleaseData` -> 0x005C0511 / 0x005C0563.
- 00659164: `CTCRenownRatio::CalculateRatio` -> 0x005CDBBE.
- 007363a0 / 007366c0: `CDefPointerVector::_M_insert_overflow`, `__destroy_aux`, `__find`, `remove_copy`, `__copy`;
  `CThingCreatureBase::GetCombatTypeIndex` -> 0x00662170.
- 007be0f9: `CHitParameters::SetHitter` -> 0x008685F0; `CTCPhysicalShieldSpell::FadeOutEffect` -> 0x0072AC40.

## Item 2: rows whose own name was wrong (9 of 11, plus 1 bonus)

| row | new symbol | proof |
|---|---|---|
| 006585ee | `?NotifyHasApologised@CTCVillage@@QAEXXZ` | Ego_r name; donor order NotifyHasApologised / CreateDummyVillagersIfNecessary / GetIsHeroEnemyOfGuards = retail 0x006585EE / 0x00658603 (`ret`) / 0x00658604; stores the frame to LastFrameApologised +0xD8 |
| 00658604 | `?GetIsHeroEnemyOfGuards@CTCVillage@@QAE_NXZ` | Ego_r name + catalog; frame < HeroEnemyOfGuardsUntil +0xD4 (`// Flags: /Os`, the CTCVillage TU) |
| 006cb8f0 | `?IsCurrentlyNavigating@CTCCreatureNavigation@@QBE_NXZ` | Ego_r name + catalog + donor order; FrameLastNavigated test |
| 006ceae0 | `?ResetLastOpinionReactionFrame@CTCCreatureOpinionOfHero@@QAEXXZ` | FableWin name; sole caller 0x0092FFF8 calls it on the TCI 79 component |
| 007236e0 | `?ResetInterpolation@CTCPhysicsStandard@@UAEXXZ` | CTCPhysicsStandard vtable 0x01265F84 slot 80 (RTTI) + catalog; tail-calls the base slot |
| 00811450 | `?FrameUpdate@CTCEntityEvents@@UAEXXZ` | Ego_r / FableWin names; callee 0x00812030 = FableWin `remove_if<CPred_EventExpired>` |
| 006af620 | `?GetWorldMap@CTCPhysicsBase@@IAEAAVCWorldMap@@XZ` | Ego_r donor order (0x006EC300) + CWorld slot 12; was CThingTrackNode::DrawIsDrawable |
| 006af630 (bonus) | `?GetWorldMap@CTCPhysicsBase@@IBEABVCWorldMap@@XZ` | the const twin (0x006EC310), CWorld slot 13 PeekWorldMap; was `_unattributed::DisplayEnginePredicate13` |
| 008957c0 | `?SendEntityEvent@CGameScriptInterface@@UBEXW4EEventType@NEntityEvents@@ABVCScriptThing@@1PAVCThing@@@Z` | CGameScriptInterface vtable 0x01260F0C slot 241 (RTTI) + Ego_r; the three CEventHeroHasBeenSpotted* ctors from NF13 |
| 006c8780 | `?TransferBinaryIn@?$CPersistTraits@V?$CIntelligentPointer@VCThing@@@@@@QAEXAAVCMemoryDataInputStream@@AAV?$CIntelligentPointer@VCThing@@@@@Z` | Ego_r 0x006FF6C0 / FableWin 0x01F373C0. Rebuilt on the shared headers (no X_006c8780 namespace, no stand-in bodies); new portable test |
| 0071c360 | `?InitialiseCustomTattoos@CTCHeroMorph@@IBEXXZ` | (item-1 row) the invented class CTCHeroMorph_Retail dropped |

New tests were written for 006585ee, 00658604, 006cb8f0, 006ceae0, 007236e0, 00811450, 006af620, 006af630, 006c8780 and
007e2790. Each keeps its catalog PassPattern.

## Item 3: GetWorld binders on a header base (4 of 4)
- 0065c1fc: built on the retail CTCVillage (`_CRIMES`), the shared CTCHeroOpinionDeedLog (`_DEEDS`), COpinionDeed and
  `CDefPointer` RELEASE_COUNTREF. CWorld::PeekMyPlayer 0x0049D9D0 replaces GetCurrentPlayer, and PeekPDeedEffectsDef is 0x006C5630.
  The local CThing / CPlayer / CMainGameComponent projections are kept as landed.
- 006b0c10: SetPosition is virtual (CTCPhysicsBase vtable 0x0125D2C4 slot 30), so the self symbol is now UAE. Local CTCBase base.
- 007be0f9: OnCollision is protected virtual (MAE) taking ECollideType; the base CTCShotDefinitionBase::OnCollision is UAE 0x007BF510.
- 007e2790:
  - Rebased on the shared CTCBase (`_COMPONENT`). The address-suffixed CPlayerRecord_ / CPlayerManager_ / CWorld_007e2790 are
    replaced by the real inline accessors:
    - `PeekComponent().PeekDisplayEngine().IsScreenFadeOut()`
    - `CPlayerGui::Get().GetSubtitleRenderer().IsDisplayingText()` and `.IsInCutscene()`
    - `GetWorld().IsPerformingRegionLoad()`
  - The old reading of +0x28/+0xBC as a "player manager / busy" flag was wrong. +0x28 is CMainGameComponent::PDisplayEngine, and
    +0xBC is CDisplayEngine::ScreenFadeOutInfo.Active.

## PREFAIL (the landed test already fails under g++ 64-bit; tests unchanged, still PREFAIL)
004c7f40, 00591a66, 0059c983, 005b3c95, 005b3d7a, 005b413d, 005b4795, 005b5be6, 005bdf08, 005bdfb1, 005bec92, 005c3180 (builds
but prints FAIL on the landed source too), 005d66ba, 006700f0, 006b0780, 006bf9e0, 00706290, 00706510, 007363a0, 007366c0,
00801a50.

## Left
- **006a23e0** `?ResetAllCarriedThingsAsBypassFilter@CTCCarrying@@QAEXXZ` (FableWin 0x006C70B0). The own name is right; it is
  still in `NRemap006a23e0` with stand-in bodies. Moving it is a full remodel:
  - The element type is `CScreenFilterSThingByPass { CScriptThing thing; bool bypassSet; }` (Ego_r layout; 9 bytes packed in
    retail, `add 9` strides).
  - The vector is `std::vector<CScreenFilterSThingByPass>` with `_M_insert_overflow` 0x006A3950.
  - GetAllCarriedThings 0x006A2220 is FableWin `?GetAllCarriedThings@CTCCarrying@@QBEXAAV?$vector@PAVCThing@@..@std@@@Z`.
  - The world chain is GetWorld -> CWorld +0x18 PEnvironment -> CEnvironment +0x1964 -> 0x006B83E0, whose PDB name I did not
    prove (catalog: CEnvironmentSpellEffectInterpolator::ScreenFilterSetFlags).
  - The key is CTCCarrying +0x20 (Ego_r `void* Key`, read through GetAllCarriedThingsBypassFilterKey?).
  - This is too deep for the remaining budget.
- **008af850** CombatDialogueManager::PlayBattleCry. The own name is right; it is still in `ExternalRetry_008af850` with stand-in
  CCharString / CDefStringTable / CASoundBank / SymbolMap bodies. Moving it needs the shared CDefStringTable::GetString,
  CASoundBank::GetSoundIndexFromSymbol and CSymbolMap lookup headers (314 bytes). Not attempted for budget.
- 005b43a2's `CLongVector::push_back_005bba94` (see above).
