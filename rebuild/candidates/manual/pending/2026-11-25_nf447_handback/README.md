# NF447 hand-back (2026-11-25): GameCODA render slot rows, the PanDelay family (R-NF445-1 debt, part 1)

NF447's own summary of the batch is in `AGENT_README.md`; its decisions are in `NOTES.md`, and the per-row SDK body ties in `evidence/tie_table.tsv`.

## Landed: b1 (23 rows, one unit)
- **SalSw3DVoice.obj slice:** 0x00C55270 (owner), 0x00C55070 / 0x00C550D0 (SIMD ctors as option S thin rows), and the 8 EH funclets.
- **PanDelay rows:**
  - 0x00C6CA20 ctor, 0x00C6CAC0 dtor;
  - ClearFilterHistory 0x00C6CAD0 / 0x00C6D560 / 0x00C6F300;
  - Process 0x00C6CC60 / 0x00C6D630 / 0x00C6F3E0;
  - SetGain 0x00C6D120, SetPosition 0x00C6D150.
- **SalRenderPanDelay.obj slice:** Params3DInit 0x00C6CEF0, with UpdateFilters 0x00C6CF40 as its thin row.
- **Headers:**
  - new `CSalRenderPanDelay.h` (retail 6-channel layout, size 0x214 asserted), `CSalRenderPanDelaySSE2.h`, `CSalRenderPanDelayMMX.h`;
  - an add-only `FABLE_RETAIL_CSALMUTEX_ENTER_LEAVE` opt-in in CSalOutputKS.h.
- **Names:** each has two sources: the SDK 2.10 CodeView record plus a release-body tie (exact instruction count, `ret` size, or call order).
- **NF441 fixes carried:** 0x00C6CA20 → `??0CSalRenderPanDelay`, deleting `??0CEnvironmentSpellEffectInterface`.
- **The SIMD ctors 0x00C55070 / 0x00C550D0 no longer carry the wrong `??0C3DAnimationUtil@@QAE@XZ`** that NF441 identified. The tracker therefore moves them from agree_pdb to agree_unverified: the old "proven" was a wrong PDB name, and the SDK names are not in the PDB sets.
- **Gates:**
  - strict recheck on the landing simulation: 20/20 plus 3/3 option S members;
  - mutants 20/20;
  - pk 0 NEQ; reachflip 0;
  - vt432: HEAD 0 agree / 20 WRONG → new 32 agree / 8 WRONG (all 8 are the slot-0 `??_G` stand-ins);
  - /FORCE unresolved 54 → 45, duplicates 3 → 2.
- **Coordinator checks:** 0 HEAD-payload mismatches; baselines unchanged (funclets 1,698, s units 157); descriptive_check 0 FAIL.

## Held, in landing order (each re-ports 0x00C55270 again on b1's text)
1. **Disable** (~18 rows), with the NF441 fixes 0x00C550B0 / 0x00C55110.
2. **Pan** (~18 rows), with 0x00C55090 / 0x00C550F0.
3. **Hrtf** (~23 rows). HrtfSSE2 is a style name and needs a BEST_GUESS line. 0x00C701E0 has no size tie.
4. **Slot-0 stand-ins:** `??_G` 0x00C6D350 / 0x00C554C0 / 0x00C55550 and dtors 0x00C554E0 / 0x00C55570. b1's headers already compile them EQ at home.
5. **Tap mixers** 0x00C6CAF0 / 0x00C6CB80 / 0x00C6CC10 (SDK private statics MixFadeIn / MixFadeOut / Mix).
6. **CAudioMaster:** a local model in five rows; port them onto CSalPlatform.h `g_pSal` (check `m_nSWOutputChannels` at +0x1BC).
- **NAME_FIXES.tsv:** IAT slots 0x0143FE0C / 0x0143FE10 are `__imp__EnterCriticalSection@4` / `__imp__LeaveCriticalSection@4`, currently landed as `?g_clearFn0/1` by 0x009F26B0.

- **equiv_check (four headers):** 57,160 rows checked; 12 differ, all b1 rows. 0 differ outside the batch.
