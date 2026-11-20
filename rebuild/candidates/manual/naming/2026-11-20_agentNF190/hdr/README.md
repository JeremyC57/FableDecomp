# NF190 header overlays

## engine/CHeroSoulsCreatureDef.h (copy of rebuild/include/engine/CHeroSoulsCreatureDef.h; base in orig/engine/)
- Adds two add-only opt-ins right after the virtual destructor:
  - `FABLE_RETAIL_CHEROSOULSCREATUREDEF_COPY`: `virtual void Copy(const CDefClassBase*);` (overrides CDefClassBase slot 19; no new slot).
  - `FABLE_RETAIL_CHEROSOULSCREATUREDEF_OPERATOR_ASSIGN`: `CHeroSoulsCreatureDef& operator=(const CHeroSoulsCreatureDef&);` (0x00F27190).
- Used by row 0x00F27180 only. Without the macros the class body is token-identical to the repo header.
- pin_asserts.py pinned 1 assert: `FABLE_STATIC_ASSERT(sizeof(CHeroSoulsCreatureDef) == 0x34)` -> `FABLE_STATIC_ASSERT_AT(18, ...)` (the
  repo line number, so the typedef name does not move).
- Landed includers (5): 0x00D7FE90, 0x00F27060, 0x00F26F70, 0x00F273F0, 0x00F27190, plus engine/CHeroSoulsWaveDef.h. None defines either
  macro, so the opt-ins change none of them; the pinned assert keeps its repo name.
- The other agents' overlays (NF186..NF189) have no copy of this header.

## engine/CV_BeardyBaldyScript.h (copy of rebuild/include/engine/CV_BeardyBaldyScript.h; base in orig/engine/)
- Two add-only declaration opt-ins right after the class head (no layout change):
  - `FABLE_RETAIL_CV_BEARDYBALDYSCRIPT_GOTALKTOBARBER`: `void GoTalkToBarber();` (0x00E50E40, row 0x00E50E40).
  - `FABLE_RETAIL_CV_BEARDYBALDYSCRIPT_WATCHFORQUESTFINISHED`: `void WatchForQuestFinished();` (0x00E50100, row 0x00E50100).
- Without the macros the class body is token-identical to the repo header.
- pin_asserts.py pinned 2 asserts (lines 79 / 80: sizeof CV_BeardyBaldyScript 0xA4, CBB_BeardyBaldyMan 0x24) to the repo line numbers.
- Landed includer: 0x00E50FD0 (CBB_BeardyBaldyMan::Main) only; it defines neither macro, so nothing changes for it.
