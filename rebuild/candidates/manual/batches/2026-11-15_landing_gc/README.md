# 2026-11-15 landing gc -- the two Codex pass 22 water rows (agentP6): 4,157 bytes

0x00B6FA40 CEngineWaterRenderer::RenderSeaMidground (2019) and 0x00B70230 CEngineWaterRenderer::RenderSeaBackground (2138), both
`/d2inlT128`, RELOCATION_MATCH with behaviour tests PASS under strict headers (`logs/vland_land.log`). Codex parked them 3 bytes
off (`pending/2026-11-14_codex_pass22/return/parked/`); agentP6 closed them.

## Lever
The last residue was `test esi,eax / or eax,esi` operand order in one SetRenderState bookmark block: a c2 record-id tie set by the
function's total IL record count (windows: midground 2..9 extra records, background 4..16). The original helper inlined into
both rows, Set{SeaMidground,SeaBackground}TexturesAndUVModes, has two release ASSERTs in the FableWin donor (SeaBumpMap,
EnvironmentMap; donor lines 439/453 and 483/497). Spelled as dead `FABLE_RETAIL_ASSERT` arms (fable_assert.h, no code), they
supply exactly those records. The tests carry the same helper text.

## Headers (`HEADERS.md`)
Codex graded its six parked rows against private header profiles in its package that were never merged into the repo, so their
sources did not compile under rebuild/include. agentP6 rebuilt them from the package (diff3 onto the current headers) for five of
the six rows: 36 changed headers and 2 new (engine/Direct3DTexture9.h, engine/EMatrix34Init.h), every change behind a
FABLE_RETAIL_* opt-in. 0x00BF0B20, 0x00F3E120 and 0x0064BFBA now compile and reproduce Codex's grade, ready for the next attempt.
Gates (coordinator): every header balances; pincheck 0 mismatches; no new bare `__forceinline`; whole-tree `equiv_check.py`
over 56,367 landed rows: identical input except 0x00CBFB7D, whose only difference is the scratch tree's path in `__FILE__`.

## Names
Queued `land1115gc`: 0x01436E9C `NEngineInternal::GPLightingManager` and 0x01436EA0 `NEngineInternal::GPCamera` (PDB names;
the queue's older c722 entries proposed descriptive `g_pEngine*` names). 0x01396DC8 `CRenderStateManager::D3DTextureUVModeTable`
was already queued (cx13).

## Still open (agentP6 continues)
0x00BF0B20 (3 lines: B-channel load scheduled after the G store), 0x00F3E120 (2 lines: prologue store order), 0x0064BFBA
(2217 v 2223: height operand order), 0x00675E20 (its header profile still to merge). `LESSONS.md` has the measurements.
