# Landing hand3c b23 (2026-11-19): CCodaListener::Commit (0x00C40590)

**Result:** strict vland --land gives RELOCATION_MATCH + `00c40590_TEST PASS`, with no flags.

**Identity:** the bytes equal GameCODA 2.10 CODAApi.lib member .\Release\CodaListener.obj ?Commit@CCodaListener@@QAEXXZ (192 bytes).
- Under the owner rule (EXTRA_RULES 2026-11-19) the row was authored as plain C++ first.
- It matches, so it claims no library grade. This is the first GameCODA row closed this way.

**Source:**
- Built on the shared engine/CCodaListener.h, with the statement shape taken from the CODAApiDbg.lib Commit.
- New opt-in FABLE_RETAIL_CCODAVECTOR_ORTHOGONALRIGHT in CCodaVector.h: a Normalize declaration plus inline Dot / OrthogonalRight. The bodies come from the
  SDK Toolkit twin CCtkVector (CtkVector.h). The header has no asserts.
- At the default inline budget VC7.1 expands the first OrthogonalRight and calls the COMDAT copy at the second, as retail does.

**Header-user recheck:** 1 / 1 good.

**NAME_FIXES:**
- 00c40590 ?Commit@CCodaListener@@QAEXXZ (was AudioLib_NormalizeOrientationFrame)
- 00c402e0 ?Normalize@CCodaVector@@QAEMXZ (landed as CPKVector3_00c402e0::Normalise)
- 00c40370 ?OrthogonalRight@CCodaVector@@QAEXABV1@0@Z (landed as C3DVector::SetCrossProduct)
- 0143bc24 ?m_ffCommit@CCodaListener@@0IA
