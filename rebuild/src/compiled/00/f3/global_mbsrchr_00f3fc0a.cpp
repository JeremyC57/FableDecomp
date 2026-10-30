// _mbsrchr import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) __mbsrchr -- 6 bytes verified against Fable.exe 0x00F3FC0A
//
// Retail 0x00F3FC0A is `jmp dword ptr [__imp___mbsrchr]` (FF 25 D4 FF 43 01). The IAT slot 0x0143FFD4 is
// MSVCR71.dll!_mbsrchr in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member `MSVCR71.dll`
// that defines __mbsrchr is a short import record (CODE, NAME_NOPREFIX): the linker emits this thunk for it
// whenever a caller references __mbsrchr rather than __imp___mbsrchr. The address was outside the function
// manifest until the 2026-10-30 boundary pass added it (agent_source boundary).
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00f3fc0a -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
