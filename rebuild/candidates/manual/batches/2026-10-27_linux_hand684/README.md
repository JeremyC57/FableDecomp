# Chunk 684 (NA027, 514-828 band) -- 8 of 10 landed (2026-11-01)

Eight rows landed (`verify.log`; bindings multi-check 0): CEngineLandscapeMemoryManager::ProcessDisposalList 0x00BDA150 (private; label
CDrawResourcePool_FlushCycle wrong), the three texture-lookup siblings of the parked GetReflectionTexture (Get2DAlphaMaskTexture 0x00BA9A70,
GetBumpTexture 0x00BA95D0, GetDiffuseTexture 0x00BA9360 -- on the STLport-style vector lookup model from chunk 683; the `push ebx` placement
that parks 0x00BA9830 did not arise in these three shapes), the libpk rows MixChannels 0x00C77D90 and CEaxContext::SetEnvironmentSize 0x00C677C0
(descriptive; label HeroAttribute_SetValueWithValidation wrong), the DXUT ImeUi.cpp GetImeId 0x00CB28F0 (/GS; the matching text differs from the
public June-2010 SDK in four byte-forced places, documented in the source header -- retail compiled an earlier SDK revision), and
NVectorMath::GFInterpolateVectors (KKM) 0x00A5B870 at grade **retail_asm** (801 bytes; the hand SSE kernel 0x00A5B8A6..0x00A5B8DC with fixed
registers and memory strides that VC7.1 cannot emit from C++, cited in the `// Retail-asm:` line; same style as the landed siblings 0x00A5BEF0 /
GFFinishPartialSlerp / GFClear4DVectors; no naked/_emit/ret inside the asm; accepted under the owner exception). Labels:
`ghidra_out/labels_chunk684_2026-10-27.tsv` (10 rows + 2 NUISystem-namespaced binding corrections).

Decisions at landing (coordinator, under the owner delegation):
- 0x00A5B870 retail_asm accepted (above).
- CDspGain::ApplyRamps' sixth parameter is a bool (byte test at 0x00C74573 / 0x00C74784); the landed callers Apply 0x00C74CE0 / Crossfade
  0x00C74D30 declare `int mode` -> c684 queue row to respell and re-grade them in naming wave 19 (the row itself is parked, nothing landed).
- `ProcessDisposalLists` -> `ProcessDisposalList` (private) in the three landed Allocate* callers and `CreateExperienceOrbs` ->
  `CTCDExperienceOrb::Create(CThing&)` in landed OnDie 0x00664300: queued c684.
- Header debt (CEngineResourceManager generated header +0x230 above retail; the texture-family layouts as `// Layout:` comments; ETextureType
  enumerator 4 proven only by use; the ninth CDspOps slot): migration wave 10.

Parked: CTCDExperienceOrb::Create(CThing&) 0x0077A890 (DIFFER 724v707, 21 probes, register allocation only) and CDspGain::ApplyRamps 0x00C74510
(828v828, 29 probes: the two-pole decay loop keeps y0 in a frame slot updated in place where VC7.1 enregisters it in every spelling tried --
lane gap). Best sources in `../../pending/2026-10-27_linux_hand684/differ/`. NAME_FIXES.tsv: 10 rows (queued c684). Lessons in LESSONS.md.
