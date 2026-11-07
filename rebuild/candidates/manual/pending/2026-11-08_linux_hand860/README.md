# Chunk 860 (2026-11-08): chunk 857's six parked engine / UI rows with a real budget — 0 of 6 matched

1,160 graded attempts (`ATTEMPTS.log`); every row has a behaviour test that PASSes under VC7.1 (`differ/<addr>.test.cpp`).

| Row | Grades | Best | md (857 -> 860) | Residue |
|---|---|---|---|---|
| 0x009D8CF0 CRenderManager2D::ClearRenderTarget | 348 | 3385v3385 | 32 -> **12** | edx/ecx colouring of the bookmark test in three texture-stage blocks (stencil blocks fixed by one-statement `indices[]` fills + a `CShaderRenderManager&` local) |
| 0x005AECC0 CTCInventoryTrade::SellItem | 185 | 2446v2446 | 349 -> **76** | frame 0x58 v 0x54 (one slot unshared); still on chunk 857's local models (needs rebasing on shared headers before landing) |
| 0x00BB30A0 CEngineClothRenderer::Render2DClothPrimitive | 150 | 3522v3538 | 172 -> 167 | frame 4 bytes larger; colour bytes held in registers in retail; CRenderStateManager bookmark gap |
| 0x00A8B990 C3DMesh2::InitialiseBones | 175 | 1608v1624 | 317 | end pointer in edi / bone.Info spilled (reversed in ours) |
| 0x0049A0B0 CProgressDisplay::InitialiseTextures | 129 | 2928v2920 | 630 -> 582 | C2DExtentsI Y-before-X stores, frame 0xd8 v 0xb0; local models |
| 0x00A4EFC0 C3DAnimationSequenceData::InitialiseFromFileData | 170 | 3495v3491 | 1268 | two large x87 blocks; length 3613 -> 3495 |

- Headers (hdr/, all additive opt-ins): C3DAnimationSequenceData.h VECTORWORD, C3DAnimFileSequenceChunk.h MATRICES, CArray.h GETBYTESIZE, plus
  chunk 857's three carried over.
- Names: NAME_FIXES.tsv (5, queued `land0811j`); LABELS.tsv (5 callee labels replacing forge autonames, not yet applied).
- Lesson (the big one): VC7.1 shares an address-taken local's or a dead parameter's frame slot only when every callee that receives the
  address is visible in the same TU and does not keep it -- same-TU bodies under `#pragma auto_inline(off)` (LESSONS.md has the micro-test).
