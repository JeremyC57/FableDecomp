# 2026-11-12 landing er -- author workflow wf9: ten of ten rows landed

Source: `pending/2026-11-12_linux_handWF9/` (workflow `lane_tools/workflows/author_rows.js`, 49 agents; launched before the
Opus-only rule, so its authors were Fable). All ten rows byte-matched. Landed (all RELOCATION_MATCH, test PASS under VC7.1,
LANDVERIFY_STRICT_HEADERS=1): 0x00867B40 CCombatAction_GenericStrikeResponseKnockdown::FinishAction, 0x00762BA0
CTCShapeManager::OnSerialise, 0x00A12AB0 CPhysicsMeshObject::PeekRoughProportionOfSphereContainedInVolume, 0x00726160
CApplyAdditionalPlayerImpulseToPlayerIfInsideUnpushableCreature::operator() (the manifest class name was wrong), 0x00A90900
N3DPrimitive::CDeviceIndependent::AddTriangleList, 0x008DC720 NCreatureMode::CModeDragonStrafeBurninate::StartBeamEffect, 0x006F3A30
std::_S_sort<const CThingCreatureBase*, allocator, CThingCompare_Nearest>, 0x0085D180 NThingMap::CThingMap::InitMapwhoLevels,
0x005BE8F3 CTCInventoryBase::CreateInventoryObject, 0x008C6860 CCreatureAction_TrollThrowProjectile::PerformAction.

**0x00867B40 and a coordinator error:** the workflow reported this row DIFFER(468v465) after three author passes. The source was
right from the first pass; the ORACLE was wrong. The author pool took the row's length from the parked grade string (`468v465`,
written against an earlier truncated oracle) instead of the corrected extent in PARKED_INDEX.tsv (`corr=00867b40:468`); 465 bytes
ends inside the last `add esp,128h`. The author proved it (exe bytes, coverage.tsv size 468) and left a ready payload. Because the
workflow only verifies MATCH rows, this row had skipped both lenses: the coordinator ran the evidence verifier on it
(`verify_handback_evidence.js`): name, callees, members and test hold; one fix applied -- the Die action's +0xA8 member is `Flip`
(Ego_r.pdb), not `FlipFacing`. All other pool and in-flight row lengths were checked against `corr=` and the executable: no other
row is affected (`prep_wf.py` now takes the corrected extent).
**Other coordinator fixes:** 0x008DC720's unexplained `__declspec(novtable)` on a row-local base dropped (regraded: match);
0x006F3A30's notes claimed two landed spellings wrong without NAME_FIXES rows -- both are queued as claims to verify.
**Inferred:** 0x00762BA0 writes the first IsSaving if / else with the same Transfer statement in both arms (retail keeps two call
set-ups); 0x00A12AB0 returns `(float)(callee(...) * 1.0)` (the conversion tuple that puts `test esi,esi` before the result store,
the same family as the `/ 1.0f` finding of landing eg).
**Headers** (17; additions under new opt-ins, all unused elsewhere, defaults unchanged per hdrcheck2): CTCShapeManager.h,
NShape_CClosedShapeBase.h, CTCSolidToThing.h, C3DMeshFileXTriangleStripChunk.h, C3DMeshFileTrianglesChunk.h, stlport/fable_containers.h
(LIST_SORT), CThingCompare_Nearest.h, NThingMap_CThingMap.h, CTCObjectAugmentations.h, CTCInventoryItem.h, CThingCreatureBase.h,
NThingShot_CTCShotDefinitionBase.h, CCreatureAction_TrollThrowProjectile.h, CThingShotDef.h, CCreatureActionBase.h,
CCombatAction_GenericStrikeResponseKnockdown.h, CCreatureAction_Die.h.
Checks: reserved_check 0; bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011er): the rows' name fixes plus the two claims.
