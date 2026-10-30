// ?terminate@@YAXXZ import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) ?terminate@@YAXXZ -- 6 bytes verified against Fable.exe 0x00BFEFAA
//
// Retail 0x00BFEFAA is `jmp dword ptr [__imp_?terminate@@YAXXZ]` (FF 25 34 00 44 01). The IAT slot 0x01440034 is
// MSVCR71.dll!?terminate@@YAXXZ in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member
// `MSVCR71.dll` that defines ?terminate@@YAXXZ is a short import record (CODE, NAME): the linker emits this thunk
// for it whenever a caller references ?terminate@@YAXXZ rather than __imp_?terminate@@YAXXZ. Retail calls this
// thunk from ArrayUnwindFilter (0x00BFEE75) and from __ArrayUnwind's __except funclet (0x00BFEED2). The address
// was outside the function manifest until the 2026-10-30 boundary pass added it (agent_source boundary).
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfefaa -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
