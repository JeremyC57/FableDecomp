# CEnginePrimitiveManagerParticleSpriteTrailGroup::RenderSingleSpriteTrail

Ego_r and FableWin agree on `?RenderSingleSpriteTrail@CEnginePrimitiveManagerParticleSpriteTrailGroup@@QAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z`. The retail call from RenderSubPrimGroups identifies the same non-virtual member. Ego_d preserves the same parameters and locals in the compact 683-byte debug procedure. The larger retail body expands the state setters and shader routines at their call sites.

The donor displacement-buffer arm invokes `SetupBlendOpState(ENGINE_PRIMITIVE_BLEND_OP_ADD)` and `SetupAlphaBlendingStatesFromBlendMode(ENGINE_PRIMITIVE_BLEND_ALPHA)`. Restoring these two invocations accounts for the remaining native difference. The selected source is 2,464 bytes with all relocation-masked bytes equal and the three mapped COMDAT copies equal. The target of the trail renderer call is the const-reference form named by both PDBs, at 0x00BCA950.

The standalone test compiles the same row body with deterministic renderer interfaces. It covers missing frames, displacement enabled and disabled, stencil blending preservation, the three ordinary blend paths, both lighting branches, shader index clamping, UVs, and the submission-and-restore sequence.
