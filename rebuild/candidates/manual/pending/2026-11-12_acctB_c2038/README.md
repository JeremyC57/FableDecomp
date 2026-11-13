# acctB hand-back, chunk 2038 (1 row matched)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x00BB8750 `?RenderLandscapeDecalGroup@CEnginePrimitiveManagerParticleDecalGroup@@AAEXABVCEngineInternalPrimitiveParticleDecalGroup@@_NJ@Z` | RELOCATION_MATCH 1951v1951, test PASS (g++ and VC7.1) | 4 grades; name in both PDB name sets, only caller RenderSubPrimitive 0x00BB9533 |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade, both test runs,
read-only vland: 1 win, no RELOC_IDENTITY / RELOC_CONFLICT / NOT_GENUINE; one `WARN RELOC_NAME`, item 2). Purity and
padded-struct greps are clean.

## Review items for landing

1. **Grade `retail_asm`.** The row reaches `GFFloatToLongFloor` twice and `GFFloatToLongCeil` twice, only through the
   existing shared headers (bare `fistp dword`, 0x00BB8A75..). The `// Retail-asm:` block has the per-site attempts table
   (30 Floor and 31 Ceil C++ spellings give `call __ftol2` or `fistp qword`, 12 intrinsic spellings give SSE conversions).
2. **0x01436EA0** is spelled `NEngineInternal::GPCamera` here; the label is `g_pEngineCamera`. Same open spelling question
   as c2028 / c2037; nothing proves either.
3. **Headers** (`hdr/README.md`; no default text changed, no shared inline given a second body): opt-ins added to
   `CMatrix3x4.h` (`SetIdentity`, same text as the pending c2005 block; the agent notes SetIdentity is in FableWin.pdb
   only, not Ego_r as c2005's comment says), `CEngineInternalPrimitiveParticleGroup.h` (typed branch, `Primitives` as an
   STLport list) and `CEnginePrimitiveManagerParticleDecalGroup.h` (typed branch incl. inline `ApplyLandscapeVertexShader`);
   four new headers `CEngineInternalPrimitiveParticleDecalGroup.h`, `CEngineInternalPrimitiveParticleDecal.h`,
   `CTVertexNormTex1Base.h`, `CTVertexNormTex1.h`. Inline bodies were checked against retail's unreferenced copies
   (0x00BB97F0 RELOCATION_MATCH, 0x00BB9850 MATCH, 0x00BB98A0 same instructions).
4. **Name fixes** (`NAME_FIXES.tsv`): 7 CShaderRenderManager / CRenderManagerCore callee spellings that landed rows bind
   differently (pointer v reference parameters, access, `UpdateCameraPos` landed as `UpdateFlag8State`,
   `DrawAsIndexedTris` landed under a CRenderManagerCoreView spelling).
5. **Catalog note on the reserved sibling 0x00BB8EF0** (not authored): RenderSubPrimitive ends at 0x00BB9685, so it is
   1941 bytes, not 2659; 0x00BB9690..0x00BB9952 are unreferenced inline copies from the same source file.

## How to land

`rows/00bb8750.cpp` + `rows/00bb8750.test.cpp` with the `hdr/engine/` overlays merged, then `payload.json` through your
usual vland. `LESSONS.md`: swapped frame words with all instructions equal came from one live-range conflict (moving one
statement past another variable's last use fixed it), the `row = width + 1` index naming, `vi += 1 + (expr % row == 0)`
for `neg ; sbb ; inc`.
