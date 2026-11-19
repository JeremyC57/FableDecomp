# NF102 overlay (from NF99 / NF99P2 held opt-ins)
CFloatReverbDspEngine.h, CFloatShortReverb.h, CFloatShortReverbModule.h: NF99's held opt-ins, unchanged (see work/held_drafts/NF99P2_NOTES.md).

## engine/CHLRvbParams.h (NF102, q11560..q11576)
Base: the current repo rebuild/include/engine/CHLRvbParams.h (copy in orig/engine/CHLRvbParams.h, byte-equal at overlay time). Three new
`#ifdef` opt-ins, all inside the class after the existing `public:` GetDecayTime line; with none of them defined the header is token-identical to
the repo copy (checked: the overlay minus the three blocks equals orig/ byte for byte). pin_asserts.py: pinned 0 (the header's size check is a
`typedef char ...[...]` line, not a FABLE_STATIC_ASSERT).
- `FABLE_RETAIL_CHLRVBPARAMS_SETTERS`: declarations of the sixteen public setters (SetDecayTime, SetDecayLFRatio, SetDecayHFRatio,
  SetReflectionsGain, SetReflectionsDelay, SetReverbGain, SetReverbDelay, SetLFReference, SetHFReference, SetEchoTime, SetEchoDepth,
  SetModulationTime, SetModulationDepth, SetDensity, SetDiffusion, SetSize; all `void (float)`), the GameCODA 2.10 CodeView members
  (?Set...@CHLRvbParams@@QAEXM@Z). Defined by the 16 self rows 0x00C53440..0x00C53A40; called by 0x00C54190 / 0x00C54510.
- `FABLE_RETAIL_CHLRVBPARAMS_SETLEVEL_INLINE`: inline SetLevel / SetLevelLF / SetLevelHF(int) (+4 |0x20, +8 |0x40, +0xC |0x80), the SDK header
  inlines (CODAApi.lib SalSwReverb.obj COMDATs, byte-identical to retail 0x00C53410 / 0x00C53420 / 0x00C53430). Used by 0x00C54190 /
  0x00C54510, where retail expands them. The landed 0x00C53410/20/30 rows (CUnprovenReverbParameters::SetRoom / SetRoomHF / SetRoomLF,
  not in this slice) do not include this header; NAME_FIXES.tsv carries their SDK names.
- `FABLE_RETAIL_CHLRVBPARAMS_SETPAN_INLINE`: inline SetReflectionsPan / SetReverbPan(const _RVB_VECTOR&) (+0x4C |0x800, +0x58 |0x4000), the SDK
  header inlines (SalSwReverb.obj COMDATs ?SetReflectionsPan / ?SetReverbPan@CHLRvbParams@@QAEXABU_RVB_VECTOR@@@Z). Used by 0x00C54510.
- Header users: the landed includers of CHLRvbParams.h are 0x00C53400, 0x00C54190, 0x00C54510, 0x00C6AEF0, 0x00C6B050, 0x00C783F0,
  0x00C77800, 0x00C77FC0, 0x00C78040, 0x00C77AE0, 0x00C77C00, 0x00C783B0, 0x00C78150 (grep of rebuild/src/compiled; no shared header
  includes it). None of them defines any of the three new macros except the two binders ported here, so these opt-ins change none of the
  other twelve. NF103 / NF104 overlays: no CHLRvbParams.h (checked at hand-back).

## Short-reverb family opt-ins (NF102; HELD with the family: no finished row in rows/ uses them yet)
Base copies in orig/engine/ (each byte-equal to the repo header at overlay time). Each is one pure-addition `#ifdef` block placed right after
the class's public virtual destructor; `diff orig hdr` shows only added lines, so the default path is token-identical. pin_asserts.py: pinned
0 for each (size checks are `typedef char` lines). Used only by the drafts in work/fam/ (see work/fam/FAMILY.tsv).
- engine/CFloatCyclicBuffer.h `FABLE_RETAIL_CFLOATCYCLICBUFFER_ACCESSORS`: inline GetLength / GetWriteCursor (const), SetWriteCursor,
  AddWriteCursor (SDK CodeView public inlines; AddWriteCursor body read from the CODAApiDbg.lib out-of-line copy).
- engine/CFloatAllpassSection.h `FABLE_RETAIL_CFLOATALLPASSSECTION_PROCESSSAMPLE`: inline ProcessSample(float) (SDK public member, body
  Inferred from the expanded retail 0x00C75140; the landed CDelayLine::Allpass body with CodeView member names).
- engine/CFloatDiffuser.h `FABLE_RETAIL_CFLOATDIFFUSER_PROCESS`: Process(float*, unsigned int) declaration (0x00C75140).
- engine/CFloatReflection.h `FABLE_RETAIL_CFLOATREFLECTION_PROCESS`: Process(const CFloatReflectionParams*, CFloatCyclicBuffer*, float*,
  float*, unsigned int) declaration (0x00C75FF0).
- engine/CFloatLongReverbModule.h `FABLE_RETAIL_CFLOATLONGREVERBMODULE_PROCESS`: Process(const CFloatLongReverbModuleParams*, const
  CFloatCyclicBuffer*, float*, float*, unsigned int) declaration (0x00C891A0).
- CFloatReverbDspEngine.h / CFloatShortReverb.h / CFloatShortReverbModule.h: NF99's held opt-ins, unchanged (top of this file).
- Header users: CFloatReverbEngine.h-based landed rows reach some of these headers through CFloatReverbDspEngine.h; none of them defines
  any of the new macros (grep of rebuild/src/compiled for each macro: 0 hits), so the opt-ins change none of them. The step-1 rows were
  rechecked with this final overlay (recheck_final.log).
