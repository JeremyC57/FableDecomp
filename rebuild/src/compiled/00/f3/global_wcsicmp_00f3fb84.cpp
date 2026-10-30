// _wcsicmp import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) __wcsicmp -- 6 bytes verified against Fable.exe 0x00F3FB84
//
// Retail 0x00F3FB84 is `jmp dword ptr [__imp___wcsicmp]` (FF 25 B4 01 44 01). The IAT slot 0x014401B4 is
// MSVCR71.dll!_wcsicmp in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member `MSVCR71.dll`
// that defines __wcsicmp is a short import record (CODE, NAME_NOPREFIX): the linker emits this thunk for it
// whenever a caller references __wcsicmp rather than __imp___wcsicmp.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00f3fb84 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
