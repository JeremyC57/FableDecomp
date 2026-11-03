# Codex pass 11 consolidation 2 overlay (cons11b/hdr, retry11a merged)

Overlay for `rebuild/include` at the current `main` (d462c662a + da932e8cc). Grade with `FABLE_HDR_OVERLAY=<this dir>`.
It serves the 11 rows in `../payload.json`: the 8 cons11b rows, the 2 retry11a rows folded in by the coordinator
(0x00520670, 0x00521620), and the respelled landed row 0x0052B030. Every row is RELOCATION_MATCH with tests PASS under vland
(`../vland_payload.log`, `../vland_0052b030.log`).

It holds no copy of `fable_string.h`, `fable_containers.h`, `fable_definition_table.h`, `C2DVector.h` or `NUISystem_CUIDef.h`.

## Default-path proof

- `python3 $S/w22/hdrcheck.py <this dir>` from the repo root gives `default=same` for every changed header (`../hdrcheck.txt`).
- That script runs `cpp` on the header alone, and some headers stop at an `#error`/missing include on both sides. So
  `../tools/hdrcheck2.py` reruns it, reporting cpp errors and line counts (`../hdrcheck2.txt`).
- The headers it flags were checked textually against the repo file (`diff`):
  - `engine/CArray.h`: the old one-argument ctor is kept as the `#else` of a new `FABLE_RETAIL_CARRAY` block.
  - `stlport/fable_map.h`: lines appended at the end, nothing else touched.
  - `engine/CDefPointer.h` and `engine/CThing.h`: no default path. Both headers are reached only through
    `FABLE_RETAIL_DEF_CLASS_BASE`, and CThing.h defines that macro itself.
- Repo `FABLE_RETAIL_*` branches whose text changes:
  - `FABLE_RETAIL_DEF_CLASS_BASE`: CDefPointer.h and CThing.h.
  - `FABLE_RETAIL_CTCBASE`, `FABLE_RETAIL_CWORLD`, `FABLE_RETAIL_PLAYER`: additive.
  - `FABLE_RETAIL_CDATAINPUTSTREAM`, `FABLE_RETAIL_CPERSISTCONTEXT`: retry11a's changes.
- Every landed includer of those branches, and of `stlport/fable_map.h`, `fable_game.h` and `boost/scoped_ptr.h`, was
  rechecked under this overlay (see `../NOTES.md`, "Includer recheck").
- No landed source defines any macro that is new here (`../tmp/macro_definers.txt`, from grepping `rebuild/src`).

## Existing repo headers, new opt-in branch (the `#else` is the repo text)

| Header | Macro | Retail model | Evidence / source |
|---|---|---|---|
| engine/CResourceList.h | FABLE_RETAIL_CRESOURCELIST | `CResourceList : CFailedAllocationHandler`, CResource list head, sizeof 0x3C | Codex pass 11 model; base of CDefinitionManager |
| engine/CDefinitionManager.h | FABLE_RETAIL_CDEFINITIONMANAGER | `: CResourceList`, friend CDefClassBase, name/index lookups, template `GetDef(long, CDefPointer<const T>*)`, opaque members to 0xD0 | Codex model; 00775c60 / 0047a9f0 / 00667450 / 00667d30 call sites |
| engine/CTCPhysicsBase.h | FABLE_RETAIL_CTCPHYSICSBASE | `: CTCBase`, PDB virtual list, Position / OldPosition / ..., inline GetPosition / GetRadius / GetPhysicsMeshObject, sizeof 0x40 | Codex model; 00666ef0 / 0074a5d0 / 00667d30 |
| engine/CThingSearchTools.h | FABLE_RETAIL_CTHINGSEARCHTOOLS | Reference members, protected PeekTypeList, GetAllLeafThings, GetAllThingsInRoughArea, `template GetPFirstLeafThing` (noinline body) | Codex model; the template body is Ego_r 0x005457E0 (the comment is in the header) |
| engine/CWorldMap.h | FABLE_RETAIL_CWORLDMAP | **Merged with retry11a.** `: CBaseClassNonCopyable, CIDrawWorldMap`, Ego_r TPI 0x27B6E virtual order, named members +0x08..+0xD5 (retry11a), GetMapUID `QBEKJ` (retry11a), and cons11b's GetRegionNumberFromName / GetRegion / IsPosInMap / IsPosPassable / GetGroundSizeZAt. sizeof 0xD8 | Rows 0x00521620, 0x00606300, 0x0074A5D0 |
| engine/CThingPhysical.h, engine/CThingGameObject.h, engine/CThingCreatureBase.h | FABLE_RETAIL_CTHINGPHYSICAL / _CTHINGGAMEOBJECT / _CTHINGCREATUREBASE | The retail CThing chain. CThingCreatureBase::PeekDefinition is defined inline at the tail when engine/CThingCreatureDef.h is also included (mutual-tail pattern). | Codex model; FableWin 0x01D6DD10 (PeekDefinition) |
| engine/CPlayerManager.h | FABLE_RETAIL_CPLAYERMANAGER | `: CBaseClassNonCopyable`, ctor, IsThingMainPlayer, GetPlayer x2, GetMainPlayer x2, GetMainPlayerNumber, named private members Component..HeroSwapPlayerScriptNames, sizeof 0x2C | Hand-written from the default branch's addresses plus the PDB member list |
| engine/CRegion.h | FABLE_RETAIL_CREGION | The default members with their real types (CCharString, C2DVector), and inline GetName | FableWin |
| engine/CClassFactory.h | FABLE_RETAIL_CCLASSFACTORY | **Merged with retry11a.** `: CBaseClassNonCopyable`, opaque 4-aligned ClassTypeInfo / ClassList (retry11a's `unsigned long[]`), sizeof 0x1C | Base of CThingManager |
| engine/CTCCombat.h | FABLE_RETAIL_CTCCOMBAT | One model for 0x007355F0 and 0x00667D30: StatGetInterfaceType, IsAbleToBeKnockedDown, SetCombatType, full members incl. `multimap CombatAbilities`, sizeof 0x98 | Ego_r TPI 0x25A80 |
| engine/CCombatTypeDef.h, CCombatAbility.h, CCombatAbilityData.h, CCombatAbilityBlockingData.h, CCombatAbilityMeleeAttackData.h, CCombatAbilityStrafingData.h, CCombatAbilityProjectileAttackData.h, CCombatAbilityAttackBase.h, CCombatAbilityBlockCounterAttackDef.h, CCombatAbilityFlourishCounterAttackDef.h, CCombatAbilityStrafeDef.h | FABLE_RETAIL_<CLASS> each | Codex row 0x007355F0 models | Codex NAMING_EVIDENCE for 0x007355F0 (cited in each header) |
| engine/CCreatureActionBase.h, CCombatAction_GenericResponseToDecapitation.h, CCombatAction_GenericResponseToExplosion.h, CCombatAction_GenericStrikeResponseKnockdown.h | FABLE_RETAIL_CCREATUREACTIONBASE / _CCOMBATACTIONGENERICRESPONSETODECAPITATION / _CCOMBATACTIONGENERICRESPONSETOEXPLOSION / _CCOMBATACTIONGENERICSTRIKERESPONSEKNOCKDOWN | Codex row 0x00667450 models | Row 0x00667450 |
| engine/CPlayerGui.h | FABLE_RETAIL_CPLAYERGUI | `: CTBaseSingletonNoDefaultAlloc<CPlayerGui>`, opaque to 0x338, AddHitByMelee | TPI 0xBF85; 0x00437990 |
| engine/CTCBulletTime.h | FABLE_RETAIL_CTCBULLETTIME | `: CTCBase`, opaque to 0x58, StatGetInterfaceType, PlayCreatureOnHitEffect | TPI 0x2F400 |
| engine/CTCHeroExperience.h | FABLE_RETAIL_CTCHEROEXPERIENCE | `: CTCBase`, PDB virtuals and members, PeekHeroCombatDef, AddTrainableStatExperience, sizeof 0x60 | Codex union model |
| boost/scoped_ptr.h | FABLE_RETAIL_SCOPED_PTR | **retry11a's branch (adopted).** Boost 1.30 `scoped_ptr` with inline `operator*`, `operator->`, `get`. This replaces cons11b's earlier ungated accessor edit, so the default is the repo text again. | Retail reads the pointer through them (CMainGameComponent::PeekWorld, GetPlayerManager) |
| engine/CACompressor.h, CCompressorZlib.h, CDataOutputStream.h, CMemoryDataOutputStream.h, CArray.h | FABLE_RETAIL_CACOMPRESSOR / _CCOMPRESSORZLIB / _CDATAOUTPUTSTREAM / _CMEMORYDATAOUTPUTSTREAM / _CARRAY | retry11a, unchanged | `$S/w22/retry11a/hdr/README.md` |

## Existing repo branches extended

| Header | Macro | Change | Evidence |
|---|---|---|---|
| engine/CTCBase.h | FABLE_RETAIL_CTCBASE | Adds a forward `class CGameDefinitionManager;` and three accessors: `CThingManager& GetThingManager()`, `const CGameDefinitionManager& GetDefinitionManager() const` and `CWorldMap& GetWorldMap()`. Declarations only. | 0x00686AE0 / 0x006869D0 / 0x00686D30 (Ego_r / FableWin spellings in the header comment) |
| engine/CWorld.h | FABLE_RETAIL_CWORLD | Inline `GetThingSearchTools()`, `PeekThingSearchTools() const`, `IsSerialisingHeroState() const` | FableWin 0x018C3A00 / 0x018CE450 / 0x01B3AB20 |
| engine/CPlayer.h | FABLE_RETAIL_PLAYER | Inline `CHeroLogBook& GetPlayerHeroLogBook() const` | FableWin 0x0187F0C0 |
| engine/CDataInputStream.h | FABLE_RETAIL_CDATAINPUTSTREAM | retry11a: the destructor is compiler-generated | `~CDataStream` 0x00993B10 called directly |
| engine/CPersistContext.h | FABLE_RETAIL_CPERSISTCONTEXT | retry11a: save-side ctor and inline GetPSaveStream | 0x009BAE50 |
| engine/CDefPointer.h | (DEF_CLASS_BASE-only header) | (1) The copy `operator=` reads `other.Object` again after Release. (2) `Release()` decrements the pointee's count in place: `--object->IVRefCount == 0` then Delete. The local `object` is kept. | (1) 0x007355F0 `PCombatTypeDef=pcombat_type` (`cmp ecx,[ebp]`, release, reload `[ebp]`). (2) 0x00667450 releases with `dec dword ptr [ecx+4]; jne`, while `object->Release()==0` emits `mov/dec/mov/test/mov`. Every landed DEF_CLASS_BASE row was rechecked. |
| engine/CThing.h | (retail-only header) | (1) `GetTC<T>` / `PeekTC<T>` / `PeekTC<T>(id)` are `__forceinline`. (2) The `GetTC<T>` body is direct: HasTC, then `Components.Find`. | (1) Owner decision 2026-11-01 (EXTRA_RULES): Ego_r has 0 `??$GetTC@`/`??$PeekTC@`, FableWin 267/159. Without it 0x00667450 calls them (1625v1755). (2) 0x00667D30 tests the bit in place (`test ah,ah; jns`); the old `const bool& present` form stores it to the stack (1232v1210). 0x00667450 matches with either. |
| stlport/fable_map.h | (STLport model) | Appended `_Identity`, `set` (insert, erase), `multimap` (insert→insert_equal, clear). | STLport 4.6.2. CTCCombat::CombatAbilities is a multimap and CCombatAbilityBlockingData::ValidBlockWeaponTypes a set (0x007355F0). This is the only set/multimap model; fable_containers.h has none. |
| fable_game.h | FABLE_RETAIL_CGAMECOMPONENT, FABLE_RETAIL_CMAINGAMECOMPONENT (new) | See the next section. | |

## fable_game.h: the one CGameComponent / CMainGameComponent model

Both classes keep their home in `fable_game.h`. Their old stand-ins stay byte for byte as the `#else` branches.
No free-standing `engine/CGameComponent.h` or `engine/CMainGameComponent.h` is left in the overlay.

- `FABLE_RETAIL_CGAMECOMPONENT` defines `CGameComponent : CBaseClassNonCopyable, CDeviceResetCallback`.
  - Members: Quit +0x08, Running +0x09, `CGame* Game` +0x0C, sizeof 0x10, after Ego_r.pdb. The layout dump shows `CGame *`;
    retry11a had `CGame&`, and the layout is the same either way.
  - Virtuals: Init, pure Run and ChangeTextureColourDepth, protected SetQuit.
  - The CFrontendGameComponent / CNewFrontendGameComponent stand-ins and their two size asserts assume the 4-byte stand-in
    base. On this path they are left out (`#ifndef FABLE_RETAIL_CGAMECOMPONENT`).
- `FABLE_RETAIL_CMAINGAMECOMPONENT` defines `CMainGameComponent : CGameComponent`. It needs FABLE_RETAIL_CGAMECOMPONENT,
  plus FABLE_RETAIL_SCOPED_PTR for the inline accessors.
  - The smart-pointer members now have their PDB types: `CCountedPointer<NGameText::CDataBank> PTextBank` +0x14,
    `boost::scoped_ptr<CPlayerManager>` +0x1C, `boost::scoped_ptr<CGamePlayerInterface>` +0x20, `scoped_ptr<CWorld>` +0x24,
    `scoped_ptr<CDisplayEngine>` +0x28 and `CCountedPointer<CLUA> PLUA` +0x2C. This replaces retry11a's `unsigned long[]`
    and cons11b's `unsigned char[]`.
  - The scalar members run to +0x68. Everything from LastInterpolationInfo on is opaque (`_pad_0x70[0x16178]`). sizeof is 0x161E8.
  - It also has static `PMainGameComponent` with `Get()`, the ctor, `GenerateMetFilesFromLugFiles` (kept from the stand-in),
    the virtuals, `GetPlayerManager()` and `PeekWorld() const`.
- `engine/CDeviceResetCallback.h` (new) is retry11a's version: two pure virtuals, no data. `engine/CIDrawWorldMap.h` (new)
  is identical in both overlays.

## New headers (no repo copy)

- **From the retry11a overlay, unchanged:** CCRC, CPersistTraits, GFMaths, NThingManager_CEntityRuntimePersistence,
  CDeviceResetCallback (adopted over the cons11b non-pure copy), CIDrawWorldMap.
- **engine/CThingManager.h (merged):** retry11a's typed model. It has `NThingManager::CEntityRuntimePersistence` at +0xA0
  and 4-aligned opaque containers. It adds cons11b's `RemapUIDForSerialisation(bool,bool,unsigned long,unsigned __int64)
  const` (0x0051E210, used by 0x00606300). `CMainGameComponent` is forward-declared rather than pulled in, so the two
  retry11a rows include `fable_game.h` and `engine/CWorld.h` themselves.
- **cons11b, from Codex's resolved text** (the `string` includes are rewritten to `fable_string.h`, and
  `CDefStringTable.h` to `fable_definition_table.h`):
  - CFailedAllocationHandler, CGameDefinitionManager, CIntelligentPointer, CRightHandedSet, CShopItem, CTCShop.
  - CPhysicsMeshObject, CMeshIntersectionData, CThingFilter_None, CThingFilter_HasTC, CThingFilter_OverlappingSphere,
    CThingFilter_PosInContainmentVolume.
  - CTCShapeManager, CFilter_Bind2, CTCCreatureNavigation, CTCThingOwner, CThingBuilding, CHeroLogBook, CTCInventoryMap,
    CTCSoundPlayer, CThingPlayerCreature, CThingMarker, CTCTeleporter.
  - The combat set: CAttackingCombatantInfo, CCombatAbilityBlockDefBase, the four Block*AttackDef headers,
    GetHitCounterAttackDef, UseProjectileWeaponDef, CCombatManager, CCombatantInfo, CDefendingCombatantInfo,
    CombatDialogueManager, and the E* enums.
  - The 0x00667450 set: CCombatActionBase, CCombatAction_GenericResponseToDrain / _GenericStrikeResponse(Base),
    CExplosionDef, CHitParameters, CSpecialEffectsDef, CTCCreatureModeManager, CTCDExplosion, CTCDriverBase,
    CThingBaseDef, CThingCreatureDef, EActionRegisteredType, NCreatureMode, NHitLocations_CHitLocation.
  - The 0x00667D30 set: CTBaseSingletonNoDefaultAlloc, CCreatureDef, CHeroCombatDef, CTCFlashOnHit, CTCHaste,
    CTCScriptedControl, CTCTalk, EHeroExperienceStatCategory.
- **Duplicate check:** no class defined in a new header is defined in any repo header, and no two overlay headers define
  the same class outside one file's retail/default pair. The one name collision, `EMode`, is in two different scopes
  (`NCreatureMode::EMode` / `CPersistContext::EMode`).

## Intelligent-pointer packing (coordinator question)

The cons11b rows give no evidence for alignment 1. Every CIntelligentPointer member they touch is at a 4-aligned offset
(CHitParameters +0x58 / +0x60, CThingGameObject +0xC0, CThingCreatureBase +0xF4 / +0x150, CTCShop +0x10), so packed and unpacked
layouts are the same for them.

I confirmed retry11a's evidence: in `struct_layouts_egor.tsv`, 60 of 648 `CIntelligentPointer<...>` members sit at odd
offsets, for example CTCOwnedEntity::POwner +13. **CIntelligentPointer.h is left unpacked here**, as instructed. When
005eb9e0 is retried, the `#pragma pack(push, 1)` branch can go into this one header.
