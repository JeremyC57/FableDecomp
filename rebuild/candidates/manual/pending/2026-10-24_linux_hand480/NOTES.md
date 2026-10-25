# Hand chunk 480 (std::__find_if .. NCreatureMode::IsValidRangedWeaponTargetForShooter): 32 of 40 byte-exact

All 32 src/ rows grade RELOCATION_MATCH through gall.sh (grades.txt). Their tests PASS under g++ (tall.sh) and under
VC7.1/Wine (tall.py). The call-target check (calls.py) shows every import going to retail's thunk: malloc 0xBFEA0E,
free 0xBFEA14, memmove 0xBFEAE6, puts/abort, atoi 0xBFED28, atol 0xBFED2E, __ftol2, time.

calls.py lists no relocations for 00840b60, because objdump splits that function at `$L` labels. I checked it by hand
on a label-stripped copy of the object (probe/calls_s.py): every target is retail's.

## What matched (32)

### STLport containers and algorithms (namespace std, STLport 4.6 text verbatim)

**`__find_if`**
- 0080e060 `__find_if<pair<CIntelligentPointer<CThing>,long>*, CIsFirstElementEqualTo>` (/Gr). This is the explosion
  emitter list's find. The 4-way unrolled loop is verbatim. The 8-byte predicate is passed by value and destroyed by the
  callee before each return.
- 00841900 `__find_if<const char*, _Not_within_traits<char_traits<char> > >`. This is the find_first_not_of worker.
  - VC7.1 inlines the inner find_if only at the first of the seven predicate calls.
  - The `_Eq_char_bound` byte and the tag share the dead set-first argument slot.

**`vector::insert` / `_M_insert_overflow` (single element and growth)**
- 0080ef50 `vector<pair<CIntelligentPointer<CThing>,long> >::insert(pos, x)`. `__n = pos - begin()` comes first, then the
  STLport shift-up / overflow arms.
- 00818ca0, 0081c360, 0081c890, 0081c990, 00826450, 0082cd80 `vector<T>::_M_insert_overflow` for:
  - pair<CHitLocation,long>;
  - CParticleMorphs::CEntry;
  - the two hero-morph pairs;
  - CScriptedMapBrushUpdateArea;
  - CMinimapData.

  How they matched:
  - `(max)` takes its arguments by reference, landing in the `push ecx` word, and the allocate is inline.
  - The copies and the fill are same-TU noinline bodies that never touch the tag, so the tags share the `atend` slot.
  - Trivially copyable elements get `__forceinline` uninitialized loops (`rep movsd`).

**`vector::_M_fill_insert`**
- 0081cbb0, 0081cd00, 0081d0b0, 0081d200 `vector<T>::_M_fill_insert`, following the 0x007D24F0 instantiation.
  - Every empty tag and both spills share the dead x slot. This needs five out-of-line helpers as same-TU
    `auto_inline(off)` + noinline template bodies.
  - For trivially copyable elements, retail outlines the tag dispatchers instead of the workers.

**`__introsort_loop`**
- 0081db70, 0081dc80, 0081dd90 `__introsort_loop` for:
  - pair<EHeroMorphType,CTextureMorphEntry>;
  - pair<EHeroMorphType,CParticleMorphs::CEntry>;
  - pair<unsigned long,CSkeletalMorphs::CEntry>.

  These follow the landed 0x009B7ED0 family:
  - `--depth_limit` is written back to its argument slot;
  - `__median` is inline and returns a reference;
  - the pivot uses STLport's memberwise pair copy;
  - the comparator `operator()` is a same-TU noinline stand-in.

**`_Rb_tree::insert_unique`**
- 0082f930 `_Rb_tree<unsigned __int64, pair<const unsigned __int64, CScriptedMapBrush::CThingPosAndOrientation> >::insert_unique(hint, v)`.
  This is the hinted unique insert (static `_Rb_global::_M_increment/_M_decrement`).

### LHNetworkLib (the Lionhead HTTP library TU, STLport 12-byte strings)
- 0083f9b0 `LHHttp2::Open()`.
  - `Hostname == ""` compiles to the inline size test plus a zero-length memcmp.
  - The proxy/host choice assigns a temporary.
- 008405f0 `LHHttp2::SendRequestAsync()`. `{ Close(); Reset(); }` with Close inline. The status codes are -104 / 2 / -100.
- 008410d0 `LHHttp2::~LHHttp2()`.
  - Close is inline (the socket block, then Reset), then the dtor's own Reset.
  - The members' implicit dtors run in reverse order, with an inline `~LHHttpHeaders() { Reset(); }`.
- 00840b60 `LHHttp2::GetHeader(LHHttpHeaderStatus&)`. It copies ReceivedHeaders (+0x40) into the status, then parses:
  - ServerCode;
  - Location: (copied with operator=);
  - Content-Length (-1 when absent);
  - Transfer-Encoding (chunked sets DocumentEncoding +0x890 to 1).

  It returns 1000. The shape that matched:
  - The outer test is `!empty()`. With `begin() != end()`, VC folds the loop-entry test that retail keeps.
  - The server code goes through a named `int code = atoi(...)`, so the substr temporary is freed before the store.
  - The TU carries same-TU noinline bodies for:
    - LHHttpHeaders::GetHeader with StartsWith/toLowerCase;
    - `_M_insert_overflow`, `_M_assign_dispatch` and `_M_append`.

    This drops the literal temporaries' zero stores, keeps their block in esi across the call, and homes every tag on
    the live `status` slot.

### Gameplay / engine
- 008158d0 `NHitLocations::CPhysicalPrimitiveCylinder::GetCylinderBaseAndTop(const CThing&, C3DVector&, C3DVector&) const`:
  - the physics pointer is loaded before the def pointer is copied (esi joins the prologue pushes);
  - a named `const CRightHandedSet&` is copied into `C3DVector up`, with C3DVector's member-wise copy ctor;
  - the dummy-object name CCharString is a block-scoped temporary, homed on the dead `thing` slot;
  - the `top = ...` store sequence is tail-merged across both arms.
- 00815f00 `GetPBestHitLocationFromList` (anon namespace):
  - `equal_range` is inline over a named copy of the last key;
  - two accumulation orders (x-first then z-first) come from two statement forms;
  - the running best is kept in the dead `origin` slot.
- 00828360 `CRegionConnectionNode::GetNextNodeOnWayToRegion`:
  - a BFS over a local STLport `list<const CRegionConnectionNode*>`;
  - the graph's inline GetRegionNode;
  - LowerBound as a same-TU noinline body.
- 00836ad0 `CThingSoundEmitter::RenderUpdate`:
  - the whole body sits under `if (!Dead)`, with one shared return-false tail;
  - IS_THING_ALIVE is inlined at its first call only.
- 0083d800 `CThingPhysicalSwitch::Save(CCharString&)`:
  - an explicit empty `CCharString("")` starts the line;
  - `name == "" ? CCharString("NULL") : name` is a conditional temporary.
- 00845ce0 `CCreatureAction_DropWeapon::DropWeapon(CThing& weapon, CTCCarrying& carrying, CThing& creature)` (static
  __fastcall, `ret 4`). This follows the PickUpGoods 0x00848730 TU model:
  - same-TU GetSubDef<CCarryableDef>/<CWeaponDef> and LowerBound (non-const/const) stand-ins keep the weapon def in edi
    and drop the carryable def's null test;
  - the weapon TC is an if/else-assigned pointer (`mov ecx,[eax+4]; test ecx,ecx`);
  - GetInterface goes through an arrow-ternary GetTC, which keeps the merged solid-to-thing pointer in ecx;
  - the facing direction is `(float)sin(a * 2pi) * cosPitch` (a float product) into a named C3DVector, copied into a
    named CRightHandedSet passed to SetRHSet;
  - the AddTC name is a block-scoped CCharString.
- 0084d0e0 `C3DFrustrum::UpdateFrustrum()`:
  - the weighted point sum stays on the x87 stack;
  - the integer weight total uses fild/fadd/_ftol;
  - operator/ is inline and forms its own reciprocal.
- 00856040 `ProcessFramesRunningForOnHitWithParams` (anon namespace):
  - C3DVector differences are assigned into pre-declared locals;
  - Clamp is inline.
- 00859040 `GetThingDoingDamageFromCreatureAndEventName` (anon namespace):
  - each arm has a named CCharString per name test;
  - same-TU CCharString copy/dtor/ReleaseData stand-ins keep the copied name's block in esi.
- 0085f6f0 `NCreatureMode::GetUpFromSleeping`:
  - the mode manager is an unset local, read uninitialised on the absent path;
  - one branch builds a temporary action, the other a named action, and they share one frame slot.
- 0085f8d0 `NCreatureMode::IsValidRangedWeaponTargetForShooter`:
  - the player is a named local;
  - fcos is inline, compared as a double;
  - the early `return false` tail is shared.

## Parked for Fable 5.1 (8)
- 0081ea50 CMap::GetGroundSizeZAt DIFFER(345v391): retail rounds both coordinates through the inline-asm pointer-out helper (`fld ; mov edx,ptr ; fistp dword ptr [edx]`, 12-byte frame) where ours calls __ftol2 twice (8-byte frame); clamps, cell addressing and the triangle interpolation match | tried: (long) casts (1 grade, per the retail_asm rule) | next: retail_asm candidate (C++ body + minimal `fistp` __asm under the owner's 2026-10-24 exception)
- 0083ecf0 LHNetworkLib::toUpperCase DIFFER(307v281): retail keeps the local string's _M_start/_M_finish in edi/esi through the toupper loop (no zero stores before the 8-byte malloc); ours keeps every field in memory and re-reads after toupper | tried: 4 grades of STLport string forms (push_back / += / reserve placement, named vs temporary result) | next: grade differ/0083ecf0.cpp as it stands now. It is the 0x00840B60 TU model with a same-TU `#pragma auto_inline(off)` body for basic_string::reserve 0x777DB0, and a scratch compile of it is instruction-identical to retail (relocations only). It was not graded because the 4-grade budget is spent.
- 0083f420 LHNetworkLib::LHHttpHeaderStatus::ParseLocationData DIFFER(427v403): the "http://" temporary's block pointer is in esi in retail (no zero stores, `test esi` before free) and the `== ""` guards end in `test eax,eax`; ours zero-stores the three words, re-reads them and keeps 0 in edx | tried: 4 grades (StartsWith forms, temporaries, find forms) | next: grade differ/0083f420.cpp as it stands now. It is the 0x00840B60 TU model plus `__declspec(noinline)` find(const char*, size_type) 0x841790 and substr 0x569330 members, and a scratch compile of it is instruction-identical to retail (only the four string-literal relocations differ). It was not graded because the budget is spent.
- 0083f650 LHNetworkLib::LHHttpHeaders::GetHeader DIFFER(497v442): retail inlines the 0x567D00 range initialise at the loop copy and `return ""` but calls it at `return line`; VC7.1 RTM inlines all three, moving line's size from ebx to a spilled ebp | tried: in/out-of-class range init, STLport category layers, `#pragma inline_depth(1..3)`, auto-inline StartsWith, placement _Construct | next: lane gap: inline-budget (per-site inline choice)
- 0083fce0 LHNetworkLib::LHHttpHeaders::GetHeaderString DIFFER(661v660): int3-after-noreturn only (the trailing 0xCC after `call 0x565A30`); the same file grades RELOCATION_MATCH against the N+1 span (qn1.py) | tried: 2 grades + N+1 check | next: land by hand against the N+1 row (differ/0083fce0.cpp + differ/0083fce0.test.cpp, both lanes PASS)
- 008411d0 LHNetworkLib::LHHttp2::HelperGetDocument DIFFER(510v510): the materialised `Open()==LH_OK && PrepareRequest()==8` bool sits at [esp+0xa] (ours) v [esp+0xb] (retail, sharing the allocator temporary's byte); every other byte matches | tried: if-condition, named bool, ternary, inline helper, explicit string(""), default args, allocator with/without dtor, early return (1 grade) | next: lane gap: frame-slot packing of a bool with an empty-class temporary
- 00856310 ProcessWeaponTrailPointsToIncludeCylinderCloseToCreatureAxis DIFFER(594v600): retail keeps dying x87 operands (`fld st(i) ; fmul st(j)`, then `fstp st(3)`/`fstp st(0)` clean-up) and a 0x60 frame; ours consumes them in place (`fxch`/`fdivrp`), 0x58 frame | tried: plain floats, C2DVector temporaries (ICE C1001 fppeeps.c:1368), named offsets, operand orders, helpers, scoped blocks, /Op /O1 /Os /Ot (1 grade) | next: lane gap: x87 stackifier
- 00858e00 GetHelperPointNamesFromStrikeEventName DIFFER(575v575): retail homes the copied event name in the carried slot ([esp+8]) and the "weapon_pos_" + ToLower() temporary in the fixedPoints slot ([esp+0xc]); ours swaps the two | tried: direct-init name, explicit CCharString(...) around the sum, block-scoped fixed points, same-TU CCharString stand-ins, ctor-less __false_type (1 grade) | next: lane gap: dead-argument-slot assignment order

## Skipped
None. All 40 rows are single bodies.

## Label corrections
Catalog names (propagated / autonamed), corrected from the PDB name sets, callers and the matched bodies:

**STLport rows**
| Address | Catalog name | Correct name |
|---|---|---|
| 0080e060 | std_find_CBaseIntelligentPointer | STLport `__find_if<pair<CIntelligentPointer<CThing>,long>*, CIsFirstElementEqualTo>` |
| 0080ef50 | std_vector_InsertRange | `vector<pair<CIntelligentPointer<CThing>,long> >::insert(iterator, const T&)`, a single element, not a range |
| 00818ca0 | std_vector_InsertAndMove | `_M_insert_overflow` |
| 0081c360 | std_vector_InsertRepeatFill_36Byte | `_M_insert_overflow` |
| 0081c890 | std_vector_AllocateAndFill | `_M_insert_overflow` |
| 0081c990 | Vector_Reallocate | `_M_insert_overflow` |
| 00826450 | StdVector_ReallocateAndReserve | `_M_insert_overflow` |
| 0082cd80 | CEditPrimitiveStats_CreateDynamicTexture | `vector<CMinimapData>::_M_insert_overflow` |
| 0081cbb0 | std_vector_Resize_Insert | `_M_fill_insert` |
| 0081cd00 | std_vector_Reserve_InPlace | `_M_fill_insert` |
| 0081d0b0 | std_vector_Insert_Helper_0x14 | `_M_fill_insert` |
| 0081d200 | std_vector_Insert_Helper_0x24 | `_M_fill_insert` |
| 0081db70 | std_introsort_6ElementQuickSort | `__introsort_loop` |
| 0081dc80 | std_introsort_9ElementQuickSort | `__introsort_loop` |
| 0081dd90 | HeapSort_QuicksortPartition | `__introsort_loop` |
| 0082f930 | CMemoryAllocatorVariableSize_InsertWithBalance | `_Rb_tree<unsigned __int64, pair<..., CScriptedMapBrush::CThingPosAndOrientation> >::insert_unique(hint, v)` |
| 00841900 | String_FindFirstOf | `__find_if<const char*, _Not_within_traits>` (find_first_NOT_of) |

**Gameplay and engine rows**
| Address | Catalog name | Correct name |
|---|---|---|
| 008158d0 | GetDummyObjectOriginTransformed | `NHitLocations::CPhysicalPrimitiveCylinder::GetCylinderBaseAndTop(const CThing&, C3DVector&, C3DVector&) const` |
| 0081ea50 | Terrain_InterpolateHeightAtPosition | `CMap::GetGroundSizeZAt(const C3DVector&) const` |
| 00828360 | Navigation_SortRegionConnectionTree | `CRegionConnectionNode::GetNextNodeOnWayToRegion(long, const CRegionConnectionGraph&) const` |
| 00836ad0 | CThingFilter_IsUnconscious_Apply | `CThingSoundEmitter::RenderUpdate(const CInterpolationInfo*)` |
| 0083d800 | CThingPhysical_SaveTriggeredByThing | `CThingPhysicalSwitch::Save(CCharString&)` |
| 00845ce0 | DropWeapon (module `_global`) | static `CCreatureAction_DropWeapon::DropWeapon(CThing&, CTCCarrying&, CThing&)` (Ego_r `SIX`: static __fastcall) |
| 0084d0e0 | CThingCreatureBase_ComputeMeleeMetrics | `C3DFrustrum::UpdateFrustrum()` |
| 0085f6f0 | GetUpFromSleeping | in namespace `NCreatureMode` |
| 0085f8d0 | IsValidRangedWeaponTargetForShooter | in namespace `NCreatureMode` |

**LHNetworkLib rows**
| Address | Catalog name | Correct name |
|---|---|---|
| 0083ecf0 | CTCElectrocutedEffect_Init | `LHNetworkLib::toUpperCase(const string&)` |
| 0083f420 | LHHttp2_ParseURL | `LHHttpHeaderStatus::ParseLocationData()` |
| 0083f650 | ProcessTextWithRegex | `LHHttpHeaders::GetHeader(const string&)` |
| 0083f9b0 | LHHttp_InitializeConnection | `LHHttp2::Open()` |
| 0083fce0 | LHHttp_ParseResponseHeaders | `LHHttpHeaders::GetHeaderString()` |
| 008405f0 | LHHttp2_SendRequest | `LHHttp2::SendRequestAsync()` |
| 00840b60 | LHHttp2_ParseResponseLine | `LHHttp2::GetHeader(LHHttpHeaderStatus&)` |
| 008410d0 | CNetworkConnection_Cleanup | `LHHttp2::~LHHttp2()` |
| 008411d0 | LHHttp2_HandleResponse | `LHHttp2::HelperGetDocument(bool)` |

**Callees seen in this chunk's call-target check (coverage/manifest labels wrong)**

Grouped by the row whose decode or call site found them.

DropWeapon decode (00845ce0):
| Address | Wrong label | Correct name |
|---|---|---|
| 0x006A11F0 | "CThing_Array_FindByID" / FUN | `CTCCarrying::GetThingInCarrySlot(long)` |
| 0x005CC10E | `GetSubDef<CAbilityDef>` (propagated) | `GetSubDef<CCarryableDef>` |
| 0x0042B1A6 | `GetSubDef<CAbilityDef>` (propagated) | `GetSubDef<CWeaponDef>` |

GetHeader decode (00840b60):
| Address | Wrong label | Correct name |
|---|---|---|
| 0x00567120 | "StringSearch_FindPattern" | STLport `search<const char*, const char*, _Eq_traits<char_traits<char> > >` |
| 0x00569140 | "std::string_InsertRange" | `basic_string::_M_assign_dispatch<char*>` |
| 0x00569E30 | "Vector_ReallocateStructArray" | `vector<string>::_M_insert_overflow` |
| 0x0083EE10 | "HTTP_CompareStrings" | `LHNetworkLib::StartsWith(const string&, const string&)` |

Other call sites:
| Address | Wrong label | Correct name |
|---|---|---|
| 0x004C73D0 | "CVertexBufferWin32::DoSizeof" | `CThing::GetPosition() const` |
| 0x00856570 | — | `IsThingWithHelperPoints` (anon namespace) |
| 0x0083FB20 | — | Landed as SetConnectionDetails. At the HelperGetDocument call site it is Close/Reset, then "Open(host, port)" (stores host and port, then Open()). |

Coverage names to re-check: these come from this chunk's matched callers, one call site each.
| Address | Coverage name | Name from the matched caller |
|---|---|---|
| 0x0099EAE0 | `_Cons_val<...>` | `~CCharString` |
| 0x0088B8F0 | "RemoveAvailableQuestCardType" | `CAIBrain::SetBrainOutput` |
| 0x0073A6E0 | "CountReferences" | `CTCCreatureModeManager::GetCurrentMode` |
| 0x00415FBC | "GFGetPreMainMemoryUsed" | `CMainGameComponent::PeekDefinitionManager` |
| 0x00414EA0 | "ResetRings" | `~vector<CCharString>` |
| 0x008F9780 | "Kill" | `~LSocket` |
| 0x00A14440 | "GetScaled" | `C3DVector::Normalise` |
| 0x00693EF0 | FUN | `~CCreatureActionBase` |
