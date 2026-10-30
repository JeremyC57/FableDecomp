// __setusermatherr import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) ___setusermatherr -- 6 bytes verified against Fable.exe 0x0040136E
//
// Retail 0x0040136E is `jmp dword ptr [__imp____setusermatherr]` (FF 25 8C 01 44 01). The IAT slot 0x0144018C is
// MSVCR71.dll!__setusermatherr in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member
// `MSVCR71.dll` that defines ___setusermatherr is a short import record (CODE, NAME_NOPREFIX): the linker emits
// this thunk for it whenever a caller references ___setusermatherr rather than __imp____setusermatherr. The
// address was outside the function manifest until the 2026-10-30 boundary pass added it (agent_source boundary).
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 0040136e -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
