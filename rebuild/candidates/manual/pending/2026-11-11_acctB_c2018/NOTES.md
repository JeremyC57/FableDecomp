# Hand chunk 2018 (CEngineSurfaceCompositionManager .. CEngineSurfaceCompositionManager): 3 of 3 byte-exact

All grades were taken with `FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2018/hdr` (hdr/README.md lists every overlay change; all
are opt-ins or new headers, the default text of every existing header is unchanged). Row extents were checked from Fable.exe:
all three catalog lengths are right (first instruction the prologue, last the final `ret` / `ret 1Ch` / `ret 0Ch`), no SPLITS.tsv.
Every call target of the three matched objects was checked against retail with tools/rel.py and the read-only vland.

## What matched (3, all RELOCATION_MATCH)
- **CEngineSurfaceCompositionManager** -- 0x00B2CB40 `??0CEngineSurfaceCompositionManager@@QAE@XZ` (1637 bytes; the catalog's
  "ctor" is right: Ego_r.pdb / FableWin.pdb name, sole caller CEngine::Initialise 0x00B26360 after `operator new(0x10DC)`, both
  class vtables stored after the two base constructors). Sets the composition defaults, picks the texture formats, fetches the
  screen-copy vertex shader, builds the three disk-cache file names (comfront / comback / comuser.dat under the writeable engine
  cache path), initialises the three graphic banks and allocates the 4x4 opaque / alpha scratch texture handle tables. Shape:
  the shader resource named before the CCharString temporary; CompressedAlpha copied before CompressedOpaque; table indices as
  `GFGetTopBit(size) - GFGetTopBit(32)` (the constant site folds after c2's address CSE, keeping retail's `lea r,[y+x*4+1DFh]`);
  one page variable per table (frame-packer slot order); `<<= 1` loop steps; GFGetTopBit forced inline
  (FABLE_RETAIL_GFGETTOPBIT_FORCEINLINE; plain inline scores 81 vs 74 -- /d2inlT82 also matched and was dropped for the forced spelling).
- **CEngineSurfaceCompositionManager** -- 0x00B2B180 `?RenderToTexture@...@@AAEXAAVCTexture@@ABVC2DExtentsI@@_N2ABVCEngineTextureMorph@@ABV?$CArray@VCGraphicFrame@@@@J@Z`
  (1539 bytes): with vertex shaders on / pixel shaders off, constant layout 3 and the screen-copy vertex shader, fog / specular off,
  composite each blend layer as one quad into the scratch render texture per mip level, copy into the texture's level, halve the box
  until it is below MinRenderTargetSize; fog back on, generate the remaining mipmaps, end the scratch use. Shape: the scratch
  texture named before GetSurfaceLevel; a field-by-field C2DBoxF quad (TLY first) whose constant corners fold into the vertex stores;
  the blend referenced once per layer.
- **CEngineSurfaceCompositionManager** -- 0x00B2D330 `?UpdateDiskCacheTexture@...@@AAE_NAAVCCompositeSurfaceCache@@K_N@Z`
  (1723 bytes): bank entry "<texture>-<morph>" lookup (added with a default CGraphicBankStateBlock when missing), early false for a
  nearly identical generated morph, fit below MaxCompositeTextureSize, alpha / opaque handle pair by GFGetTopBit, per-layer frame
  fetch (false when a layer is not loaded far enough), RenderToTexture, remember morph / level / time, WriteTextureToBank. Shape:
  pair-landing TU (RenderToTexture's body and a noinline GetDimensionsAndAdjustMipmapLevel body visible, so `dimensions` /
  `mipmap_level` stay unescaped); four FABLE_FORCEINLINE Lionhead header helpers (GFGetTopBit, CGraphicInfoFrame::GetFrame,
  CGraphicFrameData::Release, CGraphicFrame::operator=: FableWin-only names whose retail copies have no caller; trace scores
  125 / 142 / 85 / 115..127 vs 74); texture_index / morph_id named before the key; one ternary copying the handle pair; the layer loop
  through the `morph` reference while RenderToTexture and the map store re-read GetCurrentMorph().

## Parked for Fable 5.1 (0)
None.

## Skipped
None.

## Pair-landing
src/00b2d330.cpp defines RenderToTexture (identical body to src/00b2b180.cpp) and GetDimensionsAndAdjustMipmapLevel (landed row
0x00B2AEF0, whose landed source uses a local model and a non-PDB signature) in the same TU. Land 0x00B2B180 and 0x00B2D330 together;
when the landed 0x00B2AEF0 is retyped to the PDB signature it can share this TU.

## Names
Proven (two sources each, PDB spelling): the three rows; GetDimensionsAndAdjustMipmapLevel, GetGraphicInfo, IsNearlyIdentical,
WriteTextureToBank (private, AAE); CGPUScratchBufferTextureManager::StartTextureUse / EndTextureUse / AllocteMemoryPage /
AllocateTexture; CRenderManagerCore::WaitWhileGPUIsBusy; CGraphicDataBank::FrameEnd / Initialise; CBankFile::FindIndexBySymbol /
AddEntry; the six CDisplayManager::GetBest*TextureFormat (CPixelFormat&); CSurfaceCompositor ctor / StartComposition /
CompositeQuad / EndComposition; CCountedPointer<CGraphicBankStateBlock>(T*); std::__destroy_aux<CGraphicFrame*> (fastcall template).
Descriptive (no PDB name found): RenderToTexture's 4th parameter `unused_flag` (never read); CBankFile::SetIsStaticMap 0x009D5230
(established spelling kept); local variable names. LABELS.tsv lists every proven name.

## Label corrections
NAME_FIXES.tsv (17 rows): PAV -> AAV CPixelFormat& on the six GetBest*TextureFormat; 0x009BE830 is GetBestCompressedOpaqueTextureFormat
(not CompressedNonAlpha / _009be830); 0x00F36EE0 / 0x00F36F00 are StartTextureUse / EndTextureUse (not StartSurfaceUse /
EndSurfaceUse / CTexturePool); 0x00A0AF80 is CRenderManagerCore::WaitWhileGPUIsBusy (not CScratchRenderMethods::PrepareShutdown);
0x009CD620 is CBankFile::AddEntry; 0x00B2E340 takes CGraphicBankStateBlock (class key V); 0x00B2BD80 / 0x00B2B050 / 0x00B2AEF0 /
0x00B2B790 are private PDB signatures; 0x00F38990 / 0x00F39A50 take CSurface& / const CTexture& + CVertexType.

## Read-only vland
3 WINS (all RELOCATION_MATCH, behaviour PASS), no RELOC_IDENTITY / purity findings. Only RELOC_NAME warnings against manifest
labels: 0x00F36EE0 'StartSurfaceUse', 0x00F36F00 'Nop_ret4_00f36f00', 0x00A0AF80 'Nop_ret0_00a0af80' (all in NAME_FIXES.tsv) and
0x009D5230 'Nop_ret4_009d5230' (the established descriptive CBankFile::SetIsStaticMap is kept; no PDB name was found for it).
