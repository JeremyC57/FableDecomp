# Hand chunk 405 (CFilter_Bind3 .. CScriptGameResourceObjectScriptedThing): 35 of 40 byte-exact

## What matched (35)

- **CFilter_Bind3** (0x008E6030, `operator()<CThing>` of `<CThingFilter_CanCollideOrInteractWithThing,
  CThingFilter_OverlappingSphere, CThingFilter_HasTC>`): a thing passes when the searcher can collide or interact with it,
  its physics bounding sphere overlaps the filter sphere, and it has the TC. Shape: `return (*F1)(t) && (*F2)(t) && (*F3)(t);`
  over pointer members (Ego_r layout). The overlapping-sphere operator must be `__forceinline`, because an in-class inline
  stays a call. Its body is the landed 0x006689E0 one, with the radius in the dead argument slot and the squares summed z, y, x.
- **NCreatureMode** (0x008E9080 GetAnimDistanceAndIndexFromAnimName): the bank index of the named animation and its
  per-frame movement distance. Shape: the landed sibling's model, with the bank as a named local fetched first and
  `speed = dist / duration` as a named float.
- **NCreatureMode::CModeHoverBase::CRadialArea** (0x008EA3F0 IsPosInArea): is the offset inside the angular sector between
  (sin, cos)(ThetaLow*2pi) and (sin, cos)(ThetaHigh*2pi) and, optionally, inside RadHigh? Shape:
  - `angle * (double)TWO_PI` gives the `fmul qword` pool constant.
  - The radius test is the CONDITION of a block (`if (!check || length < RadHigh) { ... }`) falling through to the shared
    `return false`. An early `return false` gets its own exit.
  - The normalised vector is built by two statements, so its Y load is created after X, which makes every dot Y-term first.
- **std::__uninitialized_copy<CHoverState*>** (0x008EBA90): copy-construct a hover-state range into raw storage. Shape:
  STLport `for (; first != last; ++first, ++cur) _Construct(&*cur, *first)`. The memberwise copy is a CCharString call, a
  12-byte CRadialArea and two words.
- **CTCContainer TU** (0x008ECEF0 `__find_if<…, CIsItemWithDefIndex>`, 0x008ED040 RemoveItem, 0x008ED2B0 OnSerialise,
  0x008ED3B0 CCreateItemNamesFromIndices::operator(), 0x008ED440 CCreateIndicesFromItemNames::operator()): the item
  vector is `vector<pair<long, CCharString>>`. RemoveItem is find_if + erase; OnSerialise maps indices to def names
  ("ContainerContents") through for_each. Shape:
  - The same-TU `__find_if` BODY (the STLport 4-way unrolled random-access form) makes VC know the call writes no memory,
    so RemoveItem caches `end`.
  - The functors' push_backs need the same-TU noinline `_M_insert_overflow` stand-in (reads x, never the tag).
  - A named CCharString local sits in the dead argument slot. An explicit `CCharString("")` temporary feeds the ctor's eax.
- **std::__copy<Coin*>** (0x008F02E0): assign a Coin range. Shape: STLport `for (n = last - first; n > 0; --n)` with Coin's
  implicit operator= inlined (the 0x3C-byte divide by `0x88888889`).
- **CCreatureAction_DragonSwoop** (0x008F57F0 implicit copy ctor): base copy, vtable, three swoop points. Shape: the
  compiler-generated copy ctor goes out of line when C3DVector has a user copy ctor and Clone does `new T(*this)`.
- **NPhysicsHelpers** (0x008F5B50 GetLineOutFromClosestPointOnFiniteLineToPoint, 0x008F5BA0
  GetPerpendicularDistanceFromFiniteLine): the vector from, and the distance to, the closest point on a segment. Shape: a
  named-result `operator-` (`C3DVector r; r.X = ...; return r;`) gives the X-first stores. The length is
  `s = Z*Z; s += X*X; s += Y*Y;`, because the x87 order follows statement order.
- **CMap** (0x008F71A0 InitThemePalette): empty the palette, push 256 (0, unused) entries, mark entry 0 used, then add the
  global def's InvalidThemeStandin. Shape:
  - STLport `clear()` is `erase(begin(), end())` through `__copy_ptrs(__false_type)`, which gives the random-access
    `__copy` loop over the unfolded `end - end`.
  - One entry local is declared before the loop.
  - The same-TU `_M_insert_overflow` stand-in lets VC forward the zero ThemeDefIndex from ebx.
- **NCombatActionHelpers** (0x008F78E0 GetDustCreationHelperNames, 0x008F7E70 / 0x008F7F50 CreateEffect): append the two
  foot helper names; spawn an effect thing and hand its particle emitter a DIRECTION vector parameter (optionally
  attached to a helper). Shape: CBaseClass must be polymorphic. An empty non-polymorphic base is placed at +4 and gives
  `lea ecx,[params+4]`.
- **LHNetworkLib::LTimer / LSocket** (0x008F80D0 SetSpeedUpFactor, 0x008F9CA0 Stop, 0x008F84A0 Think, 0x008F86A0 /
  0x008F86F0 ResolveHost, 0x008F87A0 CreateSocket, 0x008F8830 ConvertLAddress2Sockaddr, 0x008F8BB0
  ConvertSockaddr2LAddress): timer speed-up handling and socket helpers. Shape:
  - Running() is `SpeedUpFactor != 0.0f` (`fucompp`). Stop's inlined GetSpeedUpFactor ternary tail-duplicates the store.
  - ResolveHost(string) uses plain malloc/free for its scratch byte. ResolveHost(LAddress&) compares against a real
    `string("ANY")` temporary with a `__forceinline` ctor, and the IP is a named local.
  - CreateSocket needs `/GS`.
  - The sockaddr helpers keep the source order of the port and family stores and an inline strlen scan.
- **NCameraModes** (0x008FBCB0 CPhysicsForceModelBase::GetPointIntersection, 0x008FBF10 `list<C3DVector>::_M_insert_dispatch`,
  0x008FCA40 `transform<…, back_insert_iterator<list<CPhysicsMeshObject>>, CGetMeshObject>`): the first mesh object a
  segment hits; copy-insert a point range; append each thing's mesh object. Shape:
  - `(it++)->` advances before the call.
  - The STLport `_M_create_node` link order matters.
  - transform needs the STLport 4.6 iterator shape (`_List_iterator_base` with `_M_incr`/`==`/`!=` under `_List_iterator`
    with a user copy ctor) to keep the ebp/edi pushes above the empty-range test. CIVCountedPointer has a mutable RefCount.
- **CAIStateGroupBase / CAIStateGroupInfo / CAIStateGroup_*** (0x008FCF10 IsRunnableMain, 0x008FD21F CAIStateGroupInfo
  ctor, 0x009015B9 RecoverStealableItem, 0x00901658 CarryCrateAndFollow, 0x009016D9 TavernServer, 0x00901783 TavernKeeper
  ctors): an enabled-and-runnable test, a registration record, and state-group constructors (base ctor, vtable,
  intelligent-pointer members). Shape: `if (a && b) return true; return false;`. The constructors and the info record are
  in a `#pragma optimize("s", on)` TU, where a vtable constant used twice or more lives in a callee-saved register.
- **CScriptGameResourceObjectScriptedThing** (0x00903810 CreateNullHandle): a handle owning a new implementation
  bound to the script interface. Shape: there is no NRVO in VC7.1, so retail builds a local, copies it out and destroys
  the local. CCountedPointer has a user AddRef copy ctor and an inline null-tested release dtor that zeroes both words.
  The impl setter 0x008AB960 is an out-of-line member (`add ecx,8 ; jmp Reset`).

## Parked for Fable 5.1 (5)

- 008e8ff0 NCreatureMode::GetStrafingAnimationName DIFFER(135v135): retail loads the physics vtable `mov eax,[ecx]` between the first `fld` and `fsub` of the movement delta, the lane right before the virtual call | tried: named-result operator- copied into a local, copy + `-=`, `return C3DVector(...)`, explicit X/Y/Z stores | next: fetch the facing through an inline helper taking the physics pointer first, or declare `facing` before the delta
- 008f58c0 NPhysicsHelpers::GetLineThingSearchArea DIFFER(164v163): tail only; retail computes both TL corners into copies and stores BRX, BRY, TLX, TLY (`mov ecx,edi ; mov edx,ebx ; add edi,eax ; sub ecx,eax ...`), the lane stores TL first | tried: 4-long ctor, (C2DCoordI, C2DCoordI) from temporaries, named corner locals, PDB ctor C2DBoxI(const C2DCoordI&, long) | next: a named box returned through the PDB's user copy ctor, or a ctor body assigning BR before TL
- 008f7860 NCombatActionHelpers::IsToCreateCombatParticleEffects DIFFER(112v113): retail keeps the movie-sequence `mov al,1` exit separate and preloads `mov al,1` (reused as the `test byte ptr [esi+91h],al` mask) before one shared `test al,al ; setne al` epilogue; the lane merges the true exits and tests immediates | tried: one `||` chain, `bool create = true` + updates, if/return true + `? true : false` tail, `||` chain ending in the ternary | next: `bool create = true;` set by the ternary inside the && test, with the movie check through an inline world helper
- 008f99a0 LHNetworkLib::LSocket::Receive(vector<char>&) DIFFER(207v207): scheduling only; retail issues the first `push offset ""` before the end-of-storage store and sinks the string start/finish stores below the second push | tried: verbatim STLport string base, ServerPort-before-ServerIP ClearData, zero-free string base, STLport 4.6 `_String_base(a, n)` | next: lane gap: scheduling (inline string-ctor stores vs argument pushes, likely a compiler-build artefact)
- 008fa9f0 NCameraModes::CPhysicsCollisionPointsObject::GetPointsList DIFFER(132v133): retail rotates both loops (`mov ebp,[eax] ; cmp ebp,eax ; je` + bottom tests), the lane keeps a top-tested inner loop with a `jmp` | tried: insert(end, first, last) per entry, forceinlined _M_insert_dispatch/insert, nested for-loops with push_back | next: the STLport 4.6 `_List_iterator_base` + user-copy-ctor iterator that fixed 0x008FCA40 in the same TU, or caching first/last const_iterators in locals

## Skipped

- None. No row covers two functions, and none has int3 after a noreturn call.

## Label corrections

| Address | Manifest label (wrong) | Real name |
| --- | --- | --- |
| 008e6030 | CollisionFilter_SphereTest | `CFilter_Bind3<CThingFilter_CanCollideOrInteractWithThing,CThingFilter_OverlappingSphere,CThingFilter_HasTC>::operator()<CThing>` |
| 008e9080 | GetAbilityAnimationDuration | NCreatureMode::GetAnimDistanceAndIndexFromAnimName |
| 008ea3f0 | C3DVector_PointInConvexQuad_Plane | NCreatureMode::CModeHoverBase::CRadialArea::IsPosInArea(const C2DVector&, bool) const |
| 008eba90 | `_Copy_backward<CExplosionRing*>` | `std::__uninitialized_copy<CHoverState*>` |
| 008ecef0 | `std_find_<int_qword_aligned>` | `std::__find_if<pair<long,CCharString>*, CIsItemWithDefIndex>` |
| 008ed040 | `std_vector_erase_single<CTCSkeletalMorph>` | CTCContainer::RemoveItem(long) |
| 008ed2b0 | Persist_TransferContainerContents | CTCContainer::OnSerialise(CPersistContext&, bool) |
| 008ed3b0 | `std_vector_push_back<CCharString>` | CCreateItemNamesFromIndices::operator() |
| 008ed440 | `std_vector_push_back<DefinitionIndex_CCharString>` | CCreateIndicesFromItemNames::operator() |
| 008f02e0 | `_Move<Coin*>` | `std::__copy<Coin*>` |
| 008f57f0 | CCombatActionBase_CopyConstructor | CCreatureAction_DragonSwoop copy ctor |
| 008f5ba0 | Geometry_DistancePointToLineSegment | NPhysicsHelpers::GetPerpendicularDistanceFromFiniteLine |
| 008f71a0 | CMap_InitializePalette | CMap::InitThemePalette |
| 008f78e0 | CreateFootEffect_PushStrings | NCombatActionHelpers::GetDustCreationHelperNames |
| 008f80d0 | CMusicManager_OnMusicEntryTimeout | LHNetworkLib::LTimer::SetSpeedUpFactor(float) |
| 008f86a0 | Socket_ResolveHostname | LSocket::ResolveHost(const string&) |
| 008f86f0 | Socket_InitWithDefault | LSocket::ResolveHost(LAddress&, unsigned long&) |
| 008f99a0 | LSocket_ReceiveWithTimeout | LSocket::Receive(vector<char>&) |
| 008f9ca0 | MusicTimer_CheckAndTrigger | LHNetworkLib::LTimer::Stop() |
| 008fa9f0 | LinkedList_ClearAndReinit | NCameraModes::CPhysicsCollisionPointsObject::GetPointsList |
| 008fbcb0 | MeshContainer_RayIntersectionQuery | NCameraModes::CPhysicsForceModelBase::GetPointIntersection |
| 008fbf10 | LinkedList_CloneNodes | `std::list<C3DVector>::_M_insert_dispatch` |
| 008fca40 | DoThingEvasion_BuildMeshReferenceList | `std::transform<…, CGetMeshObject>` |
| 008fcf10 | Script_GetCurrentStateGroupPriority | CAIStateGroupBase::IsRunnableMain() |
| 008fd21f | `GetDefaultVal<CParticleMorphs>` | CAIStateGroupInfo::CAIStateGroupInfo |
| 009015b9 | CAIStateGroup_TrollLarge_Constructor | CAIStateGroup_RecoverStealableItem ctor |
| 00901658 | CAIStateGroup_IceTrollSmash_Constructor | CAIStateGroup_CarryCrateAndFollow ctor |
| 009016d9 | CAIStateGroup_MultiStateAttacker_Constructor | CAIStateGroup_TavernServer ctor |
| 00901783 | CAIStateGroup_WerewolfAttacker_Constructor | CAIStateGroup_TavernKeeper ctor |
| 00903810 | CGameScriptInterface_CreateScriptedEntity | CScriptGameResourceObjectScriptedThing::CreateNullHandle(CGameScriptInterface&) |

- Callees: 0x008AB960 is labelled `_Cons_val<…CParticleMorphs…>`, but it is the scripted-thing handle's impl setter (an
  `add ecx,8` thunk into `CCountedPointer<T>::Reset(T*)` 0x008AB230). 0x008F7390 is labelled
  `std_vector_insert_range_fill`, but it is `vector<CThemePaletteEntry>::_M_insert_overflow`.
