# Chunk 513 (re-queue, 40 rows <= 96 bytes) -- agent513 notes

Lane: Linux Wine, VC7.1 RTM 13.10.3077 (the lane default). 32 rows hand back in src/ (1 MATCH, 31 RELOCATION_MATCH),
8 are parked in differ/. No retail_asm rows. Every src row has a test that prints `<addr>_TEST PASS` under g++
(tall.sh) and VC7.1 (tall.py). gall.sh: 32/32 graded. vland dry run: 32 WINS, no RELOC_CONFLICT or RELOC_DUPLICATE;
only RELOC_NAME label warnings and SHARED_HEADER_AVAILABLE warnings (see "Header debt").

## Proven
(name from an RTTI vtable slot, a PDB symbol for the exact signature, or a landed caller/callee chain)
0x00407330 CUserProfileManager::IsTutorialCategoryGiven(ETutorialCategory) const
0x004AFD80 CQuestManager::CActiveQuest::ContainsRegion(const CRegion&) const
0x004525B3 CVillagePeopleDef::CVillagePeopleDef()
0x004E1195 CTavernGameCardBaseDef::CTavernGameCardBaseDef()
0x00455DA3 CCombatTypeDef::CCombatTypeDef()
0x0045604D CHeroAbilityDef::CHeroAbilityDef()
0x0045647D CEntitySoundDef::CEntitySoundDef()
0x004EBBA3 CObjectAugmentationsDef::CObjectAugmentationsDef()
0x00436D30 CEngineManager::CEngineManager()
0x0052AB20 CThingPlayerCreature::CThingPlayerCreature()
0x005415F0 NUISystem::CDynamicList::CDynamicList(long)
0x0046F6DC CLensFlareElementDef::CLensFlareElementDef(const CLensFlareElementDef&)
0x004C41D0 std::_Rb_tree<void*, pair<void* const, CRGBColour>, ...>::_M_create_node
0x00528930 std::_Rb_tree<unsigned __int64, pair<const unsigned __int64, NThingManager::CEntityRuntimePersistence::CPersistedDeadEntity>, ...>::_M_clone_node
0x0049B9A0 CFrameRateSmoother::CalculateSmoothedTime() (private)
0x0049BA70 CFrameRateSmoother::Init(long, long)
0x004C6170 CBulletTimeManager::SetBulletTimeSlowdownMultiplier(long, void*, float)
0x004E1DB5 std::__pop_heap_aux<pair<CCharString, CTCBase::CRegisteredTCInfo>*, ..., CKeyPairCompareLess<...> >
0x005383DD NUISystem::CList::AddScrollingChild(CCountedPointer<NUISystem::CComponent>&) -- CList vtable 0x012462E4 slot 144
0x0053D000 NUISystem::CMenuEntry::MakeUnavailableDynamically() -- CMenuEntry vtable 0x0124662C slot 93 (CComponent slot 93 = 0x00530B00)
0x0053D060 NUISystem::CMenuEntry::MakeAvailable() -- slot 94 (CComponent 0x00530CE0)
0x0053D0C0 NUISystem::CMenuEntry::MakeAvailableDynamically() -- slot 95 (CComponent 0x00530DA0)
0x00540120 NUISystem::CEditBox::ProcessEvent(EEventType) -- CObserver-in-CEditBox vtable 0x01246B64 slot 1
0x0055BAE0 NUISystem::CHoverable::OnHovered() -- CHoverable vtable 0x0124BFB4 slot 143 (between IsHovered 142 and OnUnhovered 144), inherited by CDraggableInto
0x005BDE96 NInventory::CCategory::GetNumberOfItemsOfTypeInCategory(long) const -- only caller of the landed find_if 0x005C13CD
0x005BF00B NInventory::CItem::RemoveItem(long) -- FableWin.pdb ?RemoveItem@CItem@NInventory@@QAEXJ@Z
0x005BF556 NInventory::CItem::AddItem(CThing&, long) -- FableWin.pdb ?AddItem@CItem@NInventory@@QAEXAAVCThing@@J@Z
0x005DB4C0 CAnimComponentFollowingOnAnim::Initialise(CPersistContext&, const CCharString&) -- vtable 0x01255788 slot 3
0x005DB530 CAnimComponentFollowingOnAnimFilter::Initialise(CPersistContext&, const CCharString&) -- vtable 0x012557D8 slot 3
Parked, name proven:
0x005DB5F0 CAnimComponentStrikeResponseAnim::TransferBinaryOut(CDataOutputStream&) const -- vtable 0x01255828 slot 7
0x0053848F NUISystem::CList::AddActionOnInactive (slot 70) / 0x005384DD AddActionOnSelected (slot 67)
0x00525D90 CThingManager::OnLevelUnload(long, bool)
0x0041DB1D NUISystem::CManager::CreateComponent(const CCharString&, bool) -- Ego_r.pdb symbol for the (name, bool) overload
0x0044C6C2 CGameDefinitionManager::CGameDefinitionManager() -- stored vtable 0x01232C24 is RTTI .?AVCGameDefinitionManager@@

## Probable
0x0051C760 CNavigationAreaUpdate::CNavigationLayerUpdate::`scalar deleting destructor' (no caller survives; body = the exact member destruction)
0x005C08E6 NInventory::CItem::CItem(const CItem&) -- memberwise copy of a 0x14-byte CItem, +8 through CCountedPointer<CPerItemData> copy 0x005C05D5; callers are vector<CItem> helpers
0x005C0C24 NInventory::CItem::operator=(const CItem&) -- the same with CCountedPointer<CPerItemData>::operator= 0x005C077F
0x005E38C0 (parked) CVectorMap<unsigned long, CAnimationEntry, CKeyPairCompareLess<...> >::Insert(const unsigned long&, const CAnimationEntry&) -- the landed CVectorMap::Insert shape (0x009937D0), Dirty +0x0D; CAnimationSet::Anims is the only map of that pair

## Descriptive
0x005BB190 (parked) std::_List_base<CDefPointer<const CUnprovenDef_005baa41> >::clear -- the list clear is probable (only caller 0x005BB5AE is the inline ~_List_base), but the pointee def type is unproven; it keeps the landed ??_G 0x005BAA41's descriptive spelling
Helper names that are not retail addresses: `ClampToZero` (0x005BF00B: an inline by-reference clamp, name not recovered);
the CEditBox event values 0x21 / 0x22 and the CMenuEntry text-child EType 6 (enumerator names not recovered);
CManager slot 13 (0x0042D506) is kept as the landed callers' "GetKeyPressed".

## Label corrections
- 0x0053D0C0: catalog "CMenuEntry::MakeAvailable" -> CMenuEntry::MakeAvailableDynamically (it calls CComponent::MakeAvailableDynamically 0x00530DA0 and forwards through slot 95).
- 0x005C08E6 / 0x005C0C24: catalog "CEnginePrimitive2DText::operator=" -> NInventory::CItem copy constructor / operator=.
- 0x005BB190: catalog "CNavigatorQueueManager::FrameUpdate" -> STLport _List_base::clear of a CDefPointer list.
- 0x005E38C0: catalog "CAnimationSet::Add" -> CVectorMap<unsigned long, CAnimationEntry>::Insert (CAnimationSet::Add is its caller's name).
- 0x0055BAE0: catalog row class "CDraggableInto"/"CHoverable" -> CHoverable::OnHovered (CHoverable's own vtable 0x0124BFB4 slot 143).
- 0x005C13CD: catalog "vector<pair<long,CCountedPointer<CContainmentVolume>>>::_Ucopy" -> std::find_if<const NInventory::CItem*, binder2nd<const_mem_fun1_ref_t<bool, CItem, long> > > (the landed source already says so; the label row is stale).
- 0x005C04BF: labels "CCountedPointer_Release_005c04bf" / "_Dest_val<...COptimisedPrimitive...>" -> CCountedPointer<NInventory::CPerItemData>::ReleaseData() (it clears Data and Info; FableWin.pdb has ?ReleaseData@?$CCountedPointer@VCPerItemData@NInventory@@@@QAEXXZ and ?Reset@... which RemoveItem inlines).
- 0x009D5060: landed as "CRegion::SetMiniMapRegionExitTextOffsetX" -> CDefString::SetString(const CCharString&) (it does not return this, so not the CDefString(const CCharString&) constructor).
- 0x0040E170: "Array_LinearSearchInt" -> std::__find<const ETutorialCategory*, ETutorialCategory> (the landed body binds __find<long*,long>; same bytes, ICF-free retail so the caller's element type decides).
- Earlier-session corrections (still open in the repo): landed 0x0049B9E0 calls CalculateSmoothedTime as a public struct method (it is private, class V);
  landed 0x004C4890 (_M_insert) spells the CRGBColour tree allocator struct-keyed (U); landed 0x00529230 / 0x00529150 use invented tree stand-ins
  (0x005288F0 is the matching _M_create_node); 0x00519EF0 and 0x004E1D20 are bound under several / invented spellings.
- 0x005415F0's own source used invented base names (CComponentInterface / CObserver_Retail / CEventSinkInterface); replaced with the RTTI bases
  CUIInterface @0, CObserver @4, CActionParam @0x18 under CComponent -> CChangingStateComponent (regraded, still RELOCATION_MATCH).

## Header debt
- engine/CItem.h and engine/CCategory.h are global, struct-keyed (U) flattenings of NInventory::CItem / CCategory: they would mangle
  wrongly (retail symbols are `V...@NInventory@@`), so 0x005BDE96 / 0x005BF00B / 0x005BF556 / 0x005C08E6 / 0x005C0C24 model them locally.
- engine/CList.h, CObserver.h, CManager.h are data-only (flattened vptrs); the NUISystem rows need the polymorphic chain
  (qualified base calls, the CObserver this-adjust at +4) and model it locally (0x005383DD, 0x00540120, 0x005415F0).
- No header carries CIVCountedPointer / CDefPointer / CCountedPointer with their inline release paths; every row restates them.

## Review at landing
- vland: no RELOC_CONFLICT / RELOC_DUPLICATE. RELOC_NAME warnings only (labels, not bindings):
  0x0040E170 (__find<const ETutorialCategory*>), 0x00519EF0 (__destroy_aux<CSwitchableBlockage*>), 0x005C13CD (find_if label row), 0x005C04BF (ReleaseData label).
- 0x0040E170 / 0x004B74E0: the __find spellings follow each caller's element type; the landed 0x0040E170 body is __find<long*,long>.
- 0x004C41D0: this row's allocator is class-keyed (V); the landed caller 0x004C4890 spells it U.
- 0x005BF00B binds 0x005C04BF as ?ReleaseData@?$CCountedPointer@VCPerItemData@NInventory@@@@QAEXXZ (already one of its bindings).
