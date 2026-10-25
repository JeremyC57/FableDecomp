# Hand chunk 404 (NCameraModes .. std::__partition): 37 of 40 byte-exact

All 37 sources in `src/` grade MATCH (6) or RELOCATION_MATCH (31) under `gall.sh` (`grades.txt`). Every one has a
behaviour test in `test/` that passes under g++ (`tall.sh`) and VC7.1 under Wine (`tall.py`). Lengths are the oracle
bytes with the trailing int3 padding removed. No row is mis-cut: every catalog length is the function plus padding up
to the next row.

## What matched (37)

- **NCameraModes camera/theme helpers (CTools TU)**
  - 0x874F10 IsThemeToBlockPassabilityBetween: both IsPosPassable results are taken first. The per-arm
    `if (!passableB) return true;` blocks are cross-jumped, which leaves a dead `test bl,bl`. With a shared
    `return false` after the if/else the exits stay `jne ; mov al,1 ; ret 8 ; xor al,al ; ret 8`. Per-arm
    true/false returns fold to `sete`; `if (passableB) return false;` arms swap the exits.
  - 0x874F40 GetCameraHeightfieldHeightAtPos: the ground height is a named float in the `push ecx` word; the water
    height wins only when strictly higher.
  - 0x874F80 IsLandscapeToBlockCameraBetween: the vtable is fetched once per end into a callee-saved register. The
    two early-true exits keep their own epilogues. a's height is homed in b's dead argument slot.
  - The PDB functions take `const CWorldMap&` / `const C3DVector&`; references and pointers give identical bytes.
- **NCameraModes::CGetAllThingsOfType::operator() 0x877110** (PDB functor {char* DefName; list<const CThing*>*
  ListToChange}): `thing->GetDefName() == DefName` homes the CDefString result in the dead `thing` slot (user copy
  ctor, no dtor), then an inline STLport `push_back` (sentinel read before the malloc).
- **NCameraModes::CProjectileWeapon::GetLookDirectionXYFromParams 0x878B60**: the angle is
  `phi - (offset * (1/360) - 0.25f)`. A NAMED offset local is overwritten by `C3DVector(y, -x, 0) * -1.0f` and
  returned by copy, which gives the two frame vectors and the folded -0.0f Z. Returning the product directly gives
  85 bytes.
- **CTurningAnimCompare family (InterruptableMidAttackAutoTurn TU, /Gr)**
  - 0x87C660 binary operator() (retail reads `a` twice): `if (...) return true; return false;` gives byte exits.
  - 0x87DA90 __median: the in-class comparator is inlined at its first call only.
  - 0x87DB50 __find_if: four-way unroll plus fall-through switch.
  - 0x87DC30 __unguarded_linear_insert.
  - 0x87DDD0 remove_copy_if.
  - 0x87E250 __linear_insert: pointer copy_backward reduces to one guarded `memmove`.
- **NScreenEffect bypass map (CScreenFilterEntry, packed 9-byte CThingGraphicByPassFlagState)**
  - 0x883AE0 Reset: end() is taken into a local; `_M_increment` is inlined.
  - 0x8840D0 _M_create_node and 0x884120 _M_clone_node.
  - 0x8843F0 _M_insert (STLport 4.x verbatim).
  - 0x884490 _M_copy: the inline clone is called out of line for the top node.
  - 0x884C70 map::operator[]: the packed `_Tp()` temporary sits at an odd frame offset.
- **Environment-theme STLport helpers**
  - 0x885DD0 / 0x889800 uninitialized_fill_n: the `__false_type` helper is inlined and the tag leaves no slot.
  - 0x889300 __uninitialized_copy over the 48-byte stack entry.
  - 0x8897D0 fill: out-of-line operator=.
- **Keyed-pair introsort tail (descriptive SKeyedPair/SKeyGreater)**: 0x88C780 __unguarded_linear_insert and
  0x88C9D0 __unguarded_insertion_sort_aux. The 8-byte pair needs a memberwise copy ctor; the empty comparator is
  passed by value.
- **GFRemapToThingAfterSerialisation 0x88AFF0**: an out-parameter GetInterface (0xEB) through a same-TU
  `auto_inline(off)` LowerBound stand-in, so the `CIntelligentPointer` temporary shares the key's frame word.
- **Vector filters**
  - 0x8AABC0 unary_negate<CVectorFilter_FurtherFromPointThan>: a C2DVector delta with GetLengthSquared gives
    X-first squares (named dx/dy floats square Y first).
  - 0x8AC060 / 0x8C34A0 remove_if: a same-TU noinline `__find_if` body homes the tag in the live predicate slot.
    Memberwise copy ctors on C3DVector and the predicate give the dword-by-dword by-value copies.
  - 0x8E2DB0 / 0x8E3150 swap<C3DVector>: a memberwise copy ctor routes the temporary through the frame.
  - 0x8E3430 __partition: iter_swap -> swap is inlined.
- **CScriptThing vector**
  - 0x8AC2A0 __copy: the counted loop with CCountedPointer::operator= -> ShareData inlined.
  - 0x8ADF90 push_back: a same-TU `_M_insert_overflow` stand-in homes the tag in x's live slot.
- **Combat**
  - 0x8AF220 CombatDialogueManager ctor: map ctor inline; `PCurrentTalker = 0` goes through a temporary;
    CurrentTalkerTextGroupIndex is zeroed last.
  - 0x8B3430 CCombatSequenceBase ??_G needs three things: `virtual __forceinline` dtor, `__forceinline` vector
    dtors, and a same-TU `__destroy_aux` stand-in plus an out-of-line ctor that anchors the vtable.
  - 0x8B8050 CCreatureAction_PlayCombatAnimation ctor: push_back of an rvalue address via the dead usePhysics
    slot, with the `__true_type` overflow stand-in.
  - 0x8C7E50 CompareActorRotations: `(float)atan2(dx, dy) * INV_TWO_PI` gives `fmul dword`. An inline fabsf plus
    a named float difference fixes the pop/fcomp order.
  - 0x8D45A0 CFilter_Bind4<...>::operator()<CEventHitBy*>: `return F1 && F2 && F3 && F4;` gives eax 1/0 exits;
    filters 2-4 are inlined.

## Parked for Fable 5.1 (3)

- 0087c6b0 CTurningAnimCompare::operator() DIFFER(150v146): retail `fmulp st(1)` of the two cosines on the x87 stack, then adds sinB*sinA from the frame; ours `fld [esp] ; fmul [esp+8]` (sines first) then `fxch ; fmul st(2) ; faddp ; fstp st(1)` | tried: C2DVector (sin, cos) with X- and Y-first Dot, (cos, sin) vectors, scalar sin/cos locals (160) | next: statement-order dot (`d = cosA * cosB; d += sinB * sinA;`, GOTCHAS retry-10 rule), else lane gap: single-term x87 commutation
- 00875a30 NCameraModes::CTools::Get2DRotationAngle DIFFER(228v222): dot product and length product start with the `to` copy (`fld b.Y ; fmul a.Y`, |b| first) where retail starts with `from`; `angle * sign` is tail-duplicated into the negative arm (`fstp st(0) ; fmul [-1.0f]` + own epilogue) where retail reloads -1.0f and joins at one `fmulp` | tried: scalar float copies (frame 0xc), implicit C3DVector copies (Z copied), Z-unwritten `C3DVector(x, y, 0)` copies with `to` declared first (frame, loads, clamp, acos and cross product all match), `from` first + `__forceinline` sign helper (same two diffs) | next: statement-order dot/length (`d = a.X*b.X; d += a.Y*b.Y;`, separate length locals); else lane gap: single-term x87 commutation + tail duplication
- 00889cb0 std::vector<CBlendedEnvironmentTheme>::reserve DIFFER(238v230): only the frame -- retail homes both empty `__false_type` tags on n's argument slot (`push ecx` frame), ours gives the tag its own frame byte (`sub esp,8`), shifting every esp offset | tried: temporary tags, ctor-less POD tag (248), `_Is_POD()`/`__destroy(_Tp*)` tag sources, alloc_proxy allocator | next: GOTCHAS 24l -- same-TU `auto_inline(off)` + `noinline` bodies for __uninitialized_copy 0x00889700 and __destroy_aux 0x006B67A0

## Skipped

None (no row has int3/nop padding followed by another function).

## Label corrections

- 0x874F10 CWorldMap_AreBothPosCameraBlocked_v2 -> NCameraModes::IsThemeToBlockPassabilityBetween. Evidence:
  Ego_r.pdb name set, its IsPosPassable 0x4FCC40 callee, and the twin 0x874EE0 (calls 0x4FCD60), which is
  NCameraModes::IsThemeToBlockCameraBetween. The landed 0x8754A0 declares both as CameraBlockTest_*.
- 0x874F40 / 0x874F80 -> NCameraModes::GetCameraHeightfieldHeightAtPos / NCameraModes::IsLandscapeToBlockCameraBetween:
  namespace functions taking const references.
- 0x875A30 Math_ComputeVectorAngle -> NCameraModes::CTools::Get2DRotationAngle(const C3DVector&, const C3DVector&) const.
- 0x877110 CThingList_AddThingByDefName -> NCameraModes::CGetAllThingsOfType::operator()(const CThing*).
- 0x878B60 CProjectileWeapon_CalcPitchDirection -> NCameraModes::CProjectileWeapon::GetLookDirectionXYFromParams(float, float) const.
- Landed 0x878A80 "GetRotatedPosition" -> NCameraModes::CProjectileWeapon::GetCamPosOffsetXY(float) const (the only
  const C3DVector(float) member in the PDB name set).
- 0x87C660 / 0x87C6B0 -> CTurningAnimCompare::operator() (binary / unary overloads).
- Animation-sort helpers:
  - 0x87DA90 -> std::__median
  - 0x87DB50 -> std::__find_if
  - 0x87DC30 FindClosestRotation_binary_search -> std::__unguarded_linear_insert
  - 0x87DDD0 -> std::remove_copy_if
  - 0x87E250 -> std::__linear_insert
- 0x883AE0 CEnvironmentSpellEffectInterface_UpdateAllEffects -> NScreenEffect::CScreenEffectPriorityStack::CScreenFilterEntry::Reset.
- 0x8840D0 / 0x884120 / 0x8843F0 / 0x884490 / 0x884C70 -> _Rb_tree::_M_create_node / _M_clone_node / _M_insert /
  _M_copy / map::operator[] of the screen-filter bypass map. 0x42971D -> _Rb_global<bool>::_Rebalance.
- Environment-theme helpers:
  - 0x885DD0 / 0x889800 _Fill_n -> std::uninitialized_fill_n<CThemeWeight*> / <CBlendedEnvironmentTheme*>
  - 0x889300 _Move<CMusicEntry*> -> std::__uninitialized_copy<CEnvironmentThemePriorityStackEntry*>
  - 0x8897D0 -> std::fill<CBlendedEnvironmentTheme*>
  - 0x889CB0 Vector_Reserve -> vector<CBlendedEnvironmentTheme>::reserve
- Keyed-pair helpers (pair/comparator names descriptive):
  - 0x88C780 RBTree_InsertBefore -> std::__unguarded_linear_insert<SKeyedPair, SKeyGreater>
  - 0x88C9D0 _Copy_impl<CNavigationPosition*> -> std::__unguarded_insertion_sort_aux
- Vector-filter helpers:
  - 0x8AABC0 GFWorld3DDistanceSmallerThan -> std::unary_negate<CVectorFilter_FurtherFromPointThan>::operator()
  - 0x8AC060 Position_FilterNearbyThenFind -> std::remove_if<C3DVector*, unary_negate<...>>
  - 0x8C34A0 GetValidRushPosition_impl -> std::remove_if<C3DVector*, CIsOutsideRadius> (descriptive functor, as
    landed 0x8C2ED0)
- 0x8AC2A0 _Move<CScriptThing*> -> std::__copy<CScriptThing*>; 0x8ADF90 -> vector<CScriptThing>::push_back.
- 0x8AF220 CombatSequenceManager_InitializeHeapList -> CombatDialogueManager::CombatDialogueManager().
- 0x8B3430 PairContainer_Destructor -> CCombatSequenceBase::`scalar deleting destructor'.
- 0x8B8050 CCombatActionBase_Constructor_Action0x32 -> CCreatureAction_PlayCombatAnimation ctor.
- 0x8C7E50 CompareActorRotations: descriptive name (no PDB symbol).
- 0x8D45A0 HitEvent_ParticleMorphFilter_Predicate -> CFilter_Bind4<CIsHitEventHitByThingWithScriptName,
  CEventFilter_HitEventWithWeapon, CIsEventCreatedBeforeOrOn, CIsEventCreatedAfter>::operator()<CEventHitBy*>.
  Related labels:
  - landed 0x8D43C0 is likely CEventFilter_HitEventWithWeapon::operator()
  - 0x8D43A0 is HitEventWithNoWeapon
  - 0x810FD0 is CEventHitBase::GetPWeaponUsed
- 0x8E2DB0 / 0x8E3150 GFToScreen -> std::swap<C3DVector>; 0x8E3430 -> std::__partition<C3DVector*, CIsUsableIciclePoint>.
