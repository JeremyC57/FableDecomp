# Hand chunk 614 (NCameraModes::CScriptModuleSpline .. CThingPlayerCreature): 4 of 10 byte-exact

All 10 rows were parked same-length misses from the 2026-10-30/31 retry sweep. Four now match (RELOCATION_MATCH, callee
identity checked against `objdump -r` of the graded object). Six are parked again. Five of the six now carry the right own
symbol, which the earlier parked files had spelled wrong.

## What matched (4)
- **CThingSearchTools** 0x0085B5B0 `GetAllThings<CThingGameObject, std::list<CThingGameObject*>, CFilter_Bind4<...>>() const`
  appends every scan-visible game object that is not the striker, is strike-to-strike with it, overlaps the strike sphere
  and can collide with it. Shape:
  - the landed list-result sibling 0x008AC660 (PeekClassList walked with STLport const_iterators, push_back = insert(end()));
  - a `__forceinline` Bind4 `operator()`, without which it graded 140v365;
  - the landed 0x0085B160 filter model, with a same-TU noinline `LowerBound` stand-in and the out-parameter GetInterface;
  - `CThingFilter_CombatOverlappingSphere` is outside the anonymous namespace (the other two filters are inside it).
  - The own symbol is a public const member template of CThingSearchTools. The parked file had CThingSearchToolsMethods, non-const.
  - The Leaf is a prefix, because the `?A0x` hash is per TU (retail's is `?A0xad05d24c`).
- **CMap** 0x00820840 `ScriptedMapBrushMoveThings(const CScriptedMapBrush&)` (protected virtual) moves every live thing the
  brush lists to the brush position (converted to world-map coordinates) and facing angles. Shape:
  - an STLport 4.6 `list<unsigned __int64>` ctor: `_M_node(0)`, allocate, self-link, then store;
  - the real `CScriptedMapBrush::CThingPosAndOrientation { C3DVector Position; C3DAngles FacingAngles; }` copy, with a user copy
    ctor on C3DVector only. This gives retail's `add eax,0Ch` before the angles copy. The parked STransform with two C3DVectors could not.
  - a named `CTCPhysicsBase* physics = thing->PhysicsTC;` for the ResetPosition call only. This puts `this` in ebx with a frame
    spill and the physics pointer in ebp.
  - Every callee was re-proved. The parked file used placeholder names (GetCells, GetTransform, CThingHolder::Get, GetThingAt).
- **CAIStateGroup_MinionWorkInInn** 0x0090BD40 `CleanBench()` (private) walks to a free bench action point, looks along its
  forward axis, plays ST_WIPE_TABLE and frees the point. Lever: `bool reached` in its own block, so VC7.1 packs it into the
  GetInterface presence-bool frame byte. That moves the points / index / reached slots to retail's layout.
- **CThingPlayerCreature** 0x006AB5D0 `GetCurrentMaxMovementSpeed() const` (protected) returns the speed limit by band, with
  the blocking, sneaking and sprinting overrides. Levers:
  - the out-parameter `PeekTC(&p, id)` form for both const component lookups (component in eax, flag in cl);
  - a named `float sprint` for the sprint override, which keeps the facing+position X operand order;
  - C3DVector must be `class` (V). As a struct, GetPos mangles `ABUC3DVector`.
  - The band switch follows retail's jump table: 3 -> GetMaxRunningSpeed, 4 -> GetMaxJoggingSpeed, default -> running. The
    parked body had 3 and 4 swapped, which q.py masks and vland RELOC_JUMPTABLE rejects.

## Parked for Fable 5.1 (6)
- 00854180 NCameraModes::CScriptModuleSpline::Update DIFFER(364v364): the inlined GetTC key slot is not shared with the roll local (frame 0x30 v 0x2c), which reorders the lookAt copy into SetObservedPos (30 diff lines) | tried: same-TU noinline CInterfaceMap::LowerBound + PDB inline setters SetPosition/SetObservedPos/SetFOV/SetRollAngle (69->30), no copy ctor, const& lookAt, member-wise SetObservedPos, user operator= (worse), LookAt before Position (worse), key in nested block, lookup in own block | next: lane gap: frame slot sharing between an inlined-callee local and a caller local
- 00b9e120 CEngineInternalPrimitive2DStaticMesh::Initialise DIFFER(365v365): WorldPos temporary copy, retail `mov edx,ecx` with the words in eax, ours `mov eax,ecx` with the words in edx | tried: inline SetWorldPos(const C3DVector&) (inert), member-wise stores through a reference (376, worse); earlier sweeps: reference local, named centre, user operator=, user copy ctor | next: lane gap: register choice for a temporary copy, or a real C3DVector WorldPos member once the header carries it
- 00a26ad0 C3DAnimationInfo::BuildAnimationInstance DIFFER(383v383): eax/ecx/edx rotation through the IsLoaded/IsLoading guard and the map store, `sete cl` v `sete al`, the two InitialiseAnimationInstance copies swapped | tried: correct names + inline private BuildSharedAnimation + RecoverAnimationMemory switch, for/while/do/for(;;) retry loops (inert), named CAnimationData* for the map store (92->89) | next: model CAnimationData / CMeshDataBank with real headers and the retry as the PDB's own loop shape
- 009c5890 CFileInstaller::CFileInstaller DIFFER(389v389): CreateEventW argument pushes interleave with the flag stores, and the ".biz" push v PauseFinishEvent store order | tried: real STLport 4.6 _Vector_base/_STLP_alloc_proxy/_List_base/_Rb_tree models (fixed push edi, 59->57), temporary CCharString (385, worse) | next: the member declaration order of the four events against the flags (FableWin ctor order)
- 00783a30 CTCBoastingCrowdControl::GetCrowd DIFFER(392v393): ebx/ebp swap between this and the map, the definition-manager load (`mov eax,[eax+10h]; mov ecx,eax` v `mov ecx,[eax+10h]`), UID copy registers | tried: correct private by-reference spelling, inline GetDefinitionManager accessor (394), named uid local (inert) | next: the CTCBase PeekWorld/GetDefinitionManager accessors as real header inlines plus the STLport map insert shape
- 009c38c0 CFileInstaller::CActiveFile::OnWriteFinished DIFFER(406v409): the inlined SetDirtyDiscError(true) drops retail's dead `mov al,[eax+7Ah]`, and the GetFileSize/GetFileTime/SetFileTime arguments rotate eax->edx, ecx->eax, edx->ecx | tried: `static __stdcall` noinline GetMarkerFileName (fixed the ESI result and `ret 4`), `||` (inert), `|`, `|=`, ternary, plain if (359, worse) | next: SetDirtyDiscError as a real protected static `__fastcall` member that VC7.1 inlines from another inline layer; else lane gap: dead load kept by retail

## Skipped
None.

## Names
Proven (PDB name set plus a second source: donor order, body, caller arguments or a landed file). These are the own
symbols of all 10 rows, plus:
- 0x0085B5B0:
  - CThingSearchTools::PeekClassList() const 0x0049C770;
  - CTCHitLocations::GetBoundingSphere(C3DVector&, float&, bool) const 0x00815420;
  - the three filter `operator()`s.
- 0x00820840:
  - CScriptedMapBrush::GetThingsToMove(std::list<unsigned __int64>&) const 0x0082E860;
  - CScriptedMapBrush::GetThingsMovement(unsigned __int64) const 0x0082E4E0 (find in ThingsToMove +0x20, returns node+0x18);
  - CThingManager::GetPThingWithUID(unsigned __int64) 0x0051ED80;
  - CWorldMap::ConvertLocalCoordsToWorldMapCoords(long, const C3DVector&) const 0x004FC4A0;
  - CWorld::GetThingManager() slot 3: retail 0x0049E1B0 returns +0x50 PThingManager (Ego_r UAE);
  - CWorld::GetWorldMap() slot 13: retail 0x004AE8C0 returns +0x14 PWorldMap (FableWin UAE);
  - CWorldMap::GetMapNumberAt(const C2DCoordI&) const slot 17: retail 0x004FC080 takes two longs (FableWin UBE);
  - CTCPhysicsBase::ResetPosition(const C3DVector&) slot 31: 0x006B01D0 calls slot 30 then copies to +0x18 (Ego_r U; fleet binding agrees);
  - SetFacingAngleXY/YZ/XZ(float) slots 66/67/68: FableWin CTCPhysicsBase U set, empty `ret 4` base bodies, and the
    CTCPhysicsStandard overrides in the same XY/YZ/XZ order as its Ego_r donors;
  - CMap::World +0x20 and WorldPos +0x1D24: Ego_r 0x2C/0x1D34, minus the STLport list/vector shrink.
- 0x0090BD40:
  - GetFreeBench, GetFreeActionPoint, SetActionPointAsFree;
  - the AIAction_* members;
  - the CCreatureAction_PlayAnimation ctor and ~CCreatureActionBase.
- 0x006AB5D0:
  - CTCStealth::GetSneakingSpeed() const;
  - CThingCreatureBase::IsBlocking() const;
  - CTCPhysicsBase::GetFacingVector() const slot 70;
  - GetMaxSprintingSpeed and the band slots 79-82.
- The parked rows: their callees as listed in each differ header.

Descriptive (no proving source):
- `g_bSneakSpeedOption` 0x01375749 (0x006AB5D0);
- `GetCreature()` (0x0090BD40, inline accessor, emits no symbol);
- the fleet's `LowerBound` spellings for 0x0040F020 / 0x004365B0 (inline stand-ins, no relocation);
- `g_pPrimitiveRenderer2D_01436e7c` (0x00B9E120 differ);
- every `vNN` filler virtual slot.

## Label corrections
- See NAME_FIXES.tsv (14 addresses).
- Catalog / binding labels found wrong this chunk:
  - 0x0051ED80 (CStopWatch::GetTicks and GetThingWithUID variants);
  - 0x0082EEB0, bound `FableSurfHelper_0082e4e0::compute`: it is the ThingsToMove map `find`, called by GetThingsMovement;
    not proven further;
  - 0x0082E860 and 0x0082E4E0, labelled LinkedList_BuildFromTreeIterator and CSurface::GetHeight in the parked file;
  - 0x00870BB0, bound as a global `?Create@@YI`.
- CFileInstaller has class key V in pdb_class_keys.tsv.

## Header debt
These rows use local models. In each case the shared header flattens the vptr or has the Ego_r layout:
- CThing: retail is 0x10 below Ego_r from +0x6C on; PhysicsTC +0x60, Dead bit +0x91.
- CTCPhysicsBase: filler slots v00..v69.
- CWorld: slots 3 and 13.
- CWorldMap: slot 17. CWorldMap has two vtables; 0x01244AEC is the primary.
- CMap: no engine header.
- CScriptedMapBrush: the header has a `void* __vftable` and no nested CThingPosAndOrientation.
- CAIStateGroupBase.
- CThingPlayerCreature: filler v000..v078.
- CFileInstaller / CActiveFile: no matching engine header; rebuild/include/fable_file_installer.h differs.
- C3DAnimationInfo: a local model next to engine/C3DAnimationInfo.h.

The std::list model is a minimal local STLport 4.6 shape (`_List_node_base`, `_List_iterator<T, _Nonconst_traits<T>>`, the
`_M_node(0)` + allocate ctor). A shared STLport header would replace it.
