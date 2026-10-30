// _CIfmod import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) __CIfmod -- 6 bytes verified against Fable.exe 0x00BFEB28
//
// Retail 0x00BFEB28 is `jmp dword ptr [__imp___CIfmod]` (FF 25 18 01 44 01). The IAT slot 0x01440118 is
// MSVCR71.dll!_CIfmod in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member `MSVCR71.dll`
// that defines __CIfmod is a short import record (CODE, NAME_NOPREFIX): the linker emits this thunk for it
// whenever a caller references __CIfmod rather than __imp___CIfmod.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfeb28 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
