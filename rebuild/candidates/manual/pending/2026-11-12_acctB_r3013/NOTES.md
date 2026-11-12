# Hand chunk 3013 (CEnginePrimitiveManagerRepeatedStaticMeshes .. CTCTurncoatSpell): 1 of 2 byte-exact (near-miss retry of 2028 / 2027)

## What matched (1)
- **0x00B9A4A0 CEnginePrimitiveManagerRepeatedStaticMeshes::RenderPrimitive** (RELOCATION_MATCH, 1674 bytes, on the first grade of this
  chunk; behaviour test PASS under g++ and VC7.1; read-only vland RELOCATION_MATCH PASS). Started from chunk 2028's parked source (one frame
  byte: `translucent` at [esp+18h], retail [esp+1Bh] at 0x00B9AA25 / 0x00B9AA56).
  Measurement (frame-packer trace, tools/pk.sh + tools/slots.py + tools/al.py, all free): the packer sorts slots by weight/size descending
  (shadowed 4/1, fading 3/1, the 4-byte [theme-desc / loop pointer] slot 10/4, translucent 2/1, the [i] slot 7/4 ...): retail's order is
  ours. The only difference is the 1-byte `translucent` node's ALIGNMENT: c2 node byte +0x2C (log2 align + 1) was 3 (align 4) for the
  inlined parameter home of a value loaded straight from `material->Transparent`, so its slot offset was rounded to -632 (B+8); with align 1
  it sits at -629 (B+0xB) = retail. The alignment follows the c2 symbol's +0x39 bit 0x20: set for the param home fed by a raw member load,
  clear for one fed by a computed bool (the `fading` comparison passed directly also gets align 1) and for a call result.
  Spelling: the FableWin.pdb inline accessors `material->IsTransparent()` (and, consistently, IsBooleanAlpha() / GetMapFlags() /
  GetDiffuseMapBankIndex()) -- the accessor's return value feeds the parameter, the node is byte-aligned, weight still 2, packed at
  [esp+1Bh]. A named `bool translucent = material->Transparent;` local also gets align 1 but weight 3, which re-sorts the byte slots
  (B+5..B+7, worse).
  Names: all from chunk 2028 (LABELS.tsv) plus the three C3DMeshMaterial accessors (FableWin.pdb mangled names; fields from
  struct_layouts_egor.tsv; read in place in retail). Descriptive (made-up) names in the matched row: parameter / local names only
  (primitive_base, layer, chain, primitive, distance, radius, occlusion_layer, tolerance, mesh_info, nearest, lod_error, lod, mesh,
  scene_data, shadowed, mesh_primitive, material, render_mode, map_flags, sub_primitive, fading; helper parameters fading / translucent /
  alpha_test / primitive_flags / shadowed, local alpha / flags) -- the PDB name sets carry no parameter or local names. Globals as chunk 2028
  (g_pEngineCamera 0x01436EA0 is the definer's spelling; c2018 / c2024 used NEngineInternal::GPCamera -- pick one at landing).
  Retail-asm: the SSE box-distance kernel 0x00B9A75F..0x00B9A799 is the shared engine/C3DBoundingBoxSSE.h helper (the `// Retail-asm:` line in
  the source, attempts in that header); no asm in the row.

## Parked for Fable 5.1 (1)
- 007c6e50 CTCTurncoatSpell::FrameUpdate DIFFER(1804v1806): retail 0x007C741C `mov edx,[esp+14h]` (hoisted above 0x007C7420 `mov [esi+90h],ecx`),
  0x007C7426 `mov eax,[edx+2Ch]`, 0x007C7432 `mov esi,edx`, then the temp rotation shifted (0x007C7434 lea eax/push eax, 0x007C7474..0x007C747D,
  0x007C7497..0x007C749E); ours `mov esi,[esp+14h]` after the store, `mov eax,[esi+2Ch]`, no copy | tried (this chunk, 2 grades): ParentThing
  at the XP lookup (1803v1806, re-reads [esi+4]); MakeNoise on ParentThing (1805v1806, ratio 0.971: XP code becomes retail's, but map /
  experience move to edi / ebp and MakeNoise reads [esi+4]); free c2prio probes: experience declared at case scope, named `has_experience`
  bool, SetTurncoatOriginator(ParentThing), parent declared before Timer.FrameUpdate, no `parent` local at all (no candidate: every use
  from [esi+4]), no MakeNoise -- all keep the coloured XP piece | measured: (1) the deciding step is c2's live-range SPLIT pass: when `parent`
  (#19) is split after `turncoat` takes ebp, an XP piece is formed over the counter-store block (line 165/167) and the map block (167),
  2 refs, spill 2, coloured esi by the copy bonus with the map; (2) the spill test is not the lever: forcing that piece's spill to 0 at the
  driver check 0x10782a5e only defers it (flag +5 bit 0x40, re-insert 0x1078a35c) and the recomputed spill is 1 -> coloured anyway
  (tools/wi/whatif.py); (3) the absorb lookup's HasTC reads a CSE temp of [parent+20h] defined at the hero-stats lookup (line 143), so its
  piece holds only {map copy, push}, spill 0, skipped -- that is why the absorb path already has retail's codegen-temp shape; (4) with
  `this` live through the XP block no XP piece is formed at all | next: reverse the split pass's piece growth (block groups; c2prio_b.py
  prints each piece's first / last block) to find what conflict retail had in the XP blocks with `this` dead after the counter store;
  passes 2-3 unused (context).

## Skipped
- none. Both rows start at a real function start and end where the oracle says (chunks 2027 / 2028 checked; no SPLITS.tsv).

## Label corrections
- carried from chunks 2027 / 2028: NAME_FIXES.tsv (BeginLoadingTextures 0x00A23C50, CEngineSubPrimitiveBoundingBox ctor 0x00B98B10,
  GetOccludedFrames 0x00B2FAC0, the CTCTurncoatSpell callee spellings of the landed rows, MakeTeamMemberComment).

## Header debt
- New opt-in FABLE_RETAIL_C3DMESHMATERIAL_FLAG_ACCESSORS in hdr/engine/C3DMeshMaterial.h (hdr/README.md). Everything else in hdr/ is the
  union of the chunk 2028 and 2027 overlays, unchanged (no file existed in both). Row 0x00B9A4A0 needs every opt-in its #define block names.

## Grades
- 0x00B9A4A0: pass 1: 1 grade (RELOCATION_MATCH) + 2 regrades of the final src (gall.sh) + 1 baseline confirmation of the prior source. Free: ~8 frame-
  packer traces (base, named local, fading as an argument, accessor variants).
- 0x007C6E50: pass 1: 2 grades + 1 baseline confirmation. Free: ~16 c2prio / what-if traces. Passes 2-3 not used.

## Tools (all copies, in tools/)
- pk.sh + pk/phook.py + pk/pk_host.py (frame-packer trace), slots.py (final slots with offsets, members remapped by array index -- the
  packend bitsets are array indices, not node ids), al.py (node alignment byte +0x2C), sim.py / fmap.py / nodes.py (agent3002 copies),
  c2dis.py (disassemble c2.dll), prio.sh + c2p/c2prio_b.py (c2prio with first / last referencing block of every chosen / skipped piece),
  pieces.py, wi/whatif.py + wi_host.py (force a candidate's spill at the driver's check).
