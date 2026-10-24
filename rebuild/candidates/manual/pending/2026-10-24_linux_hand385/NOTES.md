# Hand chunk 385 (_global: CCameraModeMap::_M_insert .. vector<pair<long,float> >::insert): 36 of 40 byte-exact

All 36 sources in src/ grade RELOCATION_MATCH (gall.sh -> grades.txt). Their 36 tests print `<addr>_TEST PASS`
under g++ (tall.sh) and under VC7.1 (tall.py).

## What matched (36)

- **CTCCarrying (8): 006a11f0 GetThingInCarrySlot, 006a1230 IsCarrySlotFree, 006a1540 IsThingCarryingWeapon,
  006a15a0 IsThingCarryingMeleeWeapon, 006a1620 IsThingCarryingRangedWeapon, 006a22d0 RemoveAllThingsInPrimarySlot,
  plus the two GetAnimationToUse statics below.**
  - The slot lookups inline Find over the CarrySlots sorted vector (+0xc, LowerBound 0x006A2A10).
  - IsCarrySlotFree is `if (it == End()) return false; return GetPItem() == 0;`.
  - The weapon queries are the PDB's `PeekTC<T>(const T**)`. HasTC on T::StatGetInterfaceType(), then a `Find(long)`
    whose LowerBound key is the conversion temporary `LowerBound((ETCInterfaceType)type)`.
  - With that temporary both lookups share one `push ecx` frame word and the compare keeps its immediate. Named key
    locals take two words (`sub esp,8`).
  - RemoveAllThingsInPrimarySlot inlines IsSlotPrimarySlot (0x006A1330), then RemoveThingInCarrySlot and the
    vector<CThing*> push_back. It needs a same-TU `_M_insert_overflow` stand-in that READS the value, not `&value`.
    That lets the pushed value share the dead `removed` argument slot with the def pointer.
- **CCreatureAction_(Un)SheatheItem (2): 0069f3e0, 0069f980 GetAnimationToUse.**
  - Static __fastcall functions returning a CCharString.
  - They use the PeekTC body, then `weapon->GetSheatheAnim()` / `GetUnsheatheAnim()` (0x005DA5A0 / 0x005DA540).
  - The fallback literal is "STANDARD_(UN)SHEATHE".
- **CThingFilter_* (6): 006a7050 BoundingRadiusIntersectsInfiniteLine, 006a7c60 IsInQuestOrNullQuest, 006a80a0 HasTC,
  006a84b0 HasBeenDiscovered, 006a8710 IsNavLineOfSightToThing ctor, 006a8af0 IsNotBeingExclusivelyUsed.**
  - HasTC is STLport `bitset<274>::test` inlined, with the no-exceptions `puts("bitset"); abort();`.
  - IsInQuestOrNullQuest is three inline CCharString equalities or-ed into an int-wide bool. It uses the landed
    0x0083BF90 `operator==(const char*)` model with the intrinsic `strcmp(text, "")` null arm.
  - BoundingRadius... needs the squared distance in a NAMED float local, which gives retail's
    `fxch ; fcompp ; test ah,5`.
  - `return PeekTC(&e) && !e->User.GetPItem();` gives the int-wide `mov eax,1` / `xor eax,eax`.
  - The ctor initialises `Pos(&thing.GetPosition())` and calls PeekTC into the member.
- **STLport sort / heap helpers over pair<long, CIntelligentPointer<CThing> > (10, /Gr __fastcall):**
  - The functions: 006a2d40 iter_swap, 006a2dc0 __unguarded_linear_insert, 006a3440 __unguarded_partition,
    006a34a0 __pop_heap, 006a3520 __make_heap, 006a3830 __linear_insert, 006a3b90 __insertion_sort,
    006a3bf0 __partial_sort, 006a3220 / 006a3180 __uninitialized_fill_n.
  - The pair has a declared memberwise copy ctor. CIntelligentPointer has a `__forceinline` copy ctor.
  - Empty tags (random_access_iterator_tag, __false_type) land in dead argument slots.
  - iter_swap needs the leaf `?iter_swap@std@@` and the swap body written out, because a separate __iter_swap
    template stays out of line.
- **STLport containers (8):**
  - 006a3cd0: vector::push_back.
  - 006b5ed0: vector<pair<long,float> >::insert. It needs the tags with a user ctor (no zero fill) and a
    `__forceinline __copy_backward_ptrs`.
  - 006b5a50: lower_bound. It needs a `less<>` WITHOUT the binary_function base, so that `less<_Tp>()` is POD and
    zero-filled (`mov byte ptr [esp+4],0`).
  - 006a4a70 / 006a4ab0: list::push_back. The iterator needs STLport's declared copy ctor, or the edi push is
    shrink-wrapped.
  - 0069d520 / 006ae900: _Rb_tree::_M_insert, the landed 0x006F1280 shape. The nested pair of 006ae900 must copy
    implicitly.
  - 006a6290: CThingPhysical::ModifyHealthOverTime. It is a push_back of a {frames, amount} helper onto a list at
    +0x98.
- **CEnvironment (3):**
  - 006b3880 TransitionToDefaultTheme(long, long, float).
  - 006b3b30 ConsoleRestoreDefaultTheme, which inlines 0x006B3880. The forced test folds away between the
    clear/set stores.
  - 006b3460 SetDiffVector: C3DVector from three CGenericVar floats, Normalise 0x00A14440, copied into both
    game-tick states.
  - Retail strides: the CArray is 12 bytes, the theme entry 0x30, and the game-tick state 0xc20 bytes with
    DiffuseVector at +0xbf4.
- **GF math (1):** 006adc20 GFSmoothTowards<float>, a __fastcall template. Plain if / else-if clamp, then a
  sign-selected overshoot test. It matched on the first try.

## Parked for Fable 5.1 (4)

- 006a1270 CTCCarrying::IsPrimarySlotFree DIFFER(190v190): retail has the loop index in ebp and the 12-byte offset in edi (`xor ebp,ebp` before `push edi`; `mov eax,[esi+0Ch] ; mov edx,[edi+eax]`); ours has edi index / ebp offset, and the in-loop release reads the vtable into edx, not eax | tried: unsigned / long / int index, `*(first + n)` and `first[n]` subscript (GetDef stand-in body fixed the rest) | next: `(CarrySlots.begin() + i)->first` as the landed sibling GetAllCarriedThings 0x006A2220 does
- 006afa40 CTCPhysicsBase::ImpulseRespondToCollisonWithCylinder DIFFER(170v170): length loads `fld [ecx+8] ; fld [ecx+4] ; fld [ecx]` (retail) vs `fld [ecx] ; fld [ecx+4] ; fld [ecx+8]` (ours) | tried: locals x,y,z (this), locals z,y,x (fixes the loads but flips the dot to Y-then-Z), grouped Dot `X + (Y + Z)`, swapped Dot operands | next: locals z,y,x with the dot written as an explicit z-first sum on the members, or a C3DVector copy for the length
- 006aff40 CTCPhysicsBase::GetTurnSpeedAngleXY DIFFER(225v212): retail keeps the 2nd bearing on the x87 stack (`fcom 0 ; fst [esp+4]`, `fcom 1.0`, `fstp st0 ; fld [esp+4]` before fmod, no store after) and every path joins one `fsubr [esp]`; ours spills/reloads it like the first and has a 2nd `fld [esp] ; fsub [esp+4]` tail | tried: inline WrapTurns(float) over atan2(float,float), the 2nd wrap as the return operand, a multi-return helper, the landed 0x008F4D60 __forceinline GetBearingInTurns | next: the 2nd wrap written out in place on a temporary (not through a parameter copy), else lane gap: x87 stack-allocation of an inlined helper's result
- 006b1600 CAtmosProcess::ApplyReverbSettings DIFFER(163v165): `mov edx,[edi+18h] ; imul edx,edx,54h ; mov eax,[esi+6Ch] ; add eax,edx` (retail) vs `mov eax,[edi+18h] ; imul eax,eax,54h ; add eax,[esi+6Ch]` (ours), and the eax/edx choices after it | tried: `*(_M_start + n)`, `*(begin() + n)`, a CReverbParameters(const entry&) ctor doing the copy, a named `entries = _M_start` base | next: an STLport iterator object (`begin() + n` as a class with a user copy ctor), else lane gap: memory-operand fold in the index add

## Skipped

None. No row holds two functions, and no row is cut wrong. 006a80a0's `call abort` is not its last instruction,
so the int3-after-noreturn rule does not apply.

## Label corrections

Catalog/manifest label -> identity (Ego_r.pdb name sets, retail order and callers):
- 006a11f0 CThing_Array_FindByID -> CTCCarrying::GetThingInCarrySlot(long) const
- 006a1230 InventoryFindItemByIDLowerBound -> CTCCarrying::IsCarrySlotFree(long) const
- 006a1270 CThing_Array_GetFirstPhysicalSwitch -> CTCCarrying::IsPrimarySlotFree() const
- 006a1620 IsThingCarryingMeleeWeapon -> CTCCarrying::IsThingCarryingRangedWeapon (calls IsProjectileWeapon 0x005DAA40)
- 006a22d0 CTCCarrying_RemoveAllWeapons -> CTCCarrying::RemoveAllThingsInPrimarySlot(vector<CThing*>&, bool)
- 0x005DA540 (callee, manifest GetSheatheAnim) -> CTCWeapon::GetUnsheatheAnim
- 006a2d40 _Swap_adl -> std::iter_swap; 006a2dc0 Vector_MoveBackElements -> std::__unguarded_linear_insert
- 006a3180 _Fill_n<...> / 006a3220 Vector_CopyConstructor -> std::__uninitialized_fill_n
- 006a3440 Heap_SiftDownRecursive -> __unguarded_partition; 006a34a0 Heap_PopAndSiftDown -> __pop_heap;
  006a3520 Heap_Heapify -> __make_heap; 006a3830 Heap_InsertAndAdjust -> __linear_insert;
  006a3b90 Heap_BuildHeapFromRange -> __insertion_sort; 006a3bf0 Heap_BuildHeap -> __partial_sort;
  006a3cd0 Vector_PushBackElement -> vector::push_back
- 0x006A3A70 Vector_ReserveAndFill -> vector::_M_insert_overflow
- 0069d520 TreeSet_InsertIntInt -> _Rb_tree<NCameraModes::EMode, CCountedPointer<CBaseInit>>::_M_insert
- 006ae900 TreeSet_InsertLong5 -> _Rb_tree<EGameAction, pair<C3DVector,long>>::_M_insert
- 0x0042971D CMemoryAllocatorVariableSize::BalanceTree -> _Rb_global::_Rebalance
- 006a4a70 / 006a4ab0 CTCRolling_InsertEvent(Alt) -> std::list<...>::push_back
- 006a6290 CTCInventoryProcessor_EnqueueItemEffect -> CThingPhysical::ModifyHealthOverTime(float, long)
- 006a7050 CThingFilter_CheckBoundingIntersection -> CThingFilter_BoundingRadiusIntersectsInfiniteLine::operator()
- 006a7c60 CThingFilter_MatchesScriptName -> CThingFilter_IsInQuestOrNullQuest::operator()
- 006a80a0 Bitset_Test -> CThingFilter_HasTC::operator()
- 006a84b0 ETCInterfaceType_LookupCapability -> CThingFilter_HasBeenDiscovered::operator()
- 006a8710 CVertexBufferWin32_GetSizeAndAux -> CThingFilter_IsNavLineOfSightToThing ctor
- 0x004C73D0 "DoSizeof" -> CThing::GetPosition
- 006a8af0 CTCInterface_FindInterface -> CThingFilter_IsNotBeingExclusivelyUsed::operator()
- 006afa40 Movement_CheckVelocityAgainstPlane -> CTCPhysicsBase::ImpulseRespondToCollisonWithCylinder
- 006aff40 ComputeAngleDelta -> CTCPhysicsBase::GetTurnSpeedAngleXY
- 006b1600 Sound_LoadMainThemeSetup -> CAtmosProcess::ApplyReverbSettings. The landed ctor 0x006B1960 calls it
  "LoadMainTheme_006b1600".
- 006b3460 SetDiffVector -> CEnvironment::SetDiffVector (console static)
- 006b3880 CEnvironmentThemePriorityStackEntry_TransitionToDefault -> CEnvironment::TransitionToDefaultTheme(long,long,float)
- 006b3b30 EnvironmentThemePriority_ClearFlag -> CEnvironment::ConsoleRestoreDefaultTheme
- 006b5a50 operator_delete -> std::lower_bound<pair<long,float>*, pair<long,float>>
- 006b5ed0 Vector_InsertElement -> vector<pair<long,float> >::insert
- 0x00A442E0 Vector3_ClosestPointOnLineSegment -> the squared point-to-INFINITE-line distance (returns st0, float&
  line parameter out)
- 0x00A14440 GetScaled -> C3DVector::Normalise (already corrected by chunks 217-273, still in engine_api.tsv)
