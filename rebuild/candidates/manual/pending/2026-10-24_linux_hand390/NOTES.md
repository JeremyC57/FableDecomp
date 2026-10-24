# Hand chunk 390 (CTCHeroAttachableAppearanceModifiers .. CTCHeroMorph): 38 of 40 byte-exact

All 40 rows carry the catalog module `_global`; by caller, layout and the Ego_r / FableWin PDB names they fall into five
retail TUs. Retail STL is STLport 4.x in namespace `std` (RTTI `.?AU?$pair@...@std@@`): vectors are 12 bytes, lists 4.
The algorithm templates are `__fastcall` (the /Gr TUs). Every source uses a local model of the retail layout: the
one engine header that exists here (CTCHeroAttachableAppearanceModifiers.h) carries the 16-byte PDB containers.

## What matched (38)

### CTCHeroAttachableAppearanceModifiers TU (7)
- 00706800 `GetAppearanceModifierList(CLongList&, long type) const`: copies the def index of each modifier of a type
  into a list<long>. To match, the STLport list iterator needs its user copy constructor and insert() has to read the
  head's prev into a local first. Without both, VC7.1 shrink-wraps the ebp/esi pushes. Took 5 grades.
- 00708600 `vector<CModifierEntry>::erase(iterator)`: the STLport `__copy_ptrs` call becomes the counted
  random-access `__copy`.
- 00708b60 / 00708ba0 `__uninitialized_copy` / `__uninitialized_fill_n<CModifierGraphic*>`: the `__false_type` loops
  with placement new (null test) and the CDefPointer copy constructor (RefCount at +4), as `__fastcall`.
- 007093c0 `vector<vector<CModifierEntry> >::vector(size_type n)`: `_Vector_base(n, alloc)` with the
  `_STLP_alloc_proxy`. The element has no destructor (same as the landed 0x00706D10).
- 00709c90 / 00709e60 `__uninitialized_copy` and `push_back` for `pair<CMeshInfo, CEngineTextureMorphSet>` (24 bytes).
  The pair has a memberwise copy constructor, and the morph-set copy (0x005BBDF1) stays out of line.

### CTCTextureDecal TU (3)
- 0070aaf0 `__uninitialized_fill_n<CEngineTextureDecal*>`: the STLport vector copy constructor of the blend list is
  inlined (`_Vector_base`, `get_allocator`, `__uninitialized_copy` 0x005BAF28 out of line).
- 0070aea0 `map<long, CEngineTextureDecal>::_M_insert(x, y, v, w)`: the CCameraModeMap 0x0069D520 model.
- 0070b240 `vector<CEngineTextureDecal>::_M_insert_overflow`: the two algorithm callees are same-TU stand-in bodies
  under `#pragma auto_inline(off)` + `__declspec(noinline)`. That moves the empty tag into the dead `atend` slot.

### CAnimateThing / CAnimSet TU (20)
- 0070b9b0 `list<CAnimSet>` scalar deleting destructor: `~_List_base` (clear, `if (p) free(p)`). The non-polymorphic
  `??_G` comes from a `delete` helper.
- 0070bd80 `GFGetInterpolatedLoopingValue(float, float, float, float, bool)` (Ego_r.pdb name): a looping lerp
  (`fmod` intrinsic for the forward wrap, `while (v < 0) v += range` for the backward one). The bool is the only
  register-sized argument, so it travels in cl.
- 0070be40 `CAnimateThing::GetProgressFromAlpha`: two `>=` range tests against the non-const 1e-4 epsilon global.
- 0070bfc0 `CAnimSet::SetAlpha`: clamp to [0, 1] with if / else-if, then roll the current alpha into the last one.
- 0070c770 `IsAnimStateBlendValid` (anonymous namespace): two separate `return false` statements keep the early
  epilogue inline, and `Data == 0 ? 0 : size` lays out the zero arm first. A `_Use` caller placed before the
  definition keeps the function emitted.
- 0070d0a0 `CAnimateThing::GetAllCombinationAnimsFromType` / 0070d2f0 `CAnimSet::AddAnim`: inline STLport push_backs
  whose overflow callee is a same-TU stand-in body, so the rvalue and tag temporaries share the dead argument slots.
- 0070ec30 `CAnimateThing::ClearAllAnims`: the queue is refilled with `push_back(CDelayedAnimation())` until
  `size() == 3`. The two list clears are same-TU stand-in bodies. With them, VC7.1 keeps the temporary's sentinel in
  esi, drops its zero store and frees it without a null test.
- 0070f410 `_Temporary_buffer::_M_allocate_buffer`, 0070fdc0 `swap_ranges`, 007107d0 `__merge_sort_loop`,
  00710d00 `__merge_backward`, 007114c0 `__chunk_insertion_sort`, 007116b0 `__merge_sort_with_buffer`, 00711920
  `__stable_sort_adaptive`, 00711b80 `__stable_sort_aux`: the STLport stable_sort chain over
  `list<CAnimSet>::const_iterator` with CAnimSetItPriorityLessThan (Priority at node +0x54), all written verbatim.
  Details:
  - `(min)` takes const references: the difference is stored in a dead argument slot and its address is selected.
  - copy_backward has to be the real STLport chain. VC7.1 then inlines the two early-out calls and keeps the two
    in-loop calls out of line at 0x007108A0.
  - The iterator is non-POD because of its user constructor, so the temporary buffer is filled with the
    `__false_type` placement loop.
  - `__stable_sort_aux` needed stand-in bodies for its three callees.
- 00710980 `vector<short>::_M_insert_overflow` (`__true_type`): `__copy_trivial` (memmove unless empty), `fill_n`
  (`jbe`), the inline `_M_clear`, and `(max)` by reference.
- 007119c0 `list<CAnimSet>::_M_insert_dispatch<const_iterator>`: `insert(pos, *first)` links through the n / prev
  locals.
- 00711d50 `CAnimateThing::CDelayedAnimation::CDelayedAnimation()` (FableWin.pdb names this ctor): the `_List_base`
  constructor builds the sentinel in a local and stores it last.
- 00711e30 `list<CDelayedAnimation>::_M_create_node`: the placement copy of the list (its copy constructor calls
  `insert(begin(), x.begin(), x.end())`). The iterator's user copy constructor builds pos in its stack slot. The tag
  types need a user constructor (`__false_type() {}`), or VC7.1 stores 0 to the tag byte.

### CActionTalkBase / CSubtitledMessage TU (4)
- 00718390 `__uninitialized_copy<CSubtitledMessage::CCameraTag*>` (six enums, 24 bytes), and 007196d0
  `vector<CCameraTag>::_M_insert_overflow`: the push_back overflow behind
  CActionTalkBase::InitialiseDefaultCameraTags 0x007167E0. It reuses the 0x0070B240 shape with stand-in callee bodies.
- 00718fb0 `vector<pair<float, EConversationAttitude> >::_M_allocate_and_copy`: STLport's pair carries an explicit
  `pair(const pair<_T1,_T2>&)` next to the converting template. With only the implicit copy, the second word is
  register-allocated differently (3 grades).
- 00719fa0 `vector<CSubtitledMessage>::_M_allocate_and_copy`: allocate n * 0x44, then the inline placement loop that
  calls the out-of-line copy constructor.

### CTCHeroMorph TU (4)
- 0071c120 `AdjustMorphsOfLowerPriority` (anonymous namespace): a plain `for` that scales `second` when
  `first.Priority < priority`. VC7.1 unrolls it four times and adds the remainder loop.
- 0071cf70 `AddMorph` (anonymous namespace): a local vector<CEntry>, then `GetAllMorphsOfType` and
  `push_back(pair<CEntry, float>(*it, weight))`. With the overflow as a same-TU stand-in body, VC7.1 hoists the
  weight store out of the loop and copies straight from the source.
- 0071de20 `CTCHeroMorph::GetTattooThings`: `clear(); reserve(Tattoos.size()); copy(..., back_inserter(things))`.
  With a stand-in `__copy` body, the returned back_insert_iterator lands in the dead `things` slot instead of its own
  frame word.
- 0071ec20 `std::swap` of a 12-byte trivially copied T (`pair<CSkeletalMorphs::CEntry, float>`). It is an
  unreferenced COMDAT.

## Parked for Fable 5.1 (2)
- 0070a490 CTCTextureDecal::AddDecal DIFFER(131v131): in the list-node copy retail loads both entry words (`mov ecx,[esp+10h] ; mov edx,[esp+14h]`) before the first store, ours stores the first word between the two loads | tried: implicit entry copy, memberwise user copy ctor, 8-byte POD key member with and without a ctor, QFE 4035 compiler | next: a same-TU stand-in body for the CCompositeBlendDesc copy constructor 0x005BA48A (a header-inline COMDAT the TU can see), the stand-in lesson of 0x0070EC30 / 0x0071CF70
- 0070bf50 `anonymous namespace'::IsAnimationStateBaseValid DIFFER(104v104): block layout only: retail places the type 0/1 IsLoaded block right after `cmp eax,1 ; jne` (index through esi), VC7.1 here moves it behind the shared true/false tails (`je` twice, index through ecx) | tried: if/else-if with `||`, early return true/false per block, inverted `!= 0 && != 1`, `? true : false` result | next: lane gap: basic-block placement (else an inline helper for the 0/1 case)

## Skipped
- None. All 40 rows are single functions, and the scanner lengths agree with the code (padding only after the last `ret`).

## Label corrections
- 00706800 `CArray_InsertNode` -> CTCHeroAttachableAppearanceModifiers::GetAppearanceModifierList
- 00708600 `AppearanceModifier_VectorErase` -> std::vector<CModifierEntry>::erase(iterator)
- 00708b60 `_Copy_impl<CNavigationPosition*>` -> std::__uninitialized_copy<CModifierGraphic*> (fold-sibling name)
- 00708ba0 `_Fill_n<unsigned long*>` -> std::__uninitialized_fill_n<CModifierGraphic*>
- 007093c0 `std_vector_InitFromRange` -> vector<vector<CModifierEntry> >::vector(size_type) (no range)
- 00709c90 `_Move<CMusicSetEntry*>` -> std::__uninitialized_copy<pair<NGraphicAppearance::CMeshInfo, CEngineTextureMorphSet>*>
- 00709d60 `Vector_Reallocate_CMusicSetEntry` -> that vector's _M_insert_overflow (callee, not in the chunk)
- 00709e60 `std_vector_Cons_val_CMusicSetEntry` -> vector<pair<CMeshInfo, CEngineTextureMorphSet> >::push_back
- 0070a490 `CRenderTarget_EnqueueDecalUpdate` -> CTCTextureDecal::AddDecal
- 0070aa50 `Vector_Copy_BodyPartMesh` -> std::__uninitialized_copy<CEngineTextureDecal*> (callee)
- 0070aaf0 `RandomAppearanceMorph_CopyElements` -> std::__uninitialized_fill_n<CEngineTextureDecal*>
- 0070aea0 `BinaryTree_Node_Insert` -> map<long, CEngineTextureDecal>::_M_insert (_Rb_tree)
- 0070b240 `Vector_Reallocate_RandomAppearance` -> vector<CEngineTextureDecal>::_M_insert_overflow
- 0070b8a0 `CNavigatorQueueManager::FrameUpdate` -> list<CAnimSet>::clear (callee)
- 0070b9b0 `CNavigatorQueueManager_dtor` -> list<CAnimSet> `scalar deleting destructor'
- 0070ba00 `CGuiControlTreePane::SortTreeRecursively` -> list<CAnimateThing::CDelayedAnimation>::clear (callee)
- 0070bd80 `AnimationInterpolation_CalcValue` -> GFGetInterpolatedLoopingValue (Ego_r `?GFGetInterpolatedLoopingValue@@YIMMMMM_N@Z`)
- 0070be40 `SafeDivideFloat` -> CAnimateThing::GetProgressFromAlpha
- 0070bfc0 `Animation_ClampAlpha` -> CAnimSet::SetAlpha
- 0070c770 `AnimationBank_ValidateAnimationStates` -> `anonymous namespace'::IsAnimStateBlendValid
- 0070d0a0 `CActionState_AddAnimSetToList` -> CAnimateThing::GetAllCombinationAnimsFromType
- 0070d2f0 `CThingMovement_SetAnimFrame` -> CAnimSet::AddAnim
- 0070ec30 `Navigator_UpdateBuildingTreeUI` -> CAnimateThing::ClearAllAnims
- 0070f410 `Vector_Reallocate_DownsizingLoop` -> std::_Temporary_buffer<list<CAnimSet>::const_iterator*, ...>::_M_allocate_buffer
- 0070fdc0 `_Copy_impl<CRGBColour*,CRGBColour*>` -> std::swap_ranges<list<CAnimSet>::const_iterator*> (a swap, not a copy)
- 007103c0 `RotateBuffer_GCD` -> STLport random-access __rotate (it calls swap_ranges when k == l; callee)
- 007107d0 `InPlaceMerge_AnimSets` -> std::__merge_sort_loop
- 007108a0 `_Copy_impl<pair<float,EConversationAttitude>*>` -> std::copy_backward for 4-byte elements (folded)
- 00710980 `Vector_Reallocate_uint16` -> vector<short>::_M_insert_overflow
- 00710d00 `StableSortMerge_Pair_EConversationAttitude` -> std::__merge_backward<list<CAnimSet>::const_iterator*>
- 00711340 `StableSortMerge_AnimSet` -> std::__merge_adaptive (callee); 00711640 `std::introsort_CAnimSet` -> std::__inplace_stable_sort (callee)
- 007114c0 `CAnimSet_InsertionSortChunked` -> std::__chunk_insertion_sort
- 00711520 `CNearestBuildingWithProperty` (landed as CNavigatorOwner_00711520::ResetBuffers) -> CAnimSet::Clear (Ego_r `?Clear@CAnimSet@@QAEXXZ`; ClearAllAnims calls it on MainAnimSet)
- 007116b0 `CAnimSet_InsertionSort` -> std::__merge_sort_with_buffer
- 00711920 `CAnimSet_IntrosortRecursive` -> std::__stable_sort_adaptive (no introsort)
- 007119c0 `AnimSet_CopyIntoList` -> list<CAnimSet>::_M_insert_dispatch<const_iterator>
- 00711b80 `CAnimSet_SortAndOptimize` -> std::__stable_sort_aux
- 00711cf0 landed `CAnimSet::CAnimSet` -> CAnimateThing::CCompleteAnimation::CCompleteAnimation() (MainAnimSet + TransitionAnimSets +0x54 + CombinationAnims +0x58; retail CAnimSet is 0x54 bytes and owns no lists)
- 00711d50 `std_list_ctor` -> CAnimateThing::CDelayedAnimation::CDelayedAnimation()
- 00711e30 `AnimSet_CopyArray_ctor` -> list<CAnimateThing::CDelayedAnimation>::_M_create_node
- 007167e0 `CActionTalkBase_InitMorphs_Hardcoded` -> NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags (static DefaultCameraTags @ 0x013BAD18)
- 00718390 `_Move<CTVertexLandscapeBackground*>` -> std::__uninitialized_copy<CSubtitledMessage::CCameraTag*>
- 007183e0 `_Fill<pair<EHeroMorphType,CTextureMorphEntry>*>` -> std::__uninitialized_fill_n<CCameraTag*> (callee)
- 00718fb0 `Vector_AllocAndCopy_8byte` -> vector<pair<float, EConversationAttitude> >::_M_allocate_and_copy
- 00719470 `CSubtitledMessage::Assign_Pairs` -> vector<pair<float, EConversationAttitude> >::operator= (caller)
- 007196d0 `Vector_ReallocateAndCopyMorphs` -> vector<CSubtitledMessage::CCameraTag>::_M_insert_overflow
- 00719fa0 `Vector_AllocAndConstruct_CSubtitledMessage` -> vector<CSubtitledMessage>::_M_allocate_and_copy
- 0071de20 `Tattoo_ApplyChanges` -> CTCHeroMorph::GetTattooThings
- 0071ec20 `GFToScreen` -> std::swap<12-byte T> (pair<CSkeletalMorphs::CEntry, float>; unreferenced)
- 00721f70 `Tattoo_CopyToVector` -> std::__copy<const CDefPointer<const CThingBaseDef>*, back_insert_iterator<...> > (callee)
- 005BBDF1 `CExpressionSet::Add` -> 20-byte vector copy ctor; 005BB346 `std::vector::operator=` -> vector<CCompositeBlendDesc> copy ctor; 004C7590 `CRenderTarget::DetachTarget` -> CThing forwarder; 0066C3F0 `std::vector<uint>::_Ufill` -> the 4-byte POD vector _M_insert_overflow (callees)
