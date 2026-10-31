# Chunk 603 (engine remainder, the last 8 rows 0x00AE3A60-0x00CA8120): 8 of 8 landed

All eight byte-exact (RELOCATION_MATCH + PASS; verify.log, verify_retail_asm.log; bindings multi-check 0):
0x00CA8120 CBasePin::ReceiveConnection and 0x00CA6C40 CBaseRenderer::PrepareReceive (DirectShow BaseClasses, verbatim SDK bodies),
0x00C8C3E0 ov_crosslap (vorbisfile.c, in the landed _ov_64_seek_lap TU), 0x00AE3A60 CParticleProgram::ReadGroups (Ego_r),
0x00C59E20 CCodaVoice::UpdateEnvelopeStages, 0x00C75CE0 CReverbStage94::Process, 0x00C54510 CDecodedStream::Initialise (libpk,
descriptive names bound by landed callers), 0x00C593C0 CLimiter::Process as retail_asm (libpk inline `fstp/fld/fistp dword`
float-to-long block at 0x00C59526, verified against the retail bytes; retail_asm_payload.json / retail_asm_oracle.tsv).
Lessons: inline setters through one reference fold into one register update -- an inline `&member` accessor per setter gives
retail's reload; x87 operand order follows named float locals; loop statement order sets store order and loop alignment.
6 NAME_FIXES + 5 review rows queued (prefix c603). This exhausts the released ChatGPT pass 9 engine remainder.
