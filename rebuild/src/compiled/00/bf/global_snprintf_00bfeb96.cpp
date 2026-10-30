// _snprintf import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) __snprintf -- 6 bytes verified against Fable.exe 0x00BFEB96
//
// Retail 0x00BFEB96 is `jmp dword ptr [__imp___snprintf]` (FF 25 F8 00 44 01). The IAT slot 0x014400F8 is
// MSVCR71.dll!_snprintf in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member `MSVCR71.dll`
// that defines __snprintf is a short import record (CODE, NAME_NOPREFIX): the linker emits this thunk for it
// whenever a caller references __snprintf rather than __imp___snprintf.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfeb96 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
