// __chkstk: VC7.1 CRT library code, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(build\intel\dll_obj\chkstk.obj) __chkstk -- 61 bytes verified against Fable.exe 0x00BFEA30
//
// Retail 0x00BFEA30..0x00BFEA6C is the whole 61-byte .text section of chkstk.obj in the VS .NET 2003
// Vc7/lib/msvcrt.lib that retail links (it imports MSVCR71.dll). The object is MASM 7.10 output (@comp.id 0x000F
// build 3077; the retail Rich header lists twelve 0x000F/3077 objects). The section has no relocations, so every
// byte is compared. __alloca_probe is a second label at the same offset of chkstk.obj; VC7.1 emits a call to
// __chkstk for frames of 4 KB or more and to __alloca_probe for _alloca, so both spellings are compiler-generated
// names of this one address. It was parked in the pending pool as an unauthored DIFFER (hand-written MASM or CRT
// source); vendor bytes are linked, not reconstructed (owner rule 2026-10-30).
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfea30 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
