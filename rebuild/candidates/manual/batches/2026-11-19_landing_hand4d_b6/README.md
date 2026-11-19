# Landing hand4d b6 (2026-11-19): NParticleEngine::CParticleEmitter::SetOrientation (0x00AEDE60)

**Result:** strict vland --land, RELOCATION_MATCH + PASS; no flags. The behaviour test killed all 7 one-decision mutants. No name fixes.

**Header:** new opt-in FABLE_RETAIL_CMATRIX3X4_INVERSE_NO_TRANSLATE_FORCEINLINE in CMatrix3x4.h, inside the INVERSE_NO_TRANSLATE_PASS23 block. It makes
GetDeterminant / SetInverseNoTranslate FABLE_FORCEINLINE. By default they stay plain `inline`, through a local macro that is #undef'd at the end of the header.
The header has no asserts.

**Why forced (`// Inline-budget:`):** retail expands Set(CRightHandedSet) and SetInverseNoTranslate, but calls the compiler-generated operator= 0x006B0E00 out
of line. No /d2inlT threshold gives that mix: at T221 the inverse is still called, and at T222 operator= is expanded too (867 v 796 bytes). The row uses the
existing SET_RHSET_INLINE opt-in.

**Header-user recheck:** 70 / 70 good.
