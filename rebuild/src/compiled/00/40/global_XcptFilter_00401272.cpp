// _XcptFilter import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) __XcptFilter -- 6 bytes verified against Fable.exe 0x00401272
//
// Retail 0x00401272 is `jmp dword ptr [__imp___XcptFilter]` (FF 25 AC 01 44 01). The IAT slot 0x014401AC is
// MSVCR71.dll!_XcptFilter in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member `MSVCR71.dll`
// that defines __XcptFilter is a short import record (CODE, NAME_NOPREFIX): the linker emits this thunk for it
// whenever a caller references __XcptFilter rather than __imp___XcptFilter. _WinMainCRTStartup calls this thunk
// (0x00401239) as its __except filter. The address was outside the function manifest until the 2026-10-30 boundary
// pass added it (agent_source boundary).
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00401272 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
