// GetDesktopWindow import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: user32.lib(USER32.dll) _GetDesktopWindow@0 -- 6 bytes verified against Fable.exe 0x00CB590C
//
// Retail 0x00CB590C is `jmp dword ptr [__imp__GetDesktopWindow@0]` (FF 25 3C 03 44 01). The IAT slot 0x0144033C is
// USER32.dll!GetDesktopWindow in the retail import directory. The import library is the Platform SDK user32.lib
// (the VS .NET 2003 Vc7/lib fetch has none; named from the WinDDK 3790.1830 copy in $FABLE_VC71/lib); its member
// `USER32.dll` that defines _GetDesktopWindow@0 is a short import record (CODE, NAME_UNDECORATE): the linker emits
// this thunk for it whenever a caller references _GetDesktopWindow@0 rather than __imp__GetDesktopWindow@0. The
// address was outside the function manifest until the 2026-10-30 boundary pass added it (agent_source boundary).
// The build links user32.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00cb590c -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
