// PeekMessageA import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: user32.lib(USER32.dll) _PeekMessageA@20 -- 6 bytes verified against Fable.exe 0x00CB59F0
//
// Retail 0x00CB59F0 is `jmp dword ptr [__imp__PeekMessageA@20]` (FF 25 D4 03 44 01). The IAT slot 0x014403D4 is
// USER32.dll!PeekMessageA in the retail import directory. The import library is the Platform SDK user32.lib (the
// VS .NET 2003 Vc7/lib fetch has none; named from the WinDDK 3790.1830 copy in $FABLE_VC71/lib); its member
// `USER32.dll` that defines _PeekMessageA@20 is a short import record (CODE, NAME_UNDECORATE): the linker emits
// this thunk for it whenever a caller references _PeekMessageA@20 rather than __imp__PeekMessageA@20. The address
// was outside the function manifest until the 2026-10-30 boundary pass added it (agent_source boundary).
// The build links user32.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00cb59f0 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
