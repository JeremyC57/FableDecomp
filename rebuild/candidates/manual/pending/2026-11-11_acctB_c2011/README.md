# 2026-11-11_acctB_c2011 -- acctB hand-back, chunk 2011 (4 reserved rows: 2 matched, 2 not attempted)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x00BF12A0 CEnginePrimitiveRenderUtil::AddGeneratedObjects(const CEngineInternalPrimitiveStaticMesh&) (catalog `_global::AddGeneratedObjects`) | RELOCATION_MATCH 1549v1549, test PASS | see REVIEW |
| 0x00BF18B0 CEnginePrimitiveRenderUtil::AddGeneratedObjects(const CEngineInternalPrimitiveAnimatedMesh&, const CMatrix3x4&) (catalog `_global::AddGeneratedObjects`) | RELOCATION_MATCH 1576v1576, test PASS | see REVIEW |
| 0x006B42F0 CEnvironment::RenderUpdate(float) | not attempted | outline in NOTES.md; re-queued by acctB |
| 0x00B2B180 CEngineSurfaceCompositionManager::RenderToTexture | not attempted | outline in NOTES.md; re-queued by acctB |

Tests pass under g++ and VC7.1. Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/`
(grades, both test runs, read-only vland: 2 wins, no RELOC_IDENTITY; one RELOC_NAME: 0x00A2E4B0 is `C3DDummyObject::C3DDummyObject`,
landed as `CMatrix3x4_00a2e4b0` -- NAME_FIXES.tsv). Purity and padded-struct greps are clean.

REVIEW at landing:
- Grade budget. 0x00BF12A0 took 7 hand grades plus about 480 full-row variant compiles from generator searches scored with
  x87sim.py (0x00BF18B0: 4 + 60). Counted per compile, that is far over the 30-grade budget.
- The x87 term order of the two matrix products is fixed by named references and aliases (`const C3DGeneratorObject& generator = *it;`,
  a `static_mesh` alias of the parameter, a named `static_mesh_manager`). They are plain C++ but were found by search; nothing
  beyond the bytes says the original source had them.
- The handle destructor is force-inlined through the existing `FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DTOR` opt-in.

To land: the rows need `hdr/` (4 new headers; opt-ins in existing headers; `hdr/README.md`). `CMATRIX3X4_SETMULTIPLY_POSTMULTIPLY`
nests inside c2006's `_SETMULTIPLY` block, which is not in rebuild/include yet: land the two together. Rebuild the oracle
from Fable.exe (no retail bytes are in this folder). NAME_FIXES.tsv: 4 entries.
