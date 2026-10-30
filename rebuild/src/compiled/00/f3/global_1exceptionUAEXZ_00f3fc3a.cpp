// ??1exception@@UAE@XZ import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) ??1exception@@UAE@XZ -- 6 bytes verified against Fable.exe 0x00F3FC3A
//
// Retail 0x00F3FC3A is `jmp dword ptr [__imp_??1exception@@UAE@XZ]` (FF 25 28 00 44 01). The IAT slot 0x01440028
// is MSVCR71.dll!??1exception@@UAE@XZ in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member
// `MSVCR71.dll` that defines ??1exception@@UAE@XZ is a short import record (CODE, NAME): the linker emits this
// thunk for it whenever a caller references ??1exception@@UAE@XZ rather than __imp_??1exception@@UAE@XZ. The
// address was outside the function manifest until the 2026-10-30 boundary pass added it (agent_source boundary).
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00f3fc3a -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
