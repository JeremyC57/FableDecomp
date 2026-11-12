# 2026-11-12 landing ek -- author workflow wf3: ten of ten rows landed

Source: `pending/2026-11-12_linux_handWF3/` (workflow `lane_tools/workflows/author_rows.js`, 42 agents). All ten rows byte-matched
and were VERIFIED by both independent lenses. Landed (all RELOCATION_MATCH, test PASS under VC7.1, LANDVERIFY_STRICT_HEADERS=1):
0x00931200 CAIStateGroup_MinionReactToCriminal::IsStillValid, 0x009DC6E0 CRenderManager2D::Draw2DTexture, 0x00761980
CTCShapeManager::DrawShapes (three author passes, 273 grades), 0x00A89740 C3DMesh2::GetHelperPointPositionAnimated, 0x009122A0
CBezierCurve::GetPointOnCurve, 0x00BE7BE0 CLandscapeBackgroundPatch::Render (manifest said _global::Render), 0x0081F240 CMap::CopyPos,
0x00646580 NPlayerGui::CDrawMeleeActionButtonEffects::Draw, 0x0052C920 NUISystem::CChangingStateComponent::ProcessChangeState,
0x00848B70 CCreatureAction_PickUpGenericBox::PerformAction.

**Inferred statements** (marked in the source headers; behaviour identical to retail): 0x00761980 `CThing& parent = GetParentThing();`
placed before the lifted vector (three spellings of the named parent match; the bytes prove only that the parent was read first);
0x009122A0 the inline square helper is taken to be GFSqr (the only inline float square in the PDBs); 0x00646580 the `C2DVector(position)`
copy as in the landed DrawButton 0x00645099.
**Mechanical checks:** reserved_check 0 (0x00A89740 declares the reserved callee CalcBoneTransformation 0x00AA0FA0 by its PDB name;
no body), purity clean (0x00646580 uses the HUD TU's `#pragma optimize("y", off)` / `("s", on)` and carries the landed
GFAnchorToBottomRight 0x00643D0C real body out of line), no local model of a header class in a row source, no stand-in bodies.
**Headers** (16; additions under new opt-ins, all unused elsewhere): CAIStateGroup_MinionReactToCriminal.h, CTCShapeManager.h,
NShape_CClosedShapeBase.h, CEnginePrimitiveLine.h, C3DMesh2.h (HELPERPOINTS), C3DVector.h (GETSCALED), CMatrix3x4.h
(TRANSFORMPOINT_ZYX_SUMS), CBezierCurve.h, CEngineWaterRenderer.h, CLandscapeBackgroundPatch.h, CEngineWaterBackgroundSubPatch.h,
CHeightMapCell.h, CMap.h, CPlayerGui.h, NPlayerGui_CDrawBase.h, CPlayerGuiDef.h.
**Default-path text change:** C3DMesh2.h -- the `UnmodelledBase` member moved onto its own line under `#ifndef`; the preprocessed
default differs only in white space and the line-numbered static-assert typedef names. The six other landed rows that include
C3DMesh2.h were rechecked in this landing (`logs/recheck_mesh2.log`: all RELOCATION_MATCH, PASS).
Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011ek): twelve name fixes from the rows (see NAME_FIXES.tsv).
