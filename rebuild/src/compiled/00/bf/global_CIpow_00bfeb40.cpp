// _CIpow import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) __CIpow -- 6 bytes verified against Fable.exe 0x00BFEB40
//
// Retail 0x00BFEB40 is `jmp dword ptr [__imp___CIpow]` (FF 25 08 01 44 01). The IAT slot 0x01440108 is
// MSVCR71.dll!_CIpow in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member `MSVCR71.dll` that
// defines __CIpow is a short import record (CODE, NAME_NOPREFIX): the linker emits this thunk for it whenever a
// caller references __CIpow rather than __imp___CIpow.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfeb40 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
