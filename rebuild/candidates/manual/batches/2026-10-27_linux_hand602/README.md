# Chunk 602 (engine remainder, 10 rows 0x00AE18C0-0x00CFF9A0): 7 landed, 1 parked, 2 skipped

Landed (vland --land, verify.log; 1 MATCH, 5 RELOCATION_MATCH, all PASS): 0x00AE18C0 CParticleSimulation::WriteVectorArray,
0x00C076C0 code_match (LZO 1.08 lzo1x_9x.c), 0x00C1E300 CVtable_012b33f0::Read (descriptive, libpk), 0x00C5F4E0
CImaAdpcmCodec::EncodeS16 (DirectX SDK), 0x00C891A0 CReverbDiffuserStage::Process (descriptive, libpk),
0x00CFF9A0 NScript::CQ_AwakeningTheOracleScript::PlayCutscene.
Landed later the same day: 0x00C26090 CStreamBuffer::FinishLoading (verify_00c26090.log) once the libpk build of the pool took
the distinct spelling CHandleBasedMemoryPool_012b4098 (owner decision; 34 landed rows respelled and rechecked).
Parked: 0x00C1D320 CMatrix3x4::Invert DIFFER(440v440), cofactor order c1,c2,c0 vs retail c2,c1,c0 (pending/.../differ/).
Skipped: 0x00C069D0 (landed by the coordinator as toolchain_library from LZO's lzo1x_f1.asm), 0x00C1C460 (needs three
same-TU custom-convention helpers; NOTES.md has the order: land 0x00C1D1C0 and 0x00C1D320 first).
13 NAME_FIXES + 7 review rows queued with prefix c602. Labels: ghidra_out/labels_chunk602_2026-10-27.tsv.
