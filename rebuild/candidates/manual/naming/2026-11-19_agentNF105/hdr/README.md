# NF105 overlay (the short-reverb / DSP-engine family; carries NF99's and NF102's held opt-ins)
11 headers, all in hdr/engine/ with their repo base copies in orig/engine/ (each `cmp`-equal to rebuild/include at hand-back). Every block
below is a pure-addition `#ifdef FABLE_RETAIL_<CLASS>_<MEMBER>` (`diff orig hdr`: 0 removed lines in all 11), so the default path is
token-identical to the repo header. engine/CHLRvbParams.h (NF102 step 1) is NOT in this overlay: it landed with batches/2026-11-19_landing_nf102.

## NF99's held opt-ins (q11492 / q11493), carried unchanged
- engine/CFloatReverbDspEngine.h `FABLE_RETAIL_CFLOATREVERBDSPENGINE_PROCESSREVERB`: the five private Process* members ProcessReverb
  0x00C6A5D0 calls (ProcessReflectionsInputBuffer, ProcessReflections, ProcessShortReverbInputBuffer, ProcessShortReverb, ProcessLongReverb).
- engine/CFloatShortReverb.h `FABLE_RETAIL_CFLOATSHORTREVERB_PROCESS` (Process 0x00C75CE0) and `FABLE_RETAIL_CFLOATSHORTREVERB_PROCESSFADE`
  (the fading Process 0x00C75E90 + inline MatchToken).
- engine/CFloatShortReverbModule.h `FABLE_RETAIL_CFLOATSHORTREVERBMODULE_PROCESSINPUT` (inline GetFilterHistory + GetFeedbackData /
  MixInputs / ProcessInput / ProcessCrossfeed declarations).
These three headers also carry NF105 blocks (listed in the NF105 section).

## Short-reverb family opt-ins (NF102; used by the NF105 family hand-back below)
Base copies in orig/engine/ (each byte-equal to the repo header at overlay time). Each is one pure-addition `#ifdef` block placed right after
the class's public virtual destructor; `diff orig hdr` shows only added lines, so the default path is token-identical. pin_asserts.py: pinned
0 for each (size checks are `typedef char` lines). Used by the NF105 rows (rows/).
- engine/CFloatCyclicBuffer.h `FABLE_RETAIL_CFLOATCYCLICBUFFER_ACCESSORS`: inline GetLength / GetWriteCursor (const), SetWriteCursor,
  AddWriteCursor (SDK CodeView public inlines; AddWriteCursor body read from the CODAApiDbg.lib out-of-line copy).
- engine/CFloatAllpassSection.h `FABLE_RETAIL_CFLOATALLPASSSECTION_PROCESSSAMPLE`: inline ProcessSample(float) (SDK public member, body
  Inferred from the expanded retail 0x00C75140; the landed CDelayLine::Allpass body with CodeView member names).
- engine/CFloatDiffuser.h `FABLE_RETAIL_CFLOATDIFFUSER_PROCESS`: Process(float*, unsigned int) declaration (0x00C75140).
- engine/CFloatReflection.h `FABLE_RETAIL_CFLOATREFLECTION_PROCESS`: Process(const CFloatReflectionParams*, CFloatCyclicBuffer*, float*,
  float*, unsigned int) declaration (0x00C75FF0).
- engine/CFloatLongReverbModule.h `FABLE_RETAIL_CFLOATLONGREVERBMODULE_PROCESS`: Process(const CFloatLongReverbModuleParams*, const
  CFloatCyclicBuffer*, float*, float*, unsigned int) declaration (0x00C891A0).

## NF105 (round 10, resume of NF102): the whole short-reverb / DSP-engine family, 37 rows in rows/
Base copies: orig/engine/ holds the repo header for every overlaid file (CFloatEcho.h, CFloatPitch.h, CFloatLongReverb.h added by NF105;
`cmp` against rebuild/include: all 11 byte-equal at hand-back). Every NF105 opt-in is a pure-addition `#ifdef FABLE_RETAIL_<CLASS>_<MEMBER>`
block (`diff orig hdr` shows only added lines, 0 removed, for all 11 headers), so the default path is token-identical to the repo header.
Placement: right after the class's public virtual destructor (after any opt-in blocks already there), except the two `#include <string.h>`
blocks (after the "Original GameCODA2.10" comment line, before the class) and CFloatCyclicBuffer's (after the ACCESSORS block).
pin_asserts.py: pinned 0 for each of the 11 headers (their size checks are `typedef char` lines, no FABLE_STATIC_ASSERT).
Bodies of inline opt-ins are the SDK release COMDATs (CODAApi.lib, disassembled with work/tools/sdkdis.py) or the Dbg out-of-line copies;
declarations are the CodeView publics (exact mangled names in each comment).
- CFloatCyclicBuffer.h: `_CLEAR` (inline Clear = counted memset; + `#include <string.h>`), `_CREATE` (inline Create(float*, unsigned int)).
- CFloatAllpassSection.h: `_CLEAR` (inline Clear: memset when m_pBuffer, then m_index = 0; + `#include <string.h>`).
- CFloatDiffuser.h: `_CLEAR` (Clear 0x00C75010), `_CREATE` (Create(unsigned int, void*&, int*) 0x00C75050).
- CFloatEcho.h: `_CLEAR` (inline), `_CREATE` (0x00C76AB0), `_PROCESS` (both Process overloads 0x00C76B60 / 0x00C76C20), `_TOKEN` (inline Token).
- CFloatPitch.h: `_CLEAR` (inline), `_CREATE` (0x00C76DD0), `_PROCESS` (inline Token + both Process overloads 0x00C76E90 / 0x00C770B0).
- CFloatReflection.h: `_CLEAR` (inline), `_CREATE` (0x00C75F80), `_PROCESS_FADING` (inline Token + fading Process 0x00C76120).
- CFloatLongReverb.h: `_CLEAR` (0x00C75670), `_CREATE` (0x00C755B0), `_PROCESSINPUTFILTER_FADING` (private fading ProcessInputFilter 0x00C75820;
  the block opens `private:` and closes `public:`), `_PROCESS` (inline MatchToken + both Process overloads 0x00C75A20 / 0x00C75AB0).
- CFloatLongReverbModule.h: `_PROCESS_FADING` (fading Process 0x00C89350), `_CLEAR` (inline: m_Buffer.Clear(); m_Filter.Clear()),
  `_CREATE` (0x00C89110).
- CFloatShortReverbModule.h: `_CLEAR` (inline, as above), `_CREATE` (0x00C896C0), `_PROCESSINPUT_FADING` (inline Token + fading ProcessInput
  0x00C89850).
- CFloatShortReverb.h: `_CLEAR` (0x00C75CA0), `_CREATE` (0x00C75C30).
- CFloatReverbDspEngine.h: `_CREATECOMPONENTS` (private CreateComponents 0x00C6A460 and CreateReverbBuffers 0x00C69A70).
The inline Clear opt-ins need `FABLE_RETAIL_CFLOATCYCLICBUFFER_CLEAR` (and the module ones `FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23`) defined with
them; every row that uses them defines both.
Header users: the landed rows that reach any of these 11 headers (directly or through CFloatReverbEngine.h / CFloatReverbDspEngine.h /
CFloatReverbParameters.h) are 0x00C54190, 0x00C54510, 0x00C6AEF0, 0x00C6B050, 0x00C77800, 0x00C77AE0, 0x00C77C00, 0x00C77FC0, 0x00C78040,
0x00C78150, 0x00C783F0 (grep of rebuild/src/compiled). A grep of rebuild/src/compiled and rebuild/include for every NF105 macro name gives 0
hits, so these opt-ins change none of them. NF104 / NF106 overlays: NF104 has only CDrawMiniMap.h, NF106 none of these headers (checked).
