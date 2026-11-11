# Chunk 2018 header overlay (FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2018/hdr)

Every change is an opt-in (or a new header); the default text of every existing header is unchanged, so landed includers keep
their bytes (the only removed lines are `#ifdef X` turned into `#if defined(NEW) ... #elif defined(X)` chains). Copies were taken
from rebuild/include at 2026-11-11. Rows: 0x00B2CB40 CEngineSurfaceCompositionManager::CEngineSurfaceCompositionManager,
0x00B2B180 ::RenderToTexture, 0x00B2D330 ::UpdateDiskCacheTexture (all RELOCATION_MATCH with this overlay).

| header | opt-in / change | what | evidence |
|---|---|---|---|
| engine/CEngineSurfaceCompositionManager.h | `FABLE_RETAIL_CENGINESURFACECOMPOSITIONMANAGER_MEMBERS` (new branch ahead of the landed `_BANKS`) | the Ego_r.pdb members at their retail offsets (three CGraphicDataBank banks, the morph map, the three cache file names, ScreenCopyVertexShader +0xFB8, the tolerances, MinRenderTargetSize +0x10CC, MaxCompositeTextureSize, the 4x4 `pair<long,long>` Opaque / AlphaTextureHandles tables, Enable), the virtual destructor (landed 0x00B2D1B0) and the private members RenderToTexture / GetDimensionsAndAdjustMipmapLevel / IsNearlyIdentical / GetGraphicInfo / WriteTextureToBank / UpdateDiskCacheTexture in PDB spelling; turns on `FABLE_RETAIL_CGRAPHICDATABANK_SURFACECOMPOSITION` | struct_layouts_egor.tsv; PDB mangled names (ghidra_out PDB sets); the landed dtor's offsets; matched rows |
| engine/CGraphicDataBank.h | `FABLE_RETAIL_CGRAPHICDATABANK_SURFACECOMPOSITION` | CBankFile base at +0, nested CGraphicHeader, ctor 0x009FEA20, Initialise 0x009FD4E0, ChangeScene, FrameEnd 0x009FD660, inline GetPixelFormat (PixelFormats[11] +0x288), PeekGraphic declaration, GraphicList `CArray<CGraphicInfo>` +0x1E0; includes engine/CGraphicInfo.h after the class (GraphicList's implicit destructor needs the complete record in every includer) | Ego_r.pdb layout + names; ctor 0x00B2CF43..55 (GetPixelFormat), D330 0x00B2D344..82 (PeekGraphic) |
| engine/CGraphicInfo.h | branch under `_SURFACECOMPOSITION` (not with STATICBLOCKS) | CGraphicInfo (44 bytes), inline PeekInfoFrame / PeekGraphicHeader, inline CGraphicDataBank::PeekGraphic, CGraphicInfoFrame::GetWidth / GetHeight / IsAlpha | Ego_r layout; FableWin-only inline names; D330 0x00B2D384, 0x00B2D524, landed 0x00B2AEF0 `movzx` reads |
| engine/CGraphicInfoFrame.h | branch under `_SURFACECOMPOSITION` | CGraphicInfoFrame + nested CFrameData (public fields), IsLoaded, GetCurrentLoadedMipmapLevel, `FABLE_FORCEINLINE` GetFrame (FableWin.pdb only, no retail out-of-line copy; expanded at 0x00B2D6DB..0x00B2D8A9), InternalGetFrameForcedLoading 0x009FF6B0, BeginLoading 0x009FDE80, CFrameData(const CGraphicInfoFrame*) 0x009FE380 | Ego_r layouts; GetFrame modelled on the landed GetTexture 0x00B66C10; c2 trace score 142 vs 74 (plain inline left it called) |
| engine/CGraphicFrame.h | `FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE` | CGraphicFrameData (packed, 0x1B) with GetTexture, Uninitialise 0x009FCBA0, AddReference, `FABLE_FORCEINLINE` Release; CGraphicFrame GetTexture / copy ctor / dtor / IsInitialised / UnInitialise / `FABLE_FORCEINLINE` operator= | Ego_r layout; Release / operator= retail copies 0x0041C090 / 0x0041C0E0 have no caller, every retail user expands them; c2 trace scores 85 / 115..127 vs 74 |
| engine/GFGetNextPowerOfTwo.h | `FABLE_RETAIL_GFGETTOPBIT_FORCEINLINE` | GFGetTopBit as FABLE_FORCEINLINE | FableWin-only name; 17 retail expansions, uncalled COMDAT 0x0099FDB0; c2 scores 81 (ctor) / 125 (D330) vs 74 |
| engine/CBankFile.h | `FABLE_RETAIL_CBANKFILE_ENTRIES`, `FABLE_RETAIL_CBANKFILE_SETISSTATICMAP` | FindIndexBySymbol 0x009CCDC0, AddEntry 0x009CD620; SetIsStaticMap 0x009D5230 (established descriptive name, empty `ret 4`) | Ego_r.pdb names; D330 / ctor call sites |
| engine/CDisplayManager.h | `FABLE_RETAIL_CDISPLAYMANAGER_ALLTEXTUREFORMATS` | the six GetBest*TextureFormat members with `CPixelFormat&` | Ego_r.pdb signatures + donor order; ctor 0x00B2CB40 |
| engine/CEngineResourceManager.h | `FABLE_RETAIL_CENGINERESOURCEMANAGER_CACHEPATHS` | PeekEngineCachePath / PeekWriteableEngineCachePath (+0x2AD0) | Ego_r / FableWin names; ctor expansion |
| engine/CGPUScratchBufferTextureManager.h | `FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_TEXTURES` | AllocteMemoryPage 0x00F37660, AllocateTexture 0x00F37440, StartTextureUse 0x00F36EE0, EndTextureUse 0x00F36F00 | Ego_r.pdb names (PDB spelling `Allocte`), donor order |
| engine/CGraphicBankStateBlock.h | `FABLE_RETAIL_CGRAPHICBANKSTATEBLOCK` | CGraphicBankStateBlock : CBankStateBlock (vtable 0x0129C888) with its inline constructor chain and virtual GetDefault (slot 1) | RTTI base chain; D330 0x00B2D456 vtable store and the slot-1 call |
| engine/CRenderManagerCore.h | `FABLE_RETAIL_CRENDERMANAGERCORE_WAITWHILEGPUISBUSY` | WaitWhileGPUIsBusy 0x00A0AF80 | Ego_r.pdb name + donor order, empty body |
| engine/CRenderStateCaptureBlock.h | `FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE_DTOR` | inline destructor (RestoreCaptureBlock 0x00A05840) | B180 0x00B2B5F5..0x00B2B655 expansion |
| engine/CSurface.h | `FABLE_RETAIL_CSURFACE_COPYFROMSURFACE` | CopyFromSurface 0x009F37E0 | Ego_r / FableWin `?CopyFromSurface@CSurface@@QAEXPBVC2DBoxI@@ABV1@0K@Z` |
| engine/CTexture.h | `FABLE_RETAIL_CTEXTURE_GENERATEMIPMAPS` | GenerateMipmaps 0x009F9E60 | Ego_r.pdb `?GenerateMipmaps@CTexture@@QAEXKK@Z` |
| engine/CTextureManager.h | `FABLE_RETAIL_CTEXTUREMANAGER_CLASS` | the polymorphic retail class (ctor 0x00A6A360) for the member at its offset | RTTI .?AVCTextureManager@@; ctor call in 0x00B2CB40 |
| engine/CRGBColour.h | `FABLE_RETAIL_CRGBCOLOUR_GETD3DCOLOR` | inline GetD3DCOLOR | Ego_r / FableWin name; B180 expansion |
| engine/CGraphicDataBankInit.h (new) | default | CGraphicDataBankInit (Ego_r layout, inline ctor; out of line at 0x00415B80 elsewhere) | Ego_r layout; ctor expansion |
| engine/CEngineTextureMorph.h (new) | default | CEngineTextureMorph (OriginalTextureIndex, MorphID, Blends vector<CCompositeBlendDesc> +0x08) | Ego_r layout; B180 / D330 reads |
| engine/CCompositeSurfaceCache.h (new) | default | CCompositeSurfaceCache (RTTI base chain) with GetCurrentMorph, BankFileIndex, MipmapLevel, TimeStamp | RTTI + Ego_r layout; D330 stores |
| engine/CSurfaceCompositor.h (new) | default | CSurfaceCompositor (0x30) + nested CVertexType (0x18) with inline SetPosition / SetU1V1; ctor / StartComposition / CompositeQuad / EndComposition out of line | Ego_r layout + names, RTTI vtable 0x01301CF0 |
| engine/CBankStateBlock.h (new) | default | CBankStateBlock (RTTI, vtable 0x0129C82C) | RTTI; CGraphicBankStateBlock base |

Header debt: CGraphicDataBank / CBankFileAsync / CResourceBank members between +0x110 and +0x1E0 and after +0x2B4 stay opaque;
the SURFACECOMPOSITION CGraphicDataBank / CGraphicInfo / CGraphicInfoFrame branches duplicate parts of the STATICBLOCKS models and
should be merged with them at landing. The landed sources of 0x00B2AEF0 / 0x00B2B050 / 0x00B2B790 / 0x00F38990 / 0x00F39A50 use local
models and non-PDB signatures (NAME_FIXES.tsv).
