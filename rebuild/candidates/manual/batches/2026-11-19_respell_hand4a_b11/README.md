# Respell hand4a b11 (2026-11-19): same-TU helper bodies made retail's in two landed rows

hand4a graded each same-TU helper body at its own retail address; two were not retail's. Recheck after the respell: 0x006E94D0, 0x00649E8B
(and 0x00537776, comment-only) RELOCATION_MATCH + PASS (`logs/recheck_good.txt`); diffs in this folder.
- 0x006E94D0: GetProngSeparationHalfAngle `prong_sep / 2.0` -> `/ 2.0f` (retail `fmul dword [0.5f]` at 0x006E848B). Other helpers already MATCH /
  RELOCATION_MATCH at 0x006E8520 / 0x006E8850 / 0x006E8810.
- 0x00649E8B: `#pragma optimize("y", off)` moved to just before Draw, so the same-TU UpdateMiniGame(const CRGBColour&) is frameless like retail
  0x00644F85 (now MATCH at its own address).
- 0x00537776: the forced C2DBoxI copy ctor line reworded `// Inferred:` -> `// Inline-budget:` (comment only).
