# Landing hand3c b21 (2026-11-19): NParticleEngine::CPSCDecalRenderer::PoolDecal (0x00B1D790)

Strict vland --land gives RELOCATION_MATCH + `00b1d790_TEST PASS` after 6 grades. No flags.

**Source:** FableWin 0x02F56D80 statement for statement, with Ego_d locals. A `// Retail-asm:` line covers the fistp at 0x00B1D82D and 0x00B1D889.

**What closed it:** the DecalCollides 0x00B1D550 body is defined ahead of the row as a same-TU helper, as in retail's .obj.
- That copy grades RELOCATION_MATCH at its own address.
- Without it, the frame packs 0x1C instead of 0x14 and the size-ratio divide is hoisted (880v876).

**New opt-ins** (no assert lines moved):
- CDecal.h POOLING: the FableWin accessor bodies 0x02F575F0 .. 0x02F57710.
- CPSCDecalRenderer.h POOLDECAL.

**Header-user recheck:** 11 / 11 good.
