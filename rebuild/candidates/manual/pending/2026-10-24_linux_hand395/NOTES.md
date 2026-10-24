# Hand chunk 395 (_STL::__unguarded_linear_insert .. _STL::__partial_sort): 31 of 40 byte-exact

All 31 sources grade RELOCATION_MATCH (`grades.txt`); all 31 tests PASS under g++ (`tall.sh`) and VC7.1 (`tall.py`).
No row needed the int3-after-noreturn note.

## What matched (31)
- **STLport sort<pair<CCharString, C3DVector>*, CKeyPairCompareLess> (6)** -- 0077e6a0 `__unguarded_linear_insert`,
  0077e8f0 `__unguarded_partition`, 0077e960 `__linear_insert`, 0077eda0 `__make_heap`, 0077f2c0 `__partial_sort`,
  0077f370 `__introsort_loop`. Shape: /Gr `__fastcall` free functions. `CBasicString<char>::operator<` gets its real body
  compiled ABOVE the callers under `#pragma auto_inline(off)`, as the landed 0x005E0710 does. `CCharString::operator<` is
  inline (same payload / null this / null other / payload compare). C3DVector needs a memberwise user copy ctor (moved
  as dwords). The pair's implicit assignment is CCharString::operator= plus a struct copy.
- **_global::GetDoorTypeSymbolMap (1)** -- 0077f510. A static-once `LoadFromFile(GetDefsDirectory() + L"thing_def.h")`.
  Arguments are evaluated right to left (the literal is pushed before the directory temporary); this is the landed
  0x004CDB10 shape.
- **CNamedValueTreeOwner (1)** -- 007817f0 FindAttributeByName. A linear `for (it = begin(); it != end(); ++it)` scan
  over map<CDefString, long> at +0x40 with `_M_increment` inlined; end() is re-read every step.
- **CTCBoastingCrowdControl (4)**:
  - 00783240 OnKillCrowd: `find` is modelled as returning the node pointer. A one-pointer iterator class comes back
    through a hidden pointer instead. STLport `erase(iterator)` is inlined (`_Rebalance_for_erase` 0x0042A34E, then
    `if (y) free(y)`).
  - 007832b0 Shutdown: end() is held in a local, then `clear()` is inlined.
  - 00783680 OnNoBoastsRemain: an unchecked GetInterface reproduces retail's stale-register read.
  - 007837c0 ctor: the STLport map ctor is inlined and the body calls `clear()`.
- **CTCSkeletalMorph / CArray<CSkeletalMorph> (3)**:
  - 00786670 AddSkeletalMorph: `push_back(CSkeletalMorph(...))` with push_back inlined.
  - 00786de0 `__uninitialized_fill_n` (`test ; jbe` + `dec ; jne`, keeping VC's placement-new null test).
  - 00787500 push_back. The empty `__false_type` tag is homed in a dead argument slot.
- **CTCTrapBase (1)** -- 00787c35 SetTransitionParams. A CRGBColour by-value argument is built in its own slot only when
  CRGBColour has a user copy ctor; the TU is `optimize("s", on)`.
- **_global::GetUpdateTimeForThing (1)** -- 00787c62. It needs three separate returns. `&&` merges two exits and loses
  the `fst` spill. The trap TU uses `optimize("y", off)` + `optimize("s", on)`.
- **CTCHeroRangedAttacks (1)** -- 00789a10 SetProjectileTargetingZoomDelta. `+=` followed by an `if / else if` clamp;
  the sum is compared and stored with one `fst`.
- **anonymous-namespace hero-combat helper (1)** -- 00789d90 GetAccuracyPercentageFromThingAtBuildUpTime.
  - The CDefPointer is passed by value and released with no null test.
  - `std::min(accuracy, 100.0f)` over a named float.
  - A file-scope function pointer keeps the file-local function emitted.
- **CTCDivineWrath / CTCUnholyPower (6)**:
  - CancelMove (0078f820 / 00794d90): the copy of each creature pointer inside the loop needs `__declspec(align(8))` for
    retail's `and esp,-8` frame.
  - map<CThingCreatureBase*, PerCreatureInfo>::operator[] (007934f0 / 00798330): the STLport `lower_bound` + `insert`
    form; the insert result is homed in the dead key slot.
  - vector<PerEffectInfo>::push_back (007935d0 / 00798410): the PerEffectInfo and CIntelligentPointer copy ctors must
    be `__forceinline`.
- **vector<CIntelligentPointer<CThingCreatureBase>> (1)** -- 00799cd0, the size ctor. The end-of-storage pointer must
  live in an `_STLP_alloc_proxy` member that also does `allocate` (`n != 0 ? malloc : 0`).
- **CTCPhysicalShieldSpell (1)** -- 0079e580 AllowReactionsOnHitFromAttacker. The set's insert and erase are inlined.
  The pair and the iterator need STLport constructors, or VC7.1 returns them in registers instead of through the
  hidden pointer.
- **CTCWillLightning (2)**:
  - 007a03b0 CancelMove: `if (!GetInterface(0xA7) || abilities->CurrentAbility == 0xB) Deactivate();` (VC tests bit 7
    of the bitset word as the sign of the low byte).
  - 007a0490 StopLoopingSound: the handle sits in a named local. GetInterface must use the ONE-ternary store spelling,
    which loads the component straight into ecx (the if/assign form goes through eax).
- **STLport sort<CThing**, CThingCompare_NearestToPos> (1)** -- 007a3b60 sort_heap with pop_heap inlined. C3DVector needs
  a user copy ctor; with a trivial copy, VC hoists the comparator loads out of the loop.
- **CHitParameters (1)** -- 007a3bc0, the default ctor.
  - Shape: a member-initialiser list in which the true flags share `al = 1`.
  - The STLport map ctor and vector ctor are both inlined.
  - Position must be at +2, so C3DVector is `pack(1)` (its real, unaligned PDB layout).

## Parked for Fable 5.1 (9)
- 007881ad CTCTrapBase::UpdateTriggerThings DIFFER(124v124): one scheduling slot; retail stores the X `__ftol2` result after issuing `fld [ebx+4]` for Y, ours straight after the call | tried: value-initialised filter temporaries (stosb), C2DCoordI(long,long), out-of-line C3DVector conversion, def/filter declaration orders, two long locals | next: lane gap: x87/ftol store scheduling
- 00789ce0 _global::GetDamageMultiplierFromThingAtBuildUpTime DIFFER(175v175): the inlined ~CDefPointer refcount `mov/dec/mov` lands 1-3 x87 instructions earlier than retail | tried: `s*s*scale`, `scale*s*s`, `s*(s*scale)`, `scale*(s*s)` (fixes the x87 order) | next: a release helper with a different statement shape (e.g. a member Release() called from the dtor) or lane gap: integer/x87 interleave
- 0078c000 CTCBerserk::GetNumFramesBeforeCrazyIdle DIFFER(125v116): the def release is expanded to `lea/mov/dec/mov/test/mov` when the def shares the class-name temporary's frame word (nested scope); at function scope it gets its own slot (127v116) | tried: `--Object->RefCount` dtor, inline const/non-const Release(), a local copy of Object | next: model GetDef 0x0078DA50 with its real same-TU body (auto_inline off) so the def slot does not escape
- 00792550 _STL::__uninitialized_fill_n<CTCDivineWrath::PerEffectInfo> DIFFER(86v86): ebx/edi swap (retail: cursor ebx, source edi) | tried: implicit C3DVector copy (90v86), memberwise C3DVector copy ctor, no `cur` local, explicit PerEffectInfo copy ctor | next: lane gap: register allocation
- 007979b0 _STL::__uninitialized_fill_n<CTCUnholyPower::PerEffectInfo> DIFFER(86v86): the same ebx/edi swap as 00792550 | tried: the same forms | next: lane gap: register allocation
- 0079d2d0 CTCBulletTime::FinalCleanup DIFFER(144v144): the closing 0x46 lookup reuses the dead `this` register (`mov edi,[edi+4]`) where retail re-reads the caster into eax | tried: out-parameter GetInterface, HasTC + one-ternary GetTC (149v144), component locals declared first | next: an inline CTCBase::GetThing() accessor or a callee form that keeps `this` live (same gap as the landing-queue CTCBulletTime::Pause 0x0079CF30)
- 0079eaa0 CTCPhysicalShieldSpell::FinalCleanup DIFFER(138v138): the same gap as 0079d2d0; the second (0x10 sound-player) lookup loads ParentThing into edi instead of eax | tried: `__forceinline` StopSound member (the out-of-line copy is 0x0079DDB0), named sound/caster locals | next: the 0079d2d0 idea; the one-ternary GetInterface changes the component load, so it does not apply here
- 007a34a0 _STL::__adjust_heap<CThing**, CThingCompare_NearestToPos> DIFFER(135v135): scheduling only; retail loads the comparator's X and Y (ecx, edx) before `sub esp,0Ch` and Z after, ours hoists only X | tried: trivial C3DVector (ecx pointer and push order also off), C3DVector copy ctor, comparator copy ctor, body-assign copy ctor | next: lane gap: struct-copy scheduling (land with the sibling sort_heap 007a3b60 form)
- 007a3fa0 _STL::__partial_sort<CThing**, CThingCompare_NearestToPos> DIFFER(172v172): with a trivial comparator copy, first/i land in edi/esi (retail esi/edi) and the pop step's loads rotate | tried: C3DVector copy ctor (Y/Z copied through fld/fstp, 174v172), dword C3DVector members (184v172), pack(1) copy ctor (174v172), trivial copy (172v172) | next: a C3DVector copy ctor that copies as dwords without changing the type (e.g. through `*(unsigned long*)&`), or an `operator=`-based copy ctor

## Skipped
None. No row held two functions.

## Label corrections
- 0x0077E6A0 `SortedMap_InsertOrdered` -> STLport `__unguarded_linear_insert<pair<CCharString,C3DVector>*, ...>`
- 0x0077E8F0 `Partition_ThreeWay_Quicksort` -> `__unguarded_partition` (same family)
- 0x0077E960 `SortedMap_InsertOrUpdate` -> `__linear_insert`
- 0x0077EDA0 `MakeHeap_CKeyPair` -> `__make_heap`
- 0x0077F2C0 `_Find<CCharString*,CCharString>` (propagated) -> `__partial_sort`
- 0x0077F370 `Sort_QuicksortPhase_CCharString` -> `__introsort_loop`
- Callees of the same family: 0x0077E0C0 -> `iter_swap`; 0x0077E820 -> `__median<pair<CCharString,C3DVector>>`
- 0x009E5C90 -> CSymbolMap::LoadFromFile
- 0x007817F0 `CTCAvoidRegion_FindAttributeByName`: no CTCAvoidRegion class in the PDBs; the owner is still unidentified
  (descriptive CNamedValueTreeOwner)
- 0x00783240 `CTCCreatureGroupBoastingCrowd_RemoveBoast` -> CTCBoastingCrowdControl::OnKillCrowd
- 0x0042A34E (catalogued `CMemoryAllocatorVariableSize::Erase`) -> STLport `_Rb_global<bool>::_Rebalance_for_erase`
- 0x007832B0 `LTextTree_KillAllThings` -> CTCBoastingCrowdControl::Shutdown
- 0x00782FE0 (catalogued `LTextTreeWalkThrough::BuildTreeArray`) -> `_Rb_tree::_M_erase`
- 0x00783680 `CTCCreatureGroupBoastingCrowd_DetachAll` -> CTCBoastingCrowdControl::OnNoBoastsRemain
- 0x00784B20 -> CTCCreatureGroupBoastingCrowd::OnNoBoastsRemain
- 0x007837C0 `CEngineSubPrimitive2DClockSprite_Constructor` -> CTCBoastingCrowdControl::CTCBoastingCrowdControl(CThing&)
- 0x00786670 `Vector_PushBack_TextureMorph` -> CTCSkeletalMorph::AddSkeletalMorph
- 0x00786DE0 `_Fill_n<unsigned_long*,...>` -> `__uninitialized_fill_n<CSkeletalMorph>`
- 0x00787500 `Vector_Insert_CCharString` -> CArray<CSkeletalMorph>::push_back
- 0x00787380 -> CArray<CSkeletalMorph>::_M_insert_overflow
- 0x00787C35 `ConstructString` -> CTCTrapBase::SetTransitionParams
- 0x007881AD `CTCCombatBase_UpdateEngageable` -> CTCTrapBase::UpdateTriggerThings
- 0x00789A10 `ZoomControl_Update` -> CTCHeroRangedAttacks::SetProjectileTargetingZoomDelta
- 0x0078F820 / 0x00794D90 `CTCDivineWrath_CancelAbilities` / `CTCUnholyPower_CancelAbilities` -> CTCDivineWrath::CancelMove /
  CTCUnholyPower::CancelMove
- 0x00792550 / 0x007979B0 `Vector_CopyConstruct_CBaseIntelligentPointer` / `CopyArray_CBaseIntelligentPointer` ->
  `__uninitialized_fill_n<PerEffectInfo>` (DivineWrath / UnholyPower)
- 0x007934F0 / 0x00798330 `BinarySearchTree_InsertNewNode` / `IcicleAssault_FindInsertionPoint` ->
  map<CThingCreatureBase*, PerCreatureInfo>::operator[] (DivineWrath / UnholyPower)
- 0x0078E4A0 (was SIcicleRecord) -> CTCDivineWrath::PerCreatureInfo copy ctor; 0x00793A80 is the UnholyPower twin
- 0x007935D0 / 0x00798410 `StdVector_PushBack` -> vector<PerEffectInfo>::push_back (DivineWrath / UnholyPower)
- 0x00799CD0 `Vector_Initialize_WithIntelligentPtr` -> vector<CIntelligentPointer<CThingCreatureBase>>::vector(size_type)
- 0x0079D2D0 `ETCInterface_OnUpdate` -> CTCBulletTime::FinalCleanup
- 0x0079E580 `CCreatureAction_SetDirection` -> CTCPhysicalShieldSpell::AllowReactionsOnHitFromAttacker
- 0x0079EAA0 `CThing_OnDestroy` -> CTCPhysicalShieldSpell::FinalCleanup
- 0x0058119B -> CTCHaste map<void*, float>::erase; 0x00800020 -> CTCHaste::RecalculateMultiplier
- 0x007A03B0 `LightningAbility_Deactivate_IfActive` -> CTCWillLightning::CancelMove
- 0x007A0490 `CThing_OnLevelUnload` -> CTCWillLightning::StopLoopingSound
- 0x007A34A0 / 0x007A3B60 / 0x007A3FA0 (`_Adjust_heap<...>` / `_Sort_heap<long*>` / `PartialSort_Top_CThingAICreature`) ->
  STLport `__adjust_heap` / `sort_heap` / `__partial_sort` of sort<CThing**, CThingCompare_NearestToPos>. The same
  family includes 0x007A30B0 `__push_heap`, 0x007A37B0 `__make_heap`, 0x007A3750 `__pop_heap`, 0x007A3040
  `__unguarded_linear_insert`, 0x007A40D0 `partial_sort` and 0x007A4140 `__introsort_loop`.
- 0x007A3BC0 `CWound_Constructor` -> CHitParameters::CHitParameters() (Ego_r CWound is a 68-byte decal record)
- Already-landed labels to re-check:
  - 0x007A2F90 landed as `CThingCompare_Nearest::operator()<CThingAICreature>`. Ego_r.pdb puts
    `CThingCompare_NearestToPos::operator()<CThing>` and the whole NearestToPos sort family straight after
    CTCWillLightning::FrameUpdate, which is exactly where this code sits in retail.
  - 0x007A0400 landed as `CTCWillLightning::ResumeForCutscene`, but CTCWillLightning has no such PDB member. The body
    is the mirror of StopLoopingSound, so it is `StartLoopingSound`.
