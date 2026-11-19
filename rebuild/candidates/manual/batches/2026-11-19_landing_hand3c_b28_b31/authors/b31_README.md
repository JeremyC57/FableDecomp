# b31 (hand3c chunk 1, Account B, 2026-11-19): 0x00B53F60 CEngineWeatherRenderer::FrameStart -- READY

- Grade: RELOCATION_MATCH 411v411, purity retail_asm (same-TU GFFloatToLongFloor callers; FrameStart's own bytes carry no asm).
  Strict vland `RELOCATION_MATCH PASS` (LANDVERIFY_STRICT_HEADERS=1), no warnings, run twice: with the workspace overlay
  (logs/vland_strict.log) and with FABLE_HDR_OVERLAY = this batch's hdr/ only (logs/vland_strict_batch_hdr.log), so the four headers here
  are sufficient. 2 graded attempts in ATTEMPTS (baseline draft DIFFER 418v411, then this); in addition the probe harnesses compared
  probes/f0, fA..fF and the /d2inlT93 / T94 / T422 / T423 sweeps (and the parked retry1, 418v411) against the oracle. Flags: `/d2inlT94`. Leaf
  ?FrameStart@CEngineWeatherRenderer@@UAEXABVCEngineComponentParams@@@Z.

## Identity
Ego_r.pdb / FableWin.pdb ?FrameStart@CEngineWeatherRenderer@@UAEXABVCEngineComponentParams@@@Z (Ego_r 0x009F7360, object order 38 of
Release\Engine_Weather_Renderer.obj). Ego_d.pdb locals: cparams; interpolate (const float); mist block {old_mipmap_delay,
old_reuse_busy, graphic_bank (CGraphicDataBank&), index}. The Debug Engine_Weather_Renderer.obj COMDATs name every inline the body
uses: CTextureManager::Get/SetReuseBusyTextures, CGraphicDataBank::GetReduceSizeFrameDelay / GetVolumeTexture.

## What closed it (measured)
1. Same TU: retail keeps `this` in ecx across `call UpdateBoxPositions` (0x00B53F77). VC7.1 does that only when it has already
   compiled the callee (and so knows it leaves ecx alone). Ego_r compiles BuildBoxPositionMatrix (order 14) and UpdateBoxPositions (34)
   before FrameStart (38); this batch defines both ahead of the row: BuildBoxPositionMatrix = b30's text, UpdateBoxPositions = a
   RESPELLING of the landed 0x00B52990 row (same statements on the shared Settings members, Ego_d names, Ego_d block scope
   `if (!ParticleBoxLocked) { ... }`), not its verbatim text. The coordinator must choose one text for 0x00B52990 (this one also
   matches there) and must land the three rows so the definitions do not collide (one TU, or the callee rows re-pointed). Both are RELOCATION_MATCH in this TU too (0x00B51AD0 969, 0x00B52990 406-byte body).
   Without them `this` moves to esi (418v411).
2. Inline budget: CGraphicDataBank::GetVolumeTexture (PeekGraphic's range test 0x00B540A3..0x00B540D5, then CGraphicInfo::GetVolumeTexture
   0x009FFB20) is expanded in retail. c2 trace: c1 = 82, site score 93, so at the default threshold 74 it stays a call (358v411).
   /d2inlT93 DIFFER 358v411; /d2inlT94 .. /d2inlT422 RELOCATION_MATCH; /d2inlT423 DIFFER 798v411 (UpdateBoxPositions expands, score 422).
   Smallest N = 94 (owner rule 2026-11-08, preference 2; source untouched).
3. FITTED (among equivalents): the mist block reads `index` (g_pEngineEnvironmentDef->MistAlphaGraphicPC) before taking the bank: retail
   loads the environment def into ecx before the resource manager into edx (0x00B5406B / 0x00B54071); every bank-first order swaps the
   two registers (still 411 bytes, not a match).

## Overlay headers (copies of TODAY's repo headers + new opt-ins; diffs alongside)
- CEngineWeatherRenderer.h: as b30 (BOXMATRIX) + FABLE_RETAIL_CENGINEWEATHERRENDERER_FRAMESTART declares the private UpdateBoxPositions
  and `virtual void FrameStart(const CEngineComponentParams&)`. No layout change.
- CEngineWeatherSettings.h: as b30 (BOXCENTER, C3DCoordI ParticleBoxCenter; asserts pinned).
- C3DCoordI.h: NEW (as b30).
- CTextureManager.h: FABLE_RETAIL_CTEXTUREMANAGER_REUSEBUSY adds the static FASTCALL inlines GetReuseBusyTextures / SetReuseBusyTextures
  (FableWin.pdb ?GetReuseBusyTextures@CTextureManager@@SI_NXZ / ?SetReuseBusyTextures@CTextureManager@@SIX_N@Z) and the private static
  `bool ReuseBusyTextures` (0x013D28E8). No layout change.
Other opt-ins used are existing repo ones (CEngineComponentParams INTERPOLATIONINFO, C3DVector INTERPOLATE, CEngineResourceManager
GETGRAPHICBANK, CGraphicDataBank PASS22 / PASS23, CResourceList, CGraphicInfo GETVOLUMETEXTURE, CEnvironmentDef PARENT, ...).
CGraphicDataBank::PeekGraphic / GetVolumeTexture are defined `inline` in the row TU (with these opt-ins no shared header supplies them).
Evidence for the inline bodies the /d2inlT94 score depends on (odsize.py, /Od /Gy /EHsc vs the Debug Engine_Weather_Renderer.obj
COMDAT sizes): GetVolumeTexture 40 = 40 (`return PeekGraphic(index)->GetVolumeTexture(texture, level);`), GetReduceSizeFrameDelay 20 = 20,
GetReuseBusyTextures 10 = 10, SetReuseBusyTextures 19 = 19. PeekGraphic is not a COMDAT of the weather object (Debug ui_sprite.obj
carries it, 620 bytes /Od, so the debug original has debug-only checks); its release text `index < GraphicList.size() ?
&GraphicList[index] : 0` is the established shared form (engine/CGraphicInfo.h's other branch; landed 0x00BB0970, 0x00B1DC70, ...)
and matches retail 0x00B540A3..0x00B540D5. Not tried: the range test written directly inside GetVolumeTexture (it would contradict
the 40-byte debug forwarder).

## Test (test/00b53f60.cpp)
Links the real row object (with the same-TU bodies). Defines GPCamera, GPResourceManager, g_pEngineEnvironmentDef and
CTextureManager::ReuseBusyTextures; recording doubles for SetReduceSizeFrameDelay (0x009FD100 semantics) and
CGraphicInfo::GetVolumeTexture. Native-layout images, offsets cited. Reference model: box rebuild only when raining or snowing, not
locked, and the camera moved half a span (floored) on an axis, with b30's snap model (Z kept when Z-locked); time passed and wind
interpolated; MistVolumeTexture cleared; with mist exactly delay 0 -> GetVolumeTexture(&slot, 0) on graphic[index] (null when out of
range, unsigned compare) with busy reuse on -> delay and reuse restored. 16 targeted + 3000 random. Prints `00b53f60_TEST PASS`.
Mutation check: 29 one-decision mutants (FrameStart, UpdateBoxPositions, BuildBoxPositionMatrix, GetBoxCoord, PeekGraphic), 29 KILLED
(round 1: 27; round 2: `diff_Y >` and dropped `fabs`).

## Names
- NEW LABEL (LABELS.tsv): 0x013D28E8 ?ReuseBusyTextures@CTextureManager@@0_NA (Ego_d carved PUB32 + the FableWin / Ego_d accessors).
- 0x01436E98 NEngineInternal::GPResourceManager: already labelled GPResourceManager (landing 2026-11-12ef); no fix.
- 0x01436E24 g_pEngineEnvironmentDef: established descriptive label (no PDB name), used as is.
- 0x01436EA0 GPCamera: filed in b30 NAME_FIXES.

## Probes
probes/f0.cpp, fA..fF.cpp (statement-order / inline variants), pp.py / pp2.py (FLAGS / ORACLE harnesses), oracle_extra.tsv
(0x00B52990 / 0x00B51AD0 oracle rows used to grade the same-TU bodies).
