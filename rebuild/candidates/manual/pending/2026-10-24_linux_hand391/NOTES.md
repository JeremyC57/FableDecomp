# Hand chunk 391 (pair<CSkeletalMorphs::CEntry,float> STL .. CTCCombat ability tree): 34 of 40 byte-exact

All 34 graded MATCH or RELOCATION_MATCH on the exact file in src/. Every test passes under g++ (tall.sh) and under
the VC7.1 lane (tall.py). This chunk had no int3-after-noreturn rows and no bare-fistp rows.

## What matched (34)

**CTCHeroMorph skeletal-morph priority sort: STLport algorithms over pair<CSkeletalMorphs::CEntry,float>, 12 bytes (6)**
- 0071ec60 `__unguarded_linear_insert`, 0071f570 `__unguarded_insertion_sort_aux`, 0071f530 `swap`,
  0071fc20 `__unguarded_partition`, 0071fe70 `__make_heap`, 00721400 `__partial_sort`.
  - Element model: the landed _Adjust_heap 0x0071F650 (memberwise copy ctor, trivial assignment).
  - All of these are /Gr link-time `__fastcall`: first in ecx and last in edx; the by-value element and the empty
    comparator go on the stack.
  - Key word: the value's second dword is loaded once and used for both the `cmp dx,[next+4]` compare and the final
    drop.
  - iter_swap is inlined as `tmp = *a; *a = *b; *b = tmp;`. VC7.1 parks the last two words of tmp in the pivot's
    dead argument slots.
  - __partial_sort inlines `__pop_heap`, and the float weight rides the x87 stack into the argument area.

**CTCHeroMorph texture-morph sort: pair<EHeroMorphType,CTextureMorphEntry>, 24 bytes (3)**
- 0071f5e0 `__unguarded_linear_insert`, 00720c00 `__unguarded_partition` (calls iter_swap 0x0071FDF0), and
  00721960 `__partial_sort`.
- In 00721960 the out-of-line `__pop_heap` 0x00720610 gets its by-value element built in the argument area.
  - CCompositeBlendDesc is copied memberwise inline here: each CRGBColour goes through an inline `Set(r,g,b,a)`, so
    all four bytes are loaded before they are stored.
  - The `i < last` loop becomes a down-counted trip count.

**vector<CDefPointer<const CThingBaseDef> > (CTCHeroMorph::Tattoos) (5)**
- 0071f2e0 `_M_allocate_and_copy`, 00721150 `operator=`, 00721600 `clear`,
  00721d30 `back_insert_iterator::operator=`, and 00721f70 `__copy` into a back_insert_iterator.
- All follow the STLport 4.x text; 00721150 has the same shape as the landed 0x006F3C20.
- The CDefPointer copy ctor stores before its add-ref null test.
- Empty tag temporaries sit in dead or live argument slots, or in the high byte of the `push ecx` frame word.
- 00721f70 returns the inserter through the hidden pointer in ecx, because back_insert_iterator has a user ctor.

**CTCPhysicsStandard creature-intersection filter (4)**
- 00724240 `RemoveIntersectingCreaturesFromList`: `end()` is read once and used by both remove_if and erase.
  - erase is `__copy_trivial`, which keeps its `last == first` test before the memmove.
- 007249c0 `CIsCreatureIsIntersecting::operator()`: the result byte is held in bl, and the squared distance is
  compared with the squared sum of the radii.
- 00724a50 `remove_copy_if`.
- 00724a90 is STLport's unrolled `__find_if`: four tests per pass, then `switch (last - first)` lowered to a
  `dec eax ; je` chain.

**for_each impulse functors (2)**
- 00726610 (CApplyAdditionalPlayerImpulseIfTooClose, 20 bytes) and 00727500 (CApplyAdditionalImpulseIfInCombatAndTooClose,
  24 bytes).
- The functor is returned by value through the hidden pointer. The return copy must be the implicit memberwise copy
  over a C3DVector that has a USER copy ctor: three loads, then the stores. A trivially copyable C3DVector gives a
  block copy instead (79 v 77).

**CTCSpecialAbilities (2)**
- 0072b3b0 `ReclaimUndeadFollower`: `GetUndeadFollower` is followed by `Health = (long)thing.Health` (_ftol2).
  - The appearance seed comes from interface 3 through the out-pointer `GetInterface(&morph, 3)` (the landed
    SetupBerserk form, which reuses eax).
  - Then `Active = false; thing.Kill(false)`.
- 0072c6e0 constructor: the CTCBase base, then the member inits in declaration order. The STLport list ctor
  allocates and self-links its 0x1c sentinel node.
  - The vector<bool> body `resize(20, true)` is fully inlined.
  - The vector ctor must be STLport's `explicit vector(const allocator_type& = allocator_type())` with an
    `_STLP_alloc_proxy` end-of-storage member. Plain zero initialisers get forwarded as registers (162 v 161).

**CTCClock (1)**
- 007329e0 `PlaySoundW` (the `PlaySound` name as altered by the windows.h macro).
  - HasTC/GetTC(0x10) runs first. Then `"STF_" + name` and `"STL_" + name` go through
    `__fastcall operator+(const char*, const CCharString&)` 0x0099F690, with the hidden result in ecx.
  - CClockDef::GetSound is inlined over the map's `_M_find` 0x0042B467.
  - Finally PlayAttachedSample(CSoundPair, CSoundInit(), true, false, false).

**CTCCombat (STLport-container retail layout) (6)**
- 007334f0 `ClearCombatVariables`: stores in source order, then each ability gets an inline `CCombatAbility::Reset()`.
  Reset() must go through `this`, or the right-link load is hoisted.
- 00733620 `GetCombatAbilityData`: the iterator is the caller's reference. `++it` must be the static
  `_M_node = _Rb_global<bool>::_M_increment(_M_node)`; the in-place member increment gave 122 v 106.
- 00733f70 `SetBlockBroken`: the attacker lookup uses the out-pointer GetInterface(0x30), then BlockCounterTimer is
  set to -1. Then UnsetBlocking 0x007338A0 is called, and finally `last = upper_bound(1); for (it = lower_bound(1); ...)
  Reset()`.
  - `equal_range` stays out of line (190 v 219).
- 007340b0 `OnKnockDown`: the same key-1 reset loop, then `SetBodyReorienterActive(true)` 0x00663490, then
  GetInterface(0xA7) followed by `CancelCurrentMutexAbility()` 0x0072B420.
- 00734340 `GetBlockAbilityIterator`: the same bounds loop through the caller's reference. It makes a virtual
  `GetDataType()` (slot 3) == 1 call, then the WeaponClass +0x2c test.
- 007352d0 `AddThingToAlwaysBlock`: a named CIntelligentPointer local, then `end()` read once, then the `__find`
  0x007379D0 (__fastcall) call, then push_back 0x007398B0. The iterator tag needs a user ctor, or VC7.1 stores a
  zero byte.

**Combat-ability definitions and tree (5)**
- 00738720 `remove_copy` over CDefPointer<const CMeleeCombatAbilityDef>: the decrement must live in a pointee member
  `RemoveRef()` so the `dec [ecx+4]` flags are tested directly (99 v 92 otherwise).
- 00738c90 `CGetMaxComboStage::operator()`: the def out-pointer local sits in the dead `entry` slot.
- 00738d10 CombatAbilities `lower_bound`: `_M_lower_bound` is inlined, and the iterator is returned through the
  hidden pointer.
- 00739670 `_Rb_tree::_M_insert`: the STLport 4.6 _tree.c text with `_M_create_node` inlined in both arms (throw()
  placement new keeps the null test, and the counted-pointer copy does `if (Count) ++*Count`), then the `__cdecl`
  `_Rebalance` 0x0042971D.
- 00739b00 `insert_equal`: the STLport descent, then a tail call to `_M_insert(x, y, v, 0)` that forwards the hidden
  result pointer.

## Parked for Fable 5.1 (6)
- 00721a50 CVectorMap<long,CTCHeroMorph::CTextureLayer>::Add DIFFER(125v125): in the inlined placement copy retail loads pair.first and second.TextureID (ecx, edx) before the first store; ours stores [eax] between the two loads | tried: STLport memberwise pair copy ctor, implicit pair copy, /Oa, CTextureLayer memberwise copy ctor (with and without the pair one), same-TU auto_inline(off) CCompositeBlendDesc copy-ctor body | next: lane gap: load/store scheduling
- 00722bd0 _global::__uninitialized_fill_n DIFFER(86v86): retail keeps cur in ebx and x in edi (push ebx first, x loaded after the three pushes); ours has cur in edi and x in ebx | tried: cur local, stepping first directly, memberwise SThingRecord copy ctor, STLport value-type-pointer outer layer over a __forceinline tag version | next: lane gap: register allocation
- 00729960 list<CExpressionState>::push_front DIFFER(73v73): retail pushes edi at entry and pops it in the common tail; ours shrink-wraps edi into the placement-copy block | tried: _Construct placement copy, direct placement new with explicit padding bytes | next: lane gap: shrink-wrap
- 0072aaf0 CTCSpecialAbilities::SetupMultiStrike DIFFER(66v66): retail does `mov ecx,[esi+4]` right after the name ctor, before `push 0 ; push 1 ; lea eax ; push eax`; ours pushes first and loads ecx last | tried: named CCharString local, ParentThing accessor, implicit conversion temporary, explicit CCharString temporary, local CThing* | next: author it together with the unlanded twins 0x0072A970 / 0x0072A9B0 (same shape) and look for a reference-returning accessor; otherwise lane gap: argument scheduling
- 0072ab70 CTCSpecialAbilities::CreateParticleEffect DIFFER(90v88): retail keeps the null-defaulted emitter in ecx (`xor ecx,ecx ; test ah,8` on a dword in eax, `mov ecx,[eax+4]` into the SetAlwaysUpdate this); ours keeps it in eax (`xor eax,eax ; test ch,8 ... mov eax,[eax+4] ; mov ecx,eax`) | tried: template GetTC (not inlined, 45v88), __forceinline GetTC, out-pointer GetInterface, ternary on the iterator | next: a non-template inline CThing::GetTC(long) with the default-null ternary written on the dword presence word
- 00732d40 CTCWeatherVane::FrameUpdate DIFFER(185v185): retail `mov ecx,[eax+18h] ; mov edx,[ecx]` for the PeekWindDirection vcall (ours `mov eax,[eax+18h] ; mov edx,[eax] ; mov ecx,eax`), sums the squares z,y,x (ours z,x,y), and forms Y*inv with `fxch st(1) ; fmul st,st(1)` | tried: struct copy through int registers (253v185), inline Normalise + WrapAngle, a smart-pointer wrapper (189v185) | next: a PEnvironment accessor returning the pointer by value, plus the literal `Z*Z + Y*Y + X*X` sum order

## Skipped
None. No chunk row held padding plus a second function.

## Label corrections
Each catalog label on the left is wrong; the real name is on the right.

This session's 17 rows (Ego_r.pdb names matched by name and checked against callers):
- 0072AAF0 "UseSpecialAbility_CTCMultiStrike" -> CTCSpecialAbilities::SetupMultiStrike (case of UseSpecialAbility, call site 0x0072D04E)
- 0072AB70 "CreateParticleEffect" (module _global) -> CTCSpecialAbilities::CreateParticleEffect (static __fastcall)
- 0072B3B0 "CCreatureAction_KillReclaimedUndead" -> CTCSpecialAbilities::ReclaimUndeadFollower(CThingGameObject&)
- 0072C6E0 "CEngineSubPrimitive2DClockSprite_ctor" -> CTCSpecialAbilities::CTCSpecialAbilities(CThing&)
- 007329E0 "CTCCreature_PlayMorphSound" -> CTCClock::PlaySoundW(const CCharString&)
- 00732D40 "CViewBase_UpdateLookDirection" -> CTCWeatherVane::FrameUpdate
- 007334F0 "CTCCombat_ResetState" -> CTCCombat::ClearCombatVariables
- 00733620 "CTCCombat_FindBlockAbility" -> CTCCombat::GetCombatAbilityData(iterator&, ECombatAbilityInterfaceType)
- 00733F70 "CAIStateGroup_MinionReactToNoise_Cleanup" -> CTCCombat::SetBlockBroken(CThing&)
- 007340B0 "TCBase_InvalidateAllAbilityTimers" -> CTCCombat::OnKnockDown
- 00734340 "CTCCombat_FindBlockAbilityPrioritized" -> CTCCombat::GetBlockAbilityIterator(iterator&, EWeaponClass)
- 007352D0 "CombatBlockingData_AddIfNew" -> CTCCombat::AddThingToAlwaysBlock(const CThing*)
- 00738720 "_Move<CDefPointer<CMeleeCombatAbilityDef const>*,...>" -> remove_copy over CDefPointer<const CMeleeCombatAbilityDef> (the label is propagated; the real __copy / _Move bodies are 0x0071F430 / 0x0071FA60)
- 00738C90 "TCInterface_SetMaxValue" -> CGetMaxComboStage::operator()(const pair<unsigned long, CAnimationEntry>&)
- 00738D10 "BinaryTree_FindLowerBound" -> CTCCombat::CombatAbilities multimap::lower_bound
- 00739670 "CMemoryAllocatorVariableSize_InsertNode" -> CombatAbilities _Rb_tree::_M_insert
- 00739B00 "RBTree_Find" -> CombatAbilities _Rb_tree::insert_equal

Earlier rows in this chunk:
- 0071EC60 "Insertion_InsertSorted_12byte" -> __unguarded_linear_insert<pair<CSkeletalMorphs::CEntry,float>>
- 0071F570 "Insertion_InsertSorted_12byte" -> __unguarded_insertion_sort_aux (same instantiation)
- 0071F530 "GFToScreen" (manifest, propagated across byte-identical bodies) -> swap<pair<CEntry,float>>
- 0071F5E0 "RBTree_Insert_ShiftUp" -> __unguarded_linear_insert<pair<EHeroMorphType,CTextureMorphEntry>>
- 0071FC20 "Partition_12byte" -> __unguarded_partition<pair<CEntry,float>>
- 0071FE70 "MakeHeap_CSkeletalMorphs_CEntry" -> __make_heap<pair<CEntry,float>>
- 00720C00 "Partition_CTextureLayer_24byte" -> __unguarded_partition<pair<EHeroMorphType,CTextureMorphEntry>>
- 00721150 "Vector_CopyOrMove_CountedPtr" -> vector<CDefPointer<const CThingBaseDef> >::operator=
- 0071F2E0 "Vector_AllocAndCopy_CountedPtr_4byte" -> the same vector's _M_allocate_and_copy
- 00721400 "HeapSort_TextureMorph_Partial" -> __partial_sort<pair<CEntry,float>> (skeletal, not texture)
- 00721600 "Vector_Erase_MoveNext" -> vector<CDefPointer<const CThingBaseDef> >::clear
- 00721960 "TimSort_TextureLayer_24byte" -> __partial_sort<pair<EHeroMorphType,CTextureMorphEntry>>
- 00721A50 "Vector_InsertRange_CCompositeBlendDesc" -> CVectorMap<long,CTCHeroMorph::CTextureLayer>::Add
- 00721D30 "Vector_Push_RefCounted" -> back_insert_iterator<vector<CDefPointer<const CThingBaseDef> > >::operator=
- 00721F70 "Tattoo_CopyToVector" -> __copy(first, last, back_insert_iterator, random_access_iterator_tag, int*)
- 00722BD0 "Vector_Fill_CountedPtr_Pairs" -> __uninitialized_fill_n<SThingRecord>
- 00724240 "Position_MoveElements" -> CTCPhysicsStandard::RemoveIntersectingCreaturesFromList(vector<CThing*>&) const
- 007249C0 "_Find_if<CThing**,CIsCreatureIsIntersecting>" -> CIsCreatureIsIntersecting::operator()(CThing*)
- 00724A50 "Position_CopyNonIntersecting" -> remove_copy_if<CThing**, CIsCreatureIsIntersecting>
- 00724A90 "Position_FindSortedInsertPoint" -> __find_if<CThing**, CIsCreatureIsIntersecting> (unrolled)
- 00726610 "_For_each<...,CApplyAdditionalImpulseIfInCombatAndTooClose>" (propagated from twin 0x00727500) -> for_each<..., CApplyAdditionalPlayerImpulseIfTooClose>
- 00729960 "ExpressionSet_InsertNode" -> list<CExpressionState>::push_front

Callee labels (not chunk rows):
- 004C73D0 "CVertexBufferWin32::DoSizeof" (landed; byte-identical folded body): used here as the CThing position
  accessor `&PhysicsTC->Position`.
- 005B26E9 "_Destroy_range<allocator<NCreatureHitNotification::CFlash>>": shared body; here it is
  `__destroy_aux(first, last, __false_type)` for CDefPointer<const CThingBaseDef>.
- 0071FDF0 "swap<CTCHeroMorph::CTextureLayer>": this is iter_swap of pair<EHeroMorphType,CTextureMorphEntry>.
- 007338A0: CTCCombat::UnsetBlocking (it is landed under a propagated label).
- 00686800 "CEngineSubPrimitive2DClockSprite_ctor" (propagated): CTCBase::CTCBase(CThing&).
- 00686A70: CTCBase::GetWorld.
- 0042B467: the CCharString-keyed map `_M_find` (returns the node).
