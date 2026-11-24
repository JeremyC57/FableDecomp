# 00f1eed0 overlay (B24B)
Built from CURRENT repo headers by $W/apply_patches.py:
- engine/FableScriptInlineArenaPass25.h: created from Codex pass-25 PATCHES_OPEN.json (opt-in FABLE_RETAIL_SCRIPT_INLINE_ARENA_PASS25,
  FABLE_RETAIL_ARENA_ORIGINAL_GETTER_CHAIN_PASS25) -- evidence in that file's comments (Ego_r/FableWin member names).
- engine/CTimer.h: PATCHES_OPEN CTimer addition (FABLE_RETAIL_CTIMER_ESCORT_EQUAL_SECONDS_PASS25; not used by this row, harmless).
- engine/CRGBColour.h: second CRGBColour patch of PATCHES_HELD.json (FABLE_RETAIL_CRGBCOLOUR_FORCEINLINE_CTOR3_PASS25 in the
  FABLE_RETAIL_CRGBCOLOUR branch); the first held CRGBColour patch did not apply (old text gone) and is not used by this row.
- B24B change: in FableScriptInlineArenaPass25.h, CScriptThing::GetPos and CTimer::GetTime are plain `inline` (Codex had
  FABLE_FORCEINLINE); byte-neutral on 00f1eed0 (6 hunks both ways), so the device is dropped. CTimer::operator= keeps
  FABLE_FORCEINLINE (plain inline -> 138 hunks).
