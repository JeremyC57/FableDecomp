# 2026-11-11_acctB_c2024 -- acctB hand-back, chunk 2024 (2 reserved rows: 1 matched, 1 parked)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x00B891B0 CEngineScreenEffectOutlineGlow::CEngineScreenEffectOutlineGlow(const CEngineInit&) (vtable 0x012A3B6C, sole caller CEngine::Initialise after `new 0x1E4`) | RELOCATION_MATCH 1819v1819, test PASS (g++ and VC7.1) | plain C++, no device. 4 grades |
| 0x00B4FC40 CEngineShadowRenderer::SetupShadowFuncRenderStates(long, const CMatrix3x4&, const CColorBufferDepthPackParameters&) | parked, DIFFER 1837v1837 | 454 of 457 instructions equal; only the x87 term order of one 4-term sum differs (column 0 of the expanded CMatrix4x4::PostMultiply, retail 0x00B5023F..0x00B5025E: retail `M2*m20 + m30*M3 + m10*M1 + m00*M0`, ours `M2*m20 + m00*M0 + m30*M3 + m10*M1`; columns 1-3 identical). 16 grades (passes 1 and 2); **pass 3 not used** (agent context). Source + passing test in `rows/parked/`; the source carries `// Flags: /d2inlT314` (PostMultiply scores 313) -- grade it with that flag. Queued for an acctB retry |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade, both test runs,
read-only vland: 1 win, no RELOC_IDENTITY / RELOC_CONFLICT). Purity and padded-struct greps are clean (I reworded one
comment in rows/00b891b0.cpp that contained the word "register" and regraded: RELOCATION_MATCH). Parked row regraded with
its flag: DIFFER(1837v1837), as recorded.

Review items for landing:

- **Global 0x01436E98:** the row spells it `g_pResourceManager` (the spelling reloc_bindings.tsv binds and the sibling
  glow constructor uses); c2018 used the `NEngineInternal::GP*` family for the same address. No PDB data symbol either way:
  one address, one name -- pick at landing.
- **NAME_FIXES.tsv (3):** 0x00B4FC40 and 0x00B50510 are the two SetupShadowFuncRenderStates overloads (const-reference
  spellings from Ego_r + FableWin; the landed 0x00B50510 source binds a pointer-argument spelling to 0x00B4FC40);
  0x00B89080 is `?FinishWithDynamicTextures@CEngineScreenEffectOutlineGlow@@AAEXXZ`, landed as `ReleaseTextures`.
- **Agent's claim, not checked by me:** landed rows 0x00BEAAA0 and 0x00BEB4D0 are labelled CEngineScreenEffectOutlineGlow
  but store vtables 0x012A8134 / 0x0129B860, not OutlineGlow's 0x012A3B6C.
- LABELS.tsv: 12 names, including the shadow TU's unreferenced inline copies (CMatrix4x4 Initialise 0x00B50770, ctor
  0x00B50AE0, PostMultiply 0x00B507E0, accessors). The EShadowRenderingTechnique enumerator names are descriptive.

To land: the row needs `hdr/` (opt-ins `_MEMBERS` in CEngineScreenEffectOutlineGlow.h; CRenderStateCaptureBlock.h and
CGPUScratchBufferTextureManager.h start from the c2018 copies, CRenderTarget.h from c2008 -- merge after those; the
CEngineShadowRenderer / CMatrix4x4 / CEngineLightingManager / CShadow* files are only needed by the parked row). Evidence per
block in `hdr/README.md`. Rebuild the oracle from Fable.exe (no retail bytes are in this folder).
