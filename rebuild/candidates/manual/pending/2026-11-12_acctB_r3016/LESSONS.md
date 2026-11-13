# Chunk 3016 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x00550DC0) "Every instruction right, two pairs of frame slots swapped" closed by VARIABLE REUSE: the table's colour is a named
  `CRGBFloatColour colour(Colour)` that the loop later reuses for each child's product (`colour = CRGBFloatColour(child->GetColour()) *
  render_colour`), as in the landed sibling CComponent::UpdateColour 0x0052F900 (`parent` / `colour` / `render_colour`). Temporary +
  separate loop variable = same code, but the packer puts the parent / table colour R fields and the alpha / blue products in each
  other's slots (0x00550EFE / 0x00550F53 / 0x00550F59 / 0x00550F63). Merging two values into one named variable removes a packer node:
  it changes the 16-byte list order AND the slot array the density qsort sees, so a density tie elsewhere can flip too.
- (tool) c2's frame-slot qsort runs over the slots AFTER the parameter homes (presort event field 2 = number of homes, 5 here); the
  agent3011 qs.py replay sorted the homes too. Fixed copy in tools/fs/qs.py (prints "replay == real order").
- (tool) The phook `pack` event's conflict bitsets are ARRAY indices (map through packend idx) and one-sided: symmetrise them.
  tools/sim2.py replays weight walk -> list order (per size group: append on first reference, move past strictly lower weights only)
  -> slot joins (latest slot, size <= 2 * slot size, no conflict) -> slot qsort, and reproduces the real layout exactly; `--ref
  node@k` / `--del k` / `--swap a-b` what-ifs, tools/sweep1.py / sw2.py classify them against retail's layout.
- (0x00550DC0) Operand order inside an inline CRGBFloatColour operator* (a.R * b.R vs b.R * a.R) is canonicalised by c2: byte- and
  slot-identical. Early-return copies of a tail call are not cross-jumped in this row (+33 bytes): no hidden references from there.
- (0x00550DC0) A bare named `CRGBFloatColour parent_colour` adds one outer name and moves a member-product operand order (4 lines,
  the r3004 key); the full three-name block (parent_colour, colour, render_colour + the loop's child_render_colour) keeps base code.

Review at landing:
- 0x00550DC0 MATCHED (RELOCATION_MATCH) after three parked attempts (2006 / 3002 / 3005); 3 grades this session. hdr/ = chunk 3005
  overlay unchanged (must land first: FABLE_RETAIL_CRGBFLOATCOLOUR_INITIALISE_CTOR, FABLE_RETAIL_CTABLE_DRAW, FABLE_RETAIL_STLP_MAP).
- rebuild/include/engine/CRGBFloatColour.h models the product as a free operator*; FableWin.pdb has the const member
  ??DCRGBFloatColour@@QBE?AV0@ABV0@@Z (chunk 3005: byte- and slot-neutral in this row). Worth a check at the next header merge.
