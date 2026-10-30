// fopen import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) _fopen -- 6 bytes verified against Fable.exe 0x00BFEDFA
//
// Retail 0x00BFEDFA is `jmp dword ptr [__imp__fopen]` (FF 25 40 00 44 01). The IAT slot 0x01440040 is
// MSVCR71.dll!fopen in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member `MSVCR71.dll` that
// defines _fopen is a short import record (CODE, NAME_NOPREFIX): the linker emits this thunk for it whenever a
// caller references _fopen rather than __imp__fopen.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfedfa -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
