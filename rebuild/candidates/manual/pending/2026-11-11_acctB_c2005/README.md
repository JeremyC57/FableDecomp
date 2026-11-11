# 2026-11-11_acctB_c2005 -- acctB hand-back, chunk 2005 (4 reserved rows: 1 matched, 3 parked)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x00B51570 CEngineWeatherRenderer::BuildBuffers | RELOCATION_MATCH 1254v1254, test PASS (g++ and VC7.1) | `rows/00b51570.cpp` |
| 0x00A4D920 C3DAnimation2::CalcLocalBoneTransformationRelative | parked, DIFFER 1341v1341, 2 lines | 0x00A4DAC6 retail `fld [esp+18h] ; fmul [esp+14h]`, ours swapped (inlined SetSlerp -> GFFastSin `(1-t)*omega`); no test yet |
| 0x00AB2520 C3DAnimationBuilder::BuildMeshAnimation | parked, DIFFER 1265v1265, 66 lines (x87 term order only) | needs the same-TU body of 0x00AB1CC0 BuildBoneInfoTree (in the parked source; the agent reports it RELOCATION_MATCHes its 612 bytes; 0x00AB1CC0 is not an acctB row); no test yet |
| 0x00A12EE0 CPhysicsMeshObject::GetNavigationLevelVolumes | parked, DIFFER 1118v1144 | retail keeps a second induction pointer `it+8` in [esp+14h] (frame 0x64 vs 0x60); no test yet |

The three parked rows used one to two passes of the three allowed; acctB will retry them (the two one-instruction
x87 operand-order residues 0x00A9EB90 and 0x00A4D920 first). All four reserved lengths are correct.

Coordinator re-verification of the matched row (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/`
(grade, both test runs, read-only vland: 1 win; RELOC_NAME on 0x00B3B090, landed under the placeholder
`ReturnConst_1_00b3b090`, called here as `CEngineResourceManager::GetVertexBufferMemPool` -- NAME_FIXES.tsv; RELOC_ALIAS on
`__CIfmod`, the msvcr71 import thunk). Purity and padded-struct greps are clean.

To land: the row needs `hdr/` (six new class headers; opt-in macros only in existing headers; `hdr/README.md`; the unused
experimental opt-ins CQUATERNION_MULASSIGN_INLINE and C3DKEYFRAME_POSTAPPLY can be dropped). Rebuild the oracle from
Fable.exe (no retail bytes are in this folder). NOTES.md flags 0x01436E98, which has three landed spellings.
