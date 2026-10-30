// time import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) _time -- 6 bytes verified against Fable.exe 0x00BFED22
//
// Retail 0x00BFED22 is `jmp dword ptr [__imp__time]` (FF 25 D4 00 44 01). The IAT slot 0x014400D4 is
// MSVCR71.dll!time in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member `MSVCR71.dll` that
// defines _time is a short import record (CODE, NAME_NOPREFIX): the linker emits this thunk for it whenever a
// caller references _time rather than __imp__time.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfed22 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
