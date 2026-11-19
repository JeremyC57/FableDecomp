# b30 (hand3c chunk 1, Account B, 2026-11-19): 0x00B51AD0 CEngineWeatherRenderer::BuildBoxPositionMatrix -- READY

- Grade: RELOCATION_MATCH 969v969. Strict vland `RELOCATION_MATCH PASS` (LANDVERIFY_STRICT_HEADERS=1, overlay = hdr/ of this batch);
  one RELOC_NAME warning (0x01436EA0 catalog label g_pEngineCamera; the row uses the PDB NEngineInternal::GPCamera, NAME_FIXES.tsv).
  2 grades in ATTEMPTS (baseline draft 955v969, then this). Flags: none. Leaf ?BuildBoxPositionMatrix@CEngineWeatherRenderer@@AAEXXZ.

## Identity and shape
Ego_r.pdb private BuildBoxPositionMatrix (Ego_r 0x009F2D10, object order 14) and its private inline helper GetBoxCoord (?GetBoxCoord@
CEngineWeatherRenderer@@AAEXAAJJ@Z, Ego_r 0x009F2330, order 5; no call in retail). Ego_d.pdb names, types and scopes for every local
(cam_pos, half_side_length; c_x; c_y; this_mul_x, this_mul_y, index; GetBoxCoord: coord, box_span, distance_to_nearest_box_centre,
half_box_span). Members from Ego_r.pdb CEngineWeatherSettings (ParticleBoxZLocked, ParticleBoxSpan, ParticleBoxCenter, ParticleBoxPos[81])
through the shared CEngineWeatherRenderer.h Settings member; camera through NEngineInternal::GPCamera->PeekCameraPos() (as the landed
caller UpdateBoxPositions). The parked drafts' local class model is gone.

## What closed it (measured)
The snap part matched at once on the shared headers. The 9 x 9 fill: retail keeps a 9-trip down-counter in edx (0x00B51CD3 /
0x00B51E58), this_mul_x's integer in [ebp-4] from -4 (0x00B51CC6) and the pointer IV at +0x20C; every symmetric loop
(`c_x = -half .. half`, `<`, `!=`, index / this_mul orders, reference conversion helper) keeps c_x in edx with `cmp edx,4 ; jle` (965v969,
the parked drafts' residue). A mini model (probes/m0.cpp, v2.cpp, v2e.cpp) showed the down-counter appears exactly when the loop
counter is not the value the body converts: loops from 0 to side length with `this_mul = c - half_side_length` and
`index = c_x * side + c_y` give retail's `mov edx,9` and `+0x20C` in the mini and RELOCATION_MATCH on the row, with Ego_d's locals only.

## Overlay headers (copies of TODAY's repo headers + new opt-ins; diffs alongside)
- CEngineWeatherRenderer.h: FABLE_RETAIL_CENGINEWEATHERRENDERER_BOXMATRIX declares the private GetBoxCoord / BuildBoxPositionMatrix
  (inside the BUFFERS private block). No layout change; asserts already pinned (`_AT`).
- CEngineWeatherSettings.h: FABLE_RETAIL_CENGINEWEATHERSETTINGS_BOXCENTER types ParticleBoxCenter as C3DCoordI (same 12 bytes) and
  includes the new header; the two asserts pinned with pin_asserts.py.
- C3DCoordI.h: NEW shared header, Ego_r.pdb C3DCoordI = C2DCoordI base (X, Y) + long Z. The landed C3DCoordI rows (0x00A13BE0,
  0x00A13C20) keep their local models; the coordinator may re-point them later.
GetBoxCoord is defined `inline` in the row TU, ahead of the row (Ego_r compiles it fifth in the object, long before the row).

## Inferred / fitted
- FITTED: none. The loop spelling is chosen by the bytes among Ego_d-consistent forms (only c_x / c_y / this_mul_* / index exist).
- Retail-asm: the six GFFloatToLongFloor expansions through the shared engine/GFFloatToLongFloor.h (as in UpdateBoxPositions).

## Test (test/00b51ad0.cpp)
Links the real row object (GPCamera is its only external, defined in the test). Native-layout camera / renderer images (offsets cited);
reference model: floor per axis, C truncating snap with ties up, Z only when not locked, 81 boxes each one double expression rounded once
to float; nothing else written. 11 targeted (ties, exact integers, negatives, floored spans, span 1/2/3, odd span, Z-locked) + 3000 random.
Prints `00b51ad0_TEST PASS`. Mutation check: 18 one-decision mutants, 18 KILLED.

## NAME_FIXES
0x01436EA0 ?GPCamera@NEngineInternal@@3PAVCEngineCamera@@A (catalog g_pEngineCamera). 0x00B51AD0 AAE spelling (already filed by hand747).

## For the coordinator
- 0x00B53F60 FrameStart (same TU) needs UpdateBoxPositions and this body defined ahead of it; next in this chunk.
