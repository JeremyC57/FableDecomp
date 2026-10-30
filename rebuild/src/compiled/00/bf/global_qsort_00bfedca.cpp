// qsort import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) _qsort -- 6 bytes verified against Fable.exe 0x00BFEDCA
//
// Retail 0x00BFEDCA is `jmp dword ptr [__imp__qsort]` (FF 25 60 00 44 01). The IAT slot 0x01440060 is
// MSVCR71.dll!qsort in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member `MSVCR71.dll` that
// defines _qsort is a short import record (CODE, NAME_NOPREFIX): the linker emits this thunk for it whenever a
// caller references _qsort rather than __imp__qsort.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfedca -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
