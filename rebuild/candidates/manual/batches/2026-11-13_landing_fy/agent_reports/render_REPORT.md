# render group: acctB port report
All 9 rows are STAGED. acctb_stage_verify.py $WS: 9/9 OK (RELOCATION_MATCH + PASS) against the final $WS/inc.

00aed600 STAGED. fix/stlport/fable_containers.h = the current $WS/inc copy. The row already uses the landed RESIZE_ONE block; hdr=0, no header change.
00b2b180 STAGED. Hand engine/CRenderStateCaptureBlock.h (INLINE_DTOR in the non-INLINE class, next to CAPTURE_INLINE). The auto-merge renamed the CSurface.h guard to `!defined(..._LOADGRAPHICFRAME_B)`; I restored the repo name LOADGRAPHICFRAME (it declares the same CopyFromSurface).
00b891b0 STAGED. Hand CRenderStateCaptureBlock.h: INLINE_CTOR added next to INLINE_DTOR.
00b2cb40 STAGED. Hand GFGetNextPowerOfTwo.h: GFGETTOPBIT_FORCEINLINE as `#ifdef X / FABLE_FORCEINLINE <sig> / #else / inline <sig> / #endif`, which keeps the default text exact. The headers b180 already merged went into fix/ as current $WS/inc copies.
00b2d330 STAGED. Same as cb40, using the keep-names driver.
00bea110 STAGED. Hand CGraphicInfo.h (+GETFRAMECOUNT) and CDisplayManager.h (+PEEKBACKBUFFERRESOLUTION). CEngineWorldMap.h stays the repo's: the repo GAMEWORLD has the same PeekGameWorld. The row source drops `#include "engine/CIDrawEnvironmentParams.h"`, because the repo's CIDrawEnvironment.h defines that struct. Keep-names driver.
006b42f0 STAGED. Hand CEnvironment.h: the repo's UPDATE conditions became `defined(UPDATE)||defined(TICKSTATES)` (the same CEnvironmentGameTickState); TICKSTATES additionally types InterpolatedEnvironmentParams as CIDrawEnvironmentParams; added the SPELLEFFECTINTERFACE accessor. Hand CIDrawEnvironment.h: CIDRAWENVIRONMENTPARAMS_SCREENEFFECTS (CThemeChannel ctor/dtor declarations, ScreenEffectParams +0x60, size 0xC0). That header is not in the overlay set, so the driver ignored fix/; I applied it to $WS/inc by hand.
00b64fa0 STAGED. hdr=0: the row matches today's headers as they are. The DIFFER came from re-merging an overlay that was already merged, and from false _B renames of CGRAPHICINFOFRAME_TOUCHFRAME/_GETBANK_DIRECT.
00b72180 STAGED. Same as 00b64fa0, plus 4 auto-merged headers (CEngineWaterRenderer, CGPUScratchBufferTextureManager, CScopedShadersEnable, CTexture).

Manual $WS/inc edits after staging (all re-verified):
- Removed the orphan new engine/CIDrawEnvironmentParams.h (brought by bea110 and 006b42f0). It duplicates the struct in the repo's CIDrawEnvironment.h.
- CGPUScratchBufferTextureManager.h: _STARTTEXTUREUSE is now `&& !defined(_TEXTURES)` (the same StartTextureUse declaration).
- CTexture.h: in _WATERDYNAMIC, GetNoLevels sits under `#ifndef CTEXTURE_LEVELS`; _GENERATEMIPMAPS is now `&& !defined(CTEXTURE_LEVELS)` (duplicate declarations).

Opt-in renames: FABLE_RETAIL_CRGBCOLOUR_GETD3DCOLOR -> _B (rows b180, d330). The repo body is a byte pack. Retail at 0x00B2B4BA loads the dword. I tried the repo body and it DIFFERs. REVIEW: two inline spellings of one function.
I suppressed every other auto-rename as false. They were names the earlier row of the same overlay had already merged, or `#ifndef X/#define X` guards.

Tooling: the driver renames opt-ins against $WS/inc before it reads fix/, so the second row from an already-merged overlay gets false _B renames. $WS/tools/acctb_stage_keep.py is a copy of the driver with one change: names in ACCTB_KEEP_NAMES are skipped by the rename pass (logged as kept-name:). All other checks are unchanged. I used it for d330, bea110, 00b64fa0 and 00b72180.

Review:
- SURFACECOMPOSITION branches duplicate the STATICBLOCKS models (overlay debt).
- New headers have landed local-model twins: CEngineTextureMorph, CCompositeSurfaceCache, CBankStateBlock, CGraphicDataBankInit, CSurfaceCompositor.
- In 006b42f0 the nested tick-state class now carries UPDATE's operator= declaration, which is harmless here.
- I rechecked the landed users of CEnvironment.h/CIDrawEnvironment.h (133 rows, including 006b3ff0). Every result in the 40 lines of output I read was a MATCH; the rest of the output was cut off by my `tail -40`, so I did not see it.
