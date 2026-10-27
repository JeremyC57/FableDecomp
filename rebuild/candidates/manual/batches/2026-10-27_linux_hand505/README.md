# Chunk 505 (agent505, 2026-10-27): 24 of 30 single functions byte-exact (128-256 bytes)

Queued chunk of single functions of 128-256 bytes (30 rows, address order, 0x00C1DE50..0x00C5B250): the GameCODA/libpk
audio library (queues and arrays, the buffered reader, envelopes, 2D/3D audio objects, the software mixer and voice).
One Opus 5.5 agent at high effort.

- **Landed:** 24 rows with `vland.py --land` (7 MATCH, 17 RELOCATION_MATCH, every test PASS, no RELOC_IDENTITY):
  CAudioObject set-or-clear records, C2DAudioObject SetSpeakerLevels / SetSpeakerGains, CMixBuffer::MixAdd16Ramp /
  MixStereo16Ramped, six PKCOneShotQueue / PKCStaticQueue / PKCTRArray rows, PKCBufferedReader (two rows),
  CSoftwareVoice SetBuffer / ReadChannel / StartSound, CAudioSource::MixBlock and others.
- **Parked (5, pending/2026-10-27_linux_hand505/differ/):** 0x00C35290 envelope Update (251v220, m_pos cached in a
  register), 0x00C35370 envelope GetPosition (198v198, commutative add order), 0x00C371C0 sequence-player Update
  (242v255, zero kept in ebx), 0x00C56D70 CDSoundStreamBuffer::CreateLoopingBuffer (206v206, merged false-return block
  placement), 0x00C590E0 CMixBuffer::WriteChannel16 (217v217, esi/edi swap).
- **Skipped (1):** 0x00C29DC0 is one catalog row holding seven short int3-separated functions; it needs re-splitting.
- **Identities:** none proven: GameCODA/libpk has no PDB or RTTI, and its trace-string file names prove only the class
  names PKCOneShotQueue, PKCStaticQueue, PKCTRArray and PKCBufferedReader. 10 probable, the rest descriptive (NOTES.md).

Label corrections (NOTES.md): 0x00C4B060 `PKCBufferedReader_FillBuffer` is ReturnCompletedBuffers; 0x00C54F30
`AudioBuffer_ApplyGainFilter` is CAudioSource::MixBlock; 0x00C58420 `AudioFormat_DecodeSamplesWithDeltas` is
MixStereo16Ramped; 0x00C371C0 `CTCTavernGame_OnStateUpdate` is the libpk sequence player's update; 0x00C5ACB0
`C3DMeshFileXBoundingBoxChunk::LoadData` is the software voice's scheduler. Review: landed 0x00C49100 is
SDecoderMessage::operator=, not SQueuedSoundEvent; landed C3DVector::operator!= 0x00C25730 returns bool, not int; the
one-shot queue Push returns void, not long; 0x00C0BA40 / 0x00C0BFE0 have two landed spellings (CDebugStream and
CString; this chunk uses CDebugStream), 0x0139CA20 three (this chunk uses g_performanceFrequency), 0x0143BF88 two
(g_monoRing).
