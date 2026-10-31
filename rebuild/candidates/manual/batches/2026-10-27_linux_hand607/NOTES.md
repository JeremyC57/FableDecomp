# Hand chunk 607 (CAIStateGroup_GoToBed .. CTCWife): 1 of 10 byte-exact

Chunk 607 was 10 PARKED rows from the 2026-10-30/31 retry sweep, every best attempt already the retail length. Each best
file was re-graded first (all ten reproduced their recorded residue), then changed one lever per grade.

## What matched (1)
- **0x00C1B7D0 PKCTRRedblackMap::Remove** (libpk keyed red-black map, catalog `CGameMap_RemoveByKey` is propagated) --
  RELOCATION_MATCH, test PASS (g++ and VC7.1), vland RELOCATION_MATCH + PASS. Lever: write the last-found cache test as its
  own arm that assigns z (`if (LastFound != 0 && LastFound->Key == *key) z = LastFound; else { search }`). The parked
  `z = LastFound; if (z == 0 || z->Key != *key)` form puts z in ecx; retail keeps z in edx and uses ecx as scratch. This
  was the first grade of the session's third lever. The same-TU noinline Release stand-in stays: with only a declaration
  the body grows to 231 bytes.
  Names (all DESCRIPTIVE, libpk has no PDB): the row's own symbol is the bound `?Remove@PKCTRRedblackMap@@QAEJPBK@Z`. The
  callees use the landed symbols exactly: `?DeleteFixup@PKCTRRedblackMap@@QAEXPAUPKCTRRedblackMapNode@@@Z` (0x00C1B560)
  and `?Release@CMapNodeAllocator@@QAEXPAPAUPKCTRRedblackNode@@@Z` (0x00C1B1E0). The parked file's `PKCTRAllocator::Release`
  was a second spelling for 0x00C1B1E0 and is gone. Remove passes `(PKCTRRedblackNode**)&y` to match the landed node type.

## Parked for Fable 5.1 (9)
- 0092d6f0 CAIStateGroup_GoToBed::IsRunnable DIFFER(198v198): `mov ecx,edi` sits below FindFreeBed's jne (retail hoists it above, then jumps into the middle of the OnReplace block) | tried: bool local, nested if, two-block success form (it hoists, but this/&PBed swap esi/edi, nd 42), PBed GetPItem pair, `self` alias, unnamed map, bed pointer, named stamp, named creature | next: the retail hoist needs two OnReplace blocks at allocation time; find what gives `this` the lower register priority in that form (lane gap: register colouring)
- 0087b840 NCameraModes::CTrack::OnInitLeaf DIFFER(199v199): &m_target in ebx and &PTrack in edi (retail edi / ebx; `push edi` after `mov ebp,ecx`) | tried: named &PTrack (200), named &m_target, early return, named target thing, forceinline GetInterfaceOf in the condition, all locals at the top | next: lane gap: register colouring with equal use counts
- 00807250 std::__unguarded_partition<pair<C3DVector,long>*, ...> DIFFER(199v199): `lea eax,[esp+24h]` (pivot) scheduled after the first fstp | tried: `b < a`, a one-expression comparator (evaluates b first), comparator in the outer functor, `aValue > call(b)`, /G7 | next: lane gap: two-call float comparator (EXTRA_RULES)
- 009e2020 GFABoxToVBox DIFFER(199v199): in both inlined GFAYToVY `mov ecx,eax` comes before `fstp [esp+0Ch]` (retail after) | tried on the out-of-line twin GFAYToVY 0x009E1DD0 (same residue, 49v49) with 10 source forms and 9 flag sets (listed in STATUS.tsv) | next: lane gap: x87 store vs integer move scheduling
- 0082a890 CMiniMapDisplay::AddMarker DIFFER(206v206), IMPROVED 32 -> 24 lines (differ/0082a890.cpp): the push_back node and its value pointer swap esi/edi | won: real STLport list iterators (user copy ctor, by-value begin/end, push_back -> insert(end(), x) -> _M_create_node) put all four callee-saved pushes back in the prologue; PDB signature `const CThing&` | tried: if-scope instead of early return, forceinline `||` helper (worse), _Construct, allocate layer, alloc proxy, no info reference (196), implicit CMarkerInfo copy ctor | next: the real STLport 4.6 `_List_base` / `_STLP_alloc_proxy` head with the allocator template argument
- 005954ad NUISystem::CFrontEndManager::~CFrontEndManager DIFFER(213v213): `pop edi` before the inlined CObserver vtable store (retail after `lea ecx,[esi+4]`) | tried: no optimize("s") (228), set member split as tree + PreventObservation bool, /Ow /G5 /G7 /GX | next: lane gap: epilogue pop scheduling. The parked source still uses descriptive stand-ins (TreeNext_004292c0, CObserverMember_0052e050, CPlainVector...). Before landing, respell them: NUISystem::CObserver = vptr + set<NUISystem::EEvent> EventsToObserve +4 + PreventObservation +0x10 (Ego_r.pdb), dtor `??1CObserver@NUISystem@@UAE@XZ`.
- 00807170 std::__median<pair<C3DVector,long>, ...> DIFFER(216v216): the `c` argument load sits after the fstp (retail before) | tried: expression comparator, named const& c | next: lane gap: two-call float comparator
- 004bc800 NGraphicAppearance::CTCGraphicAppearance::GetPositionMatrix DIFFER(217v217): C3DMeshObject vtable store one push early | tried: mesh object before the physics read (212), thing reference before the mesh object (215), implicit dtor (209: drops the dtor's vtable store, so the user dtor is real) | next: lane gap: store sinking past `fstp [esp]` / push
- 006d00a0 CTCWife::Murdered DIFFER(217v217): the parent load is hoisted into dead ebx before the type-4 lower bound (retail loads it into eax after the component) | tried: PDB SetMarriageState types (`const CThingAICreature*`, EMarriageState), early-return HasTC | next: lane gap: dead-register load hoist

## Skipped
None.

## Names
- Proven (PDB name set + caller or vtable): ?AddMarker@CMiniMapDisplay@@QAEXABVCThing@@J@Z (in differ/), ?OnInitLeaf@CTrack@NCameraModes@@UAEXXZ
  (virtual in both PDBs, but the parked source declares it non-virtual, which mangles QAE: fix before landing),
  GetBestTrack / GetTrackPosNearest / SetMarriageState / CWorld::GetFrame (NAME_FIXES.tsv).
- Descriptive: everything in 0x00C1B7D0 (libpk), copied from the landed siblings.

## Label corrections
- 0x00C1B7D0: catalog `CGameMap_RemoveByKey` is propagated; it is PKCTRRedblackMap::Remove (bound spelling).
- 0x005954AD: catalog `CEngineScreenEffectDisplacementRenderer::~...` is propagated; the vtable store 0x012521A8 = NUISystem::CFrontEndManager (as the parked file says).
- 0x00761B50 manifest `Map_FindByKey`, 0x0052E050 `LTextTreeWalkThrough::Destroy`, 0x004292C0 `GetNoAllocatedAreas`, 0x0042FB40 `swap` and
  0x00594FA9 `FinishWithDynamicTextures` are propagated labels (see the parked file headers for the real roles).

## Header debt
- engine/CMiniMapDisplay.h: Dinkumware sizes (16-byte vectors, 12-byte list), so Markers is at PDB +0x28; retail has it at
  +0x20. differ/0082a890.cpp models the class locally.
- No engine header for libpk (PKCTRRedblackMap). The landed DeleteFixup names the allocator `PKCTRAllocator` and the
  landed Release names it `CMapNodeAllocator` (and the node `PKCTRRedblackNode`, not `PKCTRRedblackMapNode`). Two
  descriptive names for one class; only the Release spelling reaches a symbol.
