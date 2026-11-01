# Hand chunk 663 (CFaction .. CEngineScene): 10 of 10 byte-exact

All ten rows carried the propagated catalog label `CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip`; every one is a
different class's destructor (label wrong for 10/10). Identity came from the caller (a vtable slot-0 `??_G`, or a non-virtual
`??_G` / `_Destroy` / `checked_delete` beside the body), the vtable the body stores, the RTTI direct base the tail jump
reaches, and the Ego_r.pdb / FableWin.pdb `??1` spellings. Member layouts are the Ego_r.pdb members re-laid at retail STLport
4.6 sizes (Ego_r is Dinkumware: 16-byte map/set/vector, 12-byte list; retail 12 / 12 / 12 / 4), which reconciles every offset
the bodies touch (and the ctor/`operator new` sizes where present). All ten are RELOCATION_MATCH under `q.py` and under the
read-only `vland.py` run (10 WINS, behaviour PASS under g++ and VC7.1; no RELOC_IDENTITY, no SHARED_HEADER warning left).

## What matched (10)
- **CFaction** 0x006BA6D0 `??1CFaction@@QAE@XZ` (129 B): compiler-generated dtor of the non-polymorphic faction record
  { CCharString Name; set<CCharString> AlliedFactions; set<CCharString> EnemyFactions } (+0/+4/+0x10). Shape: STLport 4.6
  `_Rb_tree_base` / `_Rb_tree` text (`~_Rb_tree() { clear(); }`, `~_Rb_tree_base()` deallocating the header) gives the
  `xor edi,edi` shared-zero form by itself; TU harness destroys a local.
- **NCameraModes::CPhysicsForceModelBase** 0x00850360 `??1CPhysicsForceModelBase@NCameraModes@@QAE@XZ` (137 B): implicit,
  non-virtual dtor (the class has virtuals but no virtual dtor: FableWin's ??1/??_G are QAE, boost::scoped_ptr deletes it).
  Three inline CPhysicsCollisionPointsObject dtors, each three STLport lists. Shape: the real STLport `_List_base` text
  (out-of-class `clear()`, `~list() {}`) and VC7.1's own inliner reproduce the mixed call pattern exactly (calls to
  ~_List_base<pair<float,long> > and ~_List_base<C3DVector>, inline ~_List_base around an out-of-line clear() for
  pair<float, list<C3DVector> >). `boost::checked_delete<CPhysicsForceModelBase>` (__fastcall, retail 0x00850510) is the
  TU function that emits the ??1 -- a real retail function rather than a harness.
- **CScriptedMapBrush** 0x0082E780 `??1CScriptedMapBrush@@UAE@XZ` (215 B): user dtor (vtable store) over engine/CBaseObject.h
  (FABLE_RETAIL_BASE_OBJECT); members BrushName, three trees, the trivially destroyed Cells vector.
- **NThingMap::CThingMapManager** 0x0051A0A0 `??1CThingMapManager@NThingMap@@UAE@XZ` (109 B): compiler-generated (no vtable
  store), set<long> + two CArray (STLport vector) members, tail jump to ~CBaseClassNonCopyable. Immediate-zero form (`test
  eax,eax`, `mov [x],0`) from the same tree text when only one tree is in the function.
- **NUISystem::CTable** 0x00550D50 `??1CTable@NUISystem@@UAE@XZ` (100 B): user dtor; three vtable stores (CUIInterface /
  CObserver +4 / CActionParam +0x18) from the local MI base chain, Sprites map<ETableSprites, CCountedPointer<CComponent> >.
- **CEngineLightingManager** 0x00B47B50 `??1CEngineLightingManager@@UAE@XZ` (110 B): user dtor `DeallocateAligned(SIMDVarBlock);
  SIMDVarBlock = 0;` over two bases (CEnginePrimitiveManager with compiler-generated dtors down to CBaseClassNonCopyable,
  CEngineComponent with its out-of-line virtual dtor); three CArrays.
- **CCombatManager** 0x006ED480 `??1CCombatManager@@UAE@XZ` (143 B): user empty dtor; DialogueManager (+0x38) destroyed
  first, then the two maps. Needed CombatDialogueManager with its real members (an opaque byte body put it at +0x35).
- **CTCTavernTable** 0x007B9D30 `??1CTCTavernTable@@UAE@XZ` (137 B): compiler-generated; STLport vector<bool>
  (`_Bvector_base`), two vector<CIntelligentPointer<CThingObject> > (explicit virtual element dtor `push 0; call [eax]`, end
  cached in a register by the by-value `_Destroy(first,last)`), vector<CThingAICreature*>.
- **CTCShop** 0x007783F0 `??1CTCShop@@UAE@XZ` (145 B): compiler-generated; Stock map<long, CShopItem>, GoodsPlaces,
  Shoppers (virtual element dtors), PShopKeeper (direct ~CBaseIntelligentPointer call).
- **CEngineScene** 0x00B94B50 `??1CEngineScene@@UAE@XZ` (159 B): user dtor clearing FirstMovingPrimitive and
  FirstNonPersistentPrimitive through the static __fastcall CEngineInternalPrimitiveBase::ClearPrimitiveList, then
  RenderStats, Name, MapList.

## Proven
- 0x006BA6D0 ??1CFaction@@QAE@XZ -- FableWin.pdb 0x02444350 (+ ??_GCFaction@@QAEPAXI@Z 0x02445980); retail non-virtual ??_G 0x006BA760 and `_Destroy` jmp 0x006BAA50 beside the ctor thunk 0x006BAA40 (??0CFaction 0x006BA420); Ego_r CFaction members = the three members destroyed; already bound so in reloc_bindings.
- 0x00850360 ??1CPhysicsForceModelBase@NCameraModes@@QAE@XZ -- FableWin.pdb 0x0288D2B0 (QAE) with ??_G 0x0288D270, checked_delete / scoped_ptr; retail ??_G 0x008503F0, checked_delete 0x00850510, ~scoped_ptr 0x00850530, reset 0x00850550, CPhysicsLeaf ctor 0x0085C950 call it; ctor 0x00850410 builds the same three 0x24-byte members.
- 0x0082E780 ??1CScriptedMapBrush@@UAE@XZ -- Ego_r.pdb 0x00735640 / FableWin.pdb 0x027C9350; stores vtable 0x01272518 whose slot-0 ??_G 0x008270C0 calls it; RTTI : CBaseObject, tail ~CBaseObject 0x0099A430.
- 0x0051A0A0 ??1CThingMapManager@NThingMap@@UAE@XZ -- Ego_r.pdb 0x004E7700 / FableWin.pdb 0x01C78890; sole caller ??_G 0x0051A080 = slot 0 of vtable 0x01244678 (RTTI NThingMap::CThingMapManager : CBaseClassNonCopyable).
- 0x00550D50 ??1CTable@NUISystem@@UAE@XZ -- Ego_r.pdb 0x005CB2B0 / FableWin.pdb 0x0228E530; sole caller ??_G 0x005540D0 = slot 0 of vtable 0x0124A224 (RTTI NUISystem::CTable); stores the three CTable vtables; tail ~CChangingStateComponent 0x0052CCF0.
- 0x00B47B50 ??1CEngineLightingManager@@UAE@XZ -- Ego_r.pdb 0x009BD7B0 / FableWin.pdb 0x02CED320; sole caller ??_G 0x00B4A4A0 = slot 0 of vtable 0x012A2274; stores 0x012A2274 / 0x012A223C (RTTI bases CEnginePrimitiveManager @0, CEngineComponent @0xC).
- 0x006ED480 ??1CCombatManager@@UAE@XZ -- Ego_r.pdb 0x00717340 / FableWin.pdb 0x02417A40; sole caller ??_G 0x006F5120 = slot 0 of vtable 0x01261EB4 (RTTI : CBaseClassNonCopyable).
- 0x007B9D30 ??1CTCTavernTable@@UAE@XZ -- Ego_r.pdb 0x007B8C20 / FableWin.pdb 0x026EE7A0; sole caller ??_G 0x007B9D10 = slot 0 of vtable 0x0126DBB4 (RTTI : CTCBase).
- 0x007783F0 ??1CTCShop@@UAE@XZ -- Ego_r.pdb 0x00784050 / FableWin.pdb 0x02618F90; sole caller ??_G 0x007783D0 = slot 0 of vtable 0x0126A94C (RTTI : CTCBase).
- 0x00B94B50 ??1CEngineScene@@UAE@XZ -- Ego_r.pdb 0x00A47C40 / FableWin.pdb 0x02E1D720; sole caller ??_G 0x00B98A10 = slot 0 of vtable 0x012A3E68 (RTTI : CBaseClassNonCopyable).
- 0x0099A300 ??1CBaseClassNonCopyable@@MAE@XZ -- PDB order (??0/??1 CBaseClassNonCopyable after the CBaseClass pair, mirrored by retail 0x0099A2D0..0x0099A300; 0x0099A2F0 stores its vtable 0x0129A7C4); ~CTCBase 0x00686830 and four RTTI-direct subclasses' dtors end in it.
- 0x00686830 ??1CTCBase@@UAE@XZ -- Ego_r.pdb 0x006B1BE0; stores CTCBase vtable 0x0125BE0C, then jmp ~CBaseClassNonCopyable.
- 0x0052CCF0 ??1CChangingStateComponent@NUISystem@@UAE@XZ -- stores the three CChangingStateComponent vtables 0x01245DE4/0x01245DBC/0x01245DB4 (RTTI); CTable's direct base.
- 0x00B8FEE0 ?ClearPrimitiveList@CEngineInternalPrimitiveBase@@SIXPAV1@@Z -- Ego_r.pdb 0x00A443E0 / FableWin.pdb 0x02E35980; called by ~CEngineScene with the list heads in ecx.
- 0x008FB170 ??0CPhysicsCollisionPointsObject@NCameraModes@@QAE@JMMJJJ@Z -- Ego_r.pdb 0x00920630 / FableWin.pdb 0x02B1E7E0; called by the CPhysicsForceModelBase ctor 0x00850410 on +0x08/+0x2C/+0x50.
- 0x006F0DD0 std::_Rb_tree<CCharString, pair<const CCharString, CCombatSequenceBase* (__fastcall*)(CThingCreatureBase&, CThingCreatureBase&)>, ...>::_M_erase -- erases CCombatManager::RegisteredCombatSequences (Ego_r member type, retail +4).
- 0x004A9CB0 set<long> _M_erase (CThingMapManager::ActiveGameMaps); 0x004ABA70 set<CCharString> _M_erase (CFaction's sets);
  0x00776AA0 map<long, CShopItem> (CTCShop::Stock); 0x00824830 set<unsigned __int64> (CScriptedMapBrush::ThingsToAdd /
  ThingsToDelete); 0x00824870 map<unsigned __int64, CScriptedMapBrush::CThingPosAndOrientation> (ThingsToMove); 0x00B95EE0
  map<EEngineRenderLayer, CEngineSceneRenderStats> (CEngineScene::RenderStats); 0x00B98350 map<long, map<long,
  CCountedPointer<CEngineSceneMap> > > (CEngineScene::MapList) -- each STLport `_M_erase`, the instantiation given by the Ego_r
  member type of the tree the owning destructor erases through it (mangled spellings in NAME_FIXES.tsv).
- 0x008501E0 ~_List_base<pair<float,long> >, 0x00850220 ~list<pair<float,long> > (`jmp` thunk), 0x00850280
  _List_base<pair<float, list<C3DVector> > >::clear, 0x0048CA60 ~_List_base<C3DVector>, 0x0048CF80 ~list<C3DVector> (`jmp`
  thunk) -- STLport list instantiations of CPhysicsCollisionPointsObject's three Ego_r list members; the ~_List_base vs ~list
  split follows the jmp-thunk pairs and is what VC7.1 calls from the STLport text.
- Descriptive (not proven): `VirtualSlot0_008fa660` (CPhysicsForceModelBase vtable slot 0, only needed so the class has a vptr);
  the ctor parameter names; the `TUHarness_<Class>` functions in 0x0051A0A0, 0x006BA6D0, 0x007783F0, 0x007B9D30 (not retail).

## Parked for Fable 5.1 (0)

## Skipped
None (every row is one function; no padding inside a row).

## Label corrections
All ten catalog rows `CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip` -> the ten `??1` above. Callee labels:
0x0099A300 "CBaseClass::~CBaseClass" -> ~CBaseClassNonCopyable; 0x00686830 -> ~CTCBase (already); 0x0052CCF0
"CTCIdleScheduler::Clear" -> NUISystem::CChangingStateComponent::~CChangingStateComponent; 0x0048CA60
"StdList_Clear_0048ca60" / "CGuiControlTreePane::SortTreeRecursively" -> std::_List_base<C3DVector>::~_List_base; 0x008501E0
"CGuiControlTreePane::SortTreeRecursively" -> std::_List_base<pair<float,long> >::~_List_base; 0x00850280
"CNavigatorQueueManager::FrameUpdate" -> std::_List_base<pair<float, list<C3DVector> > >::clear; 0x008FB170
"PhysicsForceModel_Initialize" -> CPhysicsCollisionPointsObject ctor; 0x00B8FEE0 "_global::ClearPrimitiveList" ->
CEngineInternalPrimitiveBase::ClearPrimitiveList; 0x00824830 / 0x00824870 / 0x00776AA0 / 0x004A9CB0 / 0x00B95EE0
"LTextTreeWalkThrough::BuildTreeArray" and 0x006F4C20 / 0x00B8FEE0 "ClearPrimitiveList", 0x00B98350
"Utility_DestroyRefcountedTreeNode" -> the STLport `_Rb_tree::_M_erase` instantiations above; 0x006F2F60
"CCreatureAction_PerformExpressionExtended" dtor -> ~CombatDialogueManager (landed source already says so); 0x0099EAE0
"_Cons_val<...CParticleMorphs...>" -> ~CCharString; 0x00850510 "CTCMapwho::OnDie" -> boost::checked_delete<CPhysicsForceModelBase>;
0x00850530 / 0x00850550 -> scoped_ptr<CPhysicsForceModelBase> dtor / reset; 0x006BAA50 -> std::_Destroy<CFaction>.

## Header debt
- engine/CTable.h, engine/CThingMapManager.h, engine/CScriptedMapBrush.h, engine/CEngineLightingManager.h, engine/CTCBase.h,
  engine/CObserver.h, engine/CEngineComponent.h, engine/CEngineInternalPrimitiveBase.h are flattened data structs (no base
  classes, containers as byte arrays, CScriptedMapBrush still 16-byte containers); the polymorphic base chains and STLport
  members are local models with the header offsets/names (`// Layout:` lines). A retail opt-in model of CTCBase,
  CChangingStateComponent/CComponent (MI: CUIInterface, CObserver +4, CActionParam +0x18) and CEngineComponent would let these
  rows include them.
- The STLport 4.6 `_Rb_tree` / `_Vector_base` / `_Bvector_base` / `_List_base` text is repeated per source (no shared
  rebuild/include STLport header exists).
