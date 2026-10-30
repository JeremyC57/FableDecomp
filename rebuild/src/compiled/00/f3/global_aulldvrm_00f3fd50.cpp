// __aulldvrm: VC7.1 CRT library code, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(build\intel\dll_obj\ulldvrm.obj) __aulldvrm -- 149 bytes verified against Fable.exe 0x00F3FD50
//
// Retail 0x00F3FD50..0x00F3FDE4 is the whole 149-byte .text section of ulldvrm.obj in the VS .NET 2003
// Vc7/lib/msvcrt.lib that retail links (it imports MSVCR71.dll). The object is MASM 7.10 output (@comp.id 0x000F
// build 3077; the retail Rich header lists twelve 0x000F/3077 objects). The section has no relocations, so every
// byte is compared. VC7.1 emits a call to __aulldvrm for an unsigned 64-bit divide that also needs the remainder.
// It was parked in the pending pool as an unauthored DIFFER (hand-written MASM or CRT source); vendor bytes are
// linked, not reconstructed (owner rule 2026-10-30).
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00f3fd50 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
