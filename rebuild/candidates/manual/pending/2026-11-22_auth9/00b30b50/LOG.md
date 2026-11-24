# 0x00B30B50 CEngineCamera::SetCameraState (AUTH9, 2026-11-22). VC7.1 3077, q.py / rg1.sh, no flags.
- g0: repo 0x00B31160 row: ProcessDebugScript RELOCATION_MATCH; its SetCameraState copy at 0x00B30B50 RELOCATION_MATCH (1499 B).
- A: the TU text up to ProcessDebugScript (SetCameraState alone): RELOCATION_MATCH.
- B: 0x00B31160 with SetCameraState declaration-only: DIFFER(886v886); one swapped pair at 0x00B313A4 (`mov byte ptr [esp+0ACh],0` / `fld [edi+18h]`).
  => 0x00B31160 keeps its copy as a TU-mate.
- C: A minus ProcessDebugScript-only opt-ins / includes / constants (CARRAY_INDEX_ASSERT, GFGETINTERPOLATEDVALUE_PRODUCT_PASS26, CDISPLAYMANAGER,
  SCOPED_PTR, PEEKBACKBUFFERRESOLUTION, CDisplayManager.h, GFGetTime.h, GFGetInterpolatedValue.h, GPDisplayManager, the clip-plane consts):
  RELOCATION_MATCH. leafrel 0 problems; pick-any (no function COMDAT mapped elsewhere) empty.
- Final CEngineCamera_SetCameraState_00b30b50.cpp (C + header): rg1 RELOCATION_MATCH. Test PASS first run.
- 0x00B31160 P1 (comment only): RELOCATION_MATCH; its SetCameraState copy RELOCATION_MATCH; pick-any 1 EQ; repo test PASS.
