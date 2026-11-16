# NF44 header overlay (round 6)

Each file is a copy of the CURRENT repo header (`rebuild/include/<same path>`) with `#ifdef` opt-ins added. `diff rebuild/include/<file>
hdr/<file>` shows every change. One exception is called out below (CEngineMap.h): nine body lines read the global through a selector
macro that expands to the old spelling unless the new opt-in is defined, so every existing includer gets the same tokens.

| Overlay file | Opt-in added | Used by row(s) | What it adds |
|---|---|---|---|
| engine/CEngineMap.h | `FABLE_RETAIL_CENGINEMAP_GPENGINEWORLDMAP` | 0x00B3F5E0, 0x00B40BD0, 0x00B52DD0 | Inside the existing `FABLE_RETAIL_CENGINEMAP_PEEK_BODIES` block: with the opt-in, `namespace NEngineInternal { extern CEngineWorldMap* GPEngineWorldMap; }` (Ego_r.pdb / FableWin.pdb `?GPEngineWorldMap@NEngineInternal@@3PAVCEngineWorldMap@@A`, 0x01436E8C). Without it, the old `extern CEngineWorldMap* g_pEngineWorldMap;`. Either way it defines `FABLE_CENGINEMAP_WORLDMAP` (the selected global); the nine `g_pEngineWorldMap->` reads in the PeekThemeId / PeekThemeBlend / PeekLandscapeHeight bodies now read `FABLE_CENGINEMAP_WORLDMAP->`, which is token-identical without the opt-in. |
| engine/CGPUScratchBufferTextureManager.h | `FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_FREEMEMORYPAGE` | 0x00B5BFD0, 0x00B5C050, 0x00B5EE00, 0x00B5EE80, 0x00B870A0, 0x00B898D0, 0x00BABE60, 0x00B86B00, 0x00B6D600 | declaration `void FreeMemoryPage(long page);` (Ego_r.pdb `?FreeMemoryPage@CGPUScratchBufferTextureManager@@QAEXJ@Z`, retail 0x00F37030) |
| engine/CGPUScratchBufferTextureManager.h | `FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_ENDTEXTUREUSE` | 0x00B5BD30, 0x00B5EA30, 0x00B5EC90, 0x00B6D630, 0x00B869A0, 0x00B86A30, 0x00B89140, 0x00BAC0F0 | declaration `void EndTextureUse(long handle);` (Ego_r.pdb `?EndTextureUse@..@@QAEXJ@Z`, retail 0x00F36F00). It is skipped when `_TEXTURES` is also defined, because that opt-in declares the same function. |
| engine/CGPUScratchBufferTextureManager.h | `FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_SURFACEUSE` | 0x00BAC0F0 | declarations `class CSurface* StartSurfaceUse(long);` / `void EndSurfaceUse(long);` (Ego_r.pdb `?StartSurfaceUse@..@@QAEPAVCSurface@@J@Z` 0x00F36F10, `?EndSurfaceUse@..@@QAEXJ@Z` 0x00F36F30) |
| engine/CEngineOcclusionManager.h | `FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_GETRESULT` | 0x00B2F8E0 | In the default (generated) branch: declaration `unsigned long GetResult(unsigned long handle, bool wait);` (Ego_r.pdb `?GetResult@CEngineOcclusionManager@@QAEKK_N@Z`, retail 0x00B8D0B0 `xor eax,eax ; ret 8`) |

Used without change (repo headers, existing opt-ins): engine/C2DVector.h (`FABLE_RETAIL_C2DVECTOR` where `operator+=` is needed),
engine/NUISystem_CUIState.h, stlport/fable_map.h (`FABLE_RETAIL_STLP_MAP`, `FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY`),
engine/CEngineResourceManager.h (`FABLE_RETAIL_CENGINERESOURCEMANAGER`), the existing `_STARTTEXTUREUSE` opt-in of
CGPUScratchBufferTextureManager.h, engine/CRenderManager.h, engine/CString.h (`FABLE_RETAIL_CSEGMENTFILE_PASS23`),
engine/CTCInventoryWeapons.h (`FABLE_RETAIL_CTCINVENTORYWEAPONS_METHODS`), engine/CEngineShadowRenderer.h (`_MEMBERS`), and
libpk/PKCBufferedReader.h (NF42's `_RETURNALLBUFFERS`, already landed).

To merge: add each `#ifdef` block to the repo header at the same place. For CEngineMap.h, also take the nine body-line edits.
