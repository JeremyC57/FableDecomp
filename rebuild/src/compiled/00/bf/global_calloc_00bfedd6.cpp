// calloc import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) _calloc -- 6 bytes verified against Fable.exe 0x00BFEDD6
//
// Retail 0x00BFEDD6 is `jmp dword ptr [__imp__calloc]` (FF 25 58 00 44 01). The IAT slot 0x01440058 is
// MSVCR71.dll!calloc in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member `MSVCR71.dll` that
// defines _calloc is a short import record (CODE, NAME_NOPREFIX): the linker emits this thunk for it whenever a
// caller references _calloc rather than __imp__calloc. The address was outside the function manifest until the
// 2026-10-30 boundary pass added it (agent_source boundary).
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfedd6 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
