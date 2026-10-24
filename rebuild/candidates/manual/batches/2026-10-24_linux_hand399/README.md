# Hand chunk 399 (_global .. _global): 36 of 40 byte-exact

All 40 rows came in as `_global`. They are STLport algorithm instantiations, CIntelligentPointer helpers, the
CTCDExplosion factories, the creature-group member map's `_Rb_tree`, and a few named free or static functions.
- 12 rows are MATCH and 24 are RELOCATION_MATCH.
- All 36 tests print PASS under both g++ (tall.sh) and VC7.1 (tall.py).
- 4 rows are parked and none were skipped.

## What matched (36)

**NPlayerTargeting::CTargetPoint (28 bytes) with CThingCompare_BestForTargeting2 (52 bytes)**
- 0080c020 `std::sort`: the introsort setup over the target-point vector.
  - The comparator's implicit copy constructor is called out of line (0x7F9730) because sort copies the comparator twice.
- 0080bcb0 `std::partial_sort`: the comparator is copied once, so the same implicit copy constructor is expanded inline.

**CTCDExplosion**
- 0080c460 and 0080c500, the `CTCDExplosion::Create` 1-vector and 2-vector static `__fastcall` factories.
  - Shape: a `CThing* result = 0; ... result = thing; return result;` local keeps the null in ebx, and VC duplicates the success tail.
  - An early `return thing` loses the zero register.

**CIntelligentPointer helpers**
- 0080dd90: member `CIntelligentPointer<CThingBuilding>::operator==`.
- 0080def0 and 0080df20: the `pair<CIntelligentPointer<CThing>,int/long>` constructors.
  - The copy constructor is `: CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }`.
- 0080e200: `make_pair`.
- 00810d40: `list<pair<CIntelligentPointer<CThing>,long> >::push_back`.

**back_inserter into vector<CThing*> and vector<const CThing*>**
- 0080eb40 `back_insert_iterator::operator=`.
- 0080ebe0 and 0080ec50 `std::transform` with CGetThingFromIntelligentPointer.
- Shape: push_back is inlined with a placement-new null test.
  - The overflow's empty `__true_type` temporary is homed on a dead argument slot.
  - This needs a same-TU `#pragma auto_inline(off)` + `__declspec(noinline)` `_M_insert_overflow` body that never touches its tag (GOTCHA 1276).

**Other named functions**
- 0080f240 `CTCNymph::MakeSummonedCreaturesImmuneToTrap`: a static `__fastcall` function.
- 008111f0 `NEntityEvents::CEventPerformedSpecialAbility` constructor (vtable 0x012723C0).
  - The inline base `CEventBase() : WorldFrame(0)` has its vtable store elided.
- 008137e0 `CTCCreatureGroupTag::FindNewTagger`: walks the creature-group member map.

**std::_Rb_tree<unsigned __int64, pair<const unsigned __int64, CGroupMemberData> > (CTCCreatureGroup members)**
- 00813460 `erase(first, last)`. Its Leaf is the full mangled name, because the TU also emits `erase(position)`.
- 00813530 `equal_range`: `_M_lower_bound`/`_M_upper_bound` must return a node pointer in eax.
  - An iterator return goes through a hidden pointer.
- 00813580 `_M_insert`.
- Model: header node pointer at +0, count at +4. The value sits at node+0x10, and the node is 0x30 bytes.
- `_Rebalance` (0x42971D) and `_Rebalance_for_erase` (0x42A34E) are `__cdecl` statics.

**THitLocationIntersectionElement (32 bytes, user copy ctor of a pack(1) C3DVector)**
- 00817010 `swap`.
- 008174b0 `__lower_bound` and 008174f0 `__upper_bound`.
- 008175d0 `__adjust_heap`.
- 00817840 `__unguarded_partition`.
- 00818240 `__linear_insert`: `copy_backward` must be `__forceinline`. With plain `inline` it stayed a call, because the by-value element has a non-trivial copy constructor.
- 00818450 `__partial_sort`: `__pop_heap` must be `__forceinline` for the same reason.
- 00818b00 `__introsort_loop`.
- All are /Gr `__fastcall` templates.

**Keyed records**
- SKeyedRecord, a 24-byte `pair<long, 20-byte POD>`:
  - 00819940 `__unguarded_linear_insert`.
  - 0081a8b0 `__unguarded_insertion_sort_aux`.
- SKeyedBlock40, a `pair<long, 36-byte POD>`:
  - 0081a940 `__unguarded_insertion_sort_aux`. The non-inline `__unguarded_linear_insert` defined in the TU is auto-inlined.
  - 0081ab40 `__adjust_heap`.
- `pair<unsigned long, CSoundPair>`, 12 bytes:
  - 00819a60 `__unguarded_linear_insert` and 0081a9e0 `__unguarded_insertion_sort_aux`.
  - 0081a590 `uninitialized_fill_n`. CSoundPair's scalar `long` members give the single-register copies; a 3-dword array copies through esi/edi.
  - 0081b220 `CVectorMap<unsigned long,CSoundPair,...>::Insert`, which takes the key by value. The inline push_back plus the same-TU overflow stand-in home the tag on the dead key slot, followed by `Dirty = true`, the inline Sort, and `Dirty = false`.
- 00818180 `__uninitialized_copy` of the 28-byte particle-morph record.
- 00819320 `fill<CTextureManagerPoolInfo>`.

## Parked for Fable 5.1 (4)
- 0080c190 _anon::GetExplosionEffectInterpolate DIFFER(128v122): the radius clamp reloads the stack parameter per compare (`fld [esp+4] ; fcomp`). Retail loads it once and keeps it on the x87 stack (`fcom ; fstp st0 ; fld bound`) | tried: if/else-if clamp, early-return Clamp(const&), by-value Clamp, ternary Clamp inside a single-exit `result` form | next: a Clamp template taking the value by reference into a local float with `float r = radius; r = r < lo ? lo : (r > hi ? hi : r)`, or lane gap: x87 register-keeping
- 0080e2f0 std::remove_if<pair<CIntelligentPointer<CThing>,long>*,CIsFirstElementEqualTo> DIFFER(182v182): the inline find_if's iterator-tag temporary gets its own frame word in ours (`sub esp,0Ch ; lea eax,[esp+13h]`). Retail homes it on the live predicate slot (`sub esp,8 ; lea eax,[esp+1Ch]`) | tried: iterator_category(first), a direct random_access_iterator_tag(), single exit (that fixed the tail) | next: a same-TU auto_inline(off)/noinline `__find_if` stand-in that never touches its tag (GOTCHA 1261/1276), else lane gap: GOTCHA 222 tag homing
- 0080ffb0 NExplosiveTrailHelpers::RandomiseThingOrientationAroundDirection DIFFER(229v229): only the final copy + virtual call differ. Retail loads `mov ecx,[ebx+60h]` first and block-copies the 24-byte set in dword pairs (+4,+0),(+0Ch,+8),(+14h,+10h) through eax/edx. Ours copies vector by vector through eax/ecx/edx and loads the physics pointer after | tried: up/forward locals + `CRightHandedSet(up, forward)` temporary, /Oa, trivially copyable C3DVector, one CRightHandedSet local passed through an inline by-value CThing::SetRHSet (277v229) | next: a CRightHandedSet local constructed in place (default/identity ctor, no vector temporaries) copied whole into the virtual's argument (e.g. an inline by-value wrapper on the physics TC)
- 00814c70 NHitLocations::CPhysicalPrimitiveSphere::GetSphereBase DIFFER(156v156): retail loads the appearance pointer (`mov ecx,[edi+64h]`) right after the GetString call, before the GetDummyObject argument pushes, so the last lea uses edx. Ours loads ecx last, and the fallback/result copies then rotate eax/ecx/edx vs edx/eax/ecx | tried: `||` in one condition (conditional-temp flag, 187v156), `found` flag + physics local (156v156), inline CThing::GetDummyObject wrapper (identical), named CCharString + appearance locals (173v156) | next: the same `this`-before-arguments scheduling as 0080ffb0, so try the GetDummyObject call through an inline that binds the appearance pointer first, or lane gap: call-object scheduling

## Skipped
None.

## Label corrections
| Address | Catalogued as | Correct name |
|---|---|---|
| 0080bcb0 | Resize_TargetPointVector | std::partial_sort<NPlayerTargeting::CTargetPoint*, CThingCompare_BestForTargeting2> |
| 0080c020 | IntroSort_TargetPoint_Setup | std::sort<NPlayerTargeting::CTargetPoint*, CThingCompare_BestForTargeting2> |
| 0080c190 | Explosion_CalcFalloffIntensity | anon GetExplosionEffectInterpolate(const CExplosionDef&, float). Reads ExplosionRadiusSmall +0x50 and ExplosionRadiusLarge +0x54. |
| 0080c460 / 0080c500 | Create | CTCDExplosion::Create (1-vector / 2-vector) |
| 0x0080C250 (landed) | CTCDCameraPoint::Construct | CTCDExplosion::Construct (a byte-twin label) |
| 0080dd90 | operator==<CThingBuilding> | member CIntelligentPointer<CThingBuilding>::operator== |
| 0080def0 / 0080df20 | make_pair | std::pair<CIntelligentPointer<CThing>, int / long> constructors (0080e200 is the real make_pair) |
| 0080e2f0 | ExplosionFilter_RemoveMatchingElement | std::remove_if<pair<CIntelligentPointer<CThing>,long>*, CIsFirstElementEqualTo> |
| 0080eb40 | Vector_PushBack_U32 | std::back_insert_iterator<std::vector<CThing*> >::operator= |
| 0080ebe0 | std_vector_CopyToIntelligentPointer | std::transform<..., back_insert_iterator<vector<CThing*> >, CGetThingFromIntelligentPointer> |
| 0080ec50 | vector_copy_CBaseIntelligentPointer | std::transform<..., back_insert_iterator<vector<const CThing*> >, CGetThingFromIntelligentPointer> |
| 0080f240 | MakeSummonedCreaturesImmuneToTrap (_global) | CTCNymph::MakeSummonedCreaturesImmuneToTrap (static) |
| 0080ffb0 | RandomiseThingOrientationAroundDirection (_global) | NExplosiveTrailHelpers::RandomiseThingOrientationAroundDirection |
| 00810d40 | std_vector_PushBackIntelligent | std::list<std::pair<CIntelligentPointer<CThing>,long> >::push_back |
| 008111f0 | CParticleMorphs_InitFromHeroAbility | NEntityEvents::CEventPerformedSpecialAbility::CEventPerformedSpecialAbility(EHeroAbility) |
| 00813460 | LTextTreeWalkThrough_ClearRange | std::_Rb_tree<unsigned __int64, pair<const unsigned __int64,CGroupMemberData>, ...>::erase(iterator, iterator) |
| 00813530 | BinaryTree_EqualRange | that _Rb_tree::equal_range |
| 00813580 | CMemoryAllocatorVariableSize_InsertNode | that _Rb_tree::_M_insert |
| 008137e0 | CMemoryAllocatorVariableSize_FindNextActive | CTCCreatureGroupTag::FindNewTagger. The landed callers call it ChooseNewTagger. |
| 00814c70 | GetDummyObjectOrigin | NHitLocations::CPhysicalPrimitiveSphere::GetSphereBase(const CThing&) const |
| 00817010 | swap<CDebugSphere> | std::swap<THitLocationIntersectionElement> |
| 008174b0 / 008174f0 | std_lower/upper_bound_Binary | STLport std::__lower_bound / std::__upper_bound |
| 008175d0 | _Adjust_heap<CDebugBox*> | std::__adjust_heap<THitLocationIntersectionElement*> |
| 00817840, 00818240, 00818450, 00818b00 | std_partition_QuicksortSwap / std_copy_BackwardLarge / std_partial_sort_Heap / std_introsort_Quicksort | STLport __unguarded_partition / __linear_insert / __partial_sort / __introsort_loop |
| 00818180 | _Move<CTVertexLandscapeBackground*> | std::__uninitialized_copy of the 28-byte particle-morph record |
| 00819320 | fill<CTextureManagerPoolInfo*,...> | correct (STLport std::fill) |
| 00819940 / 00819a60 | std_InsertionSort_* | std::__unguarded_linear_insert |
| 0081a590 | _Fill_n<unsigned long*> | std::uninitialized_fill_n<pair<unsigned long,CSoundPair>*>. The stride is 12. |
| 0081a8b0 / 0081a9e0 | _Copy_impl<CNavigationPosition*> | std::__unguarded_insertion_sort_aux |
| 0081a940 | _Fill<CHeroMarriageInfo*> | std::__unguarded_insertion_sort_aux |
| 0081ab40 | _Adjust_heap<CDebugCone*> | std::__adjust_heap over the (long, 36-byte) pair |
| 0081b220 | std_vector_push_back_SoundPair | CVectorMap<unsigned long,CSoundPair,CKeyPairCompareLess<...> >::Insert. FableWin.pdb has `Insert(const K&, const V&)`; the PC body takes the key by value. |
| 0x0081CA90 (callee) | Vector_Reallocate | vector<pair<unsigned long,CSoundPair> >::_M_insert_overflow |
| 0x0081E0F0 (callee) | std_introsort_setup_SoundPair | std::sort<pair<unsigned long,CSoundPair>*, CKeyPairCompareLess> |
| 0x0049D810 (callee) | CVertexBufferWin32::DoSizeof | CWorld::GetPRandomSeed (already noted in landed sources) |
| 0x0099EAE0 (callee) | _Cons_val<...CParticleMorphs::CEntry...> | CCharString::~CCharString |
| 0x0049C370 / 0x00436AC0 (callees) | (other labels) | vector<const CThing*> / vector<CThing*> `_M_insert_overflow` |
