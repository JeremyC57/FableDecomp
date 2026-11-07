# Landing 2026-11-07j: 0x00B5AF80 RenderColorFilteredQuad (owner ruling)

Owner ruling 2026-11-07 ("yes" to: should the "behaves the same and byte-matches" ruling cover a behaviour-identical `* 1.0f`?).
The expert chunk 832 parked source (`pending/2026-11-07_linux_hand832/differ/00b5af80.cpp`, clamp mix that fixes the bookmark test/or flip)
with `tint * (scaledContrast * 1.0f) * 0.5f` (its probe x6) grades RELOCATION_MATCH + PASS and landed.

- Lines 2-3 record the shape and the ruling; a `// Retail-asm:` evidence line cites 0x00B5B26B `fistp dword ptr [esp+58h]` for the
  shared GFFloatToLongNear helper (the purity gate grades the row `retail_asm`, as every landed caller of that helper).
- First attempt failed the purity gate (NOT_GENUINE) for want of that evidence line; no new bindings.
