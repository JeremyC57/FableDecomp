# NF102 notes (2026-11-19, round 10): the CHLRvbParams setters and the short-reverb / DSP-engine family

## Part 1: q11560..q11576, the sixteen CHLRvbParams::Set* setters and their two binders (done)
- 16 self rows 0x00C53440..0x00C53A40 renamed to the GameCODA 2.10 CodeView members (RENAMES s1) on engine/CHLRvbParams.h (overlay opt-in
  FABLE_RETAIL_CHLRVBPARAMS_SETTERS, declarations only). Bodies are the landed bodies, unchanged except member / parameter names. Sources per
  setter: (1) sdkfind: each retail body is byte-identical (relocations masked) to exactly one CODAApi.lib member, the SalSwReverb.obj COMDAT
  ?Set...@CHLRvbParams@@QAEXM@Z, constants resolving to the same retail words; (2) the SDK ?Update@CSalSwReverb@@UAEXPBVCCodaEnvironment@@@Z
  (889 bytes = retail 0x00C54190's length, 23 relocations at the same offsets, 60 other bytes differ) calls each setter at the REL32 offset
  where retail 0x00C54190 calls that address, and each body writes the CodeView member of CHLRvbParams at the offset it stores.
- Binders (exe scan, REFS.tsv): 0x00C54190 and 0x00C54510, nothing else. Both now hold the +0x290 block as `CHLRvbParams m_pending`
  (the shared header), call the setters on it directly, and pass `&m_pending` / `&p` to CFloatReverbEngine::Commit with no cast.
  The three level words are the SDK inline SetLevel / SetLevelHF / SetLevelLF and the two pans (0x00C54510) the SDK inline
  SetReflectionsPan / SetReverbPan(const _RVB_VECTOR&), new overlay opt-ins. Each call was mapped by address and store offset, not by the
  landed helper name: 0x00C53500 (+0x18) is SetDecayHFRatio, 0x00C534A0 (+0x14) SetDecayLFRatio; the inline writing +0xC |0x80 is
  SetLevelHF, +8 |0x40 SetLevelLF (binds.py output checked for all 16 addresses in both objects before the vland run).
- **Casts left:** `grep -c reinterpret_cast`: rows/00c54510.cpp 0, rows/00c54190.cpp 2. The two left in 0x00C54190 are
  `reinterpret_cast<C3DSoundSource*>(this)->SetPosition / SetVelocity`: 2 casts remain in 0x00C54190, on `this`, pre-existing (not on the
  +0x290 block). Removing them means renaming 0x00C53D50 / 0x00C53E40 (SDK CSalSwReverb::SetReflectionsPan / SetReverbPan, NAME_FIXES) and the
  class itself (below), which reaches Open, the ctor / dtor and the vtable rows outside this slice. Coordinator to rule.
- 0x00C54190 local-model respellings in place (accepted form; no new local model): the descriptive SSourceProperties fields at +0x1C / +0x20
  and +0x28 / +0x2C were swapped against the SDK _CENVIRONMENT (iLevelHF / iLevelLF, fDecayHFRatio / fDecayLFRatio) and +0x44 / +0x48 were
  m_44 / m_48 (fDiffusion / fDensity): renamed with their inline helpers (no symbols; same call order, same bytes). The SVoiceEffectParameters
  local model and the seven setter stand-in classes are gone from both rows; `_pad2a0` dropped (CHLRvbParams is 0x68, m_effect stays +0x2F8).
- 0x00C54510: 0x012B89E8 is now declared `extern const _RVB_VECTOR g_zeroVector_012b89e8` (was C3DVector): a data respell of its only landed
  reference (exe scan incl. +4 / +8: 0x00C54510 only), RENAMES s2 + NAME_FIXES. It is the SDK's function-local `vZero` of
  CSalSwReverb::Initialize; the descriptive name is kept (a function-local static cannot be spelled from this row).
- EH funclet 0x01216140 (includes 0x00C54510 by path) regraded against the ported parent with funclet_offset 0: RELOCATION_MATCH PASS
  (work/fun1.log). No self rename moves a funclet parent (the parents' files keep their paths; the 16 setter files are renamed by the
  coordinator's rerow, none is a funclet parent).
- Tests unchanged (self-contained local copies; they compile without the rows and still PASS). The old setter tests keep their
  descriptive local stand-ins (not CHLRvbParams), so no new local model.

## Finding: CDecodedStream is GameCODA's CSalSwReverb (candidate, not applied)
- 0x00C54190 vs SDK CSalSwReverb::Update: same length (889), 23 relocations at the same offsets resolving to the expected retail targets
  (both pans, 16 setters, sal_levelToGain twice, 2^32 twice, Commit), 60 bytes differ (member offsets). 0x00C54510 vs SDK
  CSalSwReverb::Initialize (private, 410 bytes; retail 456 with an EH frame): same call / store sequence, the SDK's vZero / defaultPreset
  statics read in the same pan-copy / rep-movsd sequences where retail reads 0x012B89E8 / 0x012B89F8 (offsets differ: 410 vs 456 bytes). 0x00C53D50 / 0x00C53E40 = SDK SetReflectionsPan / SetReverbPan (144 bytes each, 8 differ).
  The SSourceUpdate argument is the SDK CCodaEnvironment (its CENVIRONMENT m_Params at +0xC, engine/CodaEnvironment.h).
- Against: the CDecodedStream vtable is 0x012B89C8 (referenced by the ctor 0x00C54090 / dtor 0x00C54110 only), 8 slots (0x00C54780,
  0x00C547A0 Open, 0x00C54170, 0x00C54100, 0x00C540E0, 0x00C54190 at slot 5, 0x00C546E0 PumpBlock, 0x00C540F0); the SDK
  ??_7CSalSwReverb@@6B@ has 22. Retail linked another SDK build (EXTRA_RULES wording rule), so the class identity needs its own decision.
  NAME_FIXES carries only the callee names with two sources. (0x012B89C4, just before it, is a separate one-slot table: xref.py shows
  its own seven references, 0x00C54510's `format` local among them.)

## Descriptive names
- Setter parameter names (fDecayTime ... fSize), the inline `Clamp` helper of the four 89-byte rows (landed, no retail symbol), the
  CDecodedStream class / member names and the SSourceProperties field names are descriptive (no CodeView local record read). Class,
  member function and data member names of CHLRvbParams are SDK CodeView spellings.

## Part 2: the short-reverb / DSP-engine family (q11415, q11492, q11493, q11512..q11515, q11517..q11522, q11543..q11558): in progress
Hands back whole or not at all, so nothing of it is in rows/. Stopped for context; resume from work/fam/FAMILY.tsv.
- Every family address was re-checked with sdkfind against CODAApi.lib (work/sdkfind_family.txt): all 34 closure rows, including the binders
  outside rows.txt (0x00C6A8B0 = SDK CFloatReverbDspEngine::Create, 0x00C75C30 = CFloatShortReverb::Create, 0x00C75A20 / 0x00C75AB0 =
  CFloatLongReverb::Process overloads, 0x00C75820 = CFloatLongReverb::ProcessInputFilter, 0x00C76C20 = the fading CFloatEcho::Process), are
  byte-identical to exactly one SDK member.
- Drafted and graded with q.py (1 grade each, no failed form): the three risky ports the advisor named first plus the leaves:
  0x00C75140 CFloatDiffuser::Process (RELOCATION_MATCH), 0x00C75FF0 CFloatReflection::Process (RELOCATION_MATCH), 0x00C891A0
  CFloatLongReverbModule::Process (RELOCATION_MATCH), 0x00C744E0 CFloatFilter::Clear and the five CReverbFns ring wrappers 0x00C76680 /
  0x00C764E0 / 0x00C76760 / 0x00C763E0 / 0x00C76460 (MATCH). Not yet: vland + tests, and every owner / binder listed in FAMILY.tsv.
- What the ports need, as found: CReverbDspProcessor::m_pDSP is reached through the existing FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23 opt-in
  (protected static, landed precedent 0x00C74510); the CReverbFns virtual slots in engine/CReverbFns.h order give exactly the landed
  CDspOps slots (VC7.1 lays overloaded virtuals in reverse declaration order: slot 2 = ScaleSamples(float*,float,float,unsigned),
  slot 3 = ScaleSamples(float*,const float*,float,unsigned), slot 7 = ScaleAddSamples(float*,const float*,float,unsigned)); the ring
  wrappers need only CFloatCyclicBuffer::GetLength / m_pBase.
- 0x00C891A0 binds 0x00C74CE0 as ?Process@CFloatFilter@@QAEXPBVCFloatFilterParams@@PBMPAM_NI@Z: 0x00C74CE0 is already split (self
  CDspGain::Apply; binders 0x00C69C60 `?Apply@CDspGain@@QAEXPBUSGainParams@@PBMPAM_NK@Z`, 0x00C756C0 `...PBMPAMHI@Z`), so the family leaves
  that split as it is unless 0x00C74CE0's self row and 0x00C756C0 join (not in rows.txt).
- Remaining risk: 0x00C69C60 (837 bytes) needs the CReverbDspEngine fade members (m_pvParams / m_pvParamsNew as CFloatReverbParameters*,
  m_bIsFading, m_iFaderToken, m_iSamplesToFade), CFloatFilter::Process + the fading Process (0x00C74CE0 / 0x00C74D30) and m_pDSP slot 2;
  0x00C69B60 holds 13 local models and 0x00C6A460 nine.
- q11559 (0x012B89C4): left open, separable from the family (see APPLIED): a one-slot table with seven code references (xref.py); NF99P2's ??_7CReverbConstants@@6B@ reading is not contradicted by the adjacent 8-slot CDecodedStream table 0x012B89C8.

## Rule slips
- Seven setter rows first quoted their old `CUnproven...` stand-in names in the Name line (caught at review; reworded to "a descriptive stand-in, old leaf in RENAMES.tsv" and regraded in recheck_final.log).
- One Bash call exited non-zero (a python assertion while writing the 0x00C54190 port: the comment anchor I matched did not exist; no file
  was written, the script was corrected and rerun).
- One tool name clash: my disassembler was first named dis.py and shadowed the stdlib `dis` module (capstone import failed); renamed
  sdkdis.py, run with `python3 -I`. No grade spent.
- callers.py takes one address per call; my first multi-address call silently reported only the first address. Binder lists for this
  hand-back came from REFS.tsv (exe scan) and per-address callers.py / xref.py runs.
