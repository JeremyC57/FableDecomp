# Identity and validation

The full Leaf appears in both Ego_r (0058a5f0) and FableWin (01ef2920) name sets. Its const CHitParameters reference matches the map at+30 and the sole retail caller007ccd4c within CTCMultiStrike::StartMultiHit. Private combo-data, variation and update names are checked against the PDB sets; their neighboring retail bodies006d72d0/006d7120/006d7f20 establish each target. CTCHeroExperience and CHitParameters retain their proven class keys.

All nine relocations are decoded directly from the retail image and audited in history/006d8830/035_final_metadata_and_validation/RELOCATION_AUDIT.json: free bfea14, protected const CTCBase::GetWorld686a70, static fastcall CWorld::GetFrame49d870, malloc bfea0e, private const GetActiveComboMultiplierData6d72d0, CMainGameComponent::ConstantFPS1375550, CRT conversion bfea70, private const GetAttackVariationMultiplier6d7120 and private UpdateComboMultiplier(bool)6d7f20. The established ConstantFPS spelling replaces the earlier fake free global. GetWorld/GetFrame/ConstantFPS also match reloc_bindings and the correction queue. No new identity correction is needed.

The graded TU uses the additive shared engine-header overlay. CTCHeroExperience uses native fields+30/+38/+50; CComboMultiplierData has12-byte map then attenuation seconds+0c. The emitted full Leaf was checked with objdump -t.

The test copies the selected function body verbatim into a controlled fixture using standard map/list containers and counted callee stubs. It checks empty input, all64 signed-damage/multiplier subset cases, zero damage, each history entry/frame, attenuation reset, and update/variation calls. Values are dyadic and portable with64-bit long. Both native64 g++ -std=c++03 and real VC7.1 /Od /W3 pass. The fixture is for behavior only; native layouts are checked by the graded shared headers. Test logs are TEST_VC_BUILD.txt and TEST_VC_RUN.txt.

## Final match and scheduler proof

The selected321-byte function now exactly matches after grouping only the GetAttackVariationMultiplier call result. The actual031 C2 trace proves that its new0x269 tuple consumes integer cycle2; fabs uses the floating issue unit in the same cycle. Existing grouping tuples occupy integer cycles3–5, fmulp issues5 and push1 moves to6. Both the normal031 and final035 RTM objects match all119 instructions with the same9 audited external operands. The diagnostic trace object is independently checked identical to the normal unmodified compiler leaf. No compiler inline device or extra operation is involved.

The standalone test was regenerated from the exact final body, now with grouped call result, and audited for exact extracted-body equality. Both compilers pass. Main re-verification is the remaining integration step.
