# Chunk 659 notes (agent659)

| row | grade | probes | file |
|---|---|---|---|
| 0x00626E40 CTCQuickAccessMenu::ConstructItemCategoryList | DIFFER(1513v1471), 53 masked lines | 17 + 16 diagnostics | differ/00626e40.cpp |
| 0x006276F0 CTCQuickAccessMenu::GetItemsListUI | RELOCATION_MATCH, test PASS (g++ and VC) | 2 | src/006276f0.cpp, test/006276f0.cpp |
| 0x00935E40 CAIStateGroup_MinionPutOutFires::PutOutFire | DIFFER(476v468), 18 lines | 15 (+16 prior) | differ/00935e40.cpp |
| 0x009363D0 CAIStateGroup_MinionWatchFires::Update | DIFFER(442v438), 10 lines | 12 | differ/009363d0.cpp |

Pairs are authored as retail's TUs: src/006276f0.cpp and differ/00626e40.cpp are the same translation unit (both bodies), and
differ/00935e40.cpp and differ/009363d0.cpp are the same TU (both bodies). 0x006276F0 matched only after the real
ConstructItemCategoryList body went into its file. The owner's same-TU hypothesis held: the earlier 1244v1260 park came from an
invented declared-only callee.

## Proven
All four leaves are byte-identical to FableWin.pdb mangled names (objdump -t of the built object against ghidra_out/fablewin_pdb_names.tsv).
- 0x00626E40 ?ConstructItemCategoryList@CTCQuickAccessMenu@@AAEXAAVCWideString@@PAVCList@NUISystem@@AAV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@PAVCComponent@4@_N@Z -- FableWin + Ego_r name, built leaf identical; called 5x from 0x006276F0 with the five TEXT_GUI_MENU_* titles
- 0x006276F0 ?GetItemsListUI@CTCQuickAccessMenu@@AAEPAVCComponent@NUISystem@@AAJPAV23@@Z -- FableWin name, built leaf identical, RELOCATION_MATCH
- 0x00935E40 ?PutOutFire@CAIStateGroup_MinionPutOutFires@@AAE_NXZ -- FableWin/Ego_r name, built leaf identical
- 0x009363D0 ?Update@CAIStateGroup_MinionWatchFires@@UAEXXZ -- FableWin/Ego_r name, built leaf identical; the class vtable's Update slot
- 0x00626110 ?ContructItemLists@CTCQuickAccessMenu@@AAEXAAV?$map@JJU?$less@J@std@@V?$allocator@U?$pair@$$CBJJ@std@@@2@@std@@PAVCList@NUISystem@@PAVCComponent@5@AAV?$CCountedPointer@VCComponent@NUISystem@@@@@Z -- PDB spelling "Contruct" (sic; FableWin also has a vector overload), built call symbol identical; the call at the end of 0x00626E40 with (merged map, sub list, parent, actions)
- 0x0041E5F2 ?GetInstance@CManager@NUISystem@@SIPAV12@XZ -- FableWin static __fastcall; allocates 0xD0 = Ego_r sizeof(CManager)
- 0x0041DB1D NUISystem::CManager::CreateComponent and 0x00428494 CreateMenuEntryComponent -- FableWin names, argument shapes match the pushes in 0x00626E40 / 0x006276F0
- 0x005CC772 ?GetXString@CTCInventoryStats@@SI?AVCWideString@@XZ -- only spelling in both PDB name sets; no this, hidden result, TEXT_GUI_MENU_XSTRING
- 0x005D9830 ?IsExpression@CTCInventoryItem@@SI_NJ@Z -- calls GetSubDef<CExpressionSubDef> 0x005DA240; the manifest label agrees
- 0x005D96F0 ?GetUIInventoryCategory@CTCInventoryItem@@SI?AW4EUIInventoryCategory@@J@Z -- reads CInventoryItemDef +0x6c = Ego_r UIInventoryCategory +108
- 0x005BF482 NInventory::CTCInventoryBase::GetAllItems const -- FableWin name; fills the CItemVector in 0x006276F0
- 0x009C95E0 NGameText::CDataBank::GetTextBySymbol const -- FableWin name; reached through g_pMainGameComponent (0x013B86A0, ESTABLISHED_SPELLINGS) ->PTextBank
- 0x0042CD84 ??0CDeletion@NUISystem@@QAE@ABV01@@Z -- copy of the by-value SetDeletion argument (FableWin ?SetDeletion@CComponent@NUISystem@@UAEXVCDeletion@2@@Z); the same use at 0x00545B10
- 0x0042ABCA std::list<NUISystem::CComponent*>::~list -- the CDeletion::AssociatedParents destructor retail calls at both sites in 0x00626E40
- 0x004E539F std::_Rb_tree<long,...>::insert_unique(const value_type&) -- map<long,long>::insert in the 0x00626E40 merge loop
- interface 8 = CTCTargetingBase (FableWin ??$PeekTC@$$CBVCTCTargetingBase@@@CThing@@...; GetPTargetedThing at vtable +0x84); interface 0x11 = CTCInventory (StatGetInterfaceType vtable slot at 0x00594BC9 returns 0x11); interface 0x6d = CTCFlammable
- 0x00780400 ?IsOnFire@CTCFlammable@@QBE_NXZ, 0x004C73D0 ?GetPos@CThing@@QBEABVC3DVector@@XZ, 0x00A01B50 ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ -- FableWin names, reloc bindings in both Minion bodies
- 0x0097E890 AIAction_MoveToPosProximity, 0x0097D8B0 AIAction_PerformAction(const CCreatureActionBase&), 0x0097DB70 AIAction_PerformActionAndWait(const CCreatureActionBase&, bool), 0x0097E200 AIAction_DoNothing, 0x008FCF40 Terminate -- CAIStateGroupBase PDB names (FableWin), protected IAE except Terminate (QAE)
- 0x008424F0 ??0CCreatureAction_PlayAnimation@@QAE@AAVCThingCreatureBase@@ABVCCharString@@ABVCSoundPair@@_N33JJ@Z, 0x00842270 CCreatureAction_AlertToPlayer(CThingAICreature&) -- FableWin
- IS_THING_ALIVE<CIntelligentPointer<CThingBuilding> > (FableWin ??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThingBuilding@@@@@@YI_N...) and operator!=(const CIntelligentPointer<CThingBuilding>&, int) (??$?9VCThingBuilding@@@@YI_N...H@Z) -- both inlined at every site in retail (3 GetPItem calls per test)

Descriptive (not proven): the bool/long argument names of the PlayAnimation ctor and MoveToPosProximity (a, b, c, d, e);
enumerator values of EAIActionReturn / EAIActionRepeat / EMovementType / EDeletionMethod (written as casts, no names in the
name sets); local variable names.

## Label corrections
- 0x005B2A37's header swaps the roles of 0x004E539F and 0x004E5425: 0x004E539F is _Rb_tree::insert_unique(const value_type&),
  0x004E5425 is _M_insert.
- 0x005907AB (landed) models vtable slot 62 as CAction/SetAction; it is NUISystem::CComponent::SetDeletion(CDeletion) by value.
- Prior 006276f0 used CFrontEndManager::GetInstance and IsAugmentation for 0x0041E5F2 / 0x005D9830 (see NAME_FIXES.tsv).
- The prior Minion files used CThing::GetPosition / m_flags91 / GetInterfaceInline; retail names are GetPos, Dead (+0x91 bit 0), GetTC/PeekTC.

- 0x00428B10 is labelled `_global::DeleteViaVtable0_00428b10`; it is ?DeleteData@?$CCountedPointer@VCComponent@NUISystem@@@@SIXPAX@Z
  (FableWin; bound by the CCountedPointer<CComponent> releases in 0x006276F0, vland RELOC_NAME).
- 0x00626E40 is labelled `CTCQuickAccessMenu_PopulateQuickAccessList`; PDB name CTCQuickAccessMenu::ConstructItemCategoryList (vland RELOC_NAME).

## Header debt
- vland WARN SHARED_HEADER_AVAILABLE CManager / CTCInventoryItem: engine/CManager.h is a global-namespace flat layout (it declares
  CTCInventory / CTCHeroExperience members, not NUISystem::CManager's), and only static __fastcall CTCInventoryItem members are called
  here (IsExpression, GetUIInventoryCategory), so neither header models what the body uses; the local declarations stay.
- CTCQuickAccessMenu has no engine header; modelled locally on engine/CTCBase.h (ParentThing) with Ego_r member names.
- CThing has no engine header; local model with Ego_r names (UsingComponents +0x20 std::bitset<274>, Components +0x44 CInterfaceMap,
  Dead +0x91).
- engine/CAIStateGroupBase.h and engine/CAIStateGroup_MinionWatchFires.h are flat `#pragma pack(1)` layout headers (a `__vftable`
  member, opaque PBuildingOnFire bytes) and cannot carry the virtual Update or the CIntelligentPointer member functions; local models
  with the header's names (PBrain +4, PBuildingOnFire +0x14). Needs a generator mode that emits polymorphic classes.
- NUISystem::CComponent / CList / CDeletion: no compilable header with vtable slots; local slot lists (FindChild +0x0C, AddChild
  +0xEC, SetDeletion +0xF8, Clear +0x170, AddScrollingChildAndSetOffsets +0x260, InitialiseOffsets +0x270, SetSelectedChild +0x278).
