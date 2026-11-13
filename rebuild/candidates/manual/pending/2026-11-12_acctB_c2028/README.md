# acctB hand-back, chunk 2028 (2 reserved rows: 1 matched, 1 parked after pass 1)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x00B8DF50 CEngineOcclusionManager::FrameEnd(const CEngineComponentParams&) | RELOCATION_MATCH 1626v1626, test PASS (g++ and VC7.1) | **true length 1626, not the reserved 1841** (SPLITS.tsv): `ret 4` at 0x00B8E5A7; 0x00B8E5B0..0x00B8E680 are the TU's unreferenced inline COMDATs (vector<COccluderFlareEntry> size / empty / begin / end and four tiny helpers, no callers) -- uncatalogued, for the coordinator. Grade `retail_asm`: one bare `fistp dword` at 0x00B8E357 through the existing `engine/GFFloatToLongNear.h`, with a `// Retail-asm:` line and a per-site attempts table in the row. 8 grades |
| 0x00B9A4A0 CEnginePrimitiveManagerRepeatedStaticMeshes::RenderPrimitive | parked, DIFFER 1674v1674 | **one frame byte**: retail 0x00B9AA25 `mov byte ptr [esp+1Bh],cl` / 0x00B9AA56 `mov dl,byte ptr [esp+1Bh]`, ours `[esp+18h]` at both (the inlined CalculateSubPrimitiveRenderFlags stores its `translucent` parameter); the other 510 of 512 instructions are exact. 8 grades in pass 1; **passes 2 and 3 not used (agent context), no behaviour test written**. Per-helper `FABLE_FORCEINLINE` devices with an `// Inline-budget:` line (scores 80-534 expanded v 76 / 91 called; no single `/d2inlT` fits). Source in `rows/parked/`. Queued for the acctB near-miss agent |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade, both test runs,
read-only vland: 1 win, no RELOC_IDENTITY / RELOC_CONFLICT; RELOC_NAME on 0x0129BA3C, catalog label `EPSILON` v the
established `DELTA_NOTIONAL_ZERO`). Purity and padded-struct greps are clean. Parked row regraded: DIFFER(1674v1674).

Review items for landing:

- **One inline body, two retail contexts (header debt, NOTES.md):** FrameEnd needs `CMatrix3x4::TransformPoint(const&, &)`
  with rows accumulated Z term first (opt-in `_TRANSFORMPOINT_COPY_SUMS`); the repo default (E13*x first) is what another
  landed row needed. The parked row's box-distance sum is Z, Y, X where landed 0x00B9C2E0 has Z, X, Y. Possibly the
  per-function symbol-count key documented in r3004 -- not tested here.
- **Global spellings:** 0x01436EA0 is `g_pEngineCamera` here, `NEngineInternal::GPCamera` in c2018 / c2024; 0x01436E78
  `g_pPrimitiveRendererSharedData` has no queue decision. One address, one name: pick at landing.
- `hdr/engine/CIVCountedPointer.h` adds `#include "fable_compiler.h"` at the top -- the one change outside an opt-in block
  (needed for FABLE_FORCEINLINE; only the parked row uses it).
- NAME_FIXES.tsv (4): 0x00B2FC50 `CEngineCamera::RefreshD3DTransforms` (bound as `CameraMethods`); 0x00A23C50
  `C3DMeshInfo::BeginLoadingTextures`; 0x00B98B10 `CEngineSubPrimitiveBoundingBox` ctor; 0x00B2FAC0 GetOccludedFrames
  signature (FableWin only -- check Ego_r before respelling). Descriptive: ERenderMode / EBankLoadPriority enumerators
  (forward-declared enums used with casts); `PeekNonHomogenousViewToViewScaleX/Y` bodies inferred from the names.

To land: `hdr/` (table in `hdr/README.md`): new retail branches for CEngineOcclusionManager (`_MEMBERS`) and the
RepeatedStaticMesh classes, three new headers, opt-ins in copies of CEngineCamera (c2008 copy), CMatrix3x4,
CEngineStateBlocks, CRenderManagerCore, CEnginePrimitiveRenderUtil / C3DMeshInfo (c2011 copies), and the c2024 shadow
headers -- merge after c2008, c2011, c2018, c2024. Rebuild the oracle from Fable.exe (no retail bytes are in this folder).
