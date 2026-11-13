# Hand chunk 2030 (CEngineWaterSettings): 2 of 2 byte-exact

Both rows RELOCATION_MATCH under VC7.1 RTM 3077 (q.py, no flags, FABLE_HDR_OVERLAY=hdr/), behaviour tests PASS under g++ and
VC7.1 (tall.sh / tall.py), read-only vland: 2 wins, no RELOC_IDENTITY complaint. rel.py: every call / global relocation lands on
retail's target; the only unbound one is the class vtable `??_7CEngineWaterSettings@@6B@` -> 0x012A8114 (correct by RTTI).

## What matched (2)
- **0x00BEA110 CEngineWaterSettings::UpdateConstants** (1607 bytes; pass 1, grade 5). Per-render refresh of the water constants:
  unless ForceWaterSettings, the environment theme channel 0 water colour (Colours[14]) and scalers 23..29 (blend start / end ->
  CalcReflectToRefractBlendFactors, flow speed, swell oscillation speed, swell factor * 1e-4, shore swell, ref offset) through
  `g_pEngineWorldMap->PeekGameWorld()->DrawGetEnvironment()->PeekInterpolatedParams()`; the interpolated bump shifts (C2DVector
  arithmetic), edge-wake offset, the three PC bump-map frame indices (GFFloatToLongNear(lerp) % GetFrameCount(), graphics from the
  engine graphic bank by the environment def's Ice / Water / Sea BumpMapPC indices), the swell sin / cos (GFSin / GFCos of the lerped
  angle in turns) and the four shader-variable matrices via CMatrix3x4::Set. Shape lessons: plain member lerps, not
  GFGetInterpolatedValue; matrices through Set (a temporary CMatrix3x4 copies with rep movsd); the background fade X / W as float
  locals; the swell angle alone with the extra `((new - old) * t) + old` grouping (one PAREN tuple moves the last scheduling cut;
  measured with the c2 scheduler trace and replay: the same grouping on all lerps breaks the earlier region, pass 1 grade 6).
- **0x00BE9920 CEngineWaterSettings::CEngineWaterSettings** (1886 bytes; pass 1, grade 9). It is the constructor (base
  CBaseClassNonCopyable ctor call, vtable 0x012A8114, returns this; called by the CEngineWaterRenderer ctor on Settings +0x280), not
  an Initialise. Shore lookup mask: twelve phases k * 30 degrees, value 1 - cos below half a turn, cos + 3 above (the folded double
  angles k * (2pi_f / 12.0) are bit-identical to retail's pool); ShoreLookupMaskMatrix element by element, ShoreLookupMaskMatrix2 (the
  same phases one step back, the first clamped) through Set; Other*ShaderVars via SetScale(0); switches; defaults (FresnelFactor before
  Colour, the rest in declaration order, then the five theme-driven values and CalcReflectToRefractBlendFactors(0, 15) folded to
  -1/15, 1); histories in an unrolled two-pass loop, PC frame offsets `[0] = [1] = 0`, bump shifts / histories as C2DVector(0, 0)
  temporaries (water type 0's histories [1] before [0]); the bank indices from the environment def; ShoreLookupMaskMatrixForBounds =
  each pre-transposed phase minus all earlier ones (a flat float[12], copy back through Set). Each of these was a measured
  decision (see the source header and LESSONS.md).

## Parked for Fable 5.1 (0)
None.

## Skipped (0)
None. Row extents checked: 0x00BEA110 is 1607 bytes to its `ret 4`; 0x00BE9920 is 1886 bytes (`ret` at 0x00BEA07D, two int3) -- the
oracle length is right; the reservation's 2018 overran into two uncatalogued functions. SPLITS.tsv lists the gap functions:
0x00BEA080 WaterReflectBlendFactors (47) and 0x00BEA0B0 WaterBumpmaps (82) after the ctor, and 0x00BE9490 EnableWaterWaves (9),
0x00BE94A0 DisableWaterWaves (9), 0x00BE94B0 CalcReflectToRefractBlendFactors (47), 0x00BE94E0 KeepHistoryInRange(float*, float) (74)
before the landed 0x00BE9530 -- all uncatalogued, all CEngineWaterSettings members (Ego_r / FableWin name sets, donor order).

## Names
- Rows: both exact PDB spellings (Ego_r.pdb + FableWin.pdb name sets; roles proven by vtable / callers above).
- Callees / inline accessors (FableWin.pdb names, inline in retail): CEngineWorldMap::PeekGameWorld, CIDrawWorld::DrawGetEnvironment
  (slot 1; CWorld 0x0049E350), CIDrawEnvironment::PeekInterpolatedParams (slot 1; CEnvironment 0x006B7350),
  CEngineResourceManager::PeekGraphicBank, CGraphicDataBank::PeekGraphic, CGraphicInfo::GetFrameCount,
  CDisplayManager::PeekBackBufferResolution, CEngineLandscapeRenderer::GetSettings, CMatrix3x4::Set(12 floats) / SetScale, GFSin /
  GFCos, GFFloatToLongNear (shared asm helper; the row has a `// Retail-asm:` block and a 10-spelling x 2-flag C++ / intrinsic
  attempts note), CalcReflectToRefractBlendFactors 0x00BE94B0 (defined in each TU; out-of-line copy byte-identical to retail).
- Globals: the landed CEngine::Initialise 0x00B26360 spellings (g_pEngineWorldMap, g_pResourceManager, g_pEngineEnvironmentDef,
  g_pDisplayManager_01436e1c, g_LandscapeRenderer_01436EA8); reloc_bindings has several other spellings per address (pre-existing).
- **Descriptive (made up):** the file-local helper `ShoreLookupMask(long step)` in 0x00BE9920 (static inline, always expanded, no
  symbol in the object). Searched: Ego_r / FableWin name sets (Shore*, *Mask*, cos-based helpers), struct layouts, header enums,
  repo headers: no original. Indices with no recovered enum names: theme colour 14 and scalers 23..29 (written as literals).
- NAME_FIXES.tsv: 0x00BE9530 is CEngineWaterSettings::KeepHistoryInRange(C2DVector*, float) const (landed under the sky spelling),
  0x00B61AA0 is the private const sky overload, 0x00BE95F0 manifest label "UpdateWorldFrameState" -> WorldFrameUpdate.

## Header debt
- engine/CWorld.h declares CIDrawWorld slots 1 / 2 as DrawGetWorldMap / DrawGetEnvironment; retail CWorld's vtable 0x012390DC has
  DrawGetEnvironment at slot 1 (returns CWorld+0x18) and DrawGetWorldMap at slot 2. The overlay engine/CIDrawWorld.h carries the
  retail order (and the destructor in slot 3); the two definitions must not meet in one TU.
- engine/CIDrawEnvironmentParams.h / CIDrawEnvironment.h / CIDrawWorld.h are not in rebuild/include yet (pending chunk 723/727 models).
- CEngineWorldMap retail branch: only GameWorld / GameWorldMap typed (opt-in); the rest stays opaque.

## Grades
0x00BEA110: pass 1 = 6 (match at 5; the 6th tested the consistent grouping and differed) + 4 verification regrades of the unchanged
final source. 0x00BE9920: pass 1 = 9 (match at 9) + 2 verification regrades. Free probes: ~10 fragment compiles (fistp spellings,
loop unrolling, CalcReflect), ~15 c2 traces (c2prio, region dumps, DAG + scheduler replay).
