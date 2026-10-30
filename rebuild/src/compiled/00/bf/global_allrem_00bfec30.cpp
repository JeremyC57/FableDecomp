// __allrem: VC7.1 CRT library code, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(build\intel\dll_obj\llrem.obj) __allrem -- 178 bytes verified against Fable.exe 0x00BFEC30
//
// Retail 0x00BFEC30..0x00BFECE1 is the whole 178-byte .text section of llrem.obj in the VS .NET 2003
// Vc7/lib/msvcrt.lib that retail links (it imports MSVCR71.dll). The object is MASM 7.10 output (@comp.id 0x000F
// build 3077; the retail Rich header lists twelve 0x000F/3077 objects). The section has no relocations, so every
// byte is compared. VC7.1 emits a call to __allrem for a signed 64-bit remainder. It was parked in the pending
// pool as an unauthored DIFFER (hand-written MASM or CRT source); vendor bytes are linked, not reconstructed
// (owner rule 2026-10-30).
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfec30 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
