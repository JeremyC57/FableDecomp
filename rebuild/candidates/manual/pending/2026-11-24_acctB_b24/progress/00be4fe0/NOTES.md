# 0x00BE4FE0 CWaterPatchTesselatorBase<CTVertexSeaBackground>::TesselateEdge -- RELOCATION_MATCH (B24C2)
## Lessons that applied (read first)
- Q32 / owner 2026-11-24: a residue that looks like register colouring is usually a source fact; here it was helper TEXT.
- fwsize.py (FableWin /Od size oracle) is calibrated in this TU (six helpers SAME): the helpers it calls DIFF are the ones to fix.
- One merged skip store + an explicit not-found store is retail's block shape (B24C's else-chain observation was right).
- A re-read of `strip->GetWaterVertex(i)` instead of the `water` reference changes VC7.1's CSE and frees a register.
## What made it match (B24C2, 14 grades)
1. FindWaterVert rewritten to the FableWin-exact text (65i/194b): early `if (WaterVerts.Size() == 0) return 0;` then one `while (i < Size &&
   Verts[..].World? < pos) i++;` per axis (the sibling FindVert is the same while loop, FableWin 48i/140b exact) -> frame slots matched,
   90 -> 23 hunks.
2. The bridged-span branch ends in `continue` (found / not-found as if/else) with ONE shared skip `previous = -1` -> strip no longer
   enregistered, 19 hunks.
3. `start = GetMainAxisVertexWorldPos(strip->GetWaterVertex(i).LandscapeVertexIndex)` (not `water.`) -> retail's reload and ebp = -1 hoist,
   4 hunks; 4. not-found `previous = -1;` BEFORE the index store -> RELOCATION_MATCH.
Not needed / no effect: the FindFirstWaterAdjacentVertex text (FableWin +4i/+10b still open: pos local, count local tried), found/fan scope
(both match; inner scope kept). FABLE_FORCEINLINE is required (plain inline: 1844 / 736 bytes).
Headers / flags: stock flags; overlay opt-ins in hdr/README.md. Test: production bodies copied verbatim over fixtures (mutation-checked).
Self-check: strict vland RELOCATION_MATCH PASS, no WARN / RELOC_ lines; pick-any checker: both COMDATs EQ.

## Hand-back state (B24C2, final)
- Source = vv/fanvert.cpp (m1a + AddFanBaseVertex with a named `CVertex& vert`, FableWin-exact 60i/182b); test regenerated (verbatim
  bodies), vt PASS, mutation FAIL; addmatch + strict vland RELOCATION_MATCH PASS, no WARN/RELOC_; pick-any 2/2 EQ; hdr/ identical to
  the water row's.
- PROVISIONAL on the shared template text: the water twin 0x00BE4660 is closest (4 hunks) with the else-chain skip stores and NO
  `continue` (its z2), which the sea row does not accept (s_z2 22 hunks). Sea also matches with else-chain + a `continue` in the
  found arm (s_z6), in the not-found arm (s_z7) or after the span (s_z4). Whatever text finally matches BOTH must be regraded here
  (regenerate test / payload / vland / pick-any). See ../00be4660/NOTES.md.

## Earlier (B24C) notes
# 0x00BE4FE0 CWaterPatchTesselatorBase<CTVertexSeaBackground>::TesselateEdge -- in progress (B24C), best DIFFER(2404v2412) (draft)
## Lessons that apply (prior hand785 notes, LESSONS: unsigned-int loop counter; Q32: callee body visibility)
- 173 earlier attempts left only register colouring; the helpers (FindFirstWaterAdjacentVertex, IterateThroughAdjacentVerts,
  AddDuplicateEdgeVertex, AddFanBaseVertex) only expand with __forceinline (plain inline leaves them out of line).
- Draft still has a local `class CEngineLandscapeMap` (GetEdgeStrip 0x00BDC980) and bare __forceinline: both need fixing before hand-back.
## Retail facts read by hand this pass (dis_all.txt)
- Retail hoists the constant -1 into ebp for the whole main loop (`or ebp,-1` at the loop head 0x00BE512B / back edge 0x00BE5149):
  it feeds `fan = -1` (0x00BE51D3) and the shared skip-path `previous = -1` (0x00BE5925); the not-found path has its OWN immediate
  store `previous = -1` (0x00BE5917) and jumps straight to `i++`; the found path stores previous (= added, in ebp) at 0x00BE58D0.
- Prologue: retail pushes `side` straight from its argument slot (no register copy), keeps the first FindFirstWaterAdjacentVertex
  copy of `length` in ebp, and reuses the dead `vertical` argument slot [esp+0x88] as a temp; ours keeps side in edi and has one
  extra 4-byte slot below `this` ([esp+0x18] v [esp+0x14]).
## Tried this pass (grades 1-4)
- baseline draft 2404v2412 / 90 hunks; map-before-length 2388v2412; if/else length 2404 (=); adjacent_index before adjacent_strip
  2404 (=); found/fan at loop scope 2404 (=); explicit else-chain `previous = -1` (own store on the not-found path, retail's block
  shape) 2391v2412 / 83 hunks, with or without loop-scope locals (closer in hunks, further in size).
## Best next idea
- Keep the else-chain (retail's block shape) and find why side gets a register: try the helpers taking `side` last-used order, or
  passing the TesselateEdge argument through without the by-value copy (helpers declared with the exact Ego_r parameter list,
  `const CEngineMap*` first); then the shared ebp=-1 should appear once edi is free.
- Retail reloads `side` from its argument slot at EVERY GetEdgeStrip call (0x00BE50F8 / 0x00BE55B1 / 0x00BE569A: `mov r,[esp+90h] ; push r`):
  side is never enregistered; ebx = map (Data->EngineMap) in the first FindFirst copy, spilled to [esp+38h] in the nested copies;
  ebp = the helper's length copy. Ours gives side edi for the whole function, which pushes one more variable to the stack (the extra
  low slot) and leaves no register for the hoisted -1. Find the source fact that makes VC7.1 weigh side low (how the helpers receive it).
- Not yet done (needed before hand-back even if it matches): replace the bare __forceinline with FABLE_FORCEINLINE + "no retail out-of-line
  copy" comment, and move the local `class CEngineLandscapeMap` (GetEdgeStrip 0x00BDC980, Ego_r ?GetEdgeStrip@CEngineLandscapeMap@@QAEPAV
  CPatchTesselationEdgeStrip@@JJJW4LANDSCAPE_EDGE_STRIP_SIDE@@@Z; FableWin 0x02CC93B0) into a new engine/CEngineLandscapeMap.h overlay header.
- Rows 0x0070D940, 0x00BE3970, 0x00BE4660: untouched by B24C (context budget).
- (B24C3) Fifth sea-matching text, no `continue`: first test else-first (`if (water.EdgeIndexBufferIndex == 0xffff) previous = -1; else
  { ... }`, ../00be4660/c3/gx/b_gba1.cpp / gbb1 / gbc1): RELOCATION_MATCH (index-first not-found: 4 hunks). Payload unchanged (m1a);
  still held on the shared-text question (water rejects every S-form: 16 hunks).
