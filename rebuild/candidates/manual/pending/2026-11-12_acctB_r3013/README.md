# acctB hand-back, near-miss retry chunk 3013 (2 previously parked rows: 1 matched, 1 still parked)

Authored by an Opus 5.5 agent starting from the parked sources of c2028 and c2027, re-verified by the acctB local
coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x00B9A4A0 CEnginePrimitiveManagerRepeatedStaticMeshes::RenderPrimitive | RELOCATION_MATCH 1674v1674, test PASS (g++ and VC7.1) | **supersedes `2026-11-12_acctB_c2028/rows/parked/00b9a4a0.cpp`** (was one frame byte, [esp+18h] v retail [esp+1Bh]). Cause measured with frame-packer traces: the 1-byte node of the inlined `translucent` parameter had alignment 4 when fed straight from `material->Transparent`; through the FableWin.pdb inline accessors it has alignment 1, as retail. The only code change against c2028's parked source: `material->IsTransparent()`, `IsBooleanAlpha()`, `GetMapFlags()`, `GetDiffuseMapBankIndex()` instead of raw member reads (I diffed the two). Keeps c2028's per-helper `FABLE_FORCEINLINE` devices and `// Inline-budget:` line. 1 grade |
| 0x007C6E50 CTCTurncoatSpell::FrameUpdate | still parked, DIFFER 1804v1806 | residue unchanged from c2027 (retail 0x007C741C `mov edx,[esp+14h]` hoisted, 0x007C7426 `mov eax,[edx+2Ch]`, 0x007C7432 `mov esi,edx`; ours `mov esi,[esp+14h]` with no copy). 2 grades (1803v1806; 1805v1806 where the XP code becomes retail's but map / experience move to edi / ebp); passes 2 and 3 not used (agent context). Measured: c2's live-range split pass forms an XP piece of the caster over the counter-store and map blocks; a forced spill of 0 is recomputed; when `this` stays live through the XP block no piece forms. Stays in the acctB near-miss queue |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade, both test runs,
read-only vland: 1 win, no RELOC_IDENTITY / RELOC_CONFLICT; RELOC_NAME on `DELTA_NOTIONAL_ZERO` v catalog `EPSILON` and on
0x00A23C50 BeginLoadingTextures, already in NAME_FIXES.tsv). Purity and padded-struct greps are clean. Parked row regraded:
DIFFER(1804v1806).

To land 0x00B9A4A0: `hdr/` is c2028's overlay plus one new opt-in, `FABLE_RETAIL_C3DMESHMATERIAL_FLAG_ACCESSORS` in
`hdr/engine/C3DMeshMaterial.h` (`?IsTransparent@C3DMeshMaterial@@QBE_NXZ`, `?IsBooleanAlpha@...QBE_NXZ`,
`?GetMapFlags@...QBEKXZ` from FableWin.pdb, no out-of-line copy in Fable.exe; default text unchanged), plus c2027's files
for the parked row. All c2028 review items still apply (global spelling 0x01436EA0, CIVCountedPointer.h include,
TransformPoint / box-distance header debt, NAME_FIXES). Rebuild the oracle from Fable.exe (no retail bytes are in this folder).
