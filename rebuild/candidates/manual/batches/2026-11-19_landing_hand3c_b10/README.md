# Landing hand3c b10 (2026-11-19): 2 new rows + 2 re-ports

vland --land, strict headers: RELOCATION_MATCH + PASS for the new rows; 148 landed users of the changed headers rechecked (the two re-ports were
rechecked separately after their catalog PassPattern was updated to the new tests: `logs/recheck_relanded.txt`).
- 0x00B93B30 CEngineSceneMap::AddPrimitive (new): FableWin 0x02E1C860 statement for statement; first grade (the parked "3-byte temporary" is
  ConvertToLevel's return copied back). Opt-ins: CEngineSceneGridPos.h INLINE, CEngineInternalPrimitiveBase.h SETGRIDPOS, CEngineSceneMap.h ADDPRIMITIVE.
- 0x0064D404 NPlayerGui::CDrawQuestInfo::DrawMiniGameInfo (new): plain C++ (the "fistp lane gap" park was wrong); FableWin 0x020E86BF on the shared
  header + DRAWMINIGAMEINFO declarations. The x87 height stays in st(0) across GetElementByIndex only when that same-TU body is an ORDINARY definition
  earlier in the TU under `#pragma auto_inline(off)` (an in-class __declspec(noinline) COMDAT does not give c2 the FP / register summary; GOTCHAS);
  that helper body grades MATCH at its own address 0x00644FC5 with `optimize("s", on)` alone. Name fix: 0x00437870 is ?IsMiniMapDrawing@CPlayerGui@@QBE_NXZ.
- Re-ports: 0x00829C70 GetRelativeMapSize and 0x00829CE0 GetRelativePosOnMap now emit the merged header bodies (RELATIVEMAPSIZE_BODY / GETDRAWPOSITION)
  at their own addresses (one text per member); `payload_reland.json`.
Queued: re-port landed 0x00644FC5 (local model, non-const QAE spelling) to the const ?GetElementByIndex@..QBE.. text.
