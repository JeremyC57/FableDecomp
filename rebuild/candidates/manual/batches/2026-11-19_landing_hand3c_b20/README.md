# Landing hand3c b20 (2026-11-19): NParticleEngine::CPSCDecalRenderer::DecalCollides (0x00B1D550)

**Result:** strict vland --land gives RELOCATION_MATCH + `00b1d550_TEST PASS` after 2 grades; no flags.

**Source:** FableWin 0x02F567F0 statement for statement, with Ego_d locals.
- It uses the define set of the landed PreparePrimitives 0x00B1F200, plus GFPackFloat and GFSqr.
- `// Retail-asm:` line for the fistp at 0x00B1D5A9 / 0x00B1D609.

**New nested opt-ins** (pure additions; no assert lines moved):
- CDecal.h ISACTIVE_MAXLIFE: IsActive becomes `MaxLife > -1 && Active`, the FableWin body at 0x02F56430.
- CDecal.h GETSIZEFACTOR_NOARG.
- CPSCDecalRenderer.h DECALCOLLIDES.

**Follow-up (not done):** the landed PreparePrimitives row spells the MaxLife test by hand. It could use ISACTIVE_MAXLIFE instead.

**Header-user recheck:** 99 / 99 good, run together with hand4e b4.
