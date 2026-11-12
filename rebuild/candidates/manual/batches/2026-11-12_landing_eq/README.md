# 2026-11-12 landing eq -- author workflow wf8: ten of ten rows landed

Source: `pending/2026-11-12_linux_handWF8/` (workflow `lane_tools/workflows/author_rows.js`, 55 agents; launched before the
Opus-only rule, so its authors were Fable). All ten rows byte-matched; eight VERIFIED by both lenses, two with open findings fixed
by the coordinator at landing. Landed (all RELOCATION_MATCH, test PASS under VC7.1, LANDVERIFY_STRICT_HEADERS=1): 0x0091B3A0
CAIStateGroup_SeekPerceivedThing::RunToPerceivedThingPosition, 0x00B81F30 CEngineClothSimulationStaticMesh::RunSimulation, 0x008C61C0
CCreatureAction_TrollWhackGroundBase::WhackGround, 0x006D0630 CTCWife::ReceivedNiceGiftIsOpinionReactionHappening, 0x0083D990
CThingPhysicalSwitch::ConstructFromLevelScript, 0x0072EB40 CTCActionUseBuyHouse::ReceivedConfirmation, 0x00BA4A40
CEnginePrimitiveManagerStaticMeshesBase::SetupStaticBlocksBlankTextures, 0x007BBE34
NThingShot::CTCShotDefinitionSpreadShotProjectile::InitMovement, 0x00A128E0 CPhysicsMeshObject::IsInContainmentVolume, 0x00B903D0
CEnginePrimitiveManagerOcclusionBase::RenderUntexturedStaticMeshPrimitive.

**Coordinator fixes at landing (both regraded: RELOCATION_MATCH):**
- 0x00B81F30: the row-local base model padded +0x18..+0xA8 with a made-up `_transforms[0x90]`; Ego_r.pdb names those bytes as
  three CMatrix3x4 members (CurrentTransform, LastTransform, InverseTransform). The pad is now those members (shared CMatrix3x4.h).
- 0x006D0630: the stlport/fable_map.h overlay (a noinline `find<KT>` opt-in) was not needed -- the row grades the same with the
  repo header -- so the overlay and its define are dropped and the comment corrected; the Identity comment now names both callers
  with their PDB spellings (CCreatureAction_GiveItemToThing::PerformAction 0x00844BC0, CCreatureAction_GiveMoneyToThing::PerformAction
  0x00845040).
**Mechanical checks:** reserved_check 0 (two comments cite landed precedent rows that are reserved by others), purity clean (the
shot TU's `optimize("y", off)` / `("s", on)`; FABLE_FORCEINLINE with an `// Inline-budget:` line in 0x00BA4A40), no local model of a
header class in a row source, real callee bodies only. One made-up enumerator, declared: HOUSE_QUESTION_DRESS (= 3) in 0x0072EB40.
**Headers** (22 + 1 new; additions under new opt-ins, all unused elsewhere; C3DTextureStages.h's one-line class was split to hold
an `#ifdef` block -- token-identical default): CTCPerceiveThing.h, CAIStateGroup_SeekPerceivedThing.h, CThingPhysical.h,
C3DPrimitive2.h (CLOTHPRIMITIVES, RECORDEDSTATE), C3DMeshInfo.h, CParticleProgram.h, CCombatActionBase.h,
CCreatureAction_TrollWhackGroundBase.h, CTCWife.h, CThingPhysicalSwitch.h, CTCHeroStats.h, CTCBuyableHouse.h, C3DTextureStages.h,
CGraphicInfo.h, CGraphicInfoFrame.h, CEnginePrimitiveManagerStaticMeshesBase.h, CPhysicsMeshObject.h, CPhysicsMesh.h,
CEnginePrimitiveManagerOcclusionBase.h, CShaderRenderManager.h; new NThingShot_CTCShotDefinitionSpreadShotProjectile.h.
Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011eq): five name fixes from the rows (see NAME_FIXES.tsv).
