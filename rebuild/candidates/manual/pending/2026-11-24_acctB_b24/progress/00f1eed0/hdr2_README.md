# 00f1eed0 overlay hdr2 (B24B2) -- copied from the CURRENT repo header, one additive opt-in
- engine/CRGBColour.h: `FABLE_RETAIL_CRGBCOLOUR_CTOR3_FORCEINLINE` (inside the FABLE_RETAIL_CRGBCOLOUR branch) forces the
  three-channel ctor ??0CRGBColour@@QAE@EEE@Z (FableWin.pdb; Codex pass 13 rows 0x00EEDC20 / 0x00E8BEF0 cite it). Evidence: retail
  PlayWave stores R,G,B,A=0xFF in place at the final FadeScreenOut (cold); plain inline 11 hunks, 4-arg sibling spelling 9266.
  Default branch unchanged. (Replaces Codex's FABLE_RETAIL_CRGBCOLOUR_FORCEINLINE_CTOR3_PASS25; same text.)
- No other overlay: the old hdr/ FableScriptInlineArenaPass25.h bodies are in the row (landed-sibling pattern), its CTimer.h
  addition was unused.
