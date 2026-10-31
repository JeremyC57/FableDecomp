# Chunk 603 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Several inline `member = v; flags |= bit;` setters called through ONE reference fold into a single register RMW of the
  flag word. Retail's per-setter reload (`or [esi],imm` after a struct-member copy, then `mov r,[esi] ; or ; mov [esi],r`
  each time) comes from calling each setter through an inline accessor that returns `&m_member`: a fresh pointer temp per
  call (0x00C54510 CDecodedStream::Initialise). Try this before volatile, bitfields or optimize pragmas.
- x87 operand order follows named locals: `float gain = c / env; FloatToLong(gain * x)` gives `fld c ; fdiv ; fimul`, while
  the inline expression reassociates (0x00C593C0 CLimiter::Process).
- In a loop that fills two pointer arrays, the store order AND the loop-top alignment follow statement order inside the
  loop (0x00C75CE0 CReverbStage94::Process: states[i] before params[i]).
- A strmbase STDMETHODIMP whose `this` is the IPin subobject (+0x0C) shows `lea esi,[edi-0Ch]` before the primary virtuals;
  the landed CBasePin model handles it unchanged (0x00CA8120). Prove the slot from both pin IPin tables (0x012BF3C0 /
  0x012BF760).
- In reloc_bindings.tsv the `address` column is the REFERENCING row and `target` is the symbol's address; look a callee up
  by `target`.
- Under g++ on Linux, `long` is 64-bit, so a test's `typedef long HRESULT` makes `(HRESULT)0x80040200L` positive. Use
  `typedef int HRESULT` in tests (0x00CA6C40 test).

Review at landing:
- 0x00C54510 needed about 8 grades (more than 4). The matching lever is the inline accessor `GetPendingParameters()`, a
  descriptive name with no symbol. The parked DIFFER(439v456) version is kept at probe/00c54510_parked.cpp for reference.
- The vtable 0x012B89C4 (one slot, ??_G 0x00C53AE0) has several class spellings across landed rows: CVtable_012b89c4 (bound
  from 0x00C53AE0, used here), CSoundFormatBase (CReverbProcessor rows), CPlain28_00c6a780 and CTimerSlot_00c6ae90 (both
  bound). Pick one.
- Landed CDecodedStream_Open_00c547a0.cpp says "slot 2 of the CDecodedStream vtable 0x012B89C4". CDecodedStream's table
  starts at 0x012B89C8 (its ctor 0x00C54090 / dtor 0x00C54110 store 0x012B89C8), so Open is slot 1 of 0x012B89C8.
- The +0x290 pending-parameter block carries seven descriptive class names across the landed setter rows
  (CAttribute_00c534xx..., CUnprovenReverbParameters, CUnprovenPendingParams_00c53740, plus SVoiceEffectParameters in the
  landed Configure 0x00C6AEF0). This TU reinterpret_casts to each landed spelling so the relocations bind; one class would
  be better.
- CReverbStage94 (bound by 0x00C6A180) and CReverbTapBank (Allocate / Reset / Process 0x00C75C30 / 0x00C75CA0 /
  0x00C75E90) are the same class at reverb +0x94. CDelayLine and CRingBuffer also look like one type.
- Landed CBaseInputPin_Receive_00ca8b90.cpp says its caller 0x00CA6C88 is CRendererInputPin::Receive. That call site lies
  inside CBaseRenderer::PrepareReceive 0x00CA6C40.
- The SDK declares CBaseRenderer::m_bInReceive with a cv-qualifier (`volatile BOOL`). The model uses plain BOOL (byte-exact; the qualifier is banned
  in src/).
- NAME_FIXES.tsv: 0x00C89B40 is a fastcall model under a propagated CTCActivationReceptorBase::AddTrigger label, and it binds
  0x0143CCDC / 0x00C763E0 as g_target / GTarget::Handle. 0x00AC41D0 / 0x00AC4080 are C3DGroup2 methods (Ego_r name set).
  0x00CADC10 is CRendererPosPassThru::RegisterMediaTime.
- GOTCHAS.md (619 KB) and FINDINGS_LOG.md (334 KB) are too large to read whole in this lane. I read their newest sections
  and searched both for the topics of these rows; a full pass was not done.
