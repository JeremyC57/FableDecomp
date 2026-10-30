# Hand chunk 590 (std::vector<N3DAnimationBuilder::CSequenceInfo> .. NScript::CHeroBoastsScript): 9 of 10 byte-exact

All nine are RELOCATION_MATCH. None is a plain MATCH or retail_asm. Every test prints `<addr>_TEST PASS` under g++ (tall.sh) and VC7.1 (tall.py).

## What matched (9)
- **std::vector<N3DAnimationBuilder::CSequenceInfo>::_M_fill_insert** 0x00AB2E40: STLport 4.6 fill-insert for the 52-byte
  CSequenceInfo. The model is the C3DQuad2 twin 0x00AE6E60. `__uninitialized_copy` (0x00AB13A0) and
  `__copy_backward_ptrs` (0x00AB13D0) are same-TU noinline bodies, which is the live-slot tag rule.
  `uninitialized_fill_n` 0x00AB1410, `fill` 0x00AB10F0 and `_M_insert_overflow` 0x00AB2CE0 are declared out of line.
  The callee relocations were checked against retail order. 1 grade.
- **std::vector<CTVertexWaterBackground>::_M_insert_overflow** 0x00B7FFC0: STLport growth path for the flat 56-byte vertex
  (short X, Y; float Z; float ShoreLookUpArray[12]). Every helper is `__forceinline`, `_M_clear` 0x00B800F0 stays out of
  line (protected), and allocation goes through malloc 0x00BFEA0E. 1 grade.
- **CBankProcessor::AddEventTrigger** 0x00C2E7A0: a duplicate criteria string is logged with its hash. Otherwise a
  CEventTriggerRef is built and every known driver is inserted at the trigger's initial driver count; unknown drivers are
  logged. Shape: `/EHsc` without any EH frame. Plain /O2 gave 286v297; the missing bytes are the
  `mov [esp+N],esp` save before the by-value CString argument. 2 grades.
- **CRACBuffer::CRACBuffer** 0x00C4C080 (libpk, PKRACBuffer.cpp): the trace `.\PKRACBuffer.cpp(11) : Blocksize <n> not
  appropriate` fires when `alignment != 1 && !(alignment > 0 && (alignment & 3) == 0)`. The head and tail padding are
  computed from the offset and size. Built on the chunk 589 trace-block model with `/EHsc`. 1 grade.
- **CVtable_012b8ed4::ReleaseBuffer** 0x00C579C0 (OpenAL source, slot 5 of vtables 0x012B8ED4 / 0x012B8E20): stops
  the source, detaches the buffer, then deletes the source and buffers and frees the name array through
  CPKObject::operator delete 0x00C3C370. Shape: the Sourcei, DeleteSources and DeleteBuffers wrappers are inline members of
  the context (resolve on first use, then call); SourceStop 0x00C57640 stays a call. 1 grade.
- **CImaAdpcmCodec::EncodeM16** 0x00C5F3B0 (DirectX SDK imaadpcm.cpp): mono IMA ADPCM encoder. Each block has a dword
  header holding the prediction and the step index, followed by two nibbles per byte. Shape, in two parts:
  - The clamp's upper bound is `nStepIndex >= NUMELMS(m_asStep)`. That is an unsigned compare, `cmp 59h ; jb`;
    a signed `> 88` gives `cmp 58h ; jle`.
  - The step size is a call-argument temporary (`m_asStep[nStepIndex]` written in the call). A shared `nStepSize` local
    put both call sites in eax and split the epilogue.

  3 grades.
- **CDspGain::Crossfade** 0x00C74D30 (libpk DSP): fades the old gain settings out and the new ones in over 512 samples
  through the CDspOps SIMD table (ScaleRamp twice, then Add). When the fade ends inside this block it adopts the new
  settings. Shape: the object's two ramp states are `SRampState { float m_values[2]; }` members at +4 and +0xC, and
  SRampState has a user element-wise operator=. Two models failed:
  - `float m_gain[4]` recomputed the addresses (286v297).
  - A plain struct copy loaded both floats before storing.

  4 grades.
- **CBasePin::Connect** 0x00CAB470 (DirectShow strmbase): CheckPointer, CAutoLock, then two checks:
  VFW_E_ALREADY_CONNECTED and VFW_E_NOT_STOPPED. After that it calls AgreeMediaType 0x00CA7FB0. Built with `/EHsc` on
  the landed CBasePin::QueryPinInfo model. 1 grade.
- **NScript::CHeroBoastsScript::BoastBreakSiegeNoAssistance** 0x00CE5050: boast 25. It checks
  CQ_SunnyvaleMasterData::BreakSiegeNoAssistance (+0x116), then shows "TEXT_QST_BOAST_STATE_FAILED_HIRENOGUARDS" and waits
  for MsgIsGameInfoClickedPast. Built with FABLE_RETAIL_NSCRIPT on the engine headers. 1 grade.

## Parked for Fable 5.1 (1)
- 00c8a0a0 _fetch_headers DIFFER(295v295): the argument registers differ.
  - VC7.1's private convention for the same-TU static passes vf in ecx and serialno in eax, with og_ptr, vi and vc on the stack.
  - Retail passes og_ptr in eax and serialno in ecx, with vf, vi and vc on the stack.
  - The `_get_next_page` calls also take vf in esi instead of `mov eax,ebp`.

  Tried: the upstream libvorbis 1.0.1 body with `__stdcall` noinline statics for _get_data, _seek_helper and _get_next_page,
  plus three caller scaffolds (_prefetch_all_headers, _fetch_and_process_packet, _ov_open1). Next: none. Lane gap, the
  callee-chosen private register convention, the same gap as chunk 587's _get_prev_page 0x00C89D70. The behaviour test is
  differ/00c8a0a0.test.cpp and passes under g++ and VC7.1.

## Skipped
None.

## Label corrections
| Address | Manifest label | Actual function |
|---|---|---|
| 0x00AB2E40 | Vector_ResizeWithAppearanceModifiers | vector<CSequenceInfo>::_M_fill_insert (bound by resize 0x00AB3290; 52-byte stride) |
| 0x00B7FFC0 | Vector_PushBack_CActiveFile_14Elements | vector<CTVertexWaterBackground>::_M_insert_overflow |
| 0x00C2E7A0 | Thiscall_AnimationEventTriggerProcess | CBankProcessor::AddEventTrigger |
| 0x00C4C080 | CRABuffer_ctor | CRACBuffer::CRACBuffer (the trace file is PKRACBuffer.cpp) |
| 0x00C579C0 | CAudioSource_Cleanup | the slot-5 ReleaseBuffer of the OpenAL source |
| 0x00C5F3B0 | Audio_DecodeADPCM_Mono | CImaAdpcmCodec::EncodeM16 (it encodes) |
| 0x00C74D30 | Animation_InterpolateHelper | CDspGain::Crossfade |
| 0x00C8A0A0 | CreatureHitNotification_Initialize | libvorbis _fetch_headers |
| 0x00C8DB10 | std::_Dest_val<allocator<CFlash>> (already landed) | ogg_stream_packetout |
| 0x00C1EEE0 | CItemAction::CItemAction (already landed) | CEventTriggerRef ctor (see NAME_FIXES) |

## Names and evidence
- Proven:
  - **BoastBreakSiegeNoAssistance**: ControlNewBoast 0x00CE2861 stores this address beside the literal 0x012C39F0, and
    the name is in the Ego_r PDB name set.
  - **CBasePin::Connect**: strmbase source shape, error codes and AgreeMediaType. The Fable PDB names strmbase.
  - **std::vector members**: STLport 4.6 structure. The callers are resize 0x00AB3290 and the CTVertexWaterBackground
    push paths.
  - **CImaAdpcmCodec::EncodeM16**: the converter table 0x012B9274 of the landed Initialize, plus SDK source shape.
  - **_fetch_headers**: libvorbis 1.0.1 vorbisfile.c verbatim. The callers are 0x00C8A27C, 0x00C8A6AD and 0x00C8CA5E.
- **CBankProcessor::AddEventTrigger**: the spelling was already bound by the landed AddEvent.

## Descriptive names
These come from libpk, which has no PDB or RTTI:
- **CRACBuffer::CRACBuffer**: class name as already landed; the trace file name is PKRACBuffer.cpp.
- **CVtable_012b8ed4::ReleaseBuffer**: method name from the DirectSound sibling's slot-5 ReleaseBuffer 0x00C57040.
- **CDspGain::Crossfade**, with the members SRampState m_old / m_new: the class follows the landed Apply 0x00C74CE0.
- **CEventTriggerRef** and **PKCAutoDeleteArray<unsigned long>::operator[]**: spellings bound by the AddNumberTriggers rows.

## Header debt
- No engine headers exist for:
  - CSequenceInfo (and CMatrix3x4)
  - CTVertexWaterBackground
  - the strmbase classes (CBasePin, CMediaType, CCritSec)
  - CRACBuffer
  - CAudioContext / CVtable_012b8ed4
  - CQ_SunnyvaleMasterData (local model, BreakSiegeNoAssistance +0x116)
  - CBankProcessor / CEventRecord
  - CDspGain / CDspObject / CDspOps
  - CImaAdpcmCodec

  All of these are local models.
- **CDspGain**: the landed Apply 0x00C74CE0 models the state as `float m_gain[4]`. Retail's shape is two
  `SRampState { float m_values[2]; operator=; }` members, which Crossfade needs. Retype Apply when a header is generated.
- **CImaAdpcmCodec**: the landed rows (EncodeSample 0x00C5F2D0, DecodeSample 0x00C5F350, Initialize 0x00C5F9E0 and the
  other codec rows) declare a six-argument `LPFNIMAADPCMCONVERT` and six-argument converters. Retail's four converters
  0x00C5F3B0, 0x00C5F4E0, 0x00C5F6A0 and 0x00C5F7F0 all end in `ret 14h`, so they take seven arguments: the extra one is
  cSamplesPerBlock. This makes Initialize's converter-table relocations name symbols that do not exist (NAME_FIXES).
- **CBankProcessor siblings**: they flatten PKCAutoDeleteArray. The landed CItemAction rows use `struct CString`, while
  the AddEvent rows and this chunk use `class CString`.

## Callee names
Every callee was looked up in rebuild/oracles/reloc_bindings.tsv first. Where the bound spelling is right it is used
exactly. Rows using a spelling other than the bound one, or a new spelling (all listed in NAME_FIXES.tsv):
- **0x00C101D0**: `?Find@CItemDispatcher@@QAEPAUCItem@@ABVCString@@PAJ@Z`. The bound spelling has ABU, a class-key
  mismatch with every other CString in these rows.
- **0x00C0DF10 in CRACBuffer**: `??6CString@@QAIAAV0@K@Z`, which is new. It follows the chunk 589 trace model
  (CString::operator<< 0x00C0DEC0 / 0x00C0DF00 / 0x00C0DF10).
- **AddEventTrigger's stream operators**: it uses the CDebugStream spellings that AddEvent bound, for the same addresses
  0x00C0DEC0, 0x00C0DEA0 and 0x00C0DF10. That gives one address two names inside this chunk. See LESSONS.md, Review at
  landing.
- **0x00C8DB10**: `@ogg_stream_packetout@8`, a new spelling. The landed name is the wrong `_Dest_val<CFlash>`.
- **0x00B800F0 / 0x00AB10F0 / 0x00AB1410**: the STLport spellings, which are new. The landed names are placeholders.
