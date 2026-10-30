// _purecall import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) __purecall -- 6 bytes verified against Fable.exe 0x00BFEA08
//
// Retail 0x00BFEA08 is `jmp dword ptr [__imp___purecall]` (FF 25 5C 01 44 01). The IAT slot 0x0144015C is
// MSVCR71.dll!_purecall in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member `MSVCR71.dll`
// that defines __purecall is a short import record (CODE, NAME_NOPREFIX): the linker emits this thunk for it
// whenever a caller references __purecall rather than __imp___purecall.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfea08 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
