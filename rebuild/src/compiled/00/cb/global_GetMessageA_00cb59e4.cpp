// GetMessageA import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: user32.lib(USER32.dll) _GetMessageA@16 -- 6 bytes verified against Fable.exe 0x00CB59E4
//
// Retail 0x00CB59E4 is `jmp dword ptr [__imp__GetMessageA@16]` (FF 25 CC 03 44 01). The IAT slot 0x014403CC is
// USER32.dll!GetMessageA in the retail import directory. The import library is the Platform SDK user32.lib (the VS
// .NET 2003 Vc7/lib fetch has none; named from the WinDDK 3790.1830 copy in $FABLE_VC71/lib); its member
// `USER32.dll` that defines _GetMessageA@16 is a short import record (CODE, NAME_UNDECORATE): the linker emits
// this thunk for it whenever a caller references _GetMessageA@16 rather than __imp__GetMessageA@16. The address
// was outside the function manifest until the 2026-10-30 boundary pass added it (agent_source boundary).
// The build links user32.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00cb59e4 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
