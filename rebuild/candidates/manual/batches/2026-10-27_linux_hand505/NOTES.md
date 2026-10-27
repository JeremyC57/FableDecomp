# Hand chunk 505 (PKCOneShotQueue .. CSoftwareVoice): 24 of 30 byte-exact

The chunk has 30 rows, not 40. Of the 24 matched rows, 7 are MATCH and 17 are RELOCATION_MATCH. There are no retail_asm rows. 5 rows are parked and 1 is skipped.

## What matched (24)
- **libpk queues (PKCOneShotQueue.h / PKCStaticQueue.h trace strings).**
  - 0x00C1DE50 `PKCOneShotQueue<SFileReply>::Push`, 0x00C3A330 `PKCOneShotQueue<CUnprovenItem_012b54b4>::Pop` and 0x00C3A420 `::Push`.
  - 0x00C49010 `PKCStaticQueue<SDecoderMessage>::Push` (returns ++m_count).
  - Shape: the trace block is `{ CDebugStream s(g_pDebugBuffer); s << file << "(" << line << ") : " << msg << "\n"; if (g_pfnErrorReport) g_pfnErrorReport(s.m_pBuffer); } CDebugBreak();` under /EHsc.
- **PKCTRArray.cpp.** 0x00C325F0 GetFirst (line 204) and 0x00C326D0 GetLast (line 212), slots 11 and 12 of 0x012B4E80. Each returns 0 when empty and traces a null entry.
- **PKCBufferedReader.cpp.**
  - 0x00C4AB90 OnReadComplete (line 5) widens the request-id byte and posts it to the result mailbox at +0x48.
  - 0x00C4B060 ReturnCompletedBuffers (line 471) loops while requests are outstanding and the mailbox is not empty.
- **0x00C31660 `CSampleTableReader_012b4d40::OnRecord`.** It builds a CString temp key, then find-or-creates the entry (inline Get over slot 17 / 0x00C31710), then resizes and copies the values.
- **0x00C34F30 `CItemGridTable_012b501c::Insert`.** It appends to an existing row, or news a CPlaybackEntry row. Shape: a named `count` local gives retail's cmp order.
- **0x00C40650 `CTrackedTransform_00c40450::SetTransform`.** It runs the inlined position and direction setters (dirty bits 0x8000 / 0x10000), then calls SetOrientationVectors with copies of Up and Forward. Shape: a user member-wise C3DVector copy ctor keeps X in st0.
- **CAudioObject set-or-clear records.**
  - 0x00C430E0 SetEightFloatParams (+0x30 / +0x94 / 0x10), 0x00C431A0 SetVectorParam (+0x34 / +0xB4 / 0x20), 0x00C43230 SetFourFloatParams (+0x38 / +0xC0 / 0x40).
  - Shape: `__forceinline operator==`. The eight-float record repeats field 0 in its 4th test, as SFilterParams 0x00C43000 does.
- **C2DAudioObject.**
  - 0x00C445F0 SetSpeakerLevels floors to -10000 mB the speakers passed while seeking. Pending bits are masked by 0xA02 | 0x400.
  - 0x00C44680 SetSpeakerGains floors to 0.0. Pending bits are masked by 0x602 | 0x800.
  - Shape: `m_dirtyFlags = pending | bit` is written literally.
- **0x00C54F30 `CAudioSource::MixBlock`.** It copies the 0x100-byte block at the ring's read cursor, applies the effect and volume (global scaler, or EAX version 1), filters it, writes it to the stream, and calls CommitRead(0x80). Shape: the blocks must be `unsigned char[0x100]` with align(16); float blocks get no /GS cookie.
- **0x00C56F70 `CDSoundStreamBuffer::OpenDevice`.** It calls CoInitializeEx, CoCreateInstance(CLSID_DirectSound), Initialize and SetCooperativeLevel(2), and unwinds with Release / CoUninitialize.
- **0x00C57D80 `CVtable_012b8ed4::UnlockChunk`.** It reads the processed count with inline alGetSourcei, unqueues each one, queues the chunk, and restarts the source unless it is playing. Shape: `while (processed-- > 0)`.
- **CMixBuffer.**
  - 0x00C58140 MixAdd16Ramp: the indexed `dst[i]` form gives the two-element pointer bias.
  - 0x00C58420 MixStereo16Ramped: the mono arm uses half gain and `(short)steps[i] / 2`. With 4 or more channels it also mixes into channels 2 and 3.
- **CSoftwareVoice.**
  - 0x00C5A2E0 SetBuffer: the 7-word description copy is an implicit struct assignment (`rep movsd`, 7).
  - 0x00C5A5B0 ReadChannel: 8-bit samples are read as `signed char`; mono 16-bit is a memcpy. The write offset must be `m_writePos * m_frameBytes`: the other order swaps the two global loads, which grades RELOCATION_MATCH but fails vland RELOC_IDENTITY.
  - 0x00C5B250 StartSound: `#pragma pack(4)` puts the __int64 clock at +0x10. The 64-bit `freq * 128 / 44100 + t` compare matches.

## Parked for Fable 5.1 (5)
- 00c35290 CEnvelope_012b37dc::Update DIFFER(251v220): the phase bodies cache m_pos in edi with zero in ebx; retail uses `sub ecx,[esi+0Ch] ; add [esi+0Ch],ecx ; cmp [esi+0Ch],edx` with zero in edi. | tried: plain members, local caching, /Os (231, adds `and [m],0`) | next: a volatile-free memory-form source (per-phase helper taking `unsigned long&`), else lane gap: register allocation
- 00c35370 CEnvelope_012b37dc::GetPosition DIFFER(198v198): `add edx,ecx ; add edx,eax` (loop length first) vs retail `add edx,eax ; add edx,ecx` (attack first) | tried: both sum orders, a named sum local, a helper | next: lane gap: commutative operand order
- 00c371c0 CSequencePlayer_00c36ea0::Update DIFFER(242v255): zero is kept in ebx (`cmp al,bl`, `mov [..],ebx`) and force in ebp, where retail uses immediates / `test` with force in a lazily pushed ebx | tried: bool operator!=, int operator!=, optimize("g",off) (236) | next: split the re-run block into an inline member so the zero constant is not hoisted
- 00c56d70 CDSoundStreamBuffer::CreateLoopingBuffer DIFFER(206v206): the merged `xor al,al ; pop esi ; add esp,4Ch ; ret 4` false block comes after the last failure test, where retail puts it after the first | tried: six early returns, an && chain, an || chain (all give the end placement) | next: lane gap: block layout
- 00c590e0 CMixBuffer::WriteChannel16 DIFFER(217v217): the source is in esi and the output in edi, where retail has the reverse | tried: plain pointers, a named output local, indexed source (227) | next: lane gap: register allocation

## Skipped
- 00c29dc0: one catalog row holds seven short functions (forwarders `mov ecx,[ecx+8] ; jmp [eax+N]` and list walkers over +0x34 links), separated by int3 padding. The last one is cut off. The row needs re-splitting in the catalog.

## Proven
(none: GameCODA/libpk has no PDB or RTTI. The trace-string source files prove only the class names PKCOneShotQueue, PKCStaticQueue, PKCTRArray and PKCBufferedReader.)

## Probable
00C1DE50 PKCOneShotQueue<SFileReply>::Push
00C3A330 PKCOneShotQueue<CUnprovenItem_012b54b4>::Pop
00C3A420 PKCOneShotQueue<CUnprovenItem_012b54b4>::Push
00C49010 PKCStaticQueue<SDecoderMessage>::Push
00C325F0 PKCTRArray::GetFirst
00C326D0 PKCTRArray::GetLast
00C4AB90 PKCBufferedReader::OnReadComplete
00C4B060 PKCBufferedReader::ReturnCompletedBuffers
00C58140 CMixBuffer::MixAdd16Ramp
00C57D80 CVtable_012b8ed4::UnlockChunk

For each Probable row:
- **Queues and arrays:** the class is proven by the trace file. The method follows the queue interface slot (Pop 0 / Push 1) and the message text.
- **MixAdd16Ramp and UnlockChunk:** these are the names that landed sibling declarations already use.

## Descriptive
00C31660 CSampleTableReader_012b4d40::OnRecord
00C34F30 CItemGridTable_012b501c::Insert
00C40650 CTrackedTransform_00c40450::SetTransform
00C430E0 CAudioObject::SetEightFloatParams
00C431A0 CAudioObject::SetVectorParam
00C43230 CAudioObject::SetFourFloatParams
00C445F0 C2DAudioObject::SetSpeakerLevels
00C44680 C2DAudioObject::SetSpeakerGains
00C54F30 CAudioSource::MixBlock
00C56F70 CDSoundStreamBuffer::OpenDevice
00C58420 CMixBuffer::MixStereo16Ramped
00C5A2E0 CSoftwareVoice::SetBuffer
00C5A5B0 CSoftwareVoice::ReadChannel
00C5B250 CSoftwareVoice::StartSound
00C35290 CEnvelope_012b37dc::Update (parked)
00C35370 CEnvelope_012b37dc::GetPosition (parked)
00C371C0 CSequencePlayer_00c36ea0::Update (parked)
00C56D70 CDSoundStreamBuffer::CreateLoopingBuffer (parked)
00C590E0 CMixBuffer::WriteChannel16 (parked)

## Callee names introduced (descriptive unless noted)
- 0x00C42A60 CPlaybackEntry::CPlaybackEntry and 0x00C42B50 CPlaybackEntry::Add. Both conflict with landed CProfileMember / T::Init / CUnprovenPtrList_00c42b50.
- 0x00C49100 SDecoderMessage::operator=. This conflicts with the landed SQueuedSoundEvent spelling.
- 0x00C59880 CCodaVoice::SetBuffer (catalog: CVtable_012b5c10::vfunc_18).
- 0x00C5ACB0 CSoftwareVoice::ScheduleSound.
- 0x00C404F0 COrientedEngineObject::SetOrientationVectors is reached by a cast, keeping the landed spelling.
- 0x00C0BFE0 / 0x00C0BA40: CDebugStream(const char*) / ~CDebugStream. This is the libpk string; the catalog calls it CString.
- Bound spellings reused:
  - 0x00C3C4A0 CSoundBuffer::GetDesc
  - 0x00C5CE00 CAudioRingBuffer::Init
  - 0x00C5CF00 / 0x00C5CF20 CommitWrite / CommitRead
  - 0x00C59BC0 CAudioSource::BeginSound (cast from CSoftwareVoice)
  - g_monoRing 0x0143BF88
  - g_performanceFrequency 0x0139CA20

## Label corrections
- 0x00C4B060: catalog `PKCBufferedReader_FillBuffer` -> `PKCBufferedReader::ReturnCompletedBuffers` (trace line 471, "Should not be blocked here...").
- 0x00C54F30: catalog `AudioBuffer_ApplyGainFilter` -> `CAudioSource::MixBlock`.
- 0x00C58420: catalog `AudioFormat_DecodeSamplesWithDeltas` -> `CMixBuffer::MixStereo16Ramped`.
- 0x00C371C0: catalog `CTCTavernGame_OnStateUpdate` is wrong. It is the libpk sequence player update (`CSequencePlayer_00c36ea0::Update`).
- 0x00C5ACB0: autoname `C3DMeshFileXBoundingBoxChunk::LoadData` is wrong. It is the software voice's scheduler.
- 0x00C49100: landed `SQueuedSoundEvent::operator=` is the decoder-message record SDecoderMessage (0x40 bytes, PKCStaticQueue<SDecoderMessage>).
- 0x00C25730: landed `C3DVector::operator!=` declares int, but the caller tests `al`, so it returns bool (only the parked 0x00C371C0 uses it).
- One-shot queue Push (0x00C1DE50, 0x00C3A420) returns void; retail sets no eax. The landed PKCQueue declarations say long.

## Header debt
GameCODA/libpk has no engine headers. Every row carries a local model:
- CAudioRingBuffer, CSoftwareVoice / CCodaVoice, CMixBuffer, CAudioObject / C2DAudioObject, CAudioSource and CDSoundStreamBuffer.
- The PKC* queue and array classes.

A shared `rebuild/include/gamecoda/*.h` set would remove the duplication. The voice layout should come from the landed 0x00C5A3C0 SizeRing, and the ring layout from 0x00C5CF90.
