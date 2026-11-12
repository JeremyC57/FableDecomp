# Hand chunk 3021 (C3DAnimationManager): 1 of 1 byte-exact

## What matched (1)
- 00a9eb90 C3DAnimationManager::BlendAnimations RELOCATION_MATCH (1337 bytes; vland RELOCATION_MATCH + behaviour PASS under g++ and VC7.1).
  Starting point: the parked differ source of chunks 2001 / 3001 / 3005 (one SIB order left at 0x00A9EC5A). What closed it was the TEXT
  of the two table helpers, nothing in the row body:
  1. GFFastSin with the chunk-2044 text (`i -= 256; i &= 0x3ff; float a = g_CosTable[i]; float b = g_CosTable[i + 1];
     return (1.0f - frac) * a + frac * b;`, PROVEN-by-size there against FableWin ?GFFastSin@@YIMM@Z and its sibling GFFastCos):
     the SIB at 0x00A9EC5A became retail's `[eax+edx]` at the first grade, but the SSE-loop dot products 0x00A9EC40..0x00A9EC55 then
     loaded `from` first (`fld [esi-4] ; fmul [edx-0Ch]`, retail `fld [edx-0Ch] ; fmul [esi-4]`, X and Z terms).
  2. A dead-local ruler (diagnostic only) measured that residue as +6 / +7 named symbols short (mod 8), the same anywhere in the row
     (top, tail, SSE arm) or inside CalculateSlerpWeights / SetSlerp.
  3. GFFastArcCos written in the same style as GFFastSin (`float a = g_ArcCosTable[i]; float b = g_ArcCosTable[i + 1];
     return ((1.0f - frac) * a + frac * b) * 0.15915494f;`) closed it: RELOCATION_MATCH, still matching with +1..+3 diagnostic dead locals
     (not on an edge; +8 breaks it, as the mod-8 ruler predicts).
- FITTED v PROVEN: GFFastSin text PROVEN-by-size (chunk 2044). GFFastArcCos named text FITTED (FableWin's debug copy carries asserts, 119i/443b,
  so no size evidence); it keeps every LANDED user byte-identical (regraded with this text: GFFastArcCos 0x00A22530, CalculateSlerpWeights
  0x00AA3390, SetSlerpFromIdentity 0x00AA3560, SetSlerp 0x00A22590, BlendWith 0x00AA3C50, CParticle::GetInterpolatedOrientation 0x00B05620,
  pending 0x00AA0090: all RELOCATION_MATCH), but the pending twin 0x00A9F540 (agent2044, not landed) then DIFFERs 2288v2288 in the 4-term
  Dot order of its expanded BlendWith (retail 0x00A9FA56 `fld [esp+0A0h] ; fmul [esi-14h]` ... 0x00A9FA80 `fld [esp+9Ch] ; fmul [esi-18h]`;
  ours puts the X term first). Variants measured over the three rows (probe/ac/results.txt): only `a` or only `b` named: twin OK, this row
  and 0x00AA0090 DIFFER; named `angle` result / `j = i + 1` / `one_minus_frac`: twins OK, this row DIFFER; a+b (+/- named angle or result,
  either order): this row + 0x00AA0090 + 0x00A22530 OK, twin 0x00A9F540 DIFFER. The twin's residue is an add-chain linearizer order (not
  the outer-name key: k = 1..7 dead names did not move it; dropping its named index / time / relative or swapping the Dot did not either).
  So the helper texts of this TU are not yet one consistent set: either the original ArcCos is the plain text and this row needs ~6 natural
  named symbols elsewhere, or 0x00A9F540's fitted spellings need revisiting under the named ArcCos.
- Dot, CalculateSlerpWeights and SetSlerp (with `*this = q`) are unchanged from the prior chunks. Measured: Dot / CalculateSlerpWeights
  operand order of the dot is byte-neutral (c2 normalises it); chunk 2044's direct-store SetSlerp + `CQuaternion rotation; ...; from[i].Rotation
  = rotation;` loses retail's 2 alignment bytes before the scalar loop (1335v1337) with or without the named ArcCos; the
  GFGetCycleCount() function form instead of the macro also gives 1335; named loop references (from_rotation / to_rotation / keyframe)
  swap which pointer becomes the induction variable.

## Parked for Fable 5.1 (0)
None.

## Skipped
None.

## Names
- Row: ?BlendAnimations@C3DAnimationManager@@AAEXPAVC3DKeyframe@@PBV2@KM@Z (Ego_r.pdb; four landed callers bind it; chunk 2001). Callees and
  globals as in the prior chunks (LABELS.tsv). Descriptive names (unchanged): g_BlendAnimationsCycles 0x013D2D88, g_BlendAnimationsKeyframes
  0x013D2D90 (no PDB data symbol on this machine). Local helper names `a`, `b` (table values) follow chunk 2044's GFFastSin.

## Label corrections
None.

## Header debt
None new: hdr/ is the chunk 3005 overlay unchanged (hdr/README.md). GFFastSin / GFFastArcCos / CalculateSlerpWeights / SetSlerp remain
row-local copies in this row, the landed slerp rows and chunk 2044's rows; a single shared header for them is still owed.
