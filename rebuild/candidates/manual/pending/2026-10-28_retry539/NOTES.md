# Chunk 539 (retry): notes

No row matched. I graded each of the 8 parked rows once under RTM in this pass, and all 8 are re-parked in `differ/` with
the PDB spellings. `src/` and `test/` are empty.

| Row | Leaf (PDB) | Grade | Residue |
|---|---|---|---|
| 0x005542F0 | NUISystem::CMorphingSprite::GetDataFromDef | DIFFER(727v727) 23 | pair-store schedule + Touch colouring |
| 0x00555A90 | NUISystem::CScrollableList::UpdateVisibleList | DIFFER(595v595) 16 | register colouring |
| 0x005726A0 | CTCInventoryExperience::RecheckAvailabilities | DIFFER(824v824) 4 | Method store hoist |
| 0x00576C4F | CTCInventoryExperience::UndoBuyAbility | DIFFER(642v642) 2 | push ecx / fsubp |
| 0x00576ED1 | CTCInventoryExperience::UndoBuyStat | DIFFER(622v622) 4 | push ecx / fsubp + 1 load order |
| 0x0057DF85 | CTCHeroStats::KickHeroToMostRecentSafeRegion | DIFFER(876v876) 17 | one frame-slot rotation |
| 0x00584B86 | CTCInventory::AssignThingPC | DIFFER(873v854) 79 | use-arm cross-jump / temp packing |
| 0x00589A30 | CTCInventory::OpenPCMapMenu | DIFFER(549v549) 2 | esi/edi reload order |

## Names

### Proven (Ego_r.pdb spelling plus retail donor order or call-site shape)
These are all in `NAME_FIXES.tsv` (35 rows):
- RecheckAvailabilities 0x5726A0. The catalog label UpdateAbilityDisplay is in neither PDB.
- CTCInventoryMap::ConstructPCMapList(bool, bool*) 0x60EEB0. Its body ends in `ret 8`, so it is not BuildMapMenu(long, long).
- NUISystem::CManager::GetInstance 0x41E5F2.
- CUIScreenUnitConverter::GetBackBufferWidth / GetBackBufferHeight 0x41CC14 / 0x41CC2B.
- CManager::GetUIScale const 0x41CF47.
- CTCInventoryAbilities::GetAbilityLevel const 0x5EDE93 and SetAbilityLevelTo 0x5EE01A.
- GFIntToWideString 0x405F00.
- CScrollableList::HideComponent / UnhideComponent 0x555440 / 0x5556F0. They are protected and take the global C2DVector.
- The CTCBase getter block, in PDB order:
  - GetWorld 0x686A70
  - PeekWorld 0x686A80
  - PeekWorldMap 0x686D20
  - GetWorldMap 0x686D30
  - GetThingSearchTools 0x686D80
  - PeekThingSearchTools 0x686D90
- CWorldMap::GetRegion const 0x4FC180. It sits before GetRegionNumberMapIsIn 0x4FC190; 0x4FB490 is the twin body.
- CPlayerGui::SetMiniMapRegion 0x4378B0. It sits after SetMiniMapAlpha 0x437890.
- CThingSearchTools::GetPFirstLeafThing<CFilter_Bind2<HasDefName,HasScriptData>,CThingMarker> 0x5800D6 and PeekTypeList 0x49C780.
- The filter operator() pair 0x6A7BE0 / 0x6A7E70.
- CWorld::HandleMoveHeroToRegionGameEvent(const CGameEvent&) 0x49EAF0.
- CGameEvent::AddData 0x9F1760.
- STLport `_Rb_global<bool>::_M_increment` 0x4292C0 and `_Rb_tree<ulong,...>::insert_unique(iterator, const value_type&)` 0x5537F0.
- The CGraphicInfoFrame / CFrameData / CResource family 0x9FE620 / 0x9FE380 / 0x9FDE80 / 0x9FF6B0 / 0x9FBFB0 / 0x9FBF90. These are global classes with private access, not NUISystem.
- The CAction::AddParam overloads 0x53FDE0 (2-arg) and 0x53FDB0 (1-arg).
- list<CActionParam*>::push_back 0x42B646.
- static CTCInventoryItem::GetEngineGraphic 0x5D96A0.

### Probable (one strong source)
- `CWorldMap::GetLoadedRegion() const` 0x4FB150. The body is `mov eax,[ecx+9Ch]` and the PDB member exists, but the address is not anchored by order. It is not in NAME_FIXES.
- The CComponent vtable 0x0124608C slots 66-79 follow the AddAction family in PDB definition order: 74 AddActionOnDropped, 75 AddActionOnDroppedNowhere. Slot 80 OnLeftUnclicked is out of order. Slot 60 is AddChild, slot 62 is SetDeletion, and CSprite::SetGraphicIndex is slot 145.
- `CCountedPointer<NUISystem::CAction>::Reset(CAction*)` 0x5791E5. The template exists in the PDB; the member name comes from the body shape.
- CText::SetValue is slot 144, where landed code says SetText. This comes from 0x5726A0 and is not added to NAME_FIXES (it is a vtable slot, not an address).
- RTTI 0x0122F844 is NUISystem::CActionParamULONG (type 5) and 0x012366F4 is CActionParamSLONG (type 3). Both have virtual dtors.

### Descriptive (no PDB source found)
- `g_pPlayerGui` (the CPlayerGui global used by 0x57DF85).
- `g_UndoAbility`, `g_UndoStat`, `g_pUndoMousePointer`, `g_pUndoYesNoDialog`: the file-static globals of the undo dialogs in 0x576C4F / 0x576ED1.
- EnsureUndoAbility / CancelUndoAbility / EnsureUndoStat / CancelUndoStat are the existing landed callee spellings, kept as found.
- CGraphicDataBank +0x194 is named CurrentFrame.

## Header debt
- `rebuild/include/engine/CTCHeroStats.h`, `CMorphingSprite.h`, `CTCInventory.h` and `CTCInventoryExperience.h` declare no member functions and flatten the vptr. All four rows therefore keep local class models.
- CTCBase needs its protected Get/Peek getters (IBE) declared so that callers mangle correctly. Spelling them as members of the derived class mangles them under the wrong class.

## Label corrections
- 0x5726A0: UpdateAbilityDisplay should be RecheckAvailabilities.
- 0x60EEB0: BuildMapMenu should be ConstructPCMapList.
- 0x53FDE0: SetTarget should be AddParam.
- 0x53FDB0: AddUIActionParam should be AddParam.
- 0x41CC14 / 0x41CC2B: GetScreenWidth / GetScreenHeight should be GetBackBufferWidth / GetBackBufferHeight.
- 0x5800D6: the landed leaf's class is CThingSearchToolsMethods and should be CThingSearchTools.

## Review at landing
- **Same-TU bodies.** These may raise RELOC_DUPLICATE against landed leaves when the rows land later. All are real retail callees under their real names:
  - 0x5800D6 GetPFirstLeafThing, in 0x57DF85. The landed leaf's name is wrong; see NAME_FIXES.
  - 0x42B040 `CDefinitionManager::GetDef<CThingObjectDef>` and 0x5936C2 `CDefClassBase::GetSubDef<CAbilityDef>`, in 0x584B86. Their bodies are ported from the landed files.
- **0x42ABCA list dtor spelling.** It is bound as both `list<CComponent*>` and `_List_base<...>`; this is the lesson-526 question. 0x584B86 emits `??1?$list@PAVCComponent@NUISystem@@...`.
- **0x429160 / 0x4291DE.** These are the CCountedPointer<CAction> / <CComponent> dtors in this chunk's model. Other agents bound ReleaseData / Release spellings, so the dtor-vs-ReleaseData question is still open.
- **No RELOC_CONFLICT / RELOC_DUPLICATE.** There are no matched rows, so the vland dry run sees an empty payload.
