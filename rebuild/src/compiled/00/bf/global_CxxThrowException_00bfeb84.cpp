// _CxxThrowException import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) __CxxThrowException@8 -- 6 bytes verified against Fable.exe 0x00BFEB84
//
// Retail 0x00BFEB84 is `jmp dword ptr [__imp___CxxThrowException@8]` (FF 25 04 01 44 01). The IAT slot 0x01440104
// is MSVCR71.dll!_CxxThrowException in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member
// `MSVCR71.dll` that defines __CxxThrowException@8 is a short import record (CODE, NAME_UNDECORATE): the linker
// emits this thunk for it whenever a caller references __CxxThrowException@8 rather than
// __imp___CxxThrowException@8.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfeb84 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
