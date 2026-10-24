# Hand chunk 394 (std::__partial_sort .. std::vector<CIntelligentPointer<CThing> >): 38 of 40 byte-exact

All 40 rows are `_global` in the catalog. Every matched source grades MATCH / RELOCATION_MATCH under gall.sh
(grades.txt) and its test passes under g++ (tall.sh) and VC7.1 (tall.py).

## What matched (38)

STLport sort family, particle pairs (CParticleUpdate* comparators)
- 0076be30 `__introsort_loop<pair<ulong,C3DVector>>`: STLport loop with an inline `__median` and the STLport pair copy
  constructor. C3DVector needs a user memberwise copy constructor here, or the pivot copy becomes `rep movsd`.
- 0076c080 `__introsort_loop<pair<ulong,CParticleUpdateParamDesc<ulong,ulong>>>`: same model. ParamDesc derives from
  CBaseClass (vtable 0x0125FEE4, `~CBaseClass` 0x0099A2E0).
- 0076bbc0 `__partial_sort<pair<ulong,ParamDesc>>`: built with the `__value_type` / `__distance_type` helpers and an inline
  `__pop_heap` that takes `_Tp __val` by value. `__make_heap`, `__adjust_heap` and sort_heap are out of line.
- 0076bcb0 `vector<ParticleAndEndPoints>::push_back`: STLport push_back over a 20-byte element (two CCharStrings and three
  longs), with the overflow at 0x0076B850. The caller is CTCObjectAugmentations::AttachAugmentationParticles.

STLport sort family, `long` with `less<long>` (signed compares)
- 00776d40 `__unguarded_partition`, 00776df0 `__unguarded_insertion_sort_aux`, 00776e50 `__adjust_heap` (calls
  `__push_heap` 0x00776750), 007781b0 `__partial_sort`, 00778490 `__introsort_loop` and 00777920 `__insertion_sort`.
  These are plain STLport under /Gr `__fastcall`.
  - `__insertion_sort` reaches memmove 0x00BFEAE6 through `__copy_trivial_backward`.
- 007772f0 `__make_heap`: `__adjust_heap` must be `__forceinline`. Plain `inline` leaves an 81-byte out-of-line call.

STLport containers of the weapon-trail TU (CTCWeaponTrail, CEnginePrimitiveWeaponTrail::CWeaponTrailEntry)
- CWeaponTrailEntry is 28 bytes: Point0, Point1 and Time. C3DVector has a user memberwise copy constructor throughout
  this TU.
- 0076fc60 `list::insert`: malloc'd 0x24-byte node.
- 0076fe10 `list::push_back`: byte twin of 0x004CD2D0.
- 0076fd40 `list::_M_insert_dispatch` (member template): needs the C3DVector copy constructor, or the copy becomes
  `rep movsd`.
- 007702b0 `__uninitialized_fill_n<list*>` and 00770350 `__destroy_aux<list*>`:
  - `clear()` frees through `free(tmp)`.
  - `~_List_base` frees through `allocator::deallocate` (`if (p) free(p)`).
- 00770d60 `vector<list>::resize(n)`: written as `resize(n, _Tp())`. The list default constructor must build only the
  allocator proxy (`_M_node(allocator<_Node>())`); STLport's `_M_node(a, 0)` leaves a dead `mov [esp+0Ch],0`.
- 007700d0 `vector<pair<C3DVector,C3DVector>>::_M_insert_overflow`: `_M_clear` is inline (a trivial destroy plus
  `if (p) free(p)`). The tag temporaries are homed on `__atend` through same-TU `auto_inline(off)` stand-ins.
- 0076f740 `__uninitialized_fill_n<pair<CCharString,CCharString>*>`: matched on the first grade.
- 007706a0 and 007704a0, `vector<pair<CCharString,CCharString>>`: push_back is modelled on the landed 0x0056A2D0 and
  `_M_insert_overflow` on the landed 0x004ADB50 (`_M_clear` declared only, 0x007701C0).
- 0076ea60 `CTCWeaponTrail::GetAllTrailsLastWeaponTrailPoints`: the loop inlines the public
  `GetLastWeaponTrailPoints(vector&, long)`.
  - That overload must be written as one call with a `?:` argument: `GetLastWeaponTrailPoints(points, i >= 0 ? Trail[i] :
    Trail[0])`. The landed two-arm form of 0x0076E5F0 duplicates the call when inlined.
  - The `?:` form also grades RELOCATION_MATCH for 0x0076E5F0 itself.
- 0076deb0 `GetProjectileTrailElementFromPoint` (file-local): `offset * 0.5f` is written in both pair arguments and
  VC7.1 CSEs it. Together with the C3DVector copy constructor this matched on the fourth grade. The camera comes from
  CGameCameraManager::GetRenderCamera(1.0f) (virtual slot 1), and the vector used is RHSet.Up.

Strings and other STL
- 00777db0 `basic_string<char>::reserve`: `_M_throw_length_error` (0x00565A30) must be `__declspec(noreturn)` for the
  shrink-wrapped pushes.
- 00777f80 `basic_string<char>::push_back`.
- 0077e0c0 `swap<pair<CCharString,C3DVector>>` (`__fastcall`): the C3DVector copy constructor plus implicit assignment.
- 0077e580 `vector<CIntelligentPointer<CThing>>::reserve`: twin of 0x006D4680 (callees 0x004AA140 / 0x004AC1D0).
- 00777790 `_Rb_tree<long, pair<const long,CShopItem>>::_M_insert`: the landed 0x006AE900 model with STLport's pair copy
  constructor. The key is a dword move and the 28-byte CShopItem is `rep movsd` x7 inside the placement-new null test.

CTCEnemy / CTCSolidToThing / CShopDef / CTCHeroPodium
- 0076d4f0 `CTCEnemy::AreThingsAllies` (static `__fastcall`): needs a same-TU `auto_inline(off)` stand-in for the private
  `CalculateEnemyTC` 0x0076CFE0 that writes both out parameters without leaking their addresses. Then VC7.1 picks
  retail's slots and keeps enemy2 in esi across the first IsAllyOf call. The answer is `return ally1 && ally2;`
  (`mov eax,1` / `xor eax,eax`), and the early rejections are `return false` (`xor al,al`).
- 00770de0 `CTCSolidToThing::IsSolidToThing(const CThing*) const`: an empty-list early `true`, then
  `it = find(begin, end, CIntelligentPointer(thing)); return it == end();` as two statements, so end() is re-read after
  the temporary's destructor.
- 007773a0 `CShopDef::Test() const` (FableWin.pdb): two `for` walks with empty bodies over the attitude price maps
  (+0x64, +0x74). The STLport increment is inlined.
- 00771c70 `CTCHeroPodium::GetPositionForMember(long) const` (FableWin.pdb):
  - `fraction = member / 8.0f` must be a separate statement. Inside one expression VC7.1 folds `/8 * 0.5` into
    `* 0.0625`.
  - The sum is written `E_Y * cos + E_X * sin`, so the sine term (the right operand) is evaluated first.
  - The angle is multiplied by the double 2*(float)pi.

CTCDExperienceOrb
- 00779e10 `SetExperiencePoints(long)`: a map `for` with `break`. The scale lives in the dead argument slot, and the call
  is a tail jump to CTCGraphicAppearance::SetScale.
- 00779e90 `Create(long, const C3DVector&)` (static `__fastcall`): a `CThing* result = 0` local, set on success, keeps
  the null in ebx (`xor ebx,ebx` ... `mov eax,ebx`). Other forms give `xor eax,eax`, or a `neg/sbb/and` select for early
  returns.
- 0077a350 `GetExperienceOrbsToCreateForTotalExperience(long, vector<long>&) const`: a while loop over the parameter
  with an inline lower_bound, an inline `--it` (STLport `_M_decrement`) and `max(it->first, 1L)`.
  - Needs a same-TU `auto_inline(off)` stand-in for `vector<long>::_M_insert_overflow` 0x00440990.
  - With the stand-in, the `1L` temporary and the tag are homed in the dead `total` slot, and the orb value stays in esi
    across the call.

Free functions
- 00778610 `GFGetValPositionInLimitsLooping` (PDB name, `__fastcall`, the out reference in ecx):
  - A plain window returns `(v - s) / (e - s)`.
  - A wrapping window builds `offset` and `range` from the SAME sum `(end - min) + tail`; VC7.1 CSEs the two into one
    spill copied through eax and pops the dead `tail`.
  - A single shared local gives 200 bytes against retail's 221.
- 007720d0 `MoveTowards(current, target, step)` (`__stdcall`; the name is descriptive): `fabs(diff) < step` snaps,
  otherwise it steps down or up.
- 0077c360 looped-range `GetFraction(t) const` (descriptive CLoopedRange): a start/end lerp that wraps through 1, then a
  clamp to `[0, 1 - g_epsilon]`. The epsilon is a global float, so the bound is computed at run time.

## Parked for Fable 5.1 (2)
- 0076d1b0 CTCEnemy::AddEnemyThing DIFFER(177v179): before SetHeroOpinionEnemy(true), retail `mov eax,[eax+4] ; push 1 ; mov ecx,eax` vs ours `mov ecx,[eax+4] ; push 1` (everything else matches) | tried: extern __find with a direct `const_cast` binding (tag got its own frame byte, enemy reloaded); rvalue `(CThing*)enemy` (load/store of enemy into its own slot); forwarding same-TU __find stand-in (still reloaded); full-body same-TU __find stand-in reading the value without leaking it (only the tail left) | next: route the opinion component through a named local or the PDB out-pointer form `GetTC<CTCCreatureOpinionOfHero>(&opinion)` inline
- 00770790 std::vector<list<CWeaponTrailEntry> >::erase DIFFER(78v78): the __destroy_aux empty tag is homed at `lea ecx,[esp+14h]` (ours, the dead `last` slot) vs `lea ecx,[esp+10h]` (retail, the live `first` slot read back for the return) | tried: plain STLport; `__forceinline __copy` (fixed the inlining); a named `_Trivial_destructor` tag local in `_Destroy`; a POD `__false_type` | next: a same-TU `auto_inline(off)` __destroy_aux stand-in that never touches the tag (the 0x004ADB50 / 0x0077A350 recipe)

## Skipped
- None. No row holds two functions.

## Label corrections
| addr | catalog label | actual |
|---|---|---|
| 0076bbc0 | Partial_Sort_Pair16_TextureMorph | std::__partial_sort<pair<ulong, CParticleUpdateParamDesc<ulong,ulong>>*> |
| 0076bcb0 | CCombatWheel_InsertBrainUpdateZone | vector<ParticleAndEndPoints>::push_back |
| 0076be30 | Sort_IntrosortPhase_4UintElem | std::__introsort_loop<pair<ulong, C3DVector>*> |
| 0076c080 | Sort_IntrosortPhase_Sorted_4Uint | std::__introsort_loop<pair<ulong, ParamDesc>*> |
| 0076d1b0 | CCreature_AddEnemyToList | CTCEnemy::AddEnemyThing(const CThing*) |
| 0076d4f0 | AreThingsAllies | correct; the class is CTCEnemy (static `__fastcall`) |
| 0076ea60 | WeaponTrail_ProcessCollisionPoints | CTCWeaponTrail::GetAllTrailsLastWeaponTrailPoints |
| 0076f740 | _Fill_n<unsigned_long*,...> | std::__uninitialized_fill_n<pair<CCharString,CCharString>*> |
| 0076fc60 | WeaponTrail_CreateNode | std::list<CWeaponTrailEntry>::insert |
| 0076fd40 | LinkedList_InsertAll | std::list<CWeaponTrailEntry>::_M_insert_dispatch |
| 0076fe10 | WeaponTrail_Callback | std::list<CWeaponTrailEntry>::push_back |
| 007700d0 | Vector_Insert_CAnimatedBlock_MoveAndRealloc | vector<pair<C3DVector,C3DVector>>::_M_insert_overflow |
| 007702b0 | _Fill_n<unsigned_long*,...> | std::__uninitialized_fill_n<list<CWeaponTrailEntry>*> |
| 00770350 | LinkedList_DestroyRange | std::__destroy_aux<list<CWeaponTrailEntry>*> |
| 007704a0 | Vector_Insert_CCharString_Pair_MoveAndRealloc | vector<pair<CCharString,CCharString>>::_M_insert_overflow |
| 007706a0 | Vector_PushBack_String | vector<pair<CCharString,CCharString>>::push_back |
| 00770790 | LinkedList_RemoveAll_WithShift | vector<list<CWeaponTrailEntry>>::erase |
| 00770d60 | LinkedList_Phoneme_Insert | vector<list<CWeaponTrailEntry>>::resize(n) |
| 00770de0 | ContainerFindElement | CTCSolidToThing::IsSolidToThing(const CThing*) const |
| 00771c70 | CVertexBufferWin32_ComputeTransformedPosition | CTCHeroPodium::GetPositionForMember(long) const |
| 007720d0 | Math_LerpFloat | MoveTowards (a clamped step, not a lerp; descriptive name) |
| 00776d40 | Quicksort_Partition_IntElem | std::__unguarded_partition<long*, long, less<long>> |
| 00776df0 | _Copy_impl<CComplexNumber*,...> | std::__unguarded_insertion_sort_aux<long*> |
| 00776e50 | _Adjust_heap<CThing**,...> | std::__adjust_heap<long*, int, long, less<long>> |
| 007772f0 | MakeHeap | std::__make_heap<long*, less<long>> |
| 007773a0 | CTCShop_ClearLists | CShopDef::Test() const |
| 00777790 | RBTree_Insert | _Rb_tree<long, pair<const long,CShopItem>>::_M_insert |
| 00777920 | _Insertion_sort1<ENavigatorType*,...> | std::__insertion_sort<long*, less<long>> |
| 00777db0 | CString_Reserve | basic_string<char>::reserve |
| 00777f80 | CCharString_PushBack_Char | basic_string<char>::push_back |
| 007781b0 | Sort_PartialHeap_ThingNearest | std::__partial_sort<long*, less<long>> |
| 00778490 | Sort_IntrosortPhase_IntElem | std::__introsort_loop<long*, less<long>> |
| 00779e10 | CExperienceOrb_SetScale | CTCDExperienceOrb::SetExperiencePoints(long) |
| 00779e90 | Create | CTCDExperienceOrb::Create(long, const C3DVector&) |
| 0077a350 | SortedMap_FillFromSortedTree | CTCDExperienceOrb::GetExperienceOrbsToCreateForTotalExperience |
| 0077c360 | Interpolate_Clamped | a looped-range fraction getter (descriptive CLoopedRange::GetFraction) |
| 0077e0c0 | swap<C3DSkeletalBoneMorph> | std::swap<pair<CCharString,C3DVector>> |
| 0077e580 | vector_reserve_CConversation | vector<CIntelligentPointer<CThing>>::reserve |
