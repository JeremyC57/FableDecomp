# acctB hand-back, chunk 2030 (2 reserved rows: 2 matched)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x00BEA110 CEngineWaterSettings::UpdateConstants(const CEngineComponentRenderParams&) | RELOCATION_MATCH 1607v1607, test PASS (g++ and VC7.1) | grade `retail_asm`: three bare `fistp dword` sites (0x00BEA436, 0x00BEA480, 0x00BEA4C6) through the existing `engine/GFFloatToLongNear.h`, `// Retail-asm:` block + attempts note in the row. **The attempts note covers 10 spellings (each under /O2 and /O2 /QIfist), fewer than the 30+10 per site ASM_RULES asks for** -- the agent relied on the helper header's own proof for the rest; review at landing. 6 grades |
| 0x00BE9920 CEngineWaterSettings::CEngineWaterSettings() | RELOCATION_MATCH 1886v1886, test PASS (g++ and VC7.1) | really the constructor (base ctor 0x0099A2F0, vtable 0x012A8114, returns `this`; sole caller the CEngineWaterRenderer ctor). **True length 1886, not the reserved 2018** (SPLITS.tsv). `#pragma intrinsic(cos)` as in landed rows. 9 grades |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grades, both test runs,
read-only vland: 2 wins, no RELOC_IDENTITY / RELOC_CONFLICT). Purity and padded-struct greps are clean.

Review items for landing:

- **Six uncatalogued CEngineWaterSettings functions (SPLITS.tsv),** named from the Ego_r / FableWin name sets: 0x00BEA080
  WaterReflectBlendFactors (47 bytes), 0x00BEA0B0 WaterBumpmaps (82), 0x00BE9490 EnableWaterWaves (9), 0x00BE94A0
  DisableWaterWaves (9), 0x00BE94B0 CalcReflectToRefractBlendFactors (47), 0x00BE94E0 KeepHistoryInRange(float*, float) (74).
  The agent reports that the out-of-line CalcReflectToRefractBlendFactors both rows emit is byte-identical to 0x00BE94B0;
  I did not check that (no oracle row for an uncatalogued address on the acctB lane).
- **NAME_FIXES.tsv (3):** 0x00BE9530 is landed as `?KeepHistoryInRange@CEngineSkyRenderer@@...` but sits in the water TU
  and is `?KeepHistoryInRange@CEngineWaterSettings@@QBEXPAVC2DVector@@M@Z`; 0x00B61AA0 (the sky copy) shares that spelling
  and should be the private const `ABE` one; 0x00BE95F0's manifest label "UpdateWorldFrameState" is WorldFrameUpdate.
- **Header debt:** new `hdr/engine/CIDrawWorld.h` uses retail's vtable slot order; `rebuild/include/engine/CWorld.h` has
  slots 1 and 2 swapped against CWorld's vtable 0x012390DC, so the two must not meet in one TU until CWorld.h is fixed.
- One descriptive name: the file-local `static inline ShoreLookupMask(long)` in the constructor (emits no symbol). Theme
  colour index 14 and scaler indices 23..29 are literals (no recovered enum names).

To land: `hdr/` (table in `hdr/README.md`): opt-ins `_TYPED` / `_CTOR` in CEngineWaterSettings.h, `_GAMEWORLD` in
CEngineWorldMap.h, `_SETSCALE` in CMatrix3x4.h; copies with opt-ins of c2008's CEngineComponentRenderParams.h, c2018's
CDisplayManager.h / CGraphicInfo.h (+ three unchanged c2018 files), c2021's CEngineLandscapeRenderer.h; new CIDrawWorld.h;
CIDrawEnvironment*.h from hand727 / hand723. Merge after c2008, c2018, c2021. Rebuild the oracle from Fable.exe (no retail
bytes are in this folder).
