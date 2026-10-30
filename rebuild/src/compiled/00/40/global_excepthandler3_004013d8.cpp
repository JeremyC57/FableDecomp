// _except_handler3 import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) __except_handler3 -- 6 bytes verified against Fable.exe 0x004013D8
//
// Retail 0x004013D8 is `jmp dword ptr [__imp___except_handler3]` (FF 25 78 01 44 01). The IAT slot 0x01440178 is
// MSVCR71.dll!_except_handler3 in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member
// `MSVCR71.dll` that defines __except_handler3 is a short import record (CODE, NAME_NOPREFIX): the linker emits
// this thunk for it whenever a caller references __except_handler3 rather than __imp___except_handler3.
// __SEH_prolog (0x0040138C) pushes this thunk's address as the frame handler. The address was outside the function
// manifest until the 2026-10-30 boundary pass added it (agent_source boundary).
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 004013d8 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
