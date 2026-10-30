// _itoa import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) __itoa -- 6 bytes verified against Fable.exe 0x00BFED34
//
// Retail 0x00BFED34 is `jmp dword ptr [__imp___itoa]` (FF 25 C8 00 44 01). The IAT slot 0x014400C8 is
// MSVCR71.dll!_itoa in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member `MSVCR71.dll` that
// defines __itoa is a short import record (CODE, NAME_NOPREFIX): the linker emits this thunk for it whenever a
// caller references __itoa rather than __imp___itoa.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfed34 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
