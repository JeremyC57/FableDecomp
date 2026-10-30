// SHGetFolderPathW import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: shell32.lib(SHELL32.dll) _SHGetFolderPathW@20 -- 6 bytes verified against Fable.exe 0x00CB5A6E
//
// Retail 0x00CB5A6E is `jmp dword ptr [__imp__SHGetFolderPathW@20]` (FF 25 80 02 44 01). The IAT slot 0x01440280
// is SHELL32.dll!SHGetFolderPathW in the retail import directory. The import library is the Platform SDK
// shell32.lib (named from the WinDDK 3790.1830 lib/wnet/i386 copy); its member `SHELL32.dll` that defines
// _SHGetFolderPathW@20 is a short import record (CODE, NAME_UNDECORATE): the linker emits this thunk for it
// whenever a caller references _SHGetFolderPathW@20 rather than __imp__SHGetFolderPathW@20. The address was
// outside the function manifest until the 2026-10-30 boundary pass added it (agent_source boundary).
// The build links shell32.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00cb5a6e -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
