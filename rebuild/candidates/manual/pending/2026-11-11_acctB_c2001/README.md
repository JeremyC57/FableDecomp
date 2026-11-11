# 2026-11-11_acctB_c2001 -- acctB hand-back, chunk 2001 (5 reserved rows)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x007891F2 CTCTrapBase::BulletTimeFrameUpdate (catalog `vfunc_10`) | MATCH 5v5, test PASS | reserved length was 6; the row is 5 bytes, 0x007891F7 is a separate 1-byte function (vtable slot 28) |
| 0x00AED600 NParticleEngine::CParticleEmitter::InterpolateTo | RELOCATION_MATCH 1254v1254, test PASS | reaches the shared `engine/GFFloatToLongNear.h` helper (bare `fistp dword` at 0x00AED67B), so grade `retail_asm`; no asm in the row |
| 0x00AF4900 NParticleEngine::CParticleSystem::DatabaseSerialise (catalog `SerializeComponents`) | RELOCATION_MATCH 1282v1282, test PASS | |
| 0x00B3C5D0 CEngineResourceManager::SetupAlphaStippleTexture | RELOCATION_MATCH 1363v1363, test PASS | `// Inline-budget:` line (two STLport resize opt-ins); see "Review at landing" in LESSONS.md |
| 0x00A9EB90 C3DAnimationManager::BlendAnimations | parked, DIFFER 1337v1337 | one instruction: 0x00A9EC5A retail `fld dword ptr [eax+edx]`, ours `[edx+eax]`; `rows/parked/`, STATUS.tsv |

Tests pass under g++ and VC7.1. Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`):
`coordverify/` (grades, both test runs, read-only vland: 4 wins, no RELOC_IDENTITY, no warnings). Purity and padded-struct
greps are clean on rows and tests (the coordinator respelled one `wchar_t` in a comment of 00b3c5d0.cpp as `unsigned short`;
comment only, row regraded after).

To land: the rows need the headers in `hdr/` (new class headers CPSCBase.h, C3DAnimationManager.h; every change to an
existing header is an opt-in branch; `hdr/README.md`). `payload.json` holds the four matched rows; rebuild the oracle from
Fable.exe (no retail bytes are in this folder). NAME_FIXES.tsv has 3 landed spellings proven wrong (0x00411570, 0x013BC9FC,
0x00464931).
