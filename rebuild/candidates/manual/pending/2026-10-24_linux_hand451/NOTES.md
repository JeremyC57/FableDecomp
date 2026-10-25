# Hand chunk 451 (std::__partial_sort .. FadeOutThing): 37 of 40 byte-exact

All 40 rows are `_global` in the catalog. They come from two TUs: the scripting library's STLport 4.x algorithm
instantiations (0x00CB7140-0x00CBD0F0, built `/Gr` so the templates are `__fastcall`) with two CScriptBase-family
methods, and the size-optimised quest-script helper TU (0x00CBE0C7-0x00CBE814). Element types of the STL rows:
A = `pair<CCharString, CScriptInfo>` (0x18), B = `pair<unsigned __int64, CRuntimeScriptedEntityPersist::CPersistInfo>`
(0x18), C = `pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> >` (0x10),
D = `pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >` (0x0C, compared with CCharString::FastCompare).

## What matched (37)
- **std (STLport 4.x algorithms, 24 rows)** -- __partial_sort A/D (0x00CB7140, 0x00CBD0F0), __unguarded_linear_insert
  B/C (0x00CB92C0, 0x00CBA840), __uninitialized_copy / __uninitialized_fill_n B (0x00CB9B00, 0x00CB9B50), __median B/C
  (0x00CB9D30, 0x00CB9DA0), __unguarded_insertion_sort_aux B/C (0x00CB9E40, 0x00CBB270), __unguarded_partition B/D
  (0x00CBA580, 0x00CBB860), swap D (0x00CBA8E0), __make_heap B/C/D (0x00CBAAB0, 0x00CBBB70, 0x00CBBC00), __linear_insert
  B/C/D (0x00CBB920, 0x00CBC330, 0x00CBC3C0), vector<B>::push_back (0x00CBC7B0), __insertion_sort C/D (0x00CBCF20,
  0x00CBCF70). Shape lessons: write the textbook STLport bodies as `__fastcall` templates with an explicit
  instantiation, callees declared only; empty tag types (`random_access_iterator_tag`, `__false_type`) need a user
  constructor so no tag byte is stored; `copy_backward`/`make_heap` are thin inline wrappers (`__forceinline` where VC7.1
  would otherwise call them); the pair's implicit operator= is emitted out of line where retail calls 0x00CBA520 (C) /
  0x00CBA700 (D); one CCountedPointer model with the "D-form" release (decrement, re-read, test, then `Data = Info = 0`)
  matches every C and D row; u64 keys compare high dword first with no extra code.
- **CScriptBase::OnScriptedEntityInterrupted (0x00CB7BC0)** -- inline CVectorMap::Find over the out-of-line LowerBound
  0x00CB9840, then the inline `SetFuncAsInterrupted(true)` byte store at +0x29.
- **NScript::CRuntimeScriptedEntityPersist::SetEntityPersistScript (0x00CB82F0)** -- Find, then assign or Add
  (push_back of a pair temporary, Dirty = true, sort 0x00CBD370, Dirty = false). Add/Sort must be `__forceinline`, and a
  same-TU `__declspec(noinline)` LowerBound body (it only reads) lets VC7.1 keep the key and `this` in registers.
- **Quest-script helpers (12 rows)** -- FoundMessageIsThingHitBy / KnockedOutBy / TriggeredBy, IsDistanceBetweenThings
  Under / Over, Is2DDistanceBetweenThingsUnder, IsDistanceFromThingToPosition Over / Under, GetDistanceBetweenThings,
  GetDistanceBetweenThingAndPosition, FadeOutAndKillEntity, FadeInThing, FadeOutThing. Shape lessons:
  `#pragma optimize("s", on)` alone; `return (a && b) ? true : false;` for retail's `mov al,1 ; jmp ; xor al,al`;
  C3DVector/C2DVector helpers `__forceinline`; squared length `X*X + Y*Y + Z*Z`, length `(float)sqrt(X*X + Y*Y + Z*Z)`;
  the fade loops are a do-while with `break` when the thing dies (FadeOutAndKill), a do-while (FadeIn) and a top-tested
  while (FadeOut), with the interface slots NewScriptFrame (+0x1C), RemoveThing (+0x1B0), EntitySetAlpha (+0x7F8).

## Parked for Fable 5.1 (3)
- 00cb8170 CScriptBase::RunEntityThreads DIFFER(173v173): `lea ecx,[esp+16h]` v retail `lea ecx,[esp+17h]` -- the random_access_iterator_tag temporary passed to __copy 0x00CBB120 inside the inlined vector::erase gets its own byte instead of sharing the __false_type tags' slot | tried: vector snapshot (175v173), same-TU vector copy-constructor body (173v173), STLport copy() dispatch chain copy -> __copy_aux -> __copy_ptrs -> __copy, whole-CVectorMap snapshot (fixed the 0x18 frame) | next: give the __copy call its own inline scope (STLport `__copy_ptrs` called from a separate inline erase helper) or build the tag as a named local; else lane gap: temp-slot allocation
- 00cbe5f0 GetPositionInFrontOfThing DIFFER(107v114): retail `fldz ; fcos ; fstp [ebp-4] ; fmul qword [2pi] ; fld [ebp-4] ; fmul [len] ; fld st(1) ; fsin ; fld st(1) ; fmulp` ... `fstp st(0)` x2 (frame 0x10) v ours `fmul qword [2pi] ; fstp qword [ebp-8] ; fldz ; fcos ; fmul [len] ; fld qword [ebp-8] ; fsin ; fmul st(1)` (frame 0x14) | tried: by-value GetVectorFromAngles (120v114), in-place helper with float params (106v114), radian params with the 2pi multiply in the caller (107v114), nested bearing helper taking cos(z) as an argument (107v114) | next: the same inline polar helper is at 0x006CBF9D / 0x00723E7E / 0x0074E1E3 (length 1, cos(z) homed in memory) -- find its source form: cos(z) as a float local used in TWO products (homes it, as 0x00CBE693's last try showed) plus `sin(xy) * cosz * length` in that operand order (VC7.1 reassociates FP products, so the order picks which pair is shared), or a unit direction scaled by a separate `*= length`
- 00cbe693 GetRandomPositionInsideRadius DIFFER(123v131): ours hoists `fldz ; fcos ; fstp [ebp-4]` above the second GFFloatRandom call and computes `sin(xy) * cosz * length` as `fmul [cosz] ; fmul [len]` per component; retail computes cos(z) after the call and keeps one `cosz * length` product on the x87 stack (`fld st(1) ; fmulp`, late `fstp st(0)`) | tried: xy double first (124v131), float xy with flat*sin (124v131), cos(z) first + double xy (124v131), cos(z) used twice in `cosZ * length * sin(xy)` (123v131) | next: same helper as 0x00CBE5F0 (solve both together); keep cos(z) multi-use but write `sin(xy) * cosz * length`, and stop the hoist by making the angle a named local assigned from the call before the helper

## Skipped
- None: no row holds a second function.

## Label corrections
- 0x00CB7140 "ScriptInfo_Heapsort" -> `std::__partial_sort<pair<CCharString, CScriptInfo>*>`
- 0x00CB7BC0 "NavQuadTree_MarkNodeActive" -> `CScriptBase::OnScriptedEntityInterrupted(unsigned __int64)`
- 0x00CB8170 "ScriptEntity_UpdateSpawnedFunctions" -> `CScriptBase::RunEntityThreads()`
- 0x00CB82F0 "ScriptEntity_InsertOrUpdatePersist" -> `NScript::CRuntimeScriptedEntityPersist::SetEntityPersistScript`
- 0x00CB92C0 "NavigationPosition_InsertSorted" -> `std::__unguarded_linear_insert` (B)
- 0x00CB9B00 "_Move<CMusicEntry*,CMusicEntry*>" -> `std::__uninitialized_copy` (B)
- 0x00CB9B50 "fill<CTextureManagerPoolInfo*,...>" -> `std::__uninitialized_fill_n` (B)
- 0x00CB9D30 "MinOf3" / 0x00CB9DA0 "MaxOf3" -> `std::__median` (B / C; byte-identical)
- 0x00CB9E40 / 0x00CBB270 "_Copy_impl<CNavigationPosition*,...>" -> `std::__unguarded_insertion_sort_aux` (B / C)
- 0x00CBA580 "PSCInfo_Partition" / 0x00CBB860 "Quicksort_Partition" -> `std::__unguarded_partition` (B / D)
- 0x00CBA840 "SpeechFrame_InsertSorted" -> `std::__unguarded_linear_insert` (C)
- 0x00CBA8E0 "swap<C3DMeshVolume>" -> `std::swap` (D)
- 0x00CBAAB0 "ScriptEntity_BuildHeapify" / 0x00CBBB70 "CActiveEntityScript_BuildHeapify" / 0x00CBBC00
  "EntityScriptBinding_BuildHeapify" -> `std::__make_heap` (B / C / D)
- 0x00CBB920 "NavigationPosition_InsertInOrder" / 0x00CBC330 "Item_InsertInPriorityOrder" / 0x00CBC3C0
  "SoundPair_InsertInPriorityOrder" -> `std::__linear_insert` (B / C / D)
- 0x00CBC7B0 "CMusicEntry_InsertIfCapacity" -> `std::vector<pair<unsigned __int64, CPersistInfo> >::push_back`
- 0x00CBCF20 "PriorityQueue_BuildHeap" / 0x00CBCF70 "SoundPair_BuildHeap" -> `std::__insertion_sort` (C / D)
- 0x00CBD0F0 "CGameCameraManager_InitModes" -> `std::__partial_sort` (D)
- 0x00CBE512 "GetSquaredDistanceBetweenThings" -> `GetDistanceBetweenThings` (fsqrt); the squared ones are 0x00CBE583
  (manifest "CScriptBase::Activate", wrong) and 0x00CBE5C0 (thing to position)
- 0x00CBE551 "DistanceCalculator_GetVectorDistance" -> `GetDistanceBetweenThingAndPosition`
- 0x00CBE5F0 "Particle_CalcTargetPosition" -> `GetPositionInFrontOfThing`; 0x00CBE693 "Particle_CalcRandomOffset" ->
  `GetRandomPositionInsideRadius`
- Already landed, wrong identity: 0x00CB8220 (`Destval_CRegisteredMusicEntry_00cb8220.cpp`, modelled as `Close(S*)`) is
  `CScriptBase::RunSubThreads()` (RunSpawnedFunctions 0x00CB7C40, then tail-`jmp` RunEntityThreads 0x00CB8170);
  0x00CB8D30 (`CCountedPointerCEntityScriptBindingBase_DeleteData_00cb8d30.cpp`, modelled as `operator_new` calling
  `helper_bfeb1c`) calls 0x00BFE9BC operator delete, not the 0x00BFEB1C import; 0x00CBD5B0
  (`CVectorMap_T_00cbd5b0_Sort_00cbd5b0.cpp`, callee "GuiVarBool_Apply") is `CVectorMap<unsigned __int64,
  CPersistInfo>::Sort` calling `std::sort` 0x00CBD370 (the same sort SetEntityPersistScript's Add uses).
