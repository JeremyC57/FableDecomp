# NF152 header overlay (2026-11-20)

Bases: orig/engine/CEngineResourceManager.h and orig/engine/CEngineShadowRenderer.h, byte-equal to the repo working tree when copied
(re-checked with cmp at hand-back). Only those two headers are overlaid. NF149 overlays CEngineLandscapeMemoryManager.h /
CVertexBufferWin32.h and NF151 overlays CRenderStateManager.h, so no member is declared twice. The hand-backs landing next (NF147 / NF148 / NF150 / RP1; HELD.txt) overlay neither header, so no merge is
needed.

## engine/CEngineResourceManager.h (retail branch, after GetVertexBufferUsage)
- FABLE_RETAIL_CENGINERESOURCEMANAGER_ALLOCATEEXTERNALTEXTUREFROMRESOURCEPOOLS: declares
  `CTexture* AllocateExternalTextureFromResourcePools(unsigned long, const C2DExtentsI&, long, const CPixelFormat&)` (0x00B3B1F0, Ego_r + FableWin).
  User: rows/00b3b1f0.cpp (+ test).
- FABLE_RETAIL_CENGINERESOURCEMANAGER_FREEEXTERNALTEXTUREFROMRESOURCEPOOLS: declares `void FreeExternalTextureFromResourcePools(CTexture*)`
  (0x00B3B5B0). User: rows/00b3b5b0.cpp (+ test).
- Declarations only, elaborated type specifiers (no new namespace-scope declarations, no layout change).

## engine/CEngineShadowRenderer.h (the FABLE_RETAIL_CENGINESHADOWRENDERER_MEMBERS class head only, after the constructor)
- FABLE_RETAIL_CENGINESHADOWRENDERER_ONRELEASEDEFAULTPOOLRESOURCES: `virtual void OnReleaseDefaultPoolResources();` (the CEngineComponent
  override, 0x00B4B830, both PDBs UAEXXZ; the slot is the base's, so no layout or vtable-size change). User: rows/00b4fc10.cpp (+ test).
- FABLE_RETAIL_CENGINESHADOWRENDERER_CHANGEVIDEOOPTIONSHADOWQUALITY: `bool ChangeVideoOptionShadowQuality(long, float);` (0x00B4FC10).
  User: rows/00b4fc10.cpp (+ test).
- FABLE_RETAIL_CENGINESHADOWRENDERER_XBOXDEALLOCATESHADOWBUFFER: `bool XBoxDeallocateShadowBuffer();` (0x00B4B890). User: rows/00b4b890.cpp.
- FABLE_RETAIL_CENGINESHADOWRENDERER_XBOXREALLOCATESHADOWBUFFER: `void XBoxReallocateShadowBuffer();` (0x00B4B8A0). User: rows/00b4b8a0.cpp.
- The four exist only in the _MEMBERS class head (the retail branch's opaque `#else` head and the generated default are untouched); every
  user defines _MEMBERS.

## Checks
- pin_asserts.py run after each edit: pinned 0 for both headers (no FABLE_STATIC_ASSERT moved).
- Token-identical without the new macros: deleting the six `#ifdef ... #endif` blocks from hdr/ gives the orig/ file byte for byte (checked).
- Landed includers (grep -rlE 'engine/<header>' rebuild/src/compiled): CEngineResourceManager.h 152 (work/includers_CEngineResourceManager.txt),
  CEngineShadowRenderer.h 39 (work/includers_CEngineShadowRenderer.txt). None of them,
  nor any test or header, defines any of the six new macros, so the opt-ins change none of them.
- Every FABLE_RETAIL_* macro used in rows/ resolves in rebuild/include or hdr/ (14 macros).
- No FABLE_FORCEINLINE added. Existing members unchanged (no header claim needed for these opt-ins).
