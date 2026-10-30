// wcsncmp import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) _wcsncmp -- 6 bytes verified against Fable.exe 0x00BFEB0A
//
// Retail 0x00BFEB0A is `jmp dword ptr [__imp__wcsncmp]` (FF 25 2C 01 44 01). The IAT slot 0x0144012C is
// MSVCR71.dll!wcsncmp in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member `MSVCR71.dll`
// that defines _wcsncmp is a short import record (CODE, NAME_NOPREFIX): the linker emits this thunk for it
// whenever a caller references _wcsncmp rather than __imp__wcsncmp.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfeb0a -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
