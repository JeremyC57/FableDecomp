# Hand chunk 474 (CReplaceSerialise .. std::vector): 29 of 40 byte-exact

All 29 matches graded MATCH / RELOCATION_MATCH in the final `gall.sh` run (`grades.txt`). Their tests pass under g++
(`tall.sh`) and VC7.1 (`tall.py`). `calls.py` shows every new/delete/malloc/free call going to retail's import.

## What matched (29)

- **CReplaceSerialise<COldFileFormatMapHeader7,COldFileFormatMapHeader6>::ConvertPreviousUnknownVersionUp** (0051b0a0)
  - Builds a version-7 map header from an older record.
  - Shape: the landed 0x00518040 template verbatim, with ConvertFromOld as a real inline member. The packed fields copy
    as dwords plus a byte, and the palette is one implicit POD `rep movsd`.
- **STLport containers** (namespace `std`, 4.6 bodies verbatim):
  - `__uninitialized_fill_n<CNavigationLayerUpdate>` (0051bba0): unsigned n gives `jbe`. The implicit copy ctor
    inlines two `_Vector_base` copies.
  - `vector<CSwitchableBlockage>::operator=` (0051bcd0): same-TU noinline bodies for the three helpers home every tag
    in the dead x slot.
  - `vector<CRegion>::_M_insert_overflow` (0051cf10): helpers are plain inline templates. The one that stays out of
    line needs `__fastcall` (/Gr TU).
  - `_Rb_tree<CCharString, ...>::_M_insert` (005e1fc0): the landed 0x009AB120 model. The implicit pair copy inlines
    `_M_create_node`.
  - `vector<pair<unsigned long,CAnimationEntry> >::_M_range_insert` (005e2c20): with same-TU helper bodies, every tag
    temporary sits in the first-argument slot.
- **CThingManager**
  - `CopyHeroToHero` (0051f0e0): out-parameter interface lookups with the unchecked `bitset::operator[]` test. The
    lookup key reuses the dead `from` slot.
  - `SaveEntitiesToScript` (00521eb0) and `SaveEntitiesBinary` (00522160): twins. Both use a STLport `set<CCharString>`
    of section names and the range insert into `names`, with same-TU stand-ins for `Compare` (read-only, so the string
    data stays cached) and the range insert (never reads its tag).
    - Script: `script += "..." + section + ";\r\n"` and a reserved fast-extend record string.
    - Binary: a local section stream with explicit WriteULONG + devirtualised Write.
  - `CThingManager` ctor (00523540): an initialiser list in declaration order with the real STLport
    `_Vector_base` / `_Rb_tree_base`. SaveFormatVersion = 2 is set last.
  - `RuntimeSaveGameHero` (00523b40): one-element push_back folds to the growth call; its tag sits in the live `binary`
    slot via a same-TU stand-in. The empty script must be a named local.
- **NUISystem**
  - `CSpellContainer::UnAssignSpell` (00545b10): CDeletion = {Method, STLport list} passed by value. VC inlines the
    copy ctor / ~list in one branch and calls them in the other. STLport list iterator with `_Nonconst_traits` /
    `_Const_traits` and the converting ctor. `vector::operator[]` is `*(begin() + n)`. The CCountedPointer ctor
    tests the stored `Data`.
  - `CComponentContainer::AddTextLine` (00546320): named casts of the counted pointers' Data cache in esi across virtual
    calls. A same-TU `map::operator[]` stand-in puts the key temporaries after their argument push.
  - `CTextSlider::GetValueFromProfile` (00548790): the switch cases in retail block order, plus an early return for
    action 0.
  - `CKeyRedefiner::GetActionText` (00558170): `CWideString == L""` written as its two inline tests with `goto`.
- **CTCPhotojournalCapture::GetDescription** (0055d070): a local CDiskFileWin32 with an inline ctor/dtor. The named
  caption is returned by copy (no NRVO).
- **XTEA** `EncipherMorsel` / `DecipherMorsel` (0055e040 / 0055e180, exact MATCH): the reference loops. VC7.1 unrolls
  them ×4 and emits the delta as `sub/add 0x61C88647`.
- **String helpers**
  - `ReplaceDoubleApostrophes` (0055ecb0): the inner block's destructors run before the STLport `const char*` return
    construction.
  - `(anon)::Explode` (00560920): STLport `find` / `substr` / `push_back` verbatim. The `_String_base` throw helpers are
    declared noreturn.
- **CTCHeroOnlineScoreboard::OnSerialise** (00560720): each branch owns its two vectors. The `_M_insert_overflow` tag
  goes in the dead context slot through a same-TU stand-in. Declaring score/value first gives retail's load order.
- **Experience-screen yes callbacks** `EnsureUndoAbility` / `EnsureUndoStat` (00570009 / 005701da): the
  YesCloseInventory prologue, with null-tested CComponent* -> CObserver* upcasts of the globals.
- **CTCInventoryTrade**
  - `RefreshDescriptions` / `RefreshBuyDescriptionsPC` / `RefreshSellDescriptionsPC` (005ae6af / 005ae9de / 005aeaee):
    the viewport lookup takes a by-value CCharString built in its argument slot. Old descriptions are deleted through
    a by-value CDeletion copy. `this` stays in edi in the console form.
- **NInventory::CTCInventoryBase::Draw2DText** (005bcafe): a /Os frame TU. Each shadow copy assigns a fresh C3DVector
  into one named position. The final position is a named function-scope local.
- **CTCInventoryStats::GetTimeSpentInGame** (005ccc91): /Os. The hour label is an if/else with a named CCharString per
  branch. The minutes and label strings are named in a block around the concatenation.
- **(anon)::GetRotationInAnimationBetweenTimes** (005db6b0): the named info pointer (CCountedPointer dtor clears Data
  then Info). The row-vector product is written z, y, x so the zero terms fold. An `extern const` epsilon gives `fchs`.

## Parked for Fable 5.1 (11)

- 00521cf0 CThingManager::LoadMainPlayerCreatureState DIFFER(448v446): retail homes the hero in the dead `state` slot and the loaded creature in [esp+0x10] (ebp = physics vtable); ours swaps the two slots and keeps the vtable in [esp+0x18] | tried: STLport pair/iterator ctors, a named hero-name local for the set insert, a named thing local in the scan loop, hero lookup order (4 grades) | next: same-TU stand-ins for the out-of-line callees that take the hero/creature (escape lever), else lane gap: register/slot colouring
- 005223f0 CThingManager::LoadLoadableGlobalThingsFromFile DIFFER(306v306): frame 0x24 v 0x1C -- retail overlays the discarded set::insert pair<iterator,bool> result with the later vector<CThing*> local | tried: POD pair, pair+iterator ctors, by-value name getter, STLport insert with named __p (4 grades) | next: give insert_unique a same-TU noinline body (the lever that fixed 00522160/00546320), or scope the insert in an inline helper
- 00524ec0 NThingManager::CEntityRuntimePersistence::LoadFromGameStateBinary DIFFER(1006v1006): retail colours the map UID ebx (spilled to its home after the map-number conversion) and the compressed size ebp; ours keeps the UID in ebp and the size in ebx, so the sub-stream pointer spills and this / world map / loop counter / record frame slots shift (same 323 instructions) | tried: 30 free cmp.py variants -- external vs same-TU TransferBinaryIn (stand-ins and the byte-exact header bodies), traits named/temporary/DefaultVal, dead-entity record local (fixed frame 0x84), separate map index, cast reference, declaration order/scope, inline map-number helper, setter/lookup/insert stand-ins | next: lane gap: register colouring order
- 00560bb0 CTCHeroOnlineScoreboard::GetDecryptedDocument DIFFER(322v321): retail keeps `this` in ebp and reloads the buffer start after resize; ours spills `this` (frame 0x20 v 0x1C) and keeps the start in ebp | tried: strlen/memset intrinsic spellings, STLport string/vector models, early return vs nested if, same-TU EncipherMorsel/CryptoMatic stand-ins (4 grades) | next: lane gap: register allocation
- 0057d23b CTCHeroStats::UpdateAppearanceStats DIFFER(876v869): the clothing CDefPointer from GetDef is released through the escaped form (`mov ecx,eax; add eax,4; dec [eax]; cmp [eax],0`) where retail uses `dec [ecx+4]; jne`; the morph key temporary lands in [ebp-0x10] v [ebp-0x14] | tried: GetInterface vs HasTC+GetTC, joint/separate returns, Release forms, stand-ins for GetSubDef only / both GetDef and GetSubDef (the latter re-allocates the whole body) | next: lane gap: IPA/LTCG escape summary (out-pointer of GetDef treated as non-escaping)
- 00594481 std::_Rb_tree<CDateAndTime,...>::insert_unique(iterator, const value_type&) DIFFER(423v423): end() branch `mov ecx,[eax+0xc]; add ecx,0x10` in retail v `lea ecx,[eax+0x10]` ours | tried: STLport hinted insert verbatim with plain vs reference-cast _M_rightmost/_S_right/_S_key accessors, the real binary_function/less + _STLP_alloc_proxy model, /Os pragma (4 grades) | next: lane gap: register choice
- 005a887d CTCInventoryTrade::RefreshBuyPossibilityPC DIFFER(480v480): `mov [ebp-0x24],3` (deletion.Method) sits right before the insert call in retail, before the in-place iterator store in ours | tried: Method before/after push_back, STLport list iterator forms, insert stand-in (5 grades: one over budget) | next: lane gap: store scheduling
- 005a9a87 CTCInventoryTrade::RefreshBuyPossibility DIFFER(565v567): retail spills `this` to [ebp-4] only and puts the purse type 4 in ebx (ours: `this` in ebx, 4 in edi); same Method=3 store placement as 005a887d | tried: 2 grades + free variants (split-traits iterator, STLport-real insert body, external insert, same-TU CDeletion ctor/copy stand-ins: no change) | next: lane gap: register allocation + store scheduling
- 005c36c2 (anon)::PutSheatheObjectInCarrySlot DIFFER(641v645): retail zeroes the weapon def pointer after the carrying lookup (`and [ebp-0x14],0`) and keeps the weapon in ebx; ours hoists the zeroing into a shared `xor eax,eax` store run and caches the def in esi | tried: decl-order permutations x {opaque, escaping, same-TU} GetSubDef<CWeaponDef>, inline interface helper (2 grades) | next: lane gap: register/slot allocation
- 005c552c CTCInventoryWeapons::RemoveWeaponsFromSheathes DIFFER(551v546): retail rotates the weapon loop (guard at top, `add eax,4; cmp; jne` at the bottom, iterator in [ebp-0x18]); VC7.1 here cross-jumps to one bottom test (iterator [ebp-0x10]) | tried: continue vs nested if, while/do-while, plain vs forceinline lookups, same-TU LowerBound, GetTC+HasTC, vector-first declaration (3 grades) | next: lane gap: loop rotation
- 005dcd10 CAnimationSet::GetRandomLoadedAnim DIFFER(659v608): retail keeps this/esi, iterator/ebx, &it->second/ebp and only one vector finish in edi; ours also caches the loaded vector's finish and spills this (sub esp,20h), so all later register roles differ | tried: opaque vs same-TU reserve/_M_insert_overflow (stand-in and full STLport), loop forms, declaration orders (1 grade) | next: lane gap: register allocation

## Skipped

None (no row holds a second function after its padding).

## Label corrections

All new names come from the Ego_r PDB name set (used as a name set only), except the std rows, which use the STLport spelling.
- 0x0051B0A0 "CEngineLandscapeMeshBuilder_CLayer_CopyConstructor_Type4" -> CReplaceSerialise<COldFileFormatMapHeader7,COldFileFormatMapHeader6>::ConvertPreviousUnknownVersionUp.
- 0x0051BBA0 "VectorReallocateAndCopyMultipleStructures" -> std::__uninitialized_fill_n<CNavigationLayerUpdate*, unsigned int, CNavigationLayerUpdate>.
- 0x0051BCD0 "Vector_ReAllocate" -> std::vector<CNavigationAreaUpdate::CSwitchableBlockage>::operator=.
- 0x0051CF10 "VectorReallocateWithRange" -> std::vector<CRegion>::_M_insert_overflow.
- 0x0051F0E0 "CTCHeroMorph_ApplyEffects_WithLookup" -> CThingManager::CopyHeroToHero.
- 0x00521CF0 "CThingManager_LoadMorphFromScript" -> CThingManager::LoadMainPlayerCreatureState.
- 0x00521EB0 "Save_WriteEntityHierarchy" -> CThingManager::SaveEntitiesToScript.
- 0x00522160 "SavePersistentEntitiesInRegions" -> CThingManager::SaveEntitiesBinary.
- 0x005223F0 "LoadGlobalThingBatch" -> CThingManager::LoadLoadableGlobalThingsFromFile.
- 0x00523B40 "CThingManager_SavePersistentEntityData" -> CThingManager::RuntimeSaveGameHero.
- 0x00524EC0 "CDrawQuestInfo_LoadDeadCreatures" -> NThingManager::CEntityRuntimePersistence::LoadFromGameStateBinary.
- 0x00545B10 "AnimationEntry_ProcessLoad" -> NUISystem::CSpellContainer::UnAssignSpell.
- 0x00546320 "UIContainer_SetupTextSprite" -> NUISystem::CComponentContainer::AddTextLine.
- 0x00548790 "CEditControlCentre_DispatchPropertySetByEnum" -> NUISystem::CTextSlider::GetValueFromProfile.
- 0x00558170 "GetMovementActionText" -> NUISystem::CKeyRedefiner::GetActionText.
- 0x0055D070 "NHeroInformationScreens_LoadTextFromFile" -> CTCPhotojournalCapture::GetDescription.
- 0x00560720 "QuestData_PersistActivitiesAndScores" -> CTCHeroOnlineScoreboard::OnSerialise.
- 0x00560920 "DecryptResponse_ParseElements" -> (anon)::Explode.
- 0x00560BB0 "LHNetworkLib_DecryptResponseDocument" -> CTCHeroOnlineScoreboard::GetDecryptedDocument.
- 0x005701DA "EnsureUndoAbility" (propagated) -> EnsureUndoStat.
- 0x0057D23B "FrameUpdate_ApplyMorphEffect" -> CTCHeroStats::UpdateAppearanceStats.
- 0x00594481 "CMemoryAllocatorVariableSize_TransferIn_DateAndTime" -> std::_Rb_tree<CDateAndTime, pair<const CDateAndTime, CWideString>, ...>::insert_unique(iterator, const value_type&).
- 0x005A887D "PC_PopulateBuyList" -> CTCInventoryTrade::RefreshBuyPossibilityPC.
- 0x005A9A87 "Inventory_PopulateBuyList" -> CTCInventoryTrade::RefreshBuyPossibility.
- 0x005AE6AF "CTCInventoryTrade_UpdateTradeDescriptions" -> CTCInventoryTrade::RefreshDescriptions.
- 0x005AE9DE "CTCInventoryTrade_DisplayBuyItem" -> CTCInventoryTrade::RefreshBuyDescriptionsPC.
- 0x005AEAEE "CTCInventoryTrade_DisplaySellItem" -> CTCInventoryTrade::RefreshSellDescriptionsPC.
- 0x005BCAFE "Draw2DText" -> NInventory::CTCInventoryBase::Draw2DText.
- 0x005C552C "CTCCarryable_PrepareWeaponSheathing" -> CTCInventoryWeapons::RemoveWeaponsFromSheathes.
- 0x005CCC91 "GetTimeSpentInGame" -> CTCInventoryStats::GetTimeSpentInGame.
- 0x005DCD10 "CTCInventoryAbilities_GetRandomAbility" -> CAnimationSet::GetRandomLoadedAnim.
- 0x005E1FC0 "std_set_InsertStringKey" -> std::_Rb_tree<CCharString, pair<const CCharString, CIVCountedPointer<CAnimComponentBase> >, ...>::_M_insert.
- 0x005E2C20 "std_Vector_InsertRange" -> std::vector<pair<unsigned long, CAnimationEntry> >::_M_range_insert.

Callees named on the way. These are used as declarations here, not landed:
- 0x005236F0 "CDrawQuestInfo::OracleSymbolPopBack" -> CEntityRuntimePersistence::Clear (the landed file already says so).
- 0x00523910 SetGamePersistentSaveDataCompressed.
- 0x005237F0 SetGamePersistentScript.
- 0x0052A130 is the dead-entity map insert.
- 0x005458A0 "AnimationEntry_LoadOrFindIndex" -> NUISystem::CSpellContainer::AssignSpell.
- 0x00546060 -> CSpellContainer::SetEmptySlotDef.
- 0x00993EA0 CDataOutputStream::WriteULONG; 0x00993EB0 WriteSLONG (landed files call it WriteLong); 0x00993EC0 WriteUHUGE.
- 0x00994B40 CMemoryDataOutputStream::GetLength.
- 0x00996B30 ~CMemoryDataOutputStream.
- 0x0052B6A0 vector<CCharString>::_M_range_insert.
- 0x004ABA70 _Rb_tree::_M_erase.
- 0x004C84B0 CThing::SaveBinary.
- 0x006869D0 CTCBase::GetDefinitionManager.
- 0x00686A80 CTCBase::GetWorld.
- 0x0049D870 CWorld::GetFrame.
- 0x0099EF90 CCharString::Reserve.
- 0x0042B040 CDefinitionManager::GetDef.
- 0x0057FF92 GetSubDef<CAppearanceModifierDef>.
- 0x00581353 vector<CDefPointer<const CTattooDef> >::~vector.
- 0x0042CD84 "pair<...>" -> NUISystem::CDeletion copy ctor.
- 0x0042B4C3 list<CComponent*>::_M_insert_dispatch.
- 0x00A242C0 "GetAbilityLevel/GetAbilityEntry" -> CMeshDataBank::PeekAnimation.
