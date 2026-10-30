// ??0exception@@QAE@XZ import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) ??0exception@@QAE@XZ -- 6 bytes verified against Fable.exe 0x00F3FC40
//
// Retail 0x00F3FC40 is `jmp dword ptr [__imp_??0exception@@QAE@XZ]` (FF 25 F8 FF 43 01). The IAT slot 0x0143FFF8
// is MSVCR71.dll!??0exception@@QAE@XZ in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member
// `MSVCR71.dll` that defines ??0exception@@QAE@XZ is a short import record (CODE, NAME): the linker emits this
// thunk for it whenever a caller references ??0exception@@QAE@XZ rather than __imp_??0exception@@QAE@XZ. The
// address was outside the function manifest until the 2026-10-30 boundary pass added it (agent_source boundary).
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00f3fc40 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
