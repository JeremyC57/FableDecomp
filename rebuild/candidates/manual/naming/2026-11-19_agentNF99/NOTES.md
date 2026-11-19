# NF99 notes (2026-11-19, round 8): the GameCODA naming pass

Counts (from APPLIED.tsv / STATUS.tsv / recheck_final.log): 11 entries: 10 done, 1 done-already (q11476), 0 rejected / blocked / deferred /
open. 17 rows changed, all ported (2 MATCH + 15 RELOCATION_MATCH, every test PASS): the 16 rows.txt rows + 0x00C6BBD0 (nflock, NF99).
0 not-a-binder, 0 blocked. Grades per row (STATUS grades_used): 2 or 3 = a q.py precheck where run, one batch vland and the final
recheck; every grade passed (no failed form). Funclets: none of the changed
rows' files is included by a landed EH funclet (grep of rebuild/src/compiled). selfcheck.log: 0 problems.

Rule followed: B's rule (SDK member names win). Sources: the GameCODA 2.10 CODAApi.lib / CODAApiDbg.lib CodeView (one source) plus the
retail call at the matching relocation offset of a byte-identical (or same-call-order) caller, or the byte-identical body itself.
SDK files were only read (strings, a COFF reader and capstone in work/); nothing from the SDK tree is in this hand-back.

## Closure (advisor rule for this pass)
- A self rename takes every landed binder along in this hand-back (all done: see RENAMES s2).
- A ported row binds its callees under their SDK spellings even where the callee's own row is still descriptive (the 0x00C54C10 /
  0x00C6B050 / 0x00C77800 precedent); those callee rows are in NAME_FIXES.tsv (26 lines). The resulting split names are expected
  WARN RELOC_NAME lines in the logs, not new errors in the rows.
- **New splits created by these ports (8 callee addresses; the coordinator decides whether to land before their self rows follow):**
  self row and every binder agreed before, now the ported binder uses the SDK name and the self row (and any other binder) the old one:
  0x00C74BB0 (CEffectFilter::SetHighPass -> Set_HighPass), 0x00C74AB0 (CEffectFilter::SetLowPass -> Set_LpButterworth2),
  0x00C89B60 (CReverbFilterTap::MixHistory -> ProcessInput_MixInputs_SseOptimisation), 0x00C89750 (CReverbFilterTap::Process ->
  CFloatShortReverbModule::ProcessInput; 0x00C75E90 still binds the old name), 0x00C89AA0 (CReverbFilterTap::Feedback -> ProcessCrossfeed;
  0x00C75E90 still binds the old name), 0x00C69C60 (CReverbProcessor::ProcessInput -> ProcessReflectionsInputBuffer), 0x00C69FB0
  (CReverbProcessor::ProcessCombs -> ProcessReflections), 0x00C6A200 (CReverbProcessor::ProcessStage144 -> ProcessLongReverb).
  The other respelled callees were already split before (self row and binder disagreed): 0x00C78150, 0x00C780C0, 0x00C77FC0,
  0x00C78040, 0x00C77C80, 0x00C77B30, 0x00C77AE0, 0x00C77C00, 0x00C783B0, 0x00C89B40, 0x00C6A080, 0x00C75E90, 0x0143CCDC.
  The member-callee splits could not be avoided without porting the callee rows too (a header-class object cannot call a local
  model's member without a cast); all 26 target spellings are in NAME_FIXES.tsv.
- Exceptions, kept under their landed spelling because self row and every binder agree today (respell them together later):
  0x00C77D90 MixChannels_00c77d90 (SDK PanToGains), 0x00C77700 MillibelsToAmplitude (SDK mBToLinear), 0x012BF088 g_reverbMixMatrix
  (SDK _sc_fDefaultShortReverbCrossfeedGainMatrix).

## Per entry
- q11405 0x00C5A500 -> CSalSwBaseVoice::SetPitch on engine/CSalSwBaseVoice.h (existing opt-in CSALSW3D_CALCPARAMS); same body, members
  now the CodeView names; the output rate is g_SalCaps.SWMixer.nSamplesPerSec (0x0139C988 + 0x60, was the local g_outputRate). Not an
  authoring task: the landed body moved unchanged (first grade RELOCATION_MATCH).
- q11406 CSalSwBuffer::Release set: NF95's three drafts plus 0x00C5B300, all four in this hand-back. 0x00C5B300's ring tests are the SDK
  inlines IsCreated() and IsEmpty() (new overlay opt-in FABLE_RETAIL_CSALSWBUFFER_ISEMPTY; the Dbg out-of-line IsEmpty calls IsCreated
  then tests m_nValid). NF95's held test for 0x00C5CE40 modelled CSalSwBuffer locally (NEWLOCAL); rewritten on the header (raw storage,
  `#define private public` as in B's 0x00C77800 test, same PassPattern).
- q11410..q11412 CSalSwFilter: 0x00C6B720 ClearHistory (MATCH, written in the CODAApiDbg statement order: Z0, Z1, params copy, fade
  count), 0x00C6BC80 SetLP, 0x00C6BD00 SetEQ, and their callee 0x00C6BBD0 SetFrequency (claimed in nflocks; was
  CStateVariableFilter::SetCutoff, bound only by 0x00C6BC80). SetLP / SetEQ keep the same-TU noinline SetFrequency copy (nf96a form,
  `// Fidelity:` line). SetQ / SetGain / IsHistoryClear are header inlines (no retail address; the CSalSwBaseVoice::ReleasePlayBuffer
  precedent). Binders 0x00C55030 / 0x00C551D0 / 0x00C5C2D0 now hold CSalSwFilter members.
- q11414 + q11477 0x00C783F0 -> CFloatReverbParameterTranslator::Translate on the shared headers; its binder 0x00C6AEF0 now holds a
  CFloatReverbParameterTranslator at +0x260 and its CFloatReverbParameters at +0. 0x00C6AEF0 itself is the SDK
  CFloatReverbEngine::Commit (92 bytes, Translate REL32 +0x11): NAME_FIXES only, because its landed binder 0x00C54510 is outside this
  slice; meanwhile its local parameter block is handed to Translate through a reinterpret_cast (the block IS the CHLRvbParams; the
  row's own leaf stays unchanged until the Commit rename lands with 0x00C54510).
  The five inlined Translate helpers of the Dbg body (UpdateInputHpFilters, UpdateInputLpFilters, UpdateModulationTime,
  UpdateReflectionsPan, UpdateLevel) are written in place with a comment naming each (no retail address).
- q11415 + q11492 + q11493 (one family): 0x00C75CE0 -> CFloatShortReverb::Process (its 0x00C88E60 call now binds B's
  ApplyFilterOrder2_quad), 0x00C6A180 -> CFloatReverbDspEngine::ProcessShortReverb, and its only binder 0x00C6A5D0 ->
  CFloatReverbDspEngine::ProcessReverb (new finding, RENAMES s1: SDK call order + vtable slot 4; the SDK release member is 125 bytes,
  retail 123, so it is not byte-identical: another SDK build).
- q11476 0x00C74850 Set_ShelfLfHf: not landed, its only binder (B's 0x00C77800) already right: done-already, label fix in NAME_FIXES.

## Descriptive names (no Dbg local record was read for these)
- Parameter names of SetFrequency (uiFreq, fScale), SetGain (fGain), CFloatShortReverb::Process (pParams, pInputBuffer, pfFeedback,
  apfOutputs, uiSamples), ProcessReverb (pvInput, bInputValid, apvOutputs, uiSamples) and the locals of ProcessShortReverb
  (pShortReverb, pParams, pParamsNew) / Process (afBuffer, aFilterParams, apHistory, uiOutput) / SetFrequency (fFreq) / Translate (uiFlags).
  Class and member names are all SDK CodeView spellings.

## For the coordinator
- The landed 0x00C6BC80 and 0x00C6BD00 each define ?SetFrequency@CSalSwFilter@@AAEXIM@Z strongly besides the owner 0x00C6BBD0 (as they
  defined ?SetCutoff@CStateVariableFilter@@ before): the nf96a same-TU strong-copy class.
- reloc_bindings.tsv is stale for several of these rows (0x00C6A5D0 still listed as h1..h5@CEditControlCentre; 0x00C551D0 /
  0x00C5C2D0 as SRampedVector / CVoiceFilter): RENAMES s2 old symbols are taken from compiling the current landed sources.
- Follow-up family (not in this slice): rename the translator / short-reverb / engine callee self rows in NAME_FIXES (0x00C78150,
  0x00C77FC0, 0x00C78040, 0x00C77AE0, 0x00C77C00, 0x00C783B0, 0x00C74BB0, 0x00C74AB0, 0x00C89B40, 0x00C89B60, 0x00C89750,
  0x00C89AA0, 0x00C75E90, 0x00C69C60, 0x00C69FB0, 0x00C6A080, 0x00C6A200), 0x00C6AEF0 Commit with its binder 0x00C54510, and the
  three kept free spellings, so every split closes.

## Judgment calls (for the coordinator to rule on)
- 0x00C6AEF0 passes its local parameter block to Translate through `const_cast<CHLRvbParams*>(reinterpret_cast<const CHLRvbParams*>(..))`
  so that its own leaf (bound by 0x00C54510, outside this slice) stays unchanged.
- The rewritten 0x00C5CE40 test opens the header's private members with `#define private public` (the landed 0x00C77800 test does the same).
- CSalSwBuffer.h's IsEmpty opt-in is gated on two macros; four NF99 opt-in macros cover several members each (hdr/README.md).
- The header inline bodies other than IsEmpty are Inferred (hdr/README.md).

## Rule slips
- None known. One helper script was first named work/dis.py and shadowed Python's `dis` module inside capstone (a failed run, no
  grade); renamed cdis.py and run with `python3 -I`.
