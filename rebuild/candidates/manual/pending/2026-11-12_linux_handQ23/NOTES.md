# Chunk Q23 notes

Result: 2 RELOCATION_MATCH (006c1250, 00814260), 1 ported DIFFER (0063f7c0, 785v785), 3 not ported (00a1d290, 00584b86, 00622a00).
Both matched rows: strict-header clean (LANDVERIFY_STRICT_HEADERS=1, overlay hdr/), tests PASS under VC7.1 behaviour_of and g++.

## 006c1250 CTCBuyableHouse::FrameUpdate (slot 9)
- Batch/catalog name `CTCBuyableHouse::Update` / `vfunc_9` is wrong: PDBs say FrameUpdate (CTCBase slot 9). Nothing landed uses the wrong name.
- Fixes from v1 DIFFER(673v638): HasTC forceinline opt-in; count_if fed const_iterators via PeekVillagePeople() const.
- Same-TU helpers (IsHouseBeingUsed/GetRentsDue/GetPriceForNextDressLevel/NeedsDressingSignPost) defined out of line here and auto-inlined by /O2.
  Coordinator: if those helpers ever land as their own rows (0x006BFB10/0x006BF810/0x006BF920), this TU's bodies must stay identical to them.
- CBuyableHouseDef is a local model (only a quarantine header exists).

## 00814260 CTCBoastingPodium::OnInitialActivate
- CTCBoastingPodium has no shared header: local class on shared CTCBase with Ego_r members.
- Residue 839v839 was register rotation only; using CTCBase::GetParentThing() (not ParentThing->) for player number and SetBoastingPodium matched.

## 0063f7c0 ForEachInRoughArea<..Bind3.., CThingFindNavigationLayerAt> (DIFFER)
- One residue: after the node loop retail reloads cells.TLX, then BRX, BRY, map; ours BRX, BRY, map, TLX. Same as Q19 on twin 0x008780D0.
- Coordinator: landed caller 0x0063E590 declares ForEachInRoughArea with `Proc&`; every PDB instance is `const Proc&` (ABV). The overlay
  body uses const Proc&; the existing declaration path is untouched.

## Not ported
- 00a1d290 CNavigatorAStar::CalculateRoute: heavy port (CAStarState statics NoOfStates 0x013D27B8 / NoOfStatesOnOpenList 0x013D27BC /
  NoOfStatesOnClosedList 0x013D27C0, CPriorityQueue Push 0x00A22460 / UpdateElement 0x00A21C30, map find 0x00A216A0 / insert 0x00A221B0).
  The old draft relies on stand-in auto_inline bodies.
- 00584b86, 00622a00: budget.

## Names
- No made-up function names. Local variable names are descriptive (as_workplace, last_name, script_name, position_number, number).
- NAME_FIXES: 0x008147D0 SetBoastingPodium (landed as _unattributed::AssignTarget_008147d0), 0x006C1250 FrameUpdate,
  0x00A12890 IsPointContainedInBoundingBox (catalog had a propagated CMainGameComponent::BeginInputLoading).
- Grades used: 006c1250 4, 0063f7c0 2, 00814260 4 (the 4th on each matched row is the final strict-header regrade of src/).
