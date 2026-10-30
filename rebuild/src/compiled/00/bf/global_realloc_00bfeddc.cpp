// realloc import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) _realloc -- 6 bytes verified against Fable.exe 0x00BFEDDC
//
// Retail 0x00BFEDDC is `jmp dword ptr [__imp__realloc]` (FF 25 54 00 44 01). The IAT slot 0x01440054 is
// MSVCR71.dll!realloc in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member `MSVCR71.dll`
// that defines _realloc is a short import record (CODE, NAME_NOPREFIX): the linker emits this thunk for it
// whenever a caller references _realloc rather than __imp__realloc. The address was outside the function manifest
// until the 2026-10-30 boundary pass added it (agent_source boundary).
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfeddc -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
