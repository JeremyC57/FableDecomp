# Chunk 1013c notes

**Header overlay:** every grade ran with `FABLE_HDR_OVERLAY=$A/hdr` (empty: no header change this chunk; see hdr/README.md).
Free compiles (c2prio `--obj`, tools/pr.sh, pv.sh, fr.sh) were used for ~150 probes; grades are logged in ATTEMPTS.log.
Result: 1 RELOCATION_MATCH (0x0063AB90), 9 parked with measured residues, 2 not gradable under the current headers.

## Matched
- **0x0063AB90 ?GetThemeFaceNormalAt@CWorldMap@@QBE?AVC3DVector@@ABV2@@Z** (1 grade). The last store residue (retail writes the
  first C2DCoordI.X at 0x0063ABBD, between the two `_ftol` calls) is the first lookup's coordinate as a scoped local filled member by
  member: `{ C2DCoordI cell; cell.X = ix; iy = (long)p.Y; cell.Y = iy; h00 = GetThemeSizeZAtBlock(cell); }`. Unscoped, the named cell
  keeps its own slot (frame 0x30); built from the ctor, X is stored after the second `_ftol`. Calls checked: __ftol2 x2,
  GetThemeSizeZAtBlock 0x0063A7E0 x4. Test test/0063ab90.cpp PASS under g++ and VC7.1 /Od.

## Parked
- **0x00B41770 PeekInterpolatedWaterHeight**, DIFFER(773v777) with `/d2inlT301`.
  - New: VC7.1 does expand the same-TU PeekLandscapeHeight at a large inline threshold. T301 is the smallest value; T300 and the
    default call it. T301..T1000 all give the same 773-byte body, which matches the forceinline probe.
  - Left: retail CSEs this->MapWidth from the first range test into a frame slot ([esp+2Ch], 0x00B418CC) and reuses it for the x
    clamp (0x00B419A4). Ours reloads `movzx eax,word [ebp+5Ch]`, so the frame is 0x20 against 0x24 (26 hunks, nothing else).
  - Inert: a named width local (c2 still reloads), MapWidth for PeekWidth(), a named height, height and depth swapped, definition
    after the caller, a non-recursive / asm-free diagnostic copy (same reload).
  - Next: find why c2 keeps a stack copy of the width only in retail. In the out-of-line 0x00B439C0 retail reloads it too.
- **0x00B43720 PeekThemeId / 0x00B43870 PeekThemeBlend / 0x00B439C0 PeekLandscapeHeight**: one compensation-block placement (+4 bytes
  each).
  - Measured rule from the probes: c2 inserts the reload block of an edge after the first jump-ending block found walking forward from
    one end of the edge. With CL=1 / CL=2 clamp spellings the x<0 / y<0 block moves to the next such block after the clamp test.
  - Ours walks from the target (the clamp test). Retail walks from the source: the block lands after the loop-back `jmp`, ahead of the
    other exits' block.
  - Inert (all byte-identical or worse): 18 GetMap spellings (probe header ph/), 9 caller shapes, no worldMap local.
  - An inline CEngineWorldMap::MapHasDataAtPoint helper (FableWin has one, 287 debug bytes) inside GetMap stops GetMap expanding.
  - Next: read c2's edge-split placement (the routine that links the new block) to learn what chooses source v target.
- **0x00B3FC40 PeekInterpolatedMapNormal**: frame slots only (frame 0x54 against 0x60).
  - The chunk1013c oracle row is cut wrong: 816 bytes, running on into 0x00B3FE50 and 0x00B3FEE0 with no int3 between them. q.py's
    grade line therefore reads 528v813; the real body is 528 bytes (`ret 0Ch` at 0x00B3FE4D).
  - fr.sh: in this ebp frame, slots are laid out in creation order. Ours creates n11's slot first; retail creates n01's first, and
    gives the n11 and n01 terms their own slots.
  - Inert: all 24 term orders, declaration order, names, and separate assignment.
- **0x0063D0A0 / 0x0063D4A0**: scheduling only (13 hunks each). Inert: the Dot / Normalise / SquaredMagnitude opt-ins and three
  `normal` spellings.
- **0x004FEB70 HeightAreaChanged**: register choice only.
  - c2prio `--blocks`: const 0 gains +12 in the entry block and loses 24 in the blocks it lives through (priority -12). To take ebx
    before it2 (priority 54) it would need more than 54.
  - Inert: `== NULL`, IsNull(), the ternary area, a named map in loop 2.
- **0x0063B3F0 CreateLineSegment**: prior regraded only (DIFFER(836v856)).
- **0x00500540 LoadRegion / 0x00507070 BuildStaticMapGenerationVisibilityInfo**: not gradable. Both priors are SRC_FAIL under the
  current headers (opt-ins renamed or removed since they were written).
  - LoadRegion: retail calls one function, 0x006B3600 = ?SetThemeWeight@CEnvironment@@QAEXJJJM@Z, for both environment calls (see
    NAME_FIXES). The prior's SetRegionTheme / SetTheme are the inline wrappers: SetRegionTheme stores CEnvironment+0x10 and then calls
    SetThemeWeight(0, 0, theme, 1.0f); SetTheme(n+1, 0, theme) calls SetThemeWeight(n+1, 0, theme, 1.0f).

## Header debt
- No shared engine/CRegionDef.h yet. Retail offsets are Ego_r's less 4 before SoundMap and less 12 after it. Members in use:
  EnvironmentTheme +0x3C, InternalEnvironmentTheme +0x40, InternalLightingChannelCount +0x44, UseVillageTime +0x88,
  AttachToCameraFX +0x8C, EnableSpotlightShadowMode +0x90, HeroBreathParticle +0x94, HasSea +0xA4. The base class between
  CDefPointeeBase (0x1C) and +0x3C is unproven.
- Also missing: CStaticMapGenerationVisibiilityInfo (PDB spelling), CGameTimeManager (SetAsUsingVillageTime 0x006BB2F0),
  CTCDParticleEmitter (Create 0x006E0880, AttachToCamera 0x006E0DB0, AttachToThing 0x006E0BE0), CAsyncLoadingMemoryPool
  (ProcessLoading 0x00A14900), the inline CEnvironment SetRegionTheme / SetTheme over SetThemeWeight, a CWorldMap opt-in declaring
  BuildStaticMapGenerationVisibilityInfo, and CGlobalDef (still in _quarantine).

## Callee names (LABELS.tsv)
- 0x0063A7E0 ?GetThemeSizeZAtBlock@CWorldMap@@QBEMABVC2DCoordI@@@Z
- 0x006B3600 ?SetThemeWeight@CEnvironment@@QAEXJJJM@Z. Landed row 0x00890090 binds it as `?Do@Inner@@QAEXHHHH@Z`: NAME_FIXES.tsv.
