// strchr import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) _strchr -- 6 bytes verified against Fable.exe 0x00BFEDA0
//
// Retail 0x00BFEDA0 is `jmp dword ptr [__imp__strchr]` (FF 25 7C 00 44 01). The IAT slot 0x0144007C is
// MSVCR71.dll!strchr in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member `MSVCR71.dll` that
// defines _strchr is a short import record (CODE, NAME_NOPREFIX): the linker emits this thunk for it whenever a
// caller references _strchr rather than __imp__strchr.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfeda0 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
