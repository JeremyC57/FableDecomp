# NF114 header overlay

## engine/NUISystem_CUIScreenUnitConverter.h (copy of the repo header at session start; base in ../orig/engine/)
No new macro: two `#ifdef FABLE_RETAIL_CUISCREENUNITCONVERTER_ORIGINAL_DIMENSIONS` arms inside existing opt-ins, so the two inline
ConvertX / ConvertY bodies compile when ORIGINAL_DIMENSIONS (the private `@@0` statics, landed by nf112) replaces INLINE:
- inside FABLE_RETAIL_CUISCREENUNITCONVERTER_CONVERTXY_INLINE: `float t = x / DefaultBackBufferWidth;` (and the Height twin) under
  ORIGINAL_DIMENSIONS, the existing `x / ScreenWidth` text in the `#else` arm;
- inside FABLE_RETAIL_CUISCREENUNITCONVERTER_VECTOR_INLINE_CONVERTXY: the same one-line ConvertX / ConvertY bodies on
  DefaultBackBufferWidth / Height under ORIGINAL_DIMENSIONS, the existing ScreenWidth / ScreenHeight text in the `#else` arm.
  (The plain VECTOR_INLINE ConvertVector already had this ORIGINAL_DIMENSIONS arm.)
Default path token-identical: `cpp -P` of the header through orig/ and hdr/ gives the same output with no macro, with INLINE +
CONVERTXY_INLINE, with INLINE + VECTOR + VECTOR_INLINE + VECTOR_INLINE_CONVERTXY, and with ORIGINAL_DIMENSIONS alone.
pin_asserts.py: `pinned 0` (the header has no FABLE_STATIC_ASSERT).
Header users: 33 landed rows include it (`grep -rlE 'NUISystem_CUIScreenUnitConverter.h' rebuild/src/compiled`); none of them defines
ORIGINAL_DIMENSIONS together with CONVERTXY_INLINE or VECTOR_INLINE_CONVERTXY, so the overlay changes none of them. Used by rows/
0060eeb0 (VECTOR_INLINE), 00531ec0 and 0061e6d0 (VECTOR_INLINE_CONVERTXY), 00543ee0 (CONVERTXY_INLINE).
NF111 / NF113 overlays: neither has this header (read-only check).

## Not handed back
work/hdr_drafts/engine/CFilter_Bind3.h: FABLE_RETAIL_CFILTER_BIND3_CALL_FORCEINLINE (q4447's blocked self row only).
