# Hand chunk 563 (CTCInventoryMap .. CTCRandomAppearanceMorph): 11 of 12 byte-exact

All 11 matched rows are RELOCATION_MATCH. None are MATCH and none are retail_asm. Each was checked with gall.sh and has a
behaviour test that passes under g++ (tall.sh) and VC7.1 (tall.py). The vland dry run (through agent561's
`tools/vland_wrap.py`) gave WINS: 11, with no RELOC_IDENTITY, RELOC_CONFLICT or DUPLICATE (the warnings are listed below).

## What matched (11)
- **CTCInventoryMap::GetMenuEntryFromCategory 0x005FAA90** (UI_MENU_ENTRY_LOGBOOK_*) and **::GetMenuEntryFromCategoryPC
  0x005FAC20** (PC_MENU_ENTRY_LOGBOOK_*). These are protected and `this` is unused. Each case is `return
  parent->FindChild(CCharString("..."))`, with FindChild as virtual slot 3 taking a by-value CCharString built in the argument
  slot. Shape: the case bodies are laid out in source order 4,5,6,7,0,1,default, and cases 2 and 3 share the default.
- **CTCQuickAccessMenu::~CTCQuickAccessMenu 0x00625A20**. Body: RemoveFromGlobalTCs, delete each component, clear the
  vector, then CManager::GetInstance() for ClearObservers / ClearActionParams / `CList::GPreviousSelected.clear()` /
  ClearBastardChild. This is the CTCQuestCompletionUI 0x005E41E0 model, with the list clear specialised inline.
- **CPersistContext::Transfer<EOpinionAttitudeType> 0x00685CC0**. This is the agent561 Transfer<unsigned long> 0x004106F0
  template. T is proven from the RTTI of the text-arm wrapper vtable 0x0125BABC. TransferBinaryIn 0x0045ED25 is the
  two-argument void form.
- **CThingPlayerCreature::~CThingPlayerCreature 0x006AA230**: `if (IsInMelee()) GetCombatManager().DeregisterDestroyedCombatant(*this);`.
  GetCombatManager returns a reference (the non-const overload). The map<EGameAction, pair<C3DVector,long> > member brings
  the out-of-line _M_erase 0x00527910.
- **CTCBuyableHouse::SetRented 0x006C1150**. Shape lesson: the inline CThing::GetTC must take `&Components` into a local
  map pointer, as in agent561's 0x004A2F10, with a constant key from `T::INTERFACE_TYPE`. Reading `Components.Data.end()`
  directly off the hero pointer keeps the hero in esi and hoists the key store, which gave DIFFER 246v243 on grade 1.
- **CTCCreatureNavigation::~CTCCreatureNavigation 0x006CC1E0**: `if (PNavigator) { RemoveAgentListFromLists(&Agents);
  PNavigator = 0; delete + null each PossibleNavigators[i]; }`.
- **CAnimateThing::~CAnimateThing 0x0070BA90**. The destructor is user-declared and empty on a non-polymorphic class. The
  member dtors call two out-of-line STLport `_List_base::clear` instantiations (0x0070B8A0, 0x0070BA00) and tail-jump to
  ~CCompleteAnimation 0x0070B930. CAnimSet is modelled at its retail size 0x54.
- **NTCScriptedControl::CActionTalkBase::~CActionTalkBase 0x0071A430** and **CTCEmoteIcon::~CTCEmoteIcon 0x0071B120**. Both
  are implicit destructors (no vptr store), emitted through `void EmitImplicitDestructor(T* p) { p->T::~T(); }`. The real
  classes were found from the RTTI of the caller vtables; both catalog labels were wrong.
- **CTCRandomAppearanceMorph::~CTCRandomAppearanceMorph 0x00727B40**: `if (!ParentThing->Dead)
  ParentThing->PTCGraphicAppearance->UnregisterComponent(*this);`. UnregisterComponent takes a reference. After it come the
  inline CAGraphicAppearanceComponent vptr store and the CTCBase chain.

## Parked for Fable 5.1 (1)
- 0071ba60 CTCHeroMorph::GetMorphStrength DIFFER(624v624): frame `sub esp,14h`/`add esp,14h` v retail `10h`. Case 11's
  GFLimitReturn temporaries sit at [esp+0Ch..14h], while retail starts them at [esp+8], reusing case 9's value slot (every
  other instruction matches, the jump table included) | tried: plain temporaries; explicit `case 0: return 0.0f` (fixed the
  0-based table and the `dec eax`); ternary for type 18; a named local in case 11 (no change); a named local in case 9
  (moved the 1.0f/0.0f stores after the call, frame unchanged) | next: other ways to scope the case 9 / case 11
  temporaries (e.g. explicit `GFLimitReturn<float>` with `const float` locals in case 11, or case 11 written through a
  helper inline), or lane gap: VC7.1 temporary-slot packing.

## Skipped
None.

## Label corrections
- 0x005FAC20 `CTCInventoryMap::GetMenuEntryFromCategory` should be `GetMenuEntryFromCategoryPC` (PC_ strings, Ego_r donor order).
- 0x00685CC0 `CPersistContext::Transfer<signed char>` should be `Transfer<EOpinionAttitudeType>` (wrapper RTTI).
- 0x0071A430 `C3DMeshStripifier::~C3DMeshStripifier` should be `NTCScriptedControl::CActionTalkBase::~CActionTalkBase`.
- 0x0071B120 `CCreatureAction_DragonFireBlastBase::~CCreatureAction_DragonFireBlastBase` should be `CTCEmoteIcon::~CTCEmoteIcon`.
- Callee labels: 0x00661E80 `NUISystem::CText::GetAlignment` = CThingCreatureBase::IsInMelee; 0x006661B0
  `CThingGameObject_Finalize` = ~CThingCreatureBase; 0x0070B930 `CWound::Kill` = CAnimateThing::CCompleteAnimation dtor;
  0x0070B8A0 `CNavigatorQueueManager::FrameUpdate` and 0x0070BA00 `CGuiControlTreePane::SortTreeRecursively` are STLport list
  clears; 0x00527910 `LTextTreeWalkThrough::BuildTreeArray` is an _Rb_tree::_M_erase; 0x00686830
  `CChunkedFileChunk::~CChunkedFileChunk` = ~CTCBase; 0x004378C0 `CPlayerGui::Refresh` = UpdateMiniMapMarkers(bool);
  0x0057EF86 `GFLimitReturn<double>` is the float instantiation; 0x00A47D80 is the CNavigatorExternalInterface static.

## Names
### Proven (PDB name set + a second source; spelling checked with `objdump -t`)
- Rows: 0x005FAA90 `?GetMenuEntryFromCategory@CTCInventoryMap@@IAE...`, 0x005FAC20 `?GetMenuEntryFromCategoryPC@...`,
  0x00625A20 `??1CTCQuickAccessMenu@@UAE@XZ`, 0x00685CC0 `??$Transfer@W4EOpinionAttitudeType@@@CPersistContext@@QAEXPBDAAW4EOpinionAttitudeType@@ABW41@@Z`,
  0x006AA230 `??1CThingPlayerCreature@@UAE@XZ`, 0x006C1150 `?SetRented@CTCBuyableHouse@@QAEX_N@Z`, 0x006CC1E0
  `??1CTCCreatureNavigation@@UAE@XZ`, 0x0070BA90 `??1CAnimateThing@@QAE@XZ`, 0x0071A430
  `??1CActionTalkBase@NTCScriptedControl@@UAE@XZ`, 0x0071B120 `??1CTCEmoteIcon@@UAE@XZ`, 0x0071BA60
  `?GetMorphStrength@CTCHeroMorph@@QBEMW4EHeroMorphType@@@Z`, 0x00727B40 `??1CTCRandomAppearanceMorph@@UAE@XZ`.
- Callees: 0x004C0000 UnregisterComponent (AAV ref), 0x00661E80 IsInMelee (QBE), 0x00661ED0 GetCombatManager (QAEAAV),
  0x006661B0 ~CThingCreatureBase, 0x006ED9E0 DeregisterDestroyedCombatant (AAV), 0x0070B930 ~CCompleteAnimation,
  0x00686830 ~CTCBase, 0x00686DF0 RemoveFromGlobalTCs, 0x0099A2E0 ~CBaseClass, 0x0099A300 ~CBaseClassNonCopyable (MAE),
  0x0045ED25 TransferBinaryIn (FableWin), 0x006C0FD0 Evict / 0x006BFB90 NeedsMainSignPost / 0x006C0F00 CreateMainSignPost
  (all private AAE), 0x004378C0 UpdateMiniMapMarkers(bool), 0x00449970 GetMainPlayer, 0x00487DC0 GetPControlledCreature
  (settled), 0x00A47D80 RemoveAgentListFromLists, 0x0041E5F2 NUISystem::CManager::GetInstance (SI), 0x0057EF86
  GFLimitReturn<float>, 0x0099EBF0 CCharString(const char*, long), 0x004D1DB0 ~CSubtitledMessage, 0x004C9B80 CThing::Kill,
  0x00A01B50 CBaseIntelligentPointer::GetPItem, 0x0040F020 CVectorMap::LowerBound.
### Probable
- 0x00527910 `_Rb_tree<EGameAction,...>::_M_erase` (private AAE as 56 of 68 bound _M_erase spellings; STL instantiations are
  not in the PDB name sets). 0x0070B8A0 / 0x0070BA00 `_List_base<CAnimSet>::clear` / `_List_base<CAnimateThing::CDelayedAnimation>::clear`
  (QAE, STLport 4.6 source; not in the name sets).
- `CArray<T> : std::vector<T>` (CArray 0xC in retail); EEmoteType / EEmotePriority nested in CTCEmoteIcon; ECategory nested in
  CHeroLogBook::CHeroLogBookEntry (from the PDB mangling of the row's own name).
- Globals g_pMainGameComponent 0x013B86A0 and g_pPlayerGui 0x013B8790 (bound spellings; no PDB data symbols).
### Descriptive
- Enumerator values only (names not recovered): CHeroLogBookEntry::ECategory 0..7, EHeroMorphType 0..18, EGameAction,
  ETCInterfaceType key 4 (CTCHeroStats::INTERFACE_TYPE). CUnprovenSlot fillers in the NUISystem::CComponent vtable model.
  `EmitImplicitDestructor` (helper, not a retail function).

## Header debt
- engine/CTCBase.h, engine/CThingCreatureBase.h and engine/CCreatureAction_DragonFireBlastBase.h flatten the vptr into a
  `__vftable` member, so the polymorphic CTCBase / CThingCreatureBase chains are modelled locally.
- engine/CIVCountedPointeeBase.h and engine/CNavigatorBase.h flatten the vptr too. 0x0070BA90 and 0x0071B120 need
  CIVCountedPointeeBase's virtual Delete (slot 1), and 0x006CC1E0 needs CNavigatorBase's virtual dtor, so those are local
  (vland SHARED_HEADER_AVAILABLE warnings).
- engine/CList.h is a global `struct CList`, but the retail class is `NUISystem::CList` (GPreviousSelected 0x013B8A98).
  Including it would give the wrong mangling, so 0x00625A20 keeps a local NUISystem::CList (vland warning).
- 0x006C1150 now includes engine/CPlayerGui.h through the rename trick, which clears that vland warning (regraded
  RELOCATION_MATCH).
- No header exists for CTCInventoryMap, CTCQuickAccessMenu, CTCCreatureNavigation, CTCRandomAppearanceMorph, CTCEmoteIcon,
  NTCScriptedControl::CActionTalkBase, CAnimateThing or CTCHeroMorph.

## Review at landing
- RELOC_NAME warnings from vland (manifest labels, not spellings): 0x00661E80 (labelled CText::GetAlignment), 0x00527910
  (FreeTree_CTreeOwner_00527910), 0x00487DC0 (CPlayer::GetHeroCreature; spelling settled), 0x004378C0 (CPlayerGui::Refresh).
  NAME_FIXES.tsv covers each.
- 0x0070B8A0 / 0x0070BA00 / 0x00527910 / 0x004378C0 each carry several unrelated bound spellings. If any of them is an
  ICF-folded body shared by other instantiations, both names belong to the one address: the coordinator should pick the
  spelling of the class whose layout the body walks (these sources' evidence is in each file's Callees line).
- The two implicit dtors (0x0071A430, 0x0071B120) define a non-retail `EmitImplicitDestructor` helper in the object. The
  lander must pick the dtor symbol via `// Leaf:` (it did in the dry run).
