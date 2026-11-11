# Chunk 3002 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- c2 dead-parameter-home reuse is decided AFTER packing: c2 packs with the parameter homes as the first slots, and if the remaining frame
  is >= 0x70 bytes (c2.dll 0x10790D90 -> 0x10734A51 `cmp edi,70h`) it repacks without them (homes unused). A visible callee body that
  "turns on parameter-home reuse" only does so because it shrinks the frame under 0x70 (0x007F26B0: visible GetEffectFromString + the
  right temporaries = 0x8C frame, homes unused as retail). [esi+8Ch] bit 7 / [esi+46h] bit 0 of the function record also disable it.
- c2's final frame packer is reproducible offline from one phook trace: nodes by size asc, weight desc; each joins the LATEST slot with
  size <= 2*slot.size and no conflict, else opens a slot (tools/my/sim.py replays 0x007F26B0 exactly). Conflicts = the union of the
  per-node bitsets of the pack event. Use it to find which conflict changes give retail's membership, then find the spelling.
- Named C3DVector locals whose address escapes (AddControlPoint, Rotate, GetHeightZBelowPos) conflict with everything in their scope;
  non-escaping ones (velocity, direction) use real liveness. A temporary passed straight to `AddControlPoint(const C3DVector&)` dies at
  the call; written as a named block-scoped local it lives to the block end (0x007F2AB4..0x007F2B93: retail's [esp+60h] / [esp+3Ch] /
  [esp+90h] temporary slots).
- `difference /= distance; C3DVector direction = difference;` (normalise in place, then copy) = one forwarded node: retail's frame and
  its X, Z, Y squares + stored reciprocal (0x007F2802). `C3DVector direction = difference; direction /= distance;` makes the two conflict
  (+1 slot); a single in-place vector squares X, Y, Z.
- Frame-word mapping of two instruction-identical builds: track esp depth with each callee's `ret N` read from Fable.exe (virtual calls
  by hand) and map built offsets to retail offsets per instruction (tools/my/fmap.py); then name each word's nodes from the phook trace.
- c2 packer list ties (c2.dll 0x10788642 / 0x10789A99): new nodes go to the end of their size group with weight 1; each increment moves a
  node only past STRICTLY lower weights, so equal-weight nodes keep the order in which they reached that weight. The weight walk
  (0x10785C60) runs blocks then tuples in final scheduled order (0x00550DC0: the parent / table colour temporaries tie at 2 and the
  table one wins because `fld [T.R]` precedes `fmul [P.R]`).
- phook-based tools: pass the function name explicitly (a stray `-v` taken as the function name silently gives "no pack event").

Review at landing:
- 0x007F26B0 defines FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY: the object also carries the noinline COMDAT
  ?GetEffectFromString@CSpecialEffectsDef@@QBE_NABVCCharString@@AAJ@Z (retail 0x00669D30, landed elsewhere); grade the Initialise leaf.
- hdr/ is the chunk 2022 + chunk 2006 overlays unchanged (land those first; they build on c2007 / c2002).
- Landed lightning rows 0x007F23D0 / 0x007F24E0 / 0x007F39A0.. use wrong leaves / local models (chunk 2022 NAME_FIXES, repeated here).
- 0x00550DC0 stays parked (frame-slot tie, measured; see NOTES.md).
