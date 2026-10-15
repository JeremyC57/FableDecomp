# Hand chunk 149 (CMovableResource .. CObjectTypeCollectionPalette): 34 of 40 byte-exact

All 34 matched sources grade MATCH/RELOCATION_MATCH from src/ (grades.txt) and their tests print PASS under g++ (tall.sh) and the
lane's VC7.1 cl.exe (tall.py, tall_vc.txt). Shared probe models (inlined into each src file by `inl.py`): probe/nav_model.h,
probe/navleaf.h, probe/navqtn.h; the CNOVI Init rows come from `nov.py` + probe/nov_specs.py (GSI slot table generator).

## What matched (34)
- Movable resources: 0x00ADF460 AllocateParticles (owner with handle +0x138 / pool +0x13c / array +0xc0): the 64-bit guard is an
  `unsigned __int64` product compared with 0xFFFFFFFF; the handle member is read inline as the argument (a named local moves it into
  ebx). 0x00A62E40 CMovableResourceMemoryPool::AllocateResource: BeginCritical(`(flags & 1) != 0`), Allocate, InternalSetResource,
  EndCritical on both paths.
- NUISystem::CMovie (retail = Ego_r.pdb minus 0x30, like CTextContainer): GetDataFromDef 0x00547C10 reuses the landed 0x00546CD0 def
  temporary (DecRef/GetRefCount inline release, no null test once dereferenced; the float sizes go through __ftol2). The ctor
  0x005482D0 `(CIEngine&, long)` and the copy ctor 0x00548350 (label "CopyConstructor") are three vtables + members in declaration
  order (Playing, CEnginePrimitiveHandle Handle with its inline ctor, CWideString, CXMVPlaybackInfo, AttempToPlay), the engine kept in
  a static (0x013B8AB8, value ctor only), then GetDataFromDef. The classes must sit in `namespace NUISystem`.
- CMusicManager SetLoopPoints 0x00C43420 / SetVolumeParams 0x00C434A0 (both MATCH): set/clear an optional record through
  (current**, storage*, value*), dirty bit at +0x14; the change test is the negated inline `operator==` on the record.
- CNOVI quest-script Init rows (0x00DB0950 AffairMan, 0x00DB1E80 AffairWoman, 0x00DAEEB0 Victim, 0x00DB3F00 BookTrader,
  0x00DAC390 LiveFather): the landed CFH_GuildMaster::Init model (Me by value only for SetIsPushableByHero), GSI slots 360/496/516/517/
  526/529/595/844; flags/long cleared first in member order. Classes pinned through vtable -> factory -> allocation size.
- 0x00DAEBF0 CNOVI_Villager::GetVillagerSpeechIndex: `while (!IsDone()) { idx = rand() % n; if (idx != last) { if (IsDone()) return 0;
  last = idx; return IsDone() ? 0 : idx; } Game->Update(); } return 0;` (ParentClass lastVillagerSpeechIdx at retail +0x98).
- Navigation: CNavConnectorNode ctor 0x00A78A90 (members as initialisers, inline CSmallVector push_back = `resize(size()+1)` with the
  default `T()` fill homed in the dead first-argument slot + `data[size-1] = v`, first region a named local);
  CNavNavigableLeafNode AddNavigableNodeNeighbouringSwitchableNodeToRegion 0x00A76490 (MATCH; `if (GetRegionID() == 0) {loop} return
  false;` puts the early exit on the shared tail) and SetUpNeighboursWithLeaf 0x00A76B90 (the leaf is the SECOND argument: the
  fill temp lives in the dead first slot); CNavQuadTree::CalculateNeighbours 0x00A769A0 (`++i` right after the children call, then
  `i % width` / `i > width` with `nodes[i - width - 1]`); CNavQuadTreeNode GetMemoryUsage 0x00A761C0 (MATCH; statement-by-statement
  sum from `0x20 + TR`), AddToIndexTable 0x00A789F0 (`insert(make_pair(this, index))`), ctor 0x00A7A570 (LeafNode(true) set by the
  inline CANavQuadTreeNode ctor; C2DVector memberwise copy ctor); CNavSwitchableLeafNode::SaveToFile 0x00A77C30 (count homed in the
  dead C2DVector& slot); CNavigationInput::operator== 0x006CCF70 (`__forceinline` C3DVector tolerance compare with `<=`, the scalars
  `<`, `if (...) return true; return false;` for byte exits); CNavigatorAStar::GetCostFromNodeToNode 0x00A1A0A0 (named distance,
  factor 1.0f, switch 0/0x40/0x80/default); CNavigatorBase ctor 0x00A47110 (flags/manager initialisers, Clone and `new CSteeringAgent`
  in the body); CNavigatorManager::RemoveNavigationMap 0x00A155D0 (STLport list walk remembering the match, inline erase with the
  implicit ~CNavQuadTreeMap = ~CCharString + inline CCPPointerInfo release).
- 0x00850280 (label CNavigatorQueueManager::FrameUpdate): STLport `list<pair<long, list<long> > >::clear()` with the inner list's
  destructor inlined; explicit template instantiation, leaf `?clear@?$list@U?$pair@JV?$list@J`.
- CNewFrontendGameComponent: Update 0x0042DC94 (y-off + s size TU; float dt local clamped by `if (dt > 0.1f)`; the NUISystem manager
  re-fetched per use); ~CNewFrontendGameComponent 0x0042F50B (size TU; PDB layout holds to +0x6c, STLport 12-byte vectors after it;
  the def release, the POD vector free and the base destructor must be `__forceinline` to inline in the size TU).
- COF_MainGates::Main 0x00CDF080: CIsThingAlive filter block, Update, then a CCharString block whose result is
  `IsQuestActive(name) ? false : true` (8-bit neg/sbb/inc kept in bl), then OpenDoor.
- CObject::OnAdd 0x006C20B0: STLport vector of 28-byte entries with `size()` per iteration and `*(begin()+n)`.
- 0x00B38780 (label ~CObject): CEnginePrimitiveHandle `Clear(); Data = p;` over the landed 0x00BBC310 model (MATCH).
- Local-detail cache: 0x00BDD860 CLocalDetailCacheMap::CObjectTypeCollection::RenderUpdate (the named `insideFade` bool homed in the
  dead `near` slot); 0x00BDEB50 CObjectCacheGroupCollection::StaticUpdate (three float deltas + `sqrt(dx*dx+dy*dy+dz*dz) - Radius`
  float local; load/unload); 0x00BDE600 CObjectTypeCollection::CreatePrimitiveOfType (`switch` 0/1/default, `new (this) T(type, map)`
  through each primitive class's static `__fastcall operator new(size_t, CMovableResource*)`).

## Parked for Fable 5.1 (5)
- 00a77b00 CNavNavigableLeafNode::MakeRegion DIFFER(122v125): ours pushes ebp/esi/edi at first use (ebp after the PRegion store, esi/edi inside the loop guard), retail pushes all three at entry and keeps the early `xor al,al` exit at the tail | tried: early-return and if-wrapped bodies, STLport-5 list insert (prev loaded once -- fixed the link stores), a `made` result local | next: lane gap: callee-saved push placement (try a do/while loop or the loop over an inline Find helper)
- 00a0e790 std::__copy_aux<_Deque_iterator<C2DVector>> DIFFER(120v118): the empty random_access_iterator_tag temporary passed by reference: retail addresses it at the dead `result` argument slot (`lea edx,[esp+5Ch]`, no frame), ours opens a `push ecx` frame word (all offsets +4, `pop ecx`) | tried: tag with/without user ctor, STLport tag inheritance chain, iterator_category()/distance_type() helpers | next: lane gap: empty-class temporary placed at an existing argument slot
- 00a0ed80 std::__copy_aux<_Deque_iterator<8-byte T>> DIFFER(120v118): same tag-temporary frame word as 00a0e790 (twin instantiation) | tried: the 00a0e790 forms (one grade) | next: lane gap: empty-class temporary placed at an existing argument slot
- 00720610 std::__pop_heap<pair<EHeroMorphType,CTextureMorphEntry>*> DIFFER(116v116): retail loads the value's second word (`mov ecx,[esp+18h]`) before `sub esp,18h`, ours after it (`mov ecx,[esp+30h]`) -- scheduling only | tried: implicit copies, STLport pair memberwise copy ctor, CTextureMorphEntry memberwise copy ctor | next: lane gap: argument-area copy scheduling
- 00bdef40 CLocalDetailCacheMap::CObjectTypeCollectionPalette::GetIndexFromType DIFFER(125v123): the empty __true_type temporary for `_M_insert_overflow` -- retail gives it the address of the `type` argument slot (same as the value), ours a `push ecx` frame word (offsets +4, loop-head padding `jmp ; lea ecx,[ecx]` becomes `lea esp,[esp]`) | tried: tag with a user ctor, a named uninitialised POD tag | next: lane gap: empty-class temporary placed at an existing argument slot

## Skipped
- 0042e3ee CNewFrontendGameComponent::Input: prefix row of a larger EH-framed function (`and [ebp-4],0` state, the 101 bytes end in
  `jmp 0x42e803` and branch to 0x42e98c, both past the row end).

## Label corrections
- 0x00DAEEB0 is CNOVI_Victim::Init (catalog: CNOVI_BookTrader::Init): two cleared bools +0x1c/+0x1d and EntitySetAsScared; vtable
  0x012D87B8, factory 0x00DAEDE0 (0x20 bytes). 0x00DB3F00 is the real CNOVI_BookTrader::Init (TRADER_* strings by vtable 0x012D8E80).
- 0x00548350 "CMovie::CopyConstructor" = `NUISystem::CMovie::CMovie(const CMovie&)`; all CMovie rows are NUISystem::CMovie.
- 0x00A0E790 / 0x00A0ED80 "CNavigatorAStar::MoveToNavLineOfSightProximity" are STLport `__copy_aux` over deque iterators of 8-byte
  elements (`__fastcall`, hidden result in ecx); their callee 0x00A0E460 is `__copy`.
- 0x00850280 "CNavigatorQueueManager::FrameUpdate" is STLport `list<pair<long, list<long> > >::clear()` (0x008502F0/0x00850310 are
  that list's destructor).
- 0x00B38780 "CObject::~CObject" is a CEnginePrimitiveHandle member `Clear(); Data = p;` (takes one argument).
- 0x00720610 "CObjectAugmentationParticleSet ctor" is `__pop_heap<pair<EHeroMorphType,CTextureMorphEntry>*>`.
- 0x00BDD860 "CObjectCacheGroupCollection::RenderUpdate" is CLocalDetailCacheMap::CObjectTypeCollection::RenderUpdate (the real
  CObjectCacheGroupCollection::RenderUpdate is its caller 0x00BDD930).
- Callees: 0x00A79290 is CNavQuadTree::OnNavMapUnload(long) (not CVectorMap_EraseRange_Keys); 0x00A7DB10 CSmallVector::resize;
  0x0041E61B static `__fastcall` NUISystem ShowErrorMessage forwarder; 0x00595A03 is a front-end-manager bool query (not
  CThingFilter_IsUnconscious); 0x00BF9900/0x00BF9980/0x00BF9A00 are the CLocalDetailPrimitiveMesh/RepeatedMesh/MeshZSpriteBatch class
  `operator new(size_t, CMovableResource*)` (not _Cons_val); 0x00BDFB50 is the CLocalDetailPrimitiveMesh ctor (0x74-byte allocation;
  label RepeatedMesh is a twin); 0x005BA48A is the CCompositeBlendDesc copy ctor; 0x00A1C700 CSteeringAgent ctor.
