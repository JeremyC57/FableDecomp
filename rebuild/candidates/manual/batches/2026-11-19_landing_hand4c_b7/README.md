# Landing hand4c b7 (2026-11-19): CSalSw3DVoice::CalcParams (0x00C54C10; catalog: CAudioSource::ApplyParameters)

**Result:** strict vland --land gives RELOCATION_MATCH 651 + PASS; no flags. The hand-back's `"flags": ["none"]` was dropped at landing.

**First GameCODA-SDK-sourced row** (FINDINGS_LOG 2026-11-19).
- The release object CODAApi.lib SalSw3DVoice.obj has the same 651-byte instruction sequence, but its member offsets, globals and call targets differ. It is
  therefore close, not identical, and the row is authored C++.
- The debug object CODAApiDbg.lib gave the callees, the statement order and the product operand order.
- The old leaf-order residue (the 0x00C54C10 escalation) disappeared with `const CODA3DSOURCE& params3D = pcSource->Get3DParams()`, at 38 grades in all.
- Test: the row body runs verbatim over fixtures, 4,096 cases; 12 of 12 mutants are killed.

**Headers:**
- New opt-in FABLE_RETAIL_CSALSW3D_CALCPARAMS, one additive block in each of: CCoda3DSource, CCodaSource, CSalBaseVoice, CSalSwBaseVoice, CSalSwBuffer,
  CSalSwFilter, CCodaContext, CSalPlatform, CodaCaps. pin_asserts finds no moved assert.
- New engine/CSalRender.h (the SDK vtable) and engine/CCodaEAX.h (native 0xF0 layout: ctor 0x00C4E2A0, allocation 0x00C3DF28, Create 0x00C52840).
- Details in `hdr_README.md`.

**Header-user recheck:** 14 / 15 good. 0x01214FE0, an EH unwind funclet row, reads DIFFER(282v21) with the pre-b7 headers too. That is the recheck tool grading a
funclet row against the whole parent, not this merge.

**NAME_FIXES.tsv** (evidence: the SDK release / debug call relocations):
- 00c54c10 CSalSw3DVoice::CalcParams;
- 00c54ae0 sal_levelToGain (was MillibelsToAmplitude);
- 00c5a500 CSalSwBaseVoice::SetPitch;
- 00c5ce40 CSalSwBuffer::Release;
- 00c599c0 CSalBaseVoice::CalcAzimuthElevation;
- 00c6c000 CSalIIRFilter::SetLowPass;
- 00c6be60 SAL_CalcMacroFxGains;
- 00c6bc80 / 00c6bd00 / 00c6b720 CSalSwFilter::SetLP / SetEQ / ClearHistory (were CStateVariableFilter::*);
- 0143bc2c g_pSal;
- 0143bb58 CODA (g_pEAXManager / g_f_0143bbe8 / g_f_0143bbf4 are its members m_EAX / m_fDistanceUnits / m_fMacroFxFactor).

**Library copies, NOT authored** (UNMATCHED_ROWS status "library copy"; owner decision on toolchain_library pending). Each is byte-identical, with relocations
masked, to exactly one CODAApi.lib member:
- 0x00C77800 = ?UpdateDecayTimes@CFloatReverbParameterTranslator@@ (FloatReverbParameterTranslator.obj);
- 0x00C88E60 = ?ApplyFilterOrder2_quad@CReverbFnsSSE@@ (ReverbFns_SSE.obj).
