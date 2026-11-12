# 2026-11-12 landing eh -- author workflow wf1: nine of ten rows landed, one held back

Source: `pending/2026-11-12_linux_handWF1/` (workflow `lane_tools/workflows/author_rows.js`: one Opus scout, Fable authors, two
independent Opus verifiers and one Opus repair pass per row; 57 agents). All ten rows byte-matched. Landed (all RELOCATION_MATCH,
test PASS under VC7.1, LANDVERIFY_STRICT_HEADERS=1): 0x00A114B0 CPhysicsMeshObject::SetPositionMatrix, 0x007FF190
CTCNamedUserActionPoints::GetFreeNamedActionPoint, 0x00BEAB40 CWaterPatchMesh::GetWaterType, 0x00BFA920
CLocalDetailPrimitiveMesh::AddObjectsToPrimitiveRenderer (/d2inlT119), 0x009E3DF0 CPixelFormat::ConvertPFDDataToRGB, 0x007F5150
CTCSoundAtmosphereVillage::GetGain, 0x00863CA0 CThingBodyReorienter::FillBodyReorientationPossibleIntersecting, 0x00688D40
CInputProcessZTarget::AddToInterface, 0x006EA0C0 CCombatManager::UpdateDefenderAttackPattern (the manifest label
CDefendingCombatantInfo::InitializeDefenderFromCreature was wrong; catalog and manifest now carry the PDB name).

**Held back:** 0x004B4260 CQuestManager::ActivateMultipleQuests matches only with a body of CQuestManager::ActivateQuests 0x004B3CE0
visible, and that body is unproven (0x004B3CE0 is parked DIFFER). It waits in `held_back/004b4260/` and lands with 0x004B3CE0.
**Verification:** seven rows VERIFIED by both lenses; two had small open findings fixed by the coordinator at landing (00beab40: the
basis of the g_pLandscapeRenderer spelling; 00bfa920: `__declspec(novtable)` dropped, descriptive global declared) -- see
`EVIDENCE_VERDICTS.md`. Mechanical checks: reserved_check 0 (one comment mentions a reserved sibling), purity clean, no local
model of a header class in a row source, no stand-in bodies.
**Headers** (12, all additions; every changed opt-in is new and unused elsewhere, every default path unchanged per hdrcheck2):
C3DVector.h (CROSS_XYZ_GROUP; the existing CROSS_XY_GROUP `#ifdef` became its `#elif`), CMatrix3x4.h (SETIDENTITY),
CPhysicsMeshObject.h, CTCNamedUserActionPoints.h, CEngineLandscapeRenderer.h (PEEKTHEME), CEngineFadeDistance.h,
CEnginePrimitiveRenderer.h (ADDSTATICMESH), CPixelFormat.h (PFDDATATORGB), CAInputProcess.h / CInputProcessGameBase.h /
CInputProcessZTarget.h (ADDTOINTERFACE), CCombatManager.h (DEFENDERATTACKPATTERN).
**Sibling cleanup:** the landed 0x00BFABA0 and 0x00BFACC0 carried the same unexplained `__declspec(novtable)`; dropped and rechecked
(`logs/recheck_novtable.log`: both RELOCATION_MATCH, PASS).
Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011eh): eight name fixes from the rows (wrong caller signatures at 0x00968860 and 0x009FBBD0, GD3DFormats 0x0129BA40,
the three AddToInterface / GetPTargetedThing spellings, IsQuestRegistered in landed 0x004B4A10, the 0x009F0570 class candidate) and
the split spelling of the global 0x01436EA8.
