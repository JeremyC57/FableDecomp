// strncpy import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) _strncpy -- 6 bytes verified against Fable.exe 0x00BFE9C2
//
// Retail 0x00BFE9C2 is `jmp dword ptr [__imp__strncpy]` (FF 25 60 01 44 01). The IAT slot 0x01440160 is
// MSVCR71.dll!strncpy in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member `MSVCR71.dll`
// that defines _strncpy is a short import record (CODE, NAME_NOPREFIX): the linker emits this thunk for it
// whenever a caller references _strncpy rather than __imp__strncpy.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfe9c2 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
