// GetFileVersionInfoA import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: version.lib(VERSION.dll) _GetFileVersionInfoA@16 -- 6 bytes verified against Fable.exe 0x00CB5588
//
// Retail 0x00CB5588 is `jmp dword ptr [__imp__GetFileVersionInfoA@16]` (FF 25 48 04 44 01). The IAT slot
// 0x01440448 is VERSION.dll!GetFileVersionInfoA in the retail import directory. The import library is the Platform
// SDK version.lib (named from the WinDDK 3790.1830 lib/wnet/i386 copy); its member `VERSION.dll` that defines
// _GetFileVersionInfoA@16 is a short import record (CODE, NAME_UNDECORATE): the linker emits this thunk for it
// whenever a caller references _GetFileVersionInfoA@16 rather than __imp__GetFileVersionInfoA@16.
// The build links version.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00cb5588 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
