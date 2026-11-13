# acctB hand-back, near-miss retry 3015 (1 previously parked row: matched)

Authored by an Opus 5.5 agent starting from c2032's parked source, re-verified by the acctB local coordinator. Nothing
is landed.

| address | result | notes |
|---|---|---|
| 0x00BA6FF0 CEnginePrimitiveManagerStaticMeshesBase::DrawSilhouette | RELOCATION_MATCH 1907v1907, test PASS (g++ and VC7.1) | **supersedes `2026-11-12_acctB_c2032/rows/parked/00ba6ff0.cpp`** (was DIFFER 1904v1907). `// Inline-budget:` line: FABLE_FORCEINLINE on GetAppropriateLOD (c2 score 182) and FindLocalLights (98); no `/d2inlT` fits because `~CCountedPointer` (94) must stay a call. 6 grades in this retry (g04 was a diagnostic, not a deliverable) on top of c2032's 5 |

What changed against the parked source (I diffed them):

1. `CalcDistanceFromCamera` is the one-line `(position - GetCurrentCamera().PeekCameraPos()).GetMagnitude()` -- the same
   body as in the matched 0x00B9A4A0 (r3013), replacing c2032's running sum. **This removes the header conflict c2032's
   README reported** (one body now serves both rows).
2. The LOD selection goes through the inline `CEnginePrimitiveManagerMeshBase::GetAppropriateLOD(mesh_info, distance,
   radius, tolerance)` with a ternary clamp; expanded, it puts the radius chain after the fsqrt as retail (0x00BA7119).
3. The three render-info stores go through a named `CEngineRenderInfoStaticMesh* render_info = primitive.GetRenderInfo();`
   placed after the GetMesh check, so GetMesh re-reads MeshInfo (0x00BA7184) and the LOD walk / FindLocalLights registers
   fall into place. The SetStencilFunc `test` / `or` operand order (0x00BA753D) came right as a side effect.

Both inline bodies were checked for free against retail's unreferenced out-of-line copies in the RepeatedStaticMeshes TU,
none of which is in the manifest (LABELS.tsv): **0x00B9C210 GetAppropriateLOD, 0x00B9C280 CalcDistanceFromCamera,
0x00B9C2B0 CalcDistanceFromCameraSquared** -- the agent reports exact byte matches when the bodies are compiled out of
line; I did not regrade those (no oracle rows for uncatalogued addresses on the acctB lane).

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade, both test runs,
read-only vland: 1 win, no RELOC_IDENTITY / RELOC_CONFLICT; RELOC_NAME on 0x00B90090 and 0x0129BA3C, both in NAME_FIXES.tsv).
Purity and padded-struct greps are clean; `hdr/` is byte-identical to c2032's overlay apart from README.md.

Review items for landing:

- **GetAppropriateLOD's body is defined in the row** (declared by the existing opt-in
  `FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_APPROPRIATELOD`): move it into the shared header at landing. The landed
  SetupMesh 0x00BCF2E0 defines GetAppropriateLOD locally with `errorTolerance *=` and says Fable.exe has no out-of-line
  copy; the copy is 0x00B9C210. That row still matches, but the header should carry one body.
- Descriptive: all locals; GetAppropriateLOD's parameter / local names; the BODIES of GetBoundingSphereRadiusWorldSpace,
  FindLocalLights, GetVisibilityRatiosValid, GetShowSilhouettes, GetLODErrorTolerance,
  GetLODVirtualScreenSizeFOVAdjusted (names are PDB spellings, bodies inferred from retail); `(ERenderMode)2`.
- NAME_FIXES.tsv: 0x00B90090 `GetSilhouetteColour` (see c2032); 0x0129BA3C `?DELTA_NOTIONAL_ZERO@@3MB`, labelled EPSILON
  in the catalog (evidence: C2DVector.h + 153 landed rows, no PDB name set).

To land: `hdr/` = c2032's overlay (which builds on c2028's). Rebuild the oracle from Fable.exe (no retail bytes are in
this folder).
