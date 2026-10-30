// wcslen import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) _wcslen -- 6 bytes verified against Fable.exe 0x00BFED94
//
// Retail 0x00BFED94 is `jmp dword ptr [__imp__wcslen]` (FF 25 84 00 44 01). The IAT slot 0x01440084 is
// MSVCR71.dll!wcslen in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member `MSVCR71.dll` that
// defines _wcslen is a short import record (CODE, NAME_NOPREFIX): the linker emits this thunk for it whenever a
// caller references _wcslen rather than __imp__wcslen.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfed94 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
