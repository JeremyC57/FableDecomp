# Chunk 506 (agent506, 2026-10-27): 19 of 30 single functions byte-exact (128-256 bytes, 1 retail_asm)

Queued chunk of single functions of 128-256 bytes (30 rows, address order, 0x00C62920..0x00CAB770): libpk audio (reverb,
DSP ops, OpenAL voices, Ogg streams), Xiph libvorbis/libogg statics, and DirectShow strmbase pins and allocators. One
Opus 5.5 agent at high effort.

- **Landed:** 18 rows with `vland.py --land` (5 MATCH, 13 RELOCATION_MATCH, every test PASS, no RELOC_IDENTITY):
  nine strmbase methods (CRendererInputPin::Receive, CBasePin::ConnectionMediaType / Disconnect, CBaseInputPin::GetAllocator /
  NotifyAllocator, CMediaSample::GetProperties, CBaseAllocator::GetBuffer / Commit, the wide CDynamicOutputPin ctor),
  three CDspOps x87 slots, CReverbProcessor::Reset, CReverbFilterTap::Process, COggVorbisStream::Read, libvorbis
  floor0_unpack, the 0x00C62920 stream-owner ctor and a 0x012BF0C8 slot. Plus 1 retail_asm row (payload_retail_asm.json):
  0x00C6B5D0, an SSE2 cvtdq2ps block conversion, RELOCATION_MATCH.
- **Parked (9, pending/2026-10-27_linux_hand506/differ/):** five LTCG Xiph statics with private register arguments (lane
  gap: _vorbis_pack_info, _preextrapolate_helper, oggpack_writecopy_helper, _dist, res0_pack); 0x00C6B7C0 (retail_asm,
  one extra fxch); 0x00C72800 / 0x00C72D00 CAudioSource (register allocation); 0x00C74AB0 low-pass coefficients
  (scheduling).
- **Skipped (2):** 0x00CA5200 and 0x00CA62D0 are gap-scan spans packing several int3-separated thunks, not one function.
- **Identities:** 15 proven, 5 probable, 8 descriptive (NOTES.md); proven landed rows in
  ghidra_out/labels_chunk506_2026-10-27.tsv.

Review at landing (NOTES.md, LESSONS.md): landed 0x00CAB370 is the wide-string CBasePin ctor, not
"CParticleMorphs::CEntry_Constructor"; landed caller 0x00C546E0 binds 0x00C6B5D0 as the invented `ResetDecoder(void*)`;
landed CVtable_012bf014 ctor takes `long` where retail passes float 800.0f. The `volatile` the agent flagged in the landed
0x00CAB370 / 0x00CAC300 strmbase models is the DirectShow SDK's own `volatile LONG m_cRef` member of CUnknown
(combase.h), a faithful declaration rather than an artificial codegen lever; purity.py does not grade it.
