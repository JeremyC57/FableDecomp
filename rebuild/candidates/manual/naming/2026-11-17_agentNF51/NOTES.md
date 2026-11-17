# NF51 notes (round 7: CAIStateGroupBase AI actions, NUISystem actions, NPlayerGui)

## Method
- AI rows: each local CAIStateGroupBase model replaced by the retail branch of engine/CAIStateGroupBase.h (`FABLE_RETAIL_CAISTATEGROUPBASE`
  + `_MEMBERS` + the action opt-ins) with the Ego_r enums of engine/fable_ai_action_enums.h; call arguments respelled to the enums and
  `bool&`; row helpers that lived on the local base (GetCreature, Random) moved into the derived struct. `work/port.py` is the transformer.
- Where the self is virtual in the retail base, overriding forced the self leaf; each one checked against egor/fablewin_pdb_names.tsv
  and set to the PDB access (UAE / MAE / UBE / AAE / IAE). RENAMES.tsv section 1 lists them.
- vcall thunk entry nf44:2 rejected (REJECTED.tsv): VC7.1 RTM names every slot-0 __fastcall vcall thunk `??_9@$BA@AE` (scratch TU
  work/vcall/vc.cpp); the 14 binders already bind it; retail calls 0x0043E420 from each (direct E8 calls / pushes): one COMDAT, not ICF.

## Findings
- 0x0042BED4 / 0x0042CD84 (CDeletion ctors): row 0x0059DE04 models the 8-byte object as a `CAction`, but retail CAction() is 0x0042BE50
  (both PDBs name ??0CAction@NUISystem@@QAE@XZ separately, with Valid) and the object goes to CComponent slot 62 = SetDeletion(CDeletion):
  the stand-in is a CDeletion {Method, AssociatedParents}.
- Descriptive names kept from the landed rows (no PDB symbol found): `lastMapRadius` 0x013BB6B4, `g_LeaderboardApproachDistance`
  0x013BB6AC, `g_LookAtTurnSpeed` 0x0138CBA8.

## Follow-ups (not in this slice; no grades spent)
- 0x0092A220 keeps its derived view `CAIStateGroupApprenticeHeroMethods` (self ?MoveToAppropriateDummy@CAIStateGroupApprenticeHeroMethods@@QAE_NXZ);
  PDB is ?MoveToAppropriateDummy@CAIStateGroup_ApprenticeHero@@IAE_NXZ. The sibling ports 0x0092AE70 / 0x0092AFE0 show the full form.
- 0x00744730's self is a free function `CTCPerceiveThing_SetLastAwareInfo` over a local CTCPerceiveThing view (catalog
  CTCPerceiveThing::SetLastAwareInfo); its Getter_d82ca0 extern is also a placeholder.
- 0x0096F050 keeps the row's local CAIStateGroupBehaviourBase view (engine/CAIStateGroupBehaviourBase.h exists); it now derives the shared
  CAIStateGroupBase and has no AI-action stand-ins.
- 0x00947A30 calls 0x00947840 `IsInWorkHours` on the derived class; Ego_r names CAIStateGroup_StayNearFoodHall ?IsMealTime@..@@ABE_NXZ
  (Ego_r 0x0096B9E0) next to IsRunnable / Update -- unverified, not changed.
- 0x0095E470's anonymous-namespace TOTAL_FRAMES_TO_RUN hash changes with the TU text (`?A0x...`), as for every anonymous namespace.

## NUISystem / NPlayerGui findings (2026-11-17)
- NUISystem slots used (work/vt.py over the shared headers): CComponent 3 FindChild, 59 AddChild, 60 AddChildDynamically,
  62 SetDeletion(CDeletion), 66 AddAction, 67 AddActionOnSelected, 68 AddActionOnBack, 69 AddPreAction, 70 AddActionOnInactive,
  71 AddActionOnLeftClicked, 83 SetName, 92 MakeUnavailable, 93 MakeUnavailableDynamically, 108 LoadAndReturnDef, 124 AddPositionChild;
  CList 145 AddNonScrollingChild, 152 AddScrollingChildAndSetOffsets, 153 AddScrollingChildDynamicallyAndSetOffsets, 156 InitialiseOffsets,
  161 SetSelectedChild; CMenuEntry 150 AddThingActionParam, 151 AddUIActionParam, 152 AddActionOnUnselected, 153 AddActionOnVisible;
  CText 144 SetValue. The descriptive local names of the landed rows (SetActionParameter / SetAction / ProcessAction 62, Clear 92,
  AddFrameMember 124, SetHeader 145, AddEntry 152, SetItemCount 156 ...) are these.
- CManager slot 5 (row 0x0053B7F0 `RemoveInputListener`) is CObservable::RemoveObserver(CObserver*) (CManager : CObservable;
  CComponent's CObserver base at +4).
- ~CList 0x0053B7F0 calls CComponent::LoadAndReturnDef directly (a virtual called in a destructor binds statically):
  ?LoadAndReturnDef@CComponent@NUISystem@@UBE?AV?$CDefPointer@$$CBVCUIDef@NUISystem@@@@XZ (both PDBs); the def field +0x20A is
  CUIDef::PartOfListTree (was "RegisterInListRegistry"); the registry 0x013B8A98 is CList::GPreviousSelected.
- NPlayerGui::CDrawAbilityButtons slot 5 (row 0x0064DB2B `GetSlotOffsets`) is GetAbilityButtonMapping (CDrawBase PASS22: slots 0-1
  ClearPrimitivesIfNecessary / ClearPrimitives; Draw 2, GetAssignableSpecialMoveIndexForButtonDef 3, DrawAbilityList 4); the
  ability list is CTCInventoryAbilities::GetPrioritisedAbilityDefs 0x005F4B30 (interface 0x6F TCI_HERO_ABILITIES); the +0x144 read is
  the FableWin.pdb inline GetHUDAssignedAbilityLevel (overlay opt-in, body inferred from the in-place read only).
- 0x0059B5D7 is std::map<FRONTEND_SCREEN, NUISystem::CComponent*>::operator[] (FRONTEND_SCREEN is global); screens 7 / 21 are
  FRONTEND_SCREEN_PROFILES_MENU / FRONTEND_SCREEN_PROFILES_FOR_DELETE_PC; action types 0x125 / 0x124 / 0xDB / 0xD7 are
  UI_ACTION_TYPE_NEW_PROFILE / _GO_TO_MAIN_MENU_FROM_PROFILE_LIST / _NEXT_SCREEN_INVALID_PROFILE / _GO_TO_DELETE_PROFILE_SCREEN.
- RENAMES section 2 spellings not in the PDB function-name sets (vftables, statics, STLport internals; header spellings):
  ??_7CActionParamSLONG / ??_7CActionParamULONG / ??_7?$CIntelligentPointer@$$CBVCThing@@@@6B@, CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass,
  _Rb_global<bool>::_M_increment (the landed 0x004292C0), two _Rb_tree members, the function-static lastMapRadius.

## Blocked rows (all unchanged; locks released)
- 0x005907AB (e36, e37): blocked/port: std::map<long,long> with ctor / dtor / find / insert called out of line (no fable_map.h spelling)
  and the CTCInventory declarations CreateMenuDescPC / ContructItemListsPC; the console twin 0x0058F016 is not landed, the PC sibling
  0x005F45B5 has no map merge. 1 grade.
- 0x0053B321 (e36, e37): blocked/port: whole-row port onto engine/NUISystem_CList.h (local CComponentBase / CStateHolder / CCountedPointer / CListDef models;
  the LTCG find 0x0053C91C has a candidate spelling in stlport/fable_algorithm.h; slots 150 / 165 / 181 and def +0x13C / +0x221 unnamed). 1 grade.
- 0x00596CCE (e36): blocked/port: the same-TU bodies the row comment names (GetProfileNames 0x005955AB, ~vector<CWideString> 0x00412130,
  map operator[] 0x0059B5D7): on the shared headers of the landed twin 0x00598463 DIFFER 838v824 (index loop; retail strength-reduces
  it). 3 grades. Draft: work/00596cce_blocked.cpp.
- 0x00597006 (e36, e37, e34): the same blocker; DIFFER 540v546 on the shared headers (the form the row comment records). 1 grade.
  Draft: work/00597006_a.cpp.
- 0x00549B20 (nf44:76): blocked/port: the Initialize declaration (engine/NUISystem_CTextSlider.h has none), the CTextSliderDef fields
  and a same-TU _M_insert_overflow 0x0054A2A0 body; the local CComponent / CAction / list / vector models stay until then. 1 grade.

## Partial ports (graded, PASS; deferred remainder)
- 0x0053B7F0 ~CList: the CCountedPointer / CDefPointer / CUIDef models moved to the shared headers; the local CList / CComponent / list /
  find models stay (the fastcall same-TU find 0x0053C8D0 on std::list<CList*> has no shared spelling yet).
- 0x00599E3F CFrontEndManager::Update: the screen map moved to std::map<FRONTEND_SCREEN, NUISystem::CComponent*>; the values are read
  through the row's existing local CComponentBase view by C-style pointer casts; the self stays the global-namespace
  ?Update@CFrontEndManager@@QAEXM@Z (PDB NUISystem::CFrontEndManager::Update) until the whole class moves to engine/CFrontEndManager.h.

## Recheck warnings checked (recheck_final.log, 154 WARN lines)
- RELOC_NAME lines whose symbol is one of this slice's new spellings: GetBuildingThing 0x005BC4C4 (catalog label
  "GetMember_dword0xb8"; Ego_r + FableWin name it), the ability vector dtor 0x005F9F14 (label "StdList_Dtor"; a folded COMDAT), and
  CAIStateGroup_WorkBase::StayInShop 0x0094BEF0 (catalog / landed file label "IsInWorkHours"; engine/CAIStateGroup_WorkBase.h
  _STAYINSHOP documents 0x0094BEF0 as StayInShop: the shop-keeper body, Ego_r donor 0x009702A0 before Cleanup). The landed row
  rebuild/src/compiled/00/94/CAIStateGroupWorkBase_IsInWorkHours_0094bef0.cpp carries the stale name (name fix for the coordinator).
- Pre-existing, not changed: 0x00599E3F calls 0x00594FA9 as a local `EnableInput(bool)`; the catalog labels it
  NUISystem::CFrontEndManager::CloseVirtualKeyboard (declared CloseVirtualKeyboard(bool) in engine/CFrontEndManager.h).
- EH funclets: no file under rebuild/src or rebuild/tests #includes the repo file of any of the 48 changed rows (846 included row
  addresses checked), so no funclet needed a regrade.
