// strncmp import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) _strncmp -- 6 bytes verified against Fable.exe 0x00BFEAF8
//
// Retail 0x00BFEAF8 is `jmp dword ptr [__imp__strncmp]` (FF 25 38 01 44 01). The IAT slot 0x01440138 is
// MSVCR71.dll!strncmp in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member `MSVCR71.dll`
// that defines _strncmp is a short import record (CODE, NAME_NOPREFIX): the linker emits this thunk for it
// whenever a caller references _strncmp rather than __imp__strncmp.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfeaf8 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
