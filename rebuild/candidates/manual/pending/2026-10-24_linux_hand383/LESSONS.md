# Chunk 383 -- lessons from the authoring agent (fold into GOTCHAS at landing)

## Inlining
- **CFilter_BindN operators: the filter `operator()`s must be `__forceinline`.**
  - Case: `CFilter_BindN<..>::operator()<T>(const T&)` evaluates `(*F1)(t) && (*F2)(t) && ...`.
  - Symptom: with plain `inline`, the filter operators stay calls (0x00669040 59v183, 0x00669110 50v134).
  - Fix: mark each filter's `operator()` `__forceinline`. A filter retail calls out of line (PosInContainmentVolume 0x006A7B20) stays declared only.
  - Leaf mangling: `??$?RV<T>@@@?$CFilter_BindN@V..@@@@QBE_NABV<T>@@@Z`.
- **The GFRandom LCG inlined into a loop must take the seed pointer by value.**
  - Case: `__forceinline GFRandom(range, unsigned long* seed)` (`*seed = *seed*0x24a1+0x24df; *seed = GFROR13(*seed); return range ? *seed % range : 0;`).
  - Effect: the seed value stays in a register across the store. A member-seed functor re-reads the seed after each store (0x00669850 random_shuffle, 124v113).
- **STLport `vector<ENum>` needs `__forceinline` copy helpers.**
  - Background: an enum element is non-POD to STLport (`__type_traits` is __false_type). Its copies are therefore the counted `__copy` loop and the placement-new null-test `__uninitialized_copy` loop, not memmove.
  - Fix: inside `vector::operator=`, `__uninitialized_copy`, `__copy` and `__copy_ptrs` must be `__forceinline` (plain inline: 200v233, 0x006714B0).
  - In the out-of-line `_M_allocate_and_copy` (0x006715A0), plain inline is enough.
- **STLport sort helpers inlined into their callers need `__forceinline`.**
  - `__insertion_sort` inside `__final_insertion_sort` (0x00672100).
  - `__unguarded_insertion_sort_aux` and `__unguarded_linear_insert` inside `__unguarded_insertion_sort` (0x00672180).
  - With plain inline they stay calls.

## Empty functors and tags
- **STLport `less<T>` / `equal_to<T>` temporaries need a user ctor.**
  - Fix: give them `less() { }`.
  - Symptom without it: every `less<T>()` temporary passed by value emits `mov byte ptr [esp+N],0` (0x006720B0 std::sort).
  - The out-of-line STLport algorithms are `__fastcall`: first in ecx, last in edx, functor by value on the stack.
- **Empty `__false_type` tag temporaries sit in a dead argument byte** (0x006566C3, 0x006568F2, 0x00656A40).
  - Where: the high byte of the atEnd/position slot, `[ebp+1Bh]` / `[ebp+0Bh]`.
  - Fix: same-TU `#pragma auto_inline(off)` noinline template bodies of the tag-taking helpers, with the tag parameter unnamed.
- **An up-cast const& functor parameter gives the conditional-temporary flag.**
  - Case: an inline predicate taking `const CCountedPointer<Base>&`, called on `*it` of a `CCountedPointer<Derived>` list inside `first != last && !pred(*first)`.
  - Effect: the up-cast copy's destructor is conditional on the `&&` (`or [ebp-4],1` / `test byte ptr [ebp-4],1`, 0x0065EA75).
  - A by-value parameter is destroyed unconditionally instead.

## Register allocation and argument order
- **A named physics local gives `lea`.**
  - Form: `CThing* thing = ParentThing; CTCPhysicsBase* physics = thing->PhysicsTC; Create(physics->Position, ...)`.
  - Effect: `lea ecx,[eax+0Ch]`. An inline `GetPosition()` gives `add ecx,0Ch` (0x0066ACE0).
- **A value that retail computes before the argument pushes must be a named local.**
  - Case: `CASoundBank& bank = ParentThing->GetSampleBank();` before the call (0x0066B280).
  - Symptom: written inline as an argument, VC7.1 evaluates it last (right to left), after the pushes.
- **Explicit float deltas give the Z, Y, X square order.**
  - Form: three named float locals in X, Y, Z order, then `dx*dx + dy*dy + dz*dz`, gives retail's Z, Y, X squares (0x006614F0).
  - An inline `C3DVector operator-` temporary gives X, Z, Y.
- **The comparison operand needs LowerBound's body in the TU.**
  - Case: a map Find that copies the key into a dead argument slot and keeps it in a register for the compare.
  - Fix: LowerBound's body must be in the TU (`auto_inline(off)`). With a declaration only, VC7.1 re-reads the escaped key after the call.
  - Test order: `it != end()` with the hit return first gives retail's `je` to the null return (0x0065FBA4 CSoundMap::GetPSoundPair).

## Frames, returns and temporaries
- **/Os (`optimize("s", on)`) plus a stack local gives an ebp frame with `leave`,** even without `optimize("y", off)`. This applies to an object local, a functor result or a CSoundInit temporary (0x0066E742, 0x006569DA).
- **A by-value STLport iterator or back_insert_iterator with user ctors is returned through the hidden pointer** (0x00655A52 list::erase, 0x006611F7 __copy into a back_inserter).
- **An explicit `_Destroy(&node->_M_data)` on a class with an implicit non-trivial dtor calls its `??_G` with 0** (0x00655A52 CChangeText, `push 0 ; call 0x0043F750`).
- **A zero-initialised CCountedPointer temporary folds into immediate stores.**
  - Case: `push_back(CCountedPointer<T>())` puts two `mov dword [ecx],0` stores into the new list node, then `back().Reset(clone)` (0x00664490 CThingCreatureBase::AddQueuedAction).

## Signatures and names
- **CSoundInit callers:**
  - The default ctor runs inline: init-list Volume/Pitch/Pan/NoPlays/PitchFaderBits/OverrideID, body PlaybackType=1, GainFaderBits=0.
  - Callers then set `GainFaderBits = 0x8010`.
  - When an init is passed in, the sample players copy it and set 0x8010 only when `GainFaderBits == 0` (0x0066A920, 0x0066ACE0, 0x0066B080, 0x0066B120).
- **Unused retail parameters stay in the signature.** PlayUnattachedCriteria(const CCharString&, const C3DVector& pos, bool, bool) never reads pos (0x0066BDF0). Keep the PDB signature.
- **The Ego_r PDB is the debug donor and spells its STL Dinkumware; retail is STLport 4.x.**
  - Catalog `_Fill`, `_Unique` and `_Copy_impl` spellings are propagated Dinkumware names.
  - The retail rows are `fill`, `adjacent_find` and `__unguarded_insertion_sort` (0x00654E67, 0x00671BB0, 0x00672180).

Review at landing:
- **Rows that needed more than 4 grades:** the three parked rows. Their differ/ sources each list the 4 forms tried.
  - 0x0066BC10 GetPotentialCriteriaStrings: try a same-TU `__forceinline` push_back.
  - 0x0066BDF0 PlayUnattachedCriteria: lane gap (zero-register CSE live range), the same as landed 0x0066BD00.
  - 0x00670470 CTCCoopSpirit::UpdateHeightAboveGround: try `--flags /Op`.
- **Local base models next to an existing engine header:**
  - 0x00658E38 uses a local CTCVillage model, because `engine/CTCVillage.h` carries VillageBuildings at +0x28 as an opaque 0xC-byte PDB list while retail is the 4-byte STLport sentinel.
  - These rows also use local class models:
    - the sound rows 0x0066A920 to 0x0066B280 and 0x0066E742;
    - the filter rows 0x00669040 and 0x00669110;
    - 0x00664490 (CThingCreatureBase);
    - 0x0065FBA4 and 0x0065FF5E.
  - The models carry retail offsets: CThing interface bitset +0x20, map +0x44, PhysicsTC +0x60, PDef +0x70; CTCBase ParentThing +4; CTCPhysicsBase Position +0xc, radius +0x34; CPlayerGuiDef InventoryNoMovementSound +0x3c.
  - Headers that already exist and should be checked or retyped against: `engine/CTCPhysicsBase.h`, `engine/CTCVillage.h`, `engine/CThingCreatureBase.h`, `engine/CTCBase.h` and `engine/CThingPhysical.h`.
  - There is no header for CThing, CTCSoundPlayer, CASoundBank, CSoundInit or CSoundMap.
- **Errors in already-landed sources:**
  - 0x0065F8B2 is filed as `CEngineSceneGrid::GetCellBoundingBox` but is `std::advance`.
  - 0x0065F943 is filed as `unattributed_ConstructCRangeString` but is `std::find_if`.
  - 0x0065605F is filed as `CActiveFile_OnReadFinished` but is `vector<CIntelligentPointer<CParticleEmitter>>::_M_clear`.
  - 0x006715F0, landed `CActiveFile_OnReadFinished_006715f0.cpp`, is a stand-in stub; the function is `vector<ENavigatorType>::_M_clear`.
  - The comments on 0x00654E90, 0x006556A8 and 0x0065F96D say Dinkumware; they are STLport.
  - 0x0066B1D0 PlayAttachedSoundEvent passes `CSoundInit*` where InternalPlaySoundEvent's third parameter is a bool (PDB `_N22J`).
  - 0x0066BD00 PlayAttachedCriteria and 0x008A8F20 carry non-retail stand-in bodies.
- **Model names to confirm:** the EScreenSoundTypes enumerator for value 2 (0x0066E742), and CASoundBank slot 9 (+0x24) `IsSoundEventValid(const CCharString&)` (base 0x00A0BD20 returns false).
- **Catalog rows that hold several functions:** none. Every row whose catalog length exceeds the function is int3 alignment padding after a `ret`.
