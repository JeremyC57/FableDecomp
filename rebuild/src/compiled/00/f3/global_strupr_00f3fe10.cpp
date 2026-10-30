// _strupr import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) __strupr -- 6 bytes verified against Fable.exe 0x00F3FE10
//
// Retail 0x00F3FE10 is `jmp dword ptr [__imp___strupr]` (FF 25 2C 00 44 01). The IAT slot 0x0144002C is
// MSVCR71.dll!_strupr in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member `MSVCR71.dll`
// that defines __strupr is a short import record (CODE, NAME_NOPREFIX): the linker emits this thunk for it
// whenever a caller references __strupr rather than __imp___strupr. The address was outside the function manifest
// until the 2026-10-30 boundary pass added it (agent_source boundary).
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00f3fe10 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
