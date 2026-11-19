# NF99 header overlay (opt-ins only; default path token-identical)

## engine/CSalSwBuffer.h (copy of rebuild/include/engine/CSalSwBuffer.h as of 2026-11-19; base copy in orig/engine/)
- NEW opt-in `FABLE_RETAIL_CSALSWBUFFER_ISEMPTY` (only together with the existing FABLE_RETAIL_PASS22_CSALSW_CALCPARAMS, which declares
  IsCreated): `bool IsEmpty() const { return IsCreated() && m_nValid == 0; }`. Source: GameCODA 2.10 CODAApiDbg.lib CodeView public
  const ?IsEmpty@CSalSwBuffer@@QBE_NXZ; its Dbg out-of-line copy (SalSwBaseVoice.obj, 50 bytes) calls IsCreated then tests [ecx+1Ch]
  (m_nValid) == 0; second source: retail 0x00C5B300's inline test (`m_pvBuffer != 0` then `[esi+0D0h] == 0`). User: 0x00C5B300.
- Without the macro the header preprocesses token-identically to the repo copy (the block is the only change).
- No FABLE_STATIC_ASSERT in this header (pin_asserts: nothing to pin).

## engine/CSalSwFilter.h (copy of rebuild/include/engine/CSalSwFilter.h as of 2026-11-19; base copy in orig/engine/)
Four NEW opt-ins, all inserted after `void ProcessEQFade(short*,unsigned int);` (still the class's leading private section, as the SDK
access letters require: AAE / ABE):
- `FABLE_RETAIL_CSALSWFILTER_SETFREQUENCY`: declaration `void SetFrequency(unsigned int,float);` (SDK ?SetFrequency@CSalSwFilter@@AAEXIM@Z,
  native 0x00C6BBD0). Users: 0x00C6BBD0 (owner), 0x00C6BC80 / 0x00C6BD00 (same-TU noinline copies, nf96a form).
- `FABLE_RETAIL_CSALSWFILTER_SETQ_INLINE`: in-class `SetQ(float)` (SDK private ?SetQ@CSalSwFilter@@AAEXM@Z; the release SetLP / SetEQ
  expand it). Users: 0x00C6BC80, 0x00C6BD00.
- `FABLE_RETAIL_CSALSWFILTER_SETGAIN_INLINE`: in-class `SetGain(float)` (SDK private ?SetGain@CSalSwFilter@@AAEXM@Z). User: 0x00C6BD00.
- `FABLE_RETAIL_CSALSWFILTER_ISHISTORYCLEAR_INLINE`: in-class `IsHistoryClear() const` (SDK private ?IsHistoryClear@CSalSwFilter@@ABE_NXZ).
  Users: 0x00C6BC80, 0x00C6BD00.
The in-class inline spelling follows the landed precedent CSalSwBaseVoice::ReleasePlayBuffer (hand4c w24): an SDK out-of-line helper
that the release build expands in place and that has no retail address. Bodies are the landed rows' inlines mapped onto the CodeView
members; the order of calls in SetLP / SetEQ is the CODAApiDbg SalSwFilter.obj order (SetFrequency, [SetGain,] SetQ, IsHistoryClear).
- Without these macros the header preprocesses token-identically to the repo copy. No FABLE_STATIC_ASSERT in this header.

## engine/CFloatReverbParameterTranslator.h (repo copy 2026-11-19, base in orig/engine/)
- NEW opt-in `FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_TRANSLATE`, inserted after b10's UPDATEDECAYTIMES block: nine private declarations
  (UpdateSize, UpdateDiffusion, UpdateEchoTime, UpdateModulationDepth, UpdateReverbDelay, UpdateReflectionsDelay,
  UpdateReflectionsOutputGain, UpdateReverbPan(CHLRvbParams*), UpdateReverbOutputGain), the SDK release Translate's REL32 targets.
  User: 0x00C783F0 (also defines b10's UPDATEDECAYTIMES for UpdateDecayTimes / SetGain / the CHLRvbParams friend).

## engine/CFloatFilterParams.h (repo copy 2026-11-19, includes b10 / b11 opt-ins; base in orig/engine/)
- NEW opt-ins `FABLE_RETAIL_CFLOATFILTERPARAMS_SET_HIGHPASS` (`public: void Set_HighPass(int,float,unsigned int);`) and
  `FABLE_RETAIL_CFLOATFILTERPARAMS_SET_LPBUTTERWORTH2` (`public: void Set_LpButterworth2(int,float,unsigned int);`), inserted before the
  `private:` data members. SDK ?Set_HighPass@CFloatFilterParams@@QAEXHMI@Z / ?Set_LpButterworth2@CFloatFilterParams@@QAEXHMI@Z. User: 0x00C783F0.

## engine/CFloatShortReverb.h (repo copy 2026-11-19, base in orig/engine/)
- NEW opt-in `FABLE_RETAIL_CFLOATSHORTREVERB_PROCESS`: public `Process(CFloatShortReverbParams*, const CFloatCyclicBuffer*, float*, float**,
  unsigned int)` (native 0x00C75CE0). Users: 0x00C75CE0, 0x00C6A180.
- NEW opt-in `FABLE_RETAIL_CFLOATSHORTREVERB_PROCESSFADE`: public fading overload `Process(CFloatShortReverbParams*, const
  CFloatShortReverbParams*, const CFloatCyclicBuffer*, float*, float**, int, unsigned int, unsigned int)` (native 0x00C75E90) and the
  in-class `MatchToken(int) const` (SDK ?MatchToken@CFloatShortReverb@@QBE_NH@Z; the release expands it). User: 0x00C6A180.

## engine/CFloatShortReverbModule.h (repo copy 2026-11-19, base in orig/engine/)
- NEW opt-in `FABLE_RETAIL_CFLOATSHORTREVERBMODULE_PROCESSINPUT`: in-class `GetFilterHistory()` (forwards to CFloatFilter::GetFilterHistory)
  and the public declarations ProcessInput_GetFeedbackData_SseOptimisation, ProcessInput_MixInputs_SseOptimisation, ProcessInput (the
  5-argument const overload), ProcessCrossfeed. User: 0x00C75CE0.

## engine/CFloatReverbDspEngine.h (repo copy 2026-11-19, base in orig/engine/)
- NEW opt-in `FABLE_RETAIL_CFLOATREVERBDSPENGINE_PROCESSREVERB`, right after the `private:` label: ProcessReflectionsInputBuffer,
  ProcessReflections, ProcessShortReverbInputBuffer, ProcessShortReverb, ProcessLongReverb (the SDK release ProcessReverb's REL32
  targets). Users: 0x00C6A5D0, 0x00C6A180.

## Checks (all seven headers)
- pin_asserts.py on each: `pinned 0` (none of them carries a FABLE_STATIC_ASSERT; the size checks are typedef arrays, untouched).
- Default path: each overlay, preprocessed with the landed macro set (PASS22_FLOATREVERB_CREATE, CSALSW3D_CALCPARAMS,
  PASS22_CSALSW_CALCPARAMS, CFLOATREVERBPARAMETERTRANSLATOR_UPDATEDECAYTIMES, CREVERBFNSSSE_APPLYFILTERORDER2_QUAD,
  CFLOATFILTER_APPLY_PASS23) and without any NF99 macro, is token-identical (md5 of `cpp -P`) to the repo copy.
- Landed includers (direct or through the CSalSw / CFloat / CReverb / CHLRvb headers): 0x00C3F800, 0x00C44090, 0x00C44100, 0x00C462E0,
  0x00C49470, 0x00C53400, 0x00C54C10, 0x00C599C0, 0x00C5A9D0, 0x00C5ACB0, 0x00C6B050, 0x00C74510, 0x00C77800, 0x00C88E60. None defines an
  NF99 macro (all new), so the opt-ins change none of them.
- The other agents' overlays (agentNF96 / agentNF98 hdr/) touch none of these headers.

## Inferred bodies (B's convention)
- Checked against the CODAApiDbg out-of-line body: CSalSwBuffer::IsEmpty only.
- Inferred (no Dbg body read; written from the landed rows' matching inlines and the release instruction shapes, each graded through
  its users): CSalSwFilter::SetQ / SetGain / IsHistoryClear, CFloatShortReverb::MatchToken, CFloatShortReverbModule::GetFilterHistory
  (forwarding to the existing CFloatFilter::GetFilterHistory). Their SDK names and access letters are CodeView facts; the bodies are
  Inferred.
- Macro naming: FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_TRANSLATE, ..._CFLOATSHORTREVERB_PROCESSFADE,
  ..._CFLOATSHORTREVERBMODULE_PROCESSINPUT and ..._CFLOATREVERBDSPENGINE_PROCESSREVERB each cover the several members one user row needs
  (named after that user member), not one macro per member; CSalSwBuffer's ISEMPTY block is gated on two macros
  (`#if defined(FABLE_RETAIL_CSALSWBUFFER_ISEMPTY) && defined(FABLE_RETAIL_PASS22_CSALSW_CALCPARAMS)`) because IsEmpty calls IsCreated.
