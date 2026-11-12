# Chunk 3021 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x00A9EB90) A SIB base/index order that three agents' 1500 probes and c2 hooks could not move by row spellings closed with the TEXT of the
  table helpers it expands: GFFastSin with named table values (`float a = table[i]; float b = table[i + 1];`, chunk 2044) fixed the SIB at
  the first grade, and the same naming in GFFastArcCos fixed the remaining dot-product operand order. Try the sibling rows' proven helper
  texts first, then the same style on the sibling helper.
- (0x00A9EB90) The dead-local ruler (k = 1..8 `long deadN = 0;`) read the same "+6 / +7 mod 8" anywhere in the row (top, tail, SSE arm) and
  inside the forced-inline CalculateSlerpWeights / SetSlerp bodies; a named-table-value text in an inline expanded several times supplied it.
- Check a helper text against EVERY row that expands it, not only the landed ones: the named GFFastArcCos keeps all landed users and pending
  0x00AA0090 byte-identical but flips the BlendWith Dot term order in pending 0x00A9F540 (an add-chain linearizer order, not the mod-8 key).

Review at landing:
- 0x00A9EB90 RELOCATION_MATCH with a FITTED GFFastArcCos text (named table values) that conflicts with chunk 2044's pending 0x00A9F540
  (plain ArcCos text). Land them consistently: either re-fit 0x00A9F540 under the named ArcCos or find this row's ~6 named symbols another
  way under the plain text (NOTES.md has the measured variants).
- The row carries `// Retail-asm:` evidence through the shared GFFloatToLongNear / GFGetCycleCount helpers (sites unchanged from the prior
  differ source: 0x00A9EB99 / 0x00A9F066 rdtsc blocks; fistp sites inside GFFastArcCos / GFFastSin).
