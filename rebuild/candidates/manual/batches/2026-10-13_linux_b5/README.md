# Batch `2026-10-13_linux_b5` — 71 commented matching candidates (53 landed)

Authored 2026-10-13 in the Linux container **with the compiler in the loop**: every candidate was
graded through the Wine lane (DDK 13.10.4035 `cl.exe`, `/O2 /Oy /W3` + the lander's pragma/flag
sweep) as it was written, re-shaped against the retail bytes, and landed with
`verify_and_land.py --land`. Targets are the next 80 smallest un-landed manifest functions
(`tools/decomp_pipeline/crawl/next_smallest.py 80`), minus 9 rows whose "function" is a mis-trimmed
fragment (0074b770, 005eb080, 00bc3f30, 0074bc20, 00d6bb80, 00995e70, 00c969b0, 00c9cb40 and the
eax-input fragment at 00c9cb40 are not functions at their recorded bounds).

## Lane grading result

**53 of 71 rows are byte-exact (MATCH / RELOCATION_MATCH) with behaviour PASS** and live in
`rebuild/src/compiled/`, `rebuild/tests/` and the catalog. The 18 still-DIFFER rows (ledgered in
`tools/decomp_pipeline/crawl/gen_tried.txt`) are instruction-order misses, each carrying the best
shape found and the remaining difference in its header comment:

- `0066106d..b5` (5): an unoptimised `__stdcall` thunk whose `push dword ptr [ebp+8]` the lane's
  flag set cannot reproduce (needs frame pointers kept with /O1-style codegen: `/Oy-`, outside the sweep).
- `00658e90`, `006590c4..f4`, `0076e5d0` (6): pass-through `push dword ptr [esp+8]` + `lea` of a
  member; every shape tried loads the argument into a register first.
- `008424a0`, `008424d0`, `00687d40` (3): retail `call ; ret` where our build emits the sibling-call
  `jmp` (the named-local trick only blocks it for a float result).
- `00c8db20`: direct `call ??_G(0)` of a polymorphic destructor from an allocator destroy helper.
- `004b9e20`, `00c42b80`, `00787bf1`: load ordering around a call / vtable fetch.

## Naming

Most manifest labels on these rows are propagated template names (`_Umove<EWeaponClass*>`,
`_Dest_val<...>`, `operator_new`, `SetStaticCast<...>`); they are NOT trusted. Rows whose class could
not be established from the bytes are landed under module `_unattributed` with a leaf that states
what the body does, and the header comment says which label was rejected and why. Trusted
identities (landed PASS callees, PDB member names at the touched offsets) are cited where used.

## Functions

| File | Address | Name | Lane verdict |
|---|---|---|---|
| `unattributed_HasNonEmptyName_0048db98.cpp` | 0048db98 | _unattributed::HasNonEmptyName | landed |
| `CWorld_GetChecksum_0049e200.cpp` | 0049e200 | CWorld::GetChecksum | landed |
| `unattributed_CallWithValueAndAddress_004b9e20.cpp` | 004b9e20 | _unattributed::CallWithValueAndAddress | DIFFER |
| `global_GFHasScriptThing_004c7e70.cpp` | 004c7e70 | _global::GFHasScriptThing | landed |
| `CThingComponentDefNameTag_ctorCTCDParticleEmitter_004d3dfc.cpp` | 004d3dfc | CThingComponentDefNameTag::ctor_CTCDParticleEmitter | landed |
| `CThingComponentDefNameTag_ctorCTCDNone_004d3ef9.cpp` | 004d3ef9 | CThingComponentDefNameTag::ctor_CTCDNone | landed |
| `CThingComponentDefNameTag_ctorCTCDNavigationSeed_004d41df.cpp` | 004d41df | CThingComponentDefNameTag::ctor_CTCDNavigationSeed | landed |
| `CUserProfileManager_IsShowingTargetingStatus_00548740.cpp` | 00548740 | CUserProfileManager::IsShowingTargetingStatus | landed |
| `global_BasicStringRaise_00565360.cpp` | 00565360 | _global::BasicStringRaise | landed |
| `global_BasicStringRaise_00565a30.cpp` | 00565a30 | _global::BasicStringRaise | landed |
| `CTCInventoryBase_ProcessButtonAReleased_005bc649.cpp` | 005bc649 | CTCInventoryBase::ProcessButtonAReleased | landed |
| `CTCInventoryBase_ProcessButtonAReleased_005bc65c.cpp` | 005bc65c | CTCInventoryBase::ProcessButtonAReleased | landed |
| `CDrawQuestInfo_GetElementCount_00647308.cpp` | 00647308 | CDrawQuestInfo::GetElementCount | landed |
| `unattributed_ForwardWithMember54_00658e90.cpp` | 00658e90 | _unattributed::ForwardWithMember54 | DIFFER |
| `unattributed_ForwardWithMember4C_006590c4.cpp` | 006590c4 | _unattributed::ForwardWithMember4C | DIFFER |
| `unattributed_ForwardWithMember50_006590d4.cpp` | 006590d4 | _unattributed::ForwardWithMember50 | DIFFER |
| `unattributed_ForwardWithMember58_006590e4.cpp` | 006590e4 | _unattributed::ForwardWithMember58 | DIFFER |
| `unattributed_ForwardWithMember5C_006590f4.cpp` | 006590f4 | _unattributed::ForwardWithMember5C | DIFFER |
| `CVillageTaskTorch_IsTaskAvailable_0065edac.cpp` | 0065edac | CVillageTaskTorch::IsTaskAvailable | landed |
| `unattributed_InvokeOnSelf006603a0_0066106d.cpp` | 0066106d | _unattributed::InvokeOnSelf_006603a0 | DIFFER |
| `unattributed_InvokeOnSelf006603dd_0066107f.cpp` | 0066107f | _unattributed::InvokeOnSelf_006603dd | DIFFER |
| `unattributed_InvokeOnSelf0066041a_00661091.cpp` | 00661091 | _unattributed::InvokeOnSelf_0066041a | DIFFER |
| `unattributed_InvokeOnSelf00660457_006610a3.cpp` | 006610a3 | _unattributed::InvokeOnSelf_00660457 | DIFFER |
| `unattributed_InvokeOnSelf00660494_006610b5.cpp` | 006610b5 | _unattributed::InvokeOnSelf_00660494 | DIFFER |
| `CThingFilterIsNot_operator_00661640.cpp` | 00661640 | CThingFilter_IsNot::operator() | landed |
| `CTCUserActionPointBase_IsActionPointFree_00661f20.cpp` | 00661f20 | CTCUserActionPointBase::IsActionPointFree | landed |
| `unattributed_GetButtonPressureForEvent_00687d10.cpp` | 00687d10 | _unattributed::GetButtonPressureForEvent | landed |
| `unattributed_ForwardToInterface00445920_00687d40.cpp` | 00687d40 | _unattributed::ForwardToInterface_00445920 | DIFFER |
| `unattributed_ForwardToMember0008645e0_006a7610.cpp` | 006a7610 | _unattributed::ForwardToMember0_008645e0 | landed |
| `CMessageEventFilterMessageIDGreaterThan_operator_006af280.cpp` | 006af280 | CMessageEventFilter_MessageIDGreaterThan::operator() | landed |
| `unattributed_CallWithPair006cb2d0_006ca8b0.cpp` | 006ca8b0 | _unattributed::CallWithPair_006cb2d0 | landed |
| `unattributed_GetScaledDefValue_007445d0.cpp` | 007445d0 | _unattributed::GetScaledDefValue | landed |
| `unattributed_ForwardWithMember60_0076e5d0.cpp` | 0076e5d0 | _unattributed::ForwardWithMember60 | DIFFER |
| `CAIStateGroupMinionBehaviourCrowdBase_Cleanup_00787bf1.cpp` | 00787bf1 | CAIStateGroup_MinionBehaviourCrowdBase::Cleanup | DIFFER |
| `unattributed_CallTwo00788555_00788b0a.cpp` | 00788b0a | _unattributed::CallTwo_00788555 | landed |
| `CTCBerserk_IncreaseBerserkDamageToEnemyMultiplier_0078bec0.cpp` | 0078bec0 | CTCBerserk::IncreaseBerserkDamageToEnemyMultiplier | landed |
| `unattributed_ForwardMember10ToSubobject_007f53a0.cpp` | 007f53a0 | _unattributed::ForwardMember10ToSubobject | landed |
| `unattributed_PerformActionMode2_008424a0.cpp` | 008424a0 | _unattributed::PerformAction_Mode2 | DIFFER |
| `unattributed_PerformActionMode10_008424d0.cpp` | 008424d0 | _unattributed::PerformAction_Mode10 | DIFFER |
| `unattributed_CallWithPair008ac9d0_008ad5c0.cpp` | 008ad5c0 | _unattributed::CallWithPair_008ac9d0 | landed |
| `CCombatSequenceGeneric_GetFramesToPredictTargetPosition_008b06e0.cpp` | 008b06e0 | CCombatSequenceGeneric::GetFramesToPredictTargetPosition | landed |
| `CAIStateGroupBirdFlyToFood_Initialise_008fd860.cpp` | 008fd860 | CAIStateGroup_BirdFlyToFood::Initialise | landed |
| `CAIStateGroupBirdGroundWander_Initialise_008fd892.cpp` | 008fd892 | CAIStateGroup_BirdGroundWander::Initialise | landed |
| `unattributed_CallWithSelf_00996970.cpp` | 00996970 | _unattributed::CallWithSelf | landed |
| `global_BasicStringRaise_0099c550.cpp` | 0099c550 | _global::BasicStringRaise | landed |
| `CDisplayManager_CopyBackBufferToTexture_009bf5e0.cpp` | 009bf5e0 | CDisplayManager::CopyBackBufferToTexture | landed |
| `unattributed_CallTwo009c8660_009c8bf0.cpp` | 009c8bf0 | _unattributed::CallTwo_009c8660 | landed |
| `unattributed_CallTwo009d6730_009d68b0.cpp` | 009d68b0 | _unattributed::CallTwo_009d6730 | landed |
| `unattributed_CopyC2DVector_009f3220.cpp` | 009f3220 | _unattributed::CopyC2DVector | landed |
| `CResourceBank_FrameEnd_009fc430.cpp` | 009fc430 | CResourceBank::FrameEnd | landed |
| `C3DMesh2_CanRelocateData_00a8ca00.cpp` | 00a8ca00 | C3DMesh2::CanRelocateData | landed |
| `unattributed_ClearFlagAndRestoreCaptureBlock_00b86bb0.cpp` | 00b86bb0 | _unattributed::ClearFlagAndRestoreCaptureBlock | landed |
| `unattributed_DiscardStructResult_00be4400.cpp` | 00be4400 | _unattributed::DiscardStructResult | landed |
| `CIsThingAlive_operator_00c0e950.cpp` | 00c0e950 | CIsThingAlive::operator() | landed |
| `unattributed_CLODMapOwner012B2FC8_00c1aea0.cpp` | 00c1aea0 | _unattributed::~CLODMapOwner_012B2FC8 | landed |
| `unattributed_FramesRemaining_00c1d7d0.cpp` | 00c1d7d0 | _unattributed::FramesRemaining | landed |
| `unattributed_GetPair_00c25790.cpp` | 00c25790 | _unattributed::GetPair | landed |
| `global_StdcallForward00c3e260_00c33470.cpp` | 00c33470 | _global::StdcallForward_00c3e260 | landed |
| `unattributed_SelfIfVirtual1_00c33cd0.cpp` | 00c33cd0 | _unattributed::SelfIfVirtual1 | landed |
| `global_StdcallForward00c3cde0_00c36090.cpp` | 00c36090 | _global::StdcallForward_00c3cde0 | landed |
| `global_StdcallForward00bfea14_00c3c390.cpp` | 00c3c390 | _global::StdcallForward_00bfea14 | landed |
| `unattributed_CallWithTableEntry_00c42b80.cpp` | 00c42b80 | _unattributed::CallWithTableEntry | DIFFER |
| `unattributed_ScaledQuery_00c46f40.cpp` | 00c46f40 | _unattributed::ScaledQuery | landed |
| `unattributed_CLODMapOwner012B80C8_00c49af0.cpp` | 00c49af0 | _unattributed::~CLODMapOwner_012B80C8 | landed |
| `unattributed_CLODMapOwner012B8364_00c4bb00.cpp` | 00c4bb00 | _unattributed::~CLODMapOwner_012B8364 | landed |
| `unattributed_CCriticalSectionOwner012B8FC8_00c5d570.cpp` | 00c5d570 | _unattributed::~CCriticalSectionOwner_012B8FC8 | landed |
| `C2DVectorI_ToC2DVectorF_00c620b0.cpp` | 00c620b0 | C2DVectorI::ToC2DVectorF | landed |
| `CWorldMap_GetNextRegionOnRouteTo_00c8a920.cpp` | 00c8a920 | CWorldMap::GetNextRegionOnRouteTo | landed |
| `global_DestroyFlash_00c8db20.cpp` | 00c8db20 | _global::DestroyFlash | DIFFER |
| `CWideScreenMagicPauseEntities_CWideScreenMagicPauseEntities_00cbe0b3.cpp` | 00cbe0b3 | CWideScreenMagicPauseEntities::~CWideScreenMagicPauseEntities | landed |
| `CSmallVector12_operator_00cd1fb1.cpp` | 00cd1fb1 | CSmallVector12::operator[] | landed |

## Provenance

- Targets: `next_smallest.py 80` over `rebuild/manifest/functions.tsv` (complete prototype, known
  calling convention, not landed, not in `gen_tried.txt`), bodies trimmed with `rowtrim.py`.
- Oracle: `oracle.tsv` (retail bytes from the shipped `Fable.exe`, 16,666,624 bytes).
- Generator: scratch `gen/batch5.py`; per-candidate grading harness: scratch `tools/gp.py`
  (`parity_of()` + lander sweep + capstone side-by-side), see the journal for the day.
