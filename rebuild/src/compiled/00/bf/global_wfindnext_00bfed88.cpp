// _wfindnext import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) __wfindnext -- 6 bytes verified against Fable.exe 0x00BFED88
//
// Retail 0x00BFED88 is `jmp dword ptr [__imp___wfindnext]` (FF 25 8C 00 44 01). The IAT slot 0x0144008C is
// MSVCR71.dll!_wfindnext in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member `MSVCR71.dll`
// that defines __wfindnext is a short import record (CODE, NAME_NOPREFIX): the linker emits this thunk for it
// whenever a caller references __wfindnext rather than __imp___wfindnext.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfed88 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
