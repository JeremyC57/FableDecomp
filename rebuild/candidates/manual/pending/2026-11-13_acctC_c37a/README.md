# acctC hand-back c37a (agentC37, 2026-11-14)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x0075D580 | CTCCameraPointScriptedSpline::UpdateCameraParams | 809 | RELOCATION_MATCH | PASS |
| 0x0077B350 | NGraphicAppearance::CTCGraphicAppearanceAnimatedMesh::OnAppearanceDraw | 767 | RELOCATION_MATCH | PASS |

Verified by the coordinator with `lane/coordverify.sh` (read-only vland, strict headers): `coordverify.log`.
Plain C++, no flag or pragma device. 0x0075D580 carries the landed text of GetKeyCamera (0x0075CFE0) above the row (needed for
retail's register use; it grades RELOCATION_MATCH at its own address from this TU); `GetLength() != 0` v `> 0` is byte-equal
(a choice). 0x0077B350 is ported from the landed sibling 0x00727D00.
Name fixes (NAME_FIXES.tsv): 0x0075C9B0 CKeyCamera::GetEvent; 0x0075C420 / 0x0075C280 protected
CTCCameraPointDefinitionBase::TransformPosition / TransformVector; 0x00A567A0 GetSplinePosition signature; 0x00708A30
~CEnginePrimitiveAnimatingMesh; 0x005BC0D3 ~vector<CEngineTextureMorph>; 0x007EA580 is CTCTargeted::GetAllTargetPoints.

`open/` (best drafts of parked / continuing rows):
- 007ea580.cpp GetAllTargetPoints DIFFER(457v457), 2 displacement hunks (successor agentC40)
- 007e15c0.cpp OnAppearanceDraw (MultipleStaticMeshes) DIFFER(967v963) + `_alt_963v963` (equal length, 15 structural hunks)
- 007bfe30.cpp UpdateCollision DIFFER(1643v1648), register permutation from the prologue
- 009fde80.cpp BeginLoading DIFFER(1269v1265), unchanged
