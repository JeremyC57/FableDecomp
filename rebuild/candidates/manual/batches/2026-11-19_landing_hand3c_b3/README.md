# Landing hand3c b3 (2026-11-19): C3DAnimationManager::ApplyBoneMorphScaling (0x00A9D750, 551 bytes)

vland --land, strict headers: RELOCATION_MATCH + PASS (`logs/vland_land.log`); g++ FABLE_PORTABLE test PASS. retail_asm through the shared
GF_SSE_MATRIX_PRESCALE(_TRANSPOSED) helpers (`// Retail-asm:` 0x00A9D7C1 / 0x00A9D8C5); no asm in the row.

- New opt-in in engine/CMatrix4x4AlignedSSE.h: FABLE_RETAIL_CMATRIX4X4_ALIGNEDPRESCALE_BODIES (shared AlignedPreScale / AlignedPreScaleTransposed
  bodies: Ego_d locals vect / dest / right / m, FableWin accessor form) with nested _FORCEINLINE for the expanding TU. Default path unchanged.
  All 12 landed rows that use FABLE_RETAIL_CMATRIX4X4 re-verified after the merge (`logs/recheck_cmatrix4x4_users_good.txt`, 12 / 12).
- What closed the last x87 operand swap (E13 at 0x00A9D927): both c_bone loops written `if (false) {} else for (...)`, the VC6 for-scope
  idiom (`#define for if (0) {} else for`): FableWin 0x03382ED0 / 0x030381B0 open every for loop with `xor reg,reg ; je`; landed precedent
  0x007C4430. It emits no bytes but shifts c2 record ids. GOTCHAS line added.

Queued (coordinator decision): one text per member -- re-point the landed 0x00A9D480 (AlignedPreScale) and 0x00A9D580 (AlignedPreScaleTransposed)
rows onto this shared body (both grade RELOCATION_MATCH with it out of line, hand3c probes) under per-member out-of-line opt-ins; hand3c.
