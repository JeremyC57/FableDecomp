# Hand chunk 419 (Introsort_Partition_CCharStringPair .. BinaryTreeTraversal_Insert): 34 of 40 byte-exact

All 40 rows are `_global` in the catalogue. 34 are byte-exact (6 MATCH, 28 RELOCATION_MATCH). Each has a behaviour test,
and all 34 tests PASS under both g++ and the lane's VC7.1. 5 rows are parked in differ/ and 1 is skipped.

## What matched (34)
- **STLport algorithms over C3DSkeletalBoneMorph** (20 bytes: CCharString, CRC, C3DVector), all `/Gr` `__fastcall`
  templates in `_STL`:
  - 00a45a70 swap
  - 00a45b00 __unguarded_linear_insert
  - 00a45bc0 __push_heap
  - 00a461d0 __unguarded_partition (iter_swap calls the out-of-line swap)
  - 00a46350 __make_heap
  - 00a46530 __linear_insert (copy_backward inlines down to the out-of-line `__copy_backward`)
  - 00a46940 __partial_sort
- **Over pair<CCharString, 4-byte value>:**
  - 00a41d50 __introsort_loop: an empty `CKeyPairCompareLess` with a user ctor; `__median`, `__unguarded_partition` and
    `__partial_sort` stay out of line.
  - 00a55500 vector::reserve: `_M_allocate_and_copy` inline, the tag on the live `n` slot through a same-TU noinline
    `__uninitialized_copy`, and a declared `_M_clear`.
- **CLipSync containers:**
  - 00a484b0 / 00a48870 `__copy` (twins)
  - 00a48670 / 00a488c0 `__uninitialized_copy` (twins)
  - 00a4a8c0 / 00a4ab10 `vector<CLipSyncPhoneme>::push_back` (twins)
  - 00a4ae00 `vector<CLipSyncPhonemeSample>::push_back`
  - 00a4a670 `map<unsigned long, CCharString>::operator[]`
  - 00a4a910 `CCountedPointer<CSpeechFrame>::DeleteData`
- **Text bank (NGameText::CDataBankTextEntry):**
  - 00a43500 `__uninitialized_fill_n`
  - 00a43670 `uninitialized_fill_n`
  - 00a43b20 `vector::_M_insert_overflow`
- **Mesh and animation containers:**
  - 00a52080 `fill<C3DTriangle2*>`
  - 00a520b0 `uninitialized_fill_n<C3DTriangle2*>`
  - 00a53650 `vector<C3DTriangle2>::_M_allocate_and_copy`
  - 00a54580 `vector<C3DKeyframe>::vector(n)`
  - 00a54fa0 `vector<C2DLineF>::_M_insert_overflow`
  - 00a547c0 `_Rb_tree<CCharString, pair<const CCharString,long>>::_M_copy`
  - 00a537f0 `C3DAnimFile::GetRoot() const`, the byte twin of the landed `C3DMeshFile::GetRoot` 0x00A32620
- **Engine functions with PDB names:**
  - 00a44660 `CSuspendableProcessMicrothread::InternalSwitchToProcess` (a `#pragma optimize("", off)` TU)
  - 00a44df0 `C3DSkeletalBoneMorph::C3DSkeletalBoneMorph(const CCharString&, const C3DVector&)`
  - 00a45470 `C3DSkeletalMorph::AddMorphParameter`
  - 00a47d80 `CNavigatorExternalInterface::RemoveAgentListFromLists`
  - 00a4b7b0 `GFWorld3DDistanceSmallerThan`
- **Descriptive name:** 00a443a0 `GFGetDistanceSquaredFromPointToLine2D`. No PDB name was found for this signature.

## Parked for Fable 5.1 (5)
- 00a441e0 GFGetSignedAngleDifference DIFFER(106v98): retail compares the live x87 copy of `to - from` (`fcom`, then
  `fst [esp+8]`). Ours reloads the homed value for both range compares (`fld ; fcomp`). The fmod arm, the wrap and the
  0.5 fold already match. | tried: the landed in-place WrapAngle; an if/return wrap with a ternary tail; a `>= 0 && < 1`
  early return; a separate `wrapped = angle` result | next: compare the result copy instead of the parameter
  (`if (wrapped < 0 || wrapped >= 1) wrapped = fmod(angle..)`), or write the whole wrap as one ternary. Same TU and the
  same residue as 00a442e0.
- 00a44250 GFGetInterpolatedColour DIFFER(124v117): retail_asm candidate. Retail is an inline-asm MMX block over C
  locals: every operand is a `movd` from a frame slot, the zero is a stored memory operand of `punpcklbw`, there is no
  `pxor`, and it uses a plain esp frame. The VC7.1 intrinsics give an aligned ebp frame with register movd/movq.
  | tried: the MMX intrinsics (the RTM include dir has no mmintrin.h, so the declarations are restated) | next: land it
  as `retail_asm` with the C++ reference body plus the minimal block, if the owner accepts MMX under the exception.
- 00a442e0 GFGetDistanceSquaredFromPointToLine DIFFER(190v188): retail keeps a register copy of |line|^2
  (`fst [esp+10h] ; fld 0 ; fld st(1)`, then `fdivrp st(3)`, plus one more `fstp st(0)` on the zero path). Ours stores it
  with fstp and reloads it for the zero test and the divide. Everything else matches. | tried: a named local; spelling
  GetLengthSquared() at each use (the best, now parked); `== 0` early return; a __forceinline helper taking the length
  by value | next: a `double` local for |line|^2, or the assignment inside the condition.
- 00a47d10 CNavigatorExternalInterface::RemoveAgentFromLists DIFFER(110v97): the lane strength-reduces `pos + 1` into
  a fifth callee-saved register, spills `agent` and splits `--_M_finish` into load/add/store. Retail keeps agent in ebp,
  copies pos to edx, forms `lea ecx,[edx+4]` only on the match arm and uses one `add [edi+4],-4`. | tried:
  `erase(i); --i;`, `erase(i--)`, `i = erase(i); --i;`, and the layered STLport erase (`__copy_ptrs` + `__true_type` +
  `_Destroy`); all four give the same 110 bytes | next: an index loop or another iterator shape that keeps pos+1 off
  the induction set.
- 00a557e0 _STL::copy(set<CCharString> range, back_insert_iterator<vector<CCharString> >) DIFFER(155v154): the only
  difference is `push edi`. Ours saves it in the prologue, before the empty-range `je`; retail shrink-wraps it into the
  loop path. | tried: __copy not inlined; `__forceinline __copy`; the static `_Rb_global<bool>::_M_increment`; an extra
  `__copy_aux` layer | next: a same-TU noinline `vector<CCharString>::_M_insert_overflow` stand-in (callee-clobber
  facts), or an early-return form.

## Skipped
- 00a43e30 (catalogued `CVertexBufferWin32_SizeofElements`, 60 bytes) is not a function start. Its first byte is the
  displacement of the `jmp short` at 0x00A43E2F, inside the function at 0x00A43DF0 (a vector Sizeof loop calling
  0x00A43960 and 0x009D5620), and xref.py finds no callers or references. The catalogue should hold 0x00A43DF0 instead.

## Label corrections
All of these are rows of this chunk unless marked "landed".

**STLport algorithms (named with the STLport spelling):**
- 0x00A41D50 `_STL::__introsort_loop<pair<CCharString,4-byte>*>`, not Introsort_Partition_CCharStringPair.
- 0x00A45B00 `__unguarded_linear_insert`, not Vector_Insert_ConditionalShift.
- 0x00A461D0 `__unguarded_partition`, not Introsort_PartitionStep.
- 0x00A46350 `__make_heap`, not MakeHeap_Generic.
- 0x00A46530 `__linear_insert`, not Vector_InsertConditionalShift_Decal.
- 0x00A46940 `__partial_sort`, not Introsort_Partition_With_Predicate.
- 0x00A484B0 and 0x00A48870 `__copy<CLipSyncPhoneme*>`, not `_Move`.
- 0x00A48670 and 0x00A488C0 `__uninitialized_copy<CLipSyncPhoneme>`, not `_Copy_backward<CBrainUpdateZone>`.
- 0x00A557E0 `copy(set<CCharString>::const_iterator, ..., back_insert_iterator<vector<CCharString> >)`, not
  BinaryTreeTraversal_Insert.

**STLport containers:**
- 0x00A43500 and 0x00A43670 `(__)uninitialized_fill_n<NGameText::CDataBankTextEntry*>`, not `_Fill_n<unsigned long*>`.
- 0x00A520B0 `uninitialized_fill_n<C3DTriangle2*>`, not `_Fill_n<unsigned long*>`.
- 0x00A43B20 `vector<NGameText::CDataBankTextEntry>::_M_insert_overflow`, not Vector_Reallocate_CDataBankGroupEntry.
- 0x00A4A670 `map<unsigned long, CCharString>::operator[]`, not BST_InsertEntry_EHeroMorphType (the key is unsigned).
- 0x00A4A8C0 and 0x00A4AB10 `vector<CLipSyncPhoneme>::push_back`.
- 0x00A4AE00 `vector<CLipSyncPhonemeSample>::push_back`.
- 0x00A53650 `vector<C3DTriangle2>::_M_allocate_and_copy`, not Array_Copy_Construct.
- 0x00A54580 `vector<C3DKeyframe>::vector(size_type)`.
- 0x00A547C0 `_Rb_tree<CCharString, pair<const CCharString,long>>::_M_copy`, not TreeNode_CloneRecursive.
  - Its caller 0x00A54A90, already landed as `CRetailTree_00a547c0::operator=`, is that tree's operator=.
- 0x00A54FA0 `vector<C2DLineF>::_M_insert_overflow`.
- 0x00A55500 `vector<pair<CCharString,4-byte> >::reserve`, not CDefendingCombatantInfo_CCombatWheel_Reallocate.

**Engine functions:**
- 0x00A441E0 is Ego_r.pdb `GFGetSignedAngleDifference` (`__fastcall`, YIMMM), not AngleDifference.
  - landed: CTCLook::GetTurningAngles 0x00751E80 declares it `__stdcall AngleDifference`. The code is identical (both
    floats on the stack, `ret 8`), but the name and convention should be the PDB's.
- 0x00A442E0 and 0x00A443A0 compute squared point-to-line distances with the projection parameter as an out-param.
  There is no clamp to the segment and no point is returned, so they are not Vector3_ClosestPointOnLineSegment or
  GetClosestPointOnLineSegment_DistanceSquared (descriptive names: GFGetDistanceSquaredFromPointToLine / ...2D).
- 0x00A44DF0 `C3DSkeletalBoneMorph::C3DSkeletalBoneMorph(const CCharString&, const C3DVector&)`, not
  CCharString_InitWithCRC.
- 0x00A45470 `C3DSkeletalMorph::AddMorphParameter`, not CSkeletalMorph_SetMorphTarget.
- 0x00A4A910 `CCountedPointer<CSpeechFrame>::DeleteData`, not FreeListNode_Destroy.
- 0x00A537F0 `C3DAnimFile::GetRoot() const` (Ego_r.pdb), not SharedPtr_Copy.
  - landed: C3DAnimationStats::InitialiseStats 0x00A4EBB0 declares it `GetRootChunk`.
- Global 0x013D2834 is `CNavigatorExternalInterface::AgentLists`, an STLport `list<vector<const CSteeringAgent*>*>`
  (Ego_r.pdb dynamic initializer name).
  - landed: 0x006CE670 calls it `g_registeredObstacles` (list<CNavigatorObstacles*>) and 0x01224320 calls it
    `g_list_013d2834`. The "obstacle record" those rows push is therefore a vector<const CSteeringAgent*>.
