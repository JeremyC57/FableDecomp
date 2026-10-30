// sscanf import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) _sscanf -- 6 bytes verified against Fable.exe 0x00BFED1C
//
// Retail 0x00BFED1C is `jmp dword ptr [__imp__sscanf]` (FF 25 D8 00 44 01). The IAT slot 0x014400D8 is
// MSVCR71.dll!sscanf in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member `MSVCR71.dll` that
// defines _sscanf is a short import record (CODE, NAME_NOPREFIX): the linker emits this thunk for it whenever a
// caller references _sscanf rather than __imp__sscanf.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfed1c -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
